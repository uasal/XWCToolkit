---
title: "XWCToolkit Resurrector Analysis"
subtitle: "Process Supervision for High-Reliability Embedded Computing"
author: "Deep Code Analysis"
date: \today
institute: "uasal/XWCToolkit"
theme: "Madrid"
colortheme: "dolphin"
fonttheme: "structurebold"
fontsize: 10pt
aspectratio: 169
header-includes:
  - \usepackage{listings}
  - \usepackage{xcolor}
  - \lstset{basicstyle=\tiny\ttfamily, breaklines=true, frame=single, backgroundcolor=\color{gray!10}}
  - \setbeamertemplate{navigation symbols}{}
  - \setbeamertemplate{footline}{\hfill\insertframenumber/\inserttotalframenumber\hspace{2mm}\vspace{2mm}}
---

# Executive Summary

## Executive Summary --- Original Findings

This presentation documents the **process-supervision architecture** that XWCToolkit inherits from MagAO-X, and identifies the gaps that motivate the `resurrector_indi` work on the [`dev-resurrector`](https://github.com/magao-x/MagAOX/tree/dev-resurrector) branch.

\footnotesize

- **No standalone resurrector in XWCToolkit.** Recovery is distributed across three nominal layers — in-process (`MagAOXApp`, `shmimMonitor`), driver-restart (`indiserver`), and an *absent* external supervisor.
- **Three concrete gaps in the current code base:**
   1. **No application-level heartbeat.** [`XWCTK/app/MagAOXApp.hpp` line 1536](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L1536): `/** \todo Need a heartbeat update here. */`
   2. **No external supervisor.** No systemd units or restart scripts found in-tree.
   3. **No lockup detection.** If [`appLogic()`](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp) deadlocks, nothing notices.
- **In-process recovery does exist** for shared-memory faults: [`XWCTK/app/dev/shmimMonitor.hpp` line 183](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/dev/shmimMonitor.hpp#L183) sets `m_restart` from a `SIGSEGV`/`SIGBUS` handler, and the indiserver driver-restart logic lives at [`lib/INDI/INDI/indiserver.c` lines 384–437](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L384-L437) (10 s `RESTARTDT`, `/tmp/noindi` lockout).
- **Ancillary defects identified:** `m_shutdown` declared as plain `int` ([`MagAOXApp.hpp:102`](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L102)) rather than `volatile sig_atomic_t`; signal handler uses non-async-signal-safe `std::cerr`/`log<text_log>` ([`MagAOXApp.hpp:1718–1747`](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L1718-L1747)).

## Executive Summary --- New Findings (Claude Opus 4.7 deep review, 2026-04-20)

The following findings were added by a deep cross-check of this deck against the current `HEAD` of [`uasal/XWCToolkit`](https://github.com/uasal/XWCToolkit) and [`magao-x/MagAOX@dev-resurrector`](https://github.com/magao-x/MagAOX/tree/dev-resurrector). All cited file:line locations were verified in this review.

\footnotesize

- **The "three layers" are not independent.** Layer 1 ([`MagAOXApp`](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp), [`shmimMonitor.hpp:177–183`](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/dev/shmimMonitor.hpp#L177-L183)) only fires if the process is alive enough to run its handler; Layer 2 ([`indiserver.c:1338–1340`](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L1338-L1340), `exit(1)` on `EXITEXFAIL`) only fires if `indiserver` itself is alive. A single segfault in the wrong process kills both. The framing as *defense in depth* over-sells the pre-resurrector state — `resurrector_indi` is the missing **independent** layer.
- **The `MagAOXApp.hpp:1536` TODO is unchanged on `dev-resurrector`.** The supervisor side of the hexbeat protocol exists in [`utils/resurrector_indi/HexbeatMonitor.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp), but the *application* side is documented (README diagram) rather than wired into `MagAOXApp`'s main loop. End-to-end lockup detection therefore depends on a separate `resurrectee` integration in [`libMagAOX/`](https://github.com/magao-x/MagAOX/tree/dev-resurrector/libMagAOX).
- **The "no systemd in repo" claim is self-referential.** A repo-wide grep for `systemd|WatchdogSec|Restart=` returns only the two presentations themselves — substantively correct, but the deck's own searches are circular. (Already flagged in the appendix; surfaced here.)
- **Cosmetic citation drift:** `shmimMonitor.hpp` `m_restart` is at line 183, not the 177–185 range cited later in the deck. Substantive content unaffected.
- **Embedded reliability uplift, not closure.** Even with `resurrector_indi` adopted: there is no upstream watchdog over the supervisor itself (`/dev/watchdog` or systemd `WatchdogSec=`), no persistent restart accounting across supervisor restarts, and no fail-safe hardware hook (mirrors→flat, motors→home, lasers→off) when restart limits are exceeded. See companion deck for traces.

\normalsize

\tiny *New-findings authorship: Claude Opus 4.7. Generated 2026-04-20; verified against `uasal/XWCToolkit@main` and `magao-x/MagAOX@dev-resurrector`. Treat as advisory, not authoritative — verify each citation before quoting.*

# Overview

## What Is the "Resurrector"?

- XWCToolkit does **not** contain a standalone "resurrector" application
- Instead: a **multi-layered, distributed process supervision architecture**
- Resurrection logic is **embedded at three distinct layers** of the stack
- Derived from MagAO-X, the software running the MagAO-X Extreme AO system

## Supervision Architecture: Three Layers

```
Layer 3: [EXTERNAL SUPERVISOR — Not in repo]
         systemd / cron / custom scripts
              |
Layer 2: [indiserver built-in driver restart]
         lib/INDI/INDI/indiserver.c
              |
Layer 1: [In-process recovery — MagAOXApp]
         XWCTK/app/MagAOXApp.hpp
         XWCTK/app/dev/shmimMonitor.hpp
```

# Layer 1: MagAOXApp — In-Process Recovery

## The Application Base Class

- All XWCToolkit applications derive from `MagAOXApp`
- Provides: lifecycle management, state machine, signal handling, PID locking
- Source: [`XWCTK/app/MagAOXApp.hpp`](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp)

Direct quote — single-instance enforcement ([line 111–114](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L111-L114)):

\lstset{language=C++}
```cpp
/// Only one MagAOXApp can be instantiated per
/// program.  Hence this c'tor will issue exit(-1)
/// if the static self-pointer m_self is already
/// initialized.
```

## The Main Event Loop

Core loop drives all application behavior — [source lines 1481–1557](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L1481-L1557):

\lstset{language=C++}
```cpp
//This is the main event loop.
/* Conditions on entry:
 * -- PID locked
 * -- Log thread running
 * -- Signal handling installed
 * -- appStartup() successful
 * -- INDI communications started successfully
 * -- power state known (if being managed)
 */
while( m_shutdown == 0)
{
   // ... power management, appLogic(), INDI sync ...
}
```

**Strength:** Well-documented entry invariants ensure loop only runs in a known-good state.

## The Shutdown Flag

The central coordination mechanism — [line 102](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L102):

\lstset{language=C++}
```cpp
int m_shutdown {0};
///< Flag to signal it's time to shutdown.
///   When not 0, the main loop exits.
```

. . .

**Weakness:** This is a plain `int`, not `std::atomic<int>`.

- Signal handlers write it; the main loop reads it
- On weakly-ordered architectures (ARM, RISC-V) used in embedded systems, this is a **potential data race**

## Application Startup Sequence

Careful multi-phase initialization — [lines 1392–1444](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L1392-L1444):

\lstset{language=C++}
```cpp
if( lockPID() < 0 )          // Phase 1: PID lock
{
   state(stateCodes::FAILURE);
   log<text_log>({"Failed to lock PID."},
                 logPrio::LOG_CRITICAL);
   return -1;  // Not safe to go on
}

m_log.logThreadStart();       // Phase 2: Logger

setSigTermHandler();          // Phase 3: Signals

state(stateCodes::INITIALIZED);
if(appStartup() < 0)         // Phase 4: App init
   m_shutdown = 1;

if(startINDI() < 0)          // Phase 5: INDI
{
   state(stateCodes::FAILURE);
   m_shutdown = 1;
}
```

**Strength:** Sequential, validated phases — each gate-checks the previous step.

## The State Machine

Defined in [`stateCodes.hpp`](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/stateCodes.hpp#L42-L58) (lines 42–58):

\lstset{language=C++}
```cpp
enum : stateCodeT {
  FAILURE=-20,      ///< should be used when m_shutdown
                    ///  is set for an error.
  ERROR=-10,        ///< encountered an error, from which
                    ///  it is recovering
  UNINITIALIZED = 0,
  INITIALIZED = 1,
  NODEVICE = 2,     POWEROFF = 4,
  POWERON = 6,      NOTCONNECTED = 8,
  CONNECTED = 10,   LOGGEDIN = 15,
  CONFIGURING = 20, NOTHOMED = 24,
  HOMING = 25,      OPERATING = 30,
  READY = 35,
  SHUTDOWN = 10000
};
```

## State Machine: Recovery Semantics

| State | Code | Recovery Behavior |
|-------|------|-------------------|
| `FAILURE` | -20 | Terminal — triggers `m_shutdown` |
| `ERROR` | -10 | **Recoverable** — `appLogic()` keeps running |
| `OPERATING` | 30 | Normal operation |

. . .

Key distinction from the source ([line 42–43](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/stateCodes.hpp#L42-L43)):

- `FAILURE`: *"should be used when m_shutdown is set for an error"*
- `ERROR`: *"recovering (with or without intervention)"*

. . .

**Strength:** Graduated error model — apps can self-heal from `ERROR` without restarting.

**Weakness:** No maximum duration in `ERROR` — an app can stay broken indefinitely.

## Log Deduplication: stateLogged()

Prevents log flooding during sustained errors — [lines 492–508](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L492-L508):

\lstset{language=C++}
```cpp
/// Returns the value of m_stateLogged.
/// Will be 0 on first call after a state change,
/// >0 afterwards.
///
/// Example usage:
///   if( connection_failed )
///   {
///      state( stateCodes::NOTCONNECTED );
///      if(!stateLogged())
///         log<text_log>("Not connected");
///   }
```

**Strength:** Elegant pattern — log error once on state entry, not every loop iteration.

## The Missing Heartbeat

[Line 1536](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L1536), inside the main event loop:

\lstset{language=C++}
```cpp
/** \todo Need a heartbeat update here.
  */
```

. . .

**Critical Weakness:**

- No watchdog timer to detect if `appLogic()` hangs or deadlocks
- No liveness signal for external supervisors
- This TODO has been present since the original MagAO-X codebase
- For high-reliability embedded systems, this is a **critical gap**

# Layer 1: Signal Handling

## Signal Handler Installation

Three termination signals handled — [lines 1665–1703](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L1665-L1703):

\lstset{language=C++}
```cpp
act.sa_sigaction =
    &MagAOXApp<_useINDI>::_handlerSigTerm;
act.sa_flags = SA_SIGINFO;
sigemptyset(&set);
act.sa_mask = set;

sigaction(SIGTERM, &act, 0);
sigaction(SIGQUIT, &act, 0);
sigaction(SIGINT, &act, 0);
```

**Strength:** Uses `SA_SIGINFO` for extended signal information.

**Strength:** Empty mask — no signal blocking during handler.

## Signal Handler Implementation

[Lines 1717–1747](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L1717-L1747) — minimal, safe handler:

\lstset{language=C++}
```cpp
void handlerSigTerm( int signum,
   siginfo_t *siginf __attribute__((unused)),
   void *ucont __attribute__((unused)) )
{
   m_shutdown = 1;
   // ... determine signal name ...
   std::string logss = "Caught signal ";
   logss += signame;
   logss += ". Shutting down.";
   std::cerr << "\n" << logss << std::endl;
   log<text_log>(logss);
}
```

**Strength:** Simply sets flag — no complex logic in signal context.

**Weakness:** Calls `std::cerr` and logging from a signal handler — technically not async-signal-safe.

## SIGUSR1: Thread Wake-Up

[Line 1749](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L1749):

\lstset{language=C++}
```cpp
/// Empty signal handler.  SIGUSR1 is used to
/// interrupt sleep in various threads.
```

**Strength:** Efficient thread wake-up pattern without busy-waiting.

# Layer 1: PID Management

## PID Lock Mechanism

[Lines 1796–1924](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L1796-L1924) — robust stale-PID detection:

\lstset{language=C++}
```cpp
// Read PID from file
pid_t testPid;
pidIn >> testPid;
// Get command line from /proc
std::stringstream procN;
procN << "/proc/" << testPid << "/cmdline";
// Search for invokedName in command line
size_t invokedPos =
    pidCmdLine.find( invokedName );
// If invokedName found, check for configName
size_t configPos = std::string::npos;
if(invokedPos != std::string::npos)
    configPos = pidCmdLine.find( m_configName );
```

**Strength:** Checks both process existence **and** command-line identity — not just PID.

## PID Lock: Stale Detection

The rejection message when a duplicate is found — [line 1882](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L1882):

\lstset{language=C++}
```cpp
logss << "PID already locked ("
      << testPid  << ").  Time to die.";
```

The clean-up path — [line 1892](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L1892):

\lstset{language=C++}
```cpp
// No PID File so we should just go on.
```

**Strength:** Gracefully handles crashes where PID file was not cleaned up.

**Weakness:** No cleanup on `SIGKILL` / hard crash — relies on stale detection at next startup.

## PID File Location

From [`paths.hpp`](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/common/paths.hpp#L18-L65) (lines 18–22, 60–64):

\lstset{language=C++}
```cpp
#define MAGAOX_path "/opt/MagAOX"
#define MAGAOX_sysRelPath "sys"
```

PID files stored at: `/opt/MagAOX/sys/<configName>/pid`

**Strength:** Well-structured, per-application file hierarchy.

## Graceful Shutdown Sequence

[Lines 1559–1586](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L1559-L1586):

\lstset{language=C++}
```cpp
appShutdown();              // 1. App cleanup
state(stateCodes::SHUTDOWN);// 2. State transition
m_indiDriver->sendDelProperty(ipSend); // 3. INDI
m_indiDriver->quitProcess();           // 4. INDI
m_indiDriver->deactivate();            // 5. INDI
unlockPID();                // 6. Release PID
sleep(1);                   // 7. Grace period
return 0;
```

**Strength:** Ordered teardown — app resources, then INDI, then PID, then exit.

# Layer 1: shmimMonitor — In-Process Resurrection

## Shared Memory Fault Recovery

[`shmimMonitor.hpp`](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/dev/shmimMonitor.hpp#L177-L185) lines 177–185 — handling buffer resets:

\lstset{language=C++}
```cpp
/** \name SIGSEGV & SIGBUS signal handling
 * These signals occur as a result of a
 * ImageStreamIO source server resetting
 * (e.g. changing frame sizes).
 * When they occur a restart of the shmim
 * monitor thread main loops is triggered.
 * @{
 */
bool m_restart{false};
///< Flag indicating tha the shared memory
///   should be reinitialized.
```

**Strength:** Recovers from SIGSEGV/SIGBUS — faults that normally kill a process.

## shmimMonitor: Restart Detection Logic

Three triggers for `m_restart` — [lines 681–698](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/dev/shmimMonitor.hpp#L681-L698):

\lstset{language=C++}
```cpp
// Check if the file has disappeared.
SM_fd = open(SM_fname, O_RDWR);
if (SM_fd == -1)
{
    m_restart = true;     // Trigger 1: file gone
}

// Check if the inode changed
int rv = stat(SM_fname, &buffer);
if (rv != 0)
{
    m_restart = true;     // Trigger 2: stat failed
}

if (buffer.st_ino != m_inode)
{
    m_restart = true;     // Trigger 3: inode changed
}
```

**Strength:** Triple detection — file removal, stat failure, and inode change.

## shmimMonitor: Reinitialize on Restart

[Lines 439–496](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/dev/shmimMonitor.hpp#L439-L496) — the recovery path:

\lstset{language=C++}
```cpp
opened = false;
m_restart = false;
// Set this up front, since we're about to restart.

int logged = 0;
while (!opened && !derived().m_shutdown
       && !m_restart
       && derived().state() == stateCodes::OPERATING)
{
    // Retry opening shared memory...
    // "be patient"
    sleep(1);
}

if (m_restart)
    continue;
// this is kinda dumb.  we just go around on
// restart, so why test in the while loop at all?
```

**Strength:** Zero-downtime recovery — no process restart needed.

. . .

*Note the candid developer comment:* `"this is kinda dumb"` — self-aware code quality flag.

# Layer 2: indiserver — Automatic Driver Restart

## indiserver Overview

[`lib/INDI/INDI/indiserver.c`](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c) — the core resurrection engine.

From the file header comment ([lines 13–14](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L13-L14)):

\lstset{language=C}
```c
/* Local Drivers are restarted if they exit or
 * their connection closes. Connection to remote
 * Devices is retried if connection is lost.
 */
```

And the threading model ([lines 46–48](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L46-L48)):

\lstset{language=C}
```c
/* Each driver structure contains a rwlock
 * write-locked when/if it is restarted.
 */
```

## Driver Information Structure

`DvrInfo` struct — [lines 149–171](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L149-L171):

\lstset{language=C}
```c
typedef struct {
    char *name;
    int pid;
    int err;          /* set on fatal error */
    int rfd;          /* driver's stdout read pipe */
    int wfd;          /* driver's stdin write pipe */
    int efd;          /* driver's stderr read pipe */
    time_t start;     /* time driver was started */
    int restarts;     /* n times restarted */
    FQ *msgq;         /* outbound Msg queue */
    pthread_rwlock_t restart_lock;
        /* lock out this device while restarting */
} DvrInfo;
```

**Strength:** Per-driver restart tracking and locking.

## Crash Detection: Method 1 — EOF on stdout

`driverStdoutReaderThread` — [lines 1039–1046](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L1039-L1046):

\lstset{language=C}
```c
nr = read (dp->rfd, dp->mp->cp + dp->mp->used,
           dp->mp->total - dp->mp->used);
if (nr <= 0) {
    if (nr < 0)
        logMessage ("from Driver %s: stdin %s\n",
                    dp->name, strerror(errno));
    else
        logMessage ("from Driver %s: stdin EOF\n",
                    dp->name);
    onDriverError (dp);
    return (NULL);  /* thread exit */
}
```

**Strength:** Immediate detection when a driver process terminates.

## Crash Detection: Method 2 — XML Parse Error

[Lines 1117–1120](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L1117-L1120):

\lstset{language=C}
```c
} else if (err[0]) {
    logMessage ("Driver %s: XML error: %s\n",
                dp->name, err);
    onDriverError (dp);
    return (NULL);  /* thread exit */
}
```

**Strength:** Detects driver corruption/malfunction, not just crashes.

## Crash Detection: Method 3 — Write Failure

[Lines 1212–1226](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L1212-L1226):

\lstset{language=C}
```c
nw = write (dp->wfd, mp->cp+nsent, nsend);
if (nw <= 0) {
    /* EPIPE errors are not reported because
     * they are too numerous to be interesting
     * as we wait for driverStdinReader to
     * detect problem and set dp->err */
    break;
}
```

**Strength:** Multi-path detection — even if one path misses a failure, another catches it.

## Error Notification: onDriverError()

[Lines 1244–1254](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L1244-L1254) — thread-safe error signaling:

\lstset{language=C}
```c
static void onDriverError (DvrInfo *dp)
{
    logMessage ("Driver %s: reader thread "
        "indicates it's time to restart\n",
        dp->name);
    pthread_mutex_lock (&dp->q_lock);
    dp->err = 1;
    pthread_cond_signal (&dp->go_cond);
    pthread_mutex_unlock (&dp->q_lock);
}
```

**Strength:** Clean mutex + condition variable pattern — no race conditions.

## The Restart Function: restartDvr()

[Lines 1318–1388](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L1318-L1388) — the complete restart procedure:

\lstset{language=C}
```c
/* close down the given driver and restart.
 * N.B. lock restart_lock so no other threads
 * try to use it until ready to go again. */
static void restartDvr (DvrInfo *dp)
{
    /* write-lock while we edit */
    pthread_rwlock_wrlock (&dp->restart_lock);

    /* make sure it's dead, reclaim resources */
    int status, wpid =
        waitpid (dp->pid, &status, 0);
    // ... status analysis ...
    close (dp->efd);
    close (dp->wfd);
    close (dp->rfd);
```

**Strength:** Write-lock prevents any other thread from using the driver during restart.

## restartDvr(): Status Analysis

[Lines 1335–1352](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L1335-L1352) — thorough exit analysis:

\lstset{language=C}
```c
if (WIFEXITED(status)) {
    int es = WEXITSTATUS(status);
    logMessage ("Driver %s: Exit status %d\n",
                dp->name, es);
    if (es == EXITEXFAIL) {
        logMessage ("Exiting because of "
            "hopeless driver: %s\n", dp->name);
        exit(1);
    }
} else if (WIFSIGNALED(status))
    logMessage ("Driver %s: Exit signal %d%s\n",
        dp->name, WTERMSIG(status),
        WCOREDUMP(status) ?
        " (core dumped)" : "");
```

**Weakness:** `exit(1)` kills the **entire indiserver** for one bad driver.

- Violates fault isolation principle
- One driver's `execlp` failure should not bring down all drivers

## restartDvr(): Resource Cleanup

[Lines 1367–1384](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L1367-L1384) — comprehensive cleanup:

\lstset{language=C}
```c
/* free memory and locks */
for (i = 0; i < dp->nsprops; i++)
    free (dp->sprops[i]);
free (dp->sprops);
delLilXML (dp->lp);
decMsg (dp->mp);
pthread_mutex_destroy (&dp->q_lock);
pthread_cond_destroy (&dp->go_cond);
drainMsgs (dp->msgq);
delFQ (dp->msgq);

/* start this driver again */
logMessage ("Driver %s: restart #%d\n",
            dp->name, ++dp->restarts);
startDvr (dp);
```

**Strength:** Thorough cleanup — destroys mutexes, drains queues, frees memory.

**Weakness:** Manual C memory management — potential for leaks on error paths in long-running embedded systems.

## Rate Limiting: RESTARTDT

[Lines 88–89](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L88-L90) — hardcoded constants:

\lstset{language=C}
```c
#define RESTARTDT  10
    /* don't restart a driver sooner than this,
     * seconds */
static char lockout_fn[] = "/tmp/noindi";
    /* do not restart local driver if this exists */
```

## Rate Limiting: Implementation

`startDvr()` — [lines 428–446](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L428-L446):

\lstset{language=C}
```c
static void startDvr (DvrInfo *dp)
{
    time_t now = time(NULL);
    long age = now - dp->start;

    if (age < RESTARTDT) {
        unsigned int sdt = RESTARTDT - age;
        logMessage ("Driver %s: delaying restart "
            "by %d secs, min restart interval "
            "is %d secs\n",
            dp->name, sdt, RESTARTDT);
        ssleep (sdt*1000);
    }
    dp->start = time(NULL);
```

**Strength:** Prevents rapid crash-loop storms.

## Rate Limiting: Weaknesses

**Weakness 1: Fixed interval, no backoff**

- A permanently-failing driver restarts every 10 seconds — **forever**
- No exponential backoff (10s, 20s, 40s, ..., cap)

. . .

**Weakness 2: No restart count limit**

- `dp->restarts` is tracked (line 164) but **never bounds-checked**
- A driver can restart thousands of times without intervention

. . .

**Weakness 3: Blocking sleep**

- `ssleep(sdt*1000)` blocks the thread entirely during the delay
- Prevents other operations on that thread

## The Lockout File: /tmp/noindi

[Lines 463–468](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L463-L468) — external restart suppression:

\lstset{language=C}
```c
/* wait while lockout file exists */
while (!ignore_lockout &&
       (fp = fopen(lockout_fn, "r")) != NULL) {
    fclose (fp);
    logMessage ("Sleeping %d secs because "
                "%s exists\n", RDRTIME, lockout_fn);
    ssleep (RDRTIME*1000);
}
```

**Strength:** Allows operators to suppress restarts during maintenance.

## Lockout File: Weaknesses

**Weakness 1: Location in `/tmp`**

- tmpfs may be cleared on reboot
- Lockout does not survive power cycles

. . .

**Weakness 2: Race condition**

- File checked in a loop with 2-second sleep
- If file is created/removed between check and `fopen`, behavior is inconsistent

. . .

**Weakness 3: No atomic check**

- Uses `fopen`/`fclose` repeatedly — not an atomic test-and-hold

## Driver Startup: Fork and Exec

`startLocalDvr()` — [lines 478–497](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L478-L497):

\lstset{language=C}
```c
/* fork&exec new process, connect pipes */
pid = fork();
if (pid == 0) {
    /* child: exec name */
    dup2 (wp[0], 0); /* stdin reads from wp[0] */
    dup2 (rp[1], 1); /* stdout writes to rp[1] */
    dup2 (ep[1], 2); /* stderr writes to ep[1] */
    for (fd = 3; fd < 100; fd++)
        (void) close (fd);

    /* go -- should never return */
    execlp (dp->name, dp->name, NULL);
    _exit (EXITEXFAIL);
    /* parent will notice EOF shortly */
}
```

**Strength:** Clean FD hygiene — closes all FDs > 2 before exec.

**Weakness:** Hardcoded `fd < 100` limit — may miss open FDs above 100.

# Layer 2: INDI Restart Resilience

## Client-Side: Surviving indiserver Restarts

[`MagAOXApp.hpp` line 2798](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L2798-L2799) — transparent re-registration:

\lstset{language=C++}
```cpp
// This is a possible INDI server restart,
// so we re-register for all notifications.
sendGetPropertySetList(true);
```

And in the main loop — [lines 1541–1545](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L1541-L1545):

\lstset{language=C++}
```cpp
// Checkup on the INDI properties we're monitoring.
// This will make sure we are up-to-date if
// indiserver restarts without us.
// And handles cases where we miss a Def because
// the other driver wasn't started up
// when we sent our Get.
sendGetPropertySetList(false);
```

**Strength:** Applications automatically re-synchronize after indiserver restart.

# Layer 2: xindiserver Wrapper

## xindiserver Architecture

[`utils/xindiserver/xindiserver.hpp`](https://github.com/uasal/XWCToolkit/blob/main/utils/xindiserver/xindiserver.hpp#L116-L148) lines 116–148:

\lstset{language=C++}
```cpp
class xindiserver : public MagAOXApp<false>
{
   // indiserver configuration passthrough
   int indiserver_m {-1};
   ///< The indiserver MB behind setting
   bool indiserver_n {false};
   ///< The indiserver ignore /tmp/noindi flag
   int indiserver_p {-1};
   ///< The indiserver port
   int indiserver_v {-1};
   ///< The indiserver verbosity

   pid_t m_isPID {0};
   ///< The PID of the indiserver process
   int m_isSTDERR {-1};
   ///< The output of stderr of the indiserver
   std::thread m_isLogThread;
   ///< A separate thread for capturing logs
};
```

**Strength:** Wraps C indiserver in the MagAOXApp lifecycle with structured logging.

## The Control FIFO

[`MagAOXApp.hpp` lines 584–587](https://github.com/uasal/XWCToolkit/blob/main/XWCTK/app/MagAOXApp.hpp#L584-L587) — restart signaling channel:

\lstset{language=C++}
```cpp
/// Full path name of the INDI driver control FIFO.
/** This is currently only used to signal restarts.
  */
std::string m_driverCtrlName;
```

**Weakness:** The comment says *"currently only used to signal restarts"* — but the implementation of this signaling mechanism is **incomplete** in the toolkit.

# Layer 3: External Supervisor (Missing)

## What Is Not in the Repository

**No systemd unit files** — search for `systemd`, `Restart=`, `WantedBy` returns zero results.

**No external process supervisor** of any kind:

- No cron-based resurrector
- No custom supervisor daemon
- No container orchestration configs

. . .

The PID file infrastructure **clearly expects** an external supervisor — but none is provided.

## The Gap This Creates

For high-reliability embedded computing:

1. If `MagAOXApp` crashes (SIGKILL, OOM, kernel panic recovery) — **nothing restarts it**
2. If `appLogic()` deadlocks — **nothing detects it** (no heartbeat)
3. After reboot — **no automatic startup** of required processes
4. No watchdog integration (e.g., `/dev/watchdog`, systemd `WatchdogSec=`)

. . .

**This is the single largest gap** for embedded deployment.

# Comprehensive Strengths Summary

## Architectural Strengths

1. **Multi-layered defense in depth** — three independent recovery layers
2. **SIGSEGV/SIGBUS recovery** in shmimMonitor — survives faults that kill normal processes
3. **Automatic driver restart** with rate limiting in indiserver
4. **Thread-safe restart** via rwlocks prevents races during driver replacement
5. **INDI re-synchronization** — apps survive indiserver restarts transparently

## Implementation Strengths

6. **Signal to flag to loop** pattern avoids deadlocks in shutdown
7. **Robust stale PID detection** — checks both PID and command-line match
8. **Log deduplication** via `stateLogged()` prevents log flooding
9. **Lockout file** provides operational safety valve
10. **Clean FD hygiene** — closes all FDs before exec in child
11. **Ordered shutdown** — resources released in correct dependency order
12. **Graduated state model** — `ERROR` vs `FAILURE` allows self-healing

# Comprehensive Weaknesses Summary

## Critical Weaknesses

| ID | Weakness | Location |
|----|----------|----------|
| 1 | **No heartbeat/watchdog** | `MagAOXApp.hpp:1536` |
| 2 | **No external supervisor** | (absent from repo) |
| 3 | **`m_shutdown` not atomic** | `MagAOXApp.hpp:102` |

## High Severity Weaknesses

| ID | Weakness | Location |
|----|----------|----------|
| 4 | No exponential backoff | `indiserver.c:434` |
| 5 | No restart count limit | `indiserver.c:164` |
| 6 | `exit(1)` on exec failure | `indiserver.c:1340` |

## Medium Severity Weaknesses

| ID | Weakness | Location |
|----|----------|----------|
| 7 | No post-restart health check | `indiserver.c:1384` |
| 8 | Lockout file in `/tmp` | `indiserver.c:90` |
| 9 | Incomplete control FIFO | `MagAOXApp.hpp:585` |
| 10 | Manual C memory mgmt | `indiserver.c:1367` |
| 11 | Hardcoded FD limit | `indiserver.c:490` |

# Embedded Computing Specific Concerns

## Memory Safety

The C-based indiserver uses manual `malloc`/`free` throughout `restartDvr()`:

\lstset{language=C}
```c
for (i = 0; i < dp->nsprops; i++)
    free (dp->sprops[i]);
free (dp->sprops);
```

In long-running embedded systems, memory leaks from error paths during restart could accumulate and cause OOM failures over days/weeks.

## Determinism

- `sleep()` calls in rate limiting are **non-deterministic**
- For real-time embedded targets, should use deadline-based waiting
- `ssleep(sdt*1000)` in `startDvr()` blocks the thread entirely

## Resource Exhaustion Risk

- No limits on open file descriptors during restart loops
- No thread count bounds
- A misbehaving driver crashing rapidly could exhaust system resources **before** the 10-second rate limiter kicks in

## Fault Containment Violation

The `exit(1)` in `restartDvr()` — [line 1340](https://github.com/uasal/XWCToolkit/blob/main/lib/INDI/INDI/indiserver.c#L1340):

\lstset{language=C}
```c
if (es == EXITEXFAIL) {
    logMessage ("Exiting because of "
        "hopeless driver: %s\n", dp->name);
    exit(1);
}
```

**Violates the principle of fault isolation** — one driver's failure to exec brings down the entire supervision layer and all other healthy drivers.

# Recommendations

## For High-Reliability Embedded Deployment

1. **Implement the heartbeat** (the TODO at line 1536)
   - Add `WatchdogSec=` integration for systemd
   - Or hardware watchdog `/dev/watchdog` support

2. **Ship systemd unit files** with `Restart=always`

3. **Make `m_shutdown` atomic**: `std::atomic<int> m_shutdown{0};`

4. **Add exponential backoff** to driver restarts: 10s, 20s, 40s, cap at 5min

5. **Add restart count limit** — after N failures, stop and alert

6. **Replace `exit(1)` in restartDvr()** — isolate driver failure, do not kill the server

7. **Add post-restart health validation** — verify driver sends valid INDI traffic within a timeout

## Summary

XWCToolkit implements a **competent but incomplete** resurrection architecture:

- **Well-engineered** for its original astronomical instrument context
- **Missing critical features** for autonomous embedded deployment:
  - No heartbeat watchdog
  - No external supervisor
  - No exponential backoff
  - Incomplete fault isolation

. . .

The foundation is sound. The gaps are addressable. Closing them would make XWCToolkit suitable for high-reliability, long-duration autonomous embedded systems.

# Appendix: Review Discussion

## Review Item 1 — "All XWCToolkit applications"

| Issue | Proposed Fix | Response |
|-------|-------------|----------|
| The report states *"All XWCToolkit applications derive from `MagAOXApp`"* which is not literally true — utility programs such as `xindidriver`, `xrif2fits`, `instGraph`, and `logdump` do not derive from `MagAOXApp`. | Change to *"Most runtime application daemons in XWCToolkit derive from `MagAOXApp`"*. | **Minor correction.** In the context of this presentation (resurrection architecture for supervised daemons), the original statement is substantially correct — all applications that *participate in the resurrection lifecycle* do derive from `MagAOXApp`. The utilities are out of scope. However, the literal wording is imprecise, and a clarifying qualifier would improve accuracy. |

## Review Item 2 — Historical TODO claim

| Issue | Proposed Fix | Response |
|-------|-------------|----------|
| The report states *"This TODO has been present since the original MagAO-X codebase"* as fact, but the repository has only one commit and the claim cannot be verified from git history. | Change to *"This TODO appears to be longstanding, likely inherited from the original MagAO-X codebase"*. | **Minor correction.** The TODO's `\todo` doxygen style and the comment's wording are consistent with longstanding code, and XWCToolkit is publicly documented as a fork of MagAO-X. The original statement is a reasonable inference, but softening it to "appears to be" is more rigorous since the git history does not extend back far enough to prove the claim. |

## Review Item 3 — "safe handler" self-contradiction

| Issue | Proposed Fix | Response |
|-------|-------------|----------|
| The heading says *"minimal, safe handler"* but the body two lines below notes that `std::cerr` and `log<text_log>()` are *"technically not async-signal-safe"*, creating an internal contradiction. | Change heading to *"minimal but not fully async-signal-safe handler"*. | **Requiring correction.** The report contradicts itself — the heading calls the handler "safe" while the weakness paragraph correctly identifies that `std::string` allocation, `std::cerr`, and logging in a signal handler are not async-signal-safe per POSIX. The heading should be updated to be consistent with the body's own analysis. |

## Review Item 4 — "incomplete" control FIFO

| Issue | Proposed Fix | Response |
|-------|-------------|----------|
| The report states the control FIFO signaling mechanism is *"incomplete in the toolkit"*. Review of `indiDriver.hpp:156–176`, `xindidriver.cpp:281–310`, and `MagAOXApp.hpp:2614` shows the mechanism is fully implemented: the FIFO is created, written to on error, read by `ctrlThread`, and triggers a process exit allowing `indiserver` to restart the driver. | Change to *"limited to restart signaling only"* with cross-references to the implementing files. | **Misunderstanding by reviewing agent.** The mechanism is not incomplete — it is functional and works as designed. The original source comment *"currently only used to signal restarts"* describes a deliberate scope limitation, not a missing implementation. The word "incomplete" mischaracterizes working code. |

## Review Item 5 — Self-referential systemd search claim

| Issue | Proposed Fix | Response |
|-------|-------------|----------|
| The report states *"search for `systemd`, `Restart=`, `WantedBy` returns zero results"* but the presentation document itself contains `systemd` five times, making the claim self-referentially false. | Qualify the claim: *"no `.service` files exist in the repository, and no source files outside this document reference `systemd`, `Restart=`, or `WantedBy`."* | **Minor correction.** The intent of the claim is correct — no actual systemd integration exists in the source code. However, the literal wording is inaccurate because the document making the claim is itself a search result. A small qualification resolves the issue without changing the substance. |

## Review Item 6 — Summary table "Incomplete control FIFO"

| Issue | Proposed Fix | Response |
|-------|-------------|----------|
| Row 9 of the weakness summary table says *"Incomplete control FIFO"* at `MagAOXApp.hpp:585`, repeating the same mischaracterization as Item 4. | Change to *"Control FIFO limited to restart signaling"*. | **Misunderstanding by reviewing agent.** Same as Item 4 — the FIFO mechanism is complete and functional. The summary table should reflect the actual scope limitation, not imply missing implementation. |

## Disclaimer

\begin{center}
\textbf{Important Notice}
\end{center}

This entire analysis — including the original resurrector report, the subsequent review, and the review responses above — was produced through a discussion between LLM (Large Language Model) agents.

While the agents have inspected the actual source code and attempted to ground all claims in specific file locations and line numbers, **LLM agents may not have the full context** of the project's:

- Design intent and architectural rationale
- Deployment environment and operational procedures
- Historical decisions and trade-offs
- External documentation, wikis, or institutional knowledge not present in the repository

All findings and assessments should be reviewed by domain experts familiar with the XWCToolkit and its target deployment environment before being acted upon.
