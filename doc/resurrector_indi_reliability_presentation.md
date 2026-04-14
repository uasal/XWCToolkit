---
title: "resurrector\\_indi: Hexbeat-Based Process Resurrection"
subtitle: "How resurrector\\_indi Changes System Reliability in MagAO-X"
author: "Deep Code Analysis"
date: \today
institute: "magao-x/MagAOX — dev-resurrector branch"
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

# Overview

## What Is resurrector\_indi?

- A **standalone process supervisor** for the MagAO-X INDI framework
- Located on the [`dev-resurrector`](https://github.com/magao-x/MagAOX/tree/dev-resurrector/utils/resurrector_indi) branch of `magao-x/MagAOX`
- Designed to **replace the `xctrl` Python script** with a compiled C++ daemon

From the [README](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/README.md):

> *"The resurrector program ensures all processes in a MagAO-X INDI framework, comprising one INDI server process and multiple INDI driver processes, will start up and continue both to communicate and to run even if some of them fail by either exiting, crashing, or locking up."*

## The Problem It Solves

From the XWCToolkit analysis (predecessor presentation):

- **No heartbeat/watchdog** — `MagAOXApp.hpp` line 1536: `/** \todo Need a heartbeat update here. */`
- **No external supervisor** — no systemd unit files, no process restarter
- **No lockup detection** — if `appLogic()` deadlocks, nothing detects it

. . .

**resurrector\_indi directly addresses all three gaps.**

## Architecture Diagram

From the [README](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/README.md):

\lstset{language={}}
```
+------------------+
|                  | <-- proclist_role.txt
| resurrector_indi | <-- /path/to/fifos/drivername.hb
|                  | ...fork(2)...> [INDI driver]
+------------------+       |
   .        ^              v
 fork(2)    |        +-------------+---------+
   .    isXXX.hb     | resurrectee |         |
   v        |        +-------------+         |
+----+-------------+ |    INDI     | Device  |
|    | resurrectee | | DRIVER      | Ctrl    |
|    +-------------+ +-------------+---------+
|    indiserver    |
+------------------+
```

## Key Source Files

| File | Role | Size |
|------|------|------|
| [`HexbeatMonitor.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp) | Per-process heartbeat monitor | 40 KB |
| [`resurrector.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector.hpp) | Orchestrates all monitors | 10 KB |
| [`resurrector_indi.cpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector_indi.cpp) | Main program — MagAO-X specific | 13 KB |
| [`resurrector_indi.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector_indi.hpp) | Config parsing and CLI | 8 KB |
| [`resuctrl`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resuctrl) | Bash control script (start/stop/status) | Shell |

# Core Concept: The Hexbeat

## What Is a Hexbeat?

From [`HexbeatMonitor.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp#L52-L58) lines 52–58:

\lstset{language=C++}
```cpp
/// The heartbeat (hexbeat) itself is a 9-digit
/// newline-terminated hexadecimal string (9dnths),
/// representing a time value (Note 1)
/// after which the process is considered to have missed
/// a (heart-)beat, and either is locked up or has died
/// or otherwise exited.
///
/// Note 1: s past the Epoch, cf. time(2) in man pages
```

**Key insight:** Each hexbeat encodes a *future* deadline — not just "I'm alive" but "I expect to still be alive until time X."

## Hexbeat Construction: time\_to\_hb()

[`HexbeatMonitor.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp#L28-L46) lines 28–46:

\lstset{language=C++}
```cpp
static inline std::string time_to_hb(int delay)
{
    char c20[20];
    if (delay<0)
    {
        // If delay is negative, use maximum 9dnths
        sprintf(c20,"%1.1lx\n",0xfL);
        for (int i=1; i<9; ++i) { c20[i] = *c20; }
        c20[9] = '\n';
    }
    else
    {
        // Build delay string from current time
        // and non-negative delay
        sprintf(c20,"%9.9lx\n",time(0) + delay);
    }
    return std::string{c20};
}
```

**Strength:** Negative delay creates `fffffffff\n` — a "never expire" sentinel for processes that don't send heartbeats.

## Why "Hexbeat" Instead of a Simple Heartbeat?

**Conventional heartbeat:** Supervisor sets the timeout. *"Ping me within N seconds or I restart you."*

**Hexbeat:** The **process itself** sets its own deadline. *"I will be alive until time X."*

. . .

From [`resurrector.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector.hpp#L83-L93) lines 83–93 — the comment in `srcr_cycle`:

\lstset{language=C++}
```cpp
// Make current time a hexbeat to compare with incoming
// hexbeats
// - The 0 is the offset from the current time
// - Incoming hexbeats will have a positive offset
//   relative to the time they are created; that offset
//   will be in effect a timeout for the hexbeater process
//   that sends it.  By using this approach, the timeout
//   can be specific to the hexbeater process, and this
//   resurrector process does not need to keep track of
//   the individual hexbeaters' timeouts.
```

**Reliability improvement:** Per-process timeout granularity without centralized configuration.

# The HexbeatMonitor: Per-Process Supervision

## Three Operational States

From [`HexbeatMonitor.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp#L67-L69) lines 67–69:

\lstset{language=C++}
```cpp
/// An instance of this class is in one of three
/// operational states:
/// 1) inactive - available to be used for a new hexbeat
/// 2) opened - file (FIFO) open for hexbeat,
///             but no known hexbeater
/// 3) started - file (FIFO) open for hexbeat,
///              known hexbeater process
```

And the public interfaces ([lines 76–83](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp#L76-L83)):

\lstset{language=C++}
```cpp
///  open_hexbeater - Open FIFO, initialize instance
/// start_hexbeater - Find or fork a hexbeater process
///    late_hexbeat - Check if latest hexbeat has expired
///  read_hexbeater - Read data from FD, parse hexbeat
///  stop_hexbeater - Stop a hexbeater process
/// close_hexbeater - Stop running process, close FIFO
/// find_hexbeater_pid - Find PID by executable and name
/// update_restart_check_expiry - Restart logic
```

## Late Hexbeat Detection

[`HexbeatMonitor.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp#L361-L374) lines 361–374:

\lstset{language=C++}
```cpp
bool
late_hexbeat(const std::string& hbnow)
{
    // If process is active, AND select is enabled,
    // AND current hexbeat argument exceeds last
    // hexbeat received, then hexbeat has expired
    return m_fd > -1 && m_sel && (hbnow > m_last_hb);
}
```

**Strength:** Detection is a single string comparison — `O(9)` constant time.

**Strength:** Lexicographic comparison of hex strings correctly orders time values.

**Reliability improvement:** Detects both **crashes** (process dies, no new hexbeats) and **lockups** (process alive but stuck, hexbeat not updated).

## SIGCHLD Handling: Zombie Prevention

[`HexbeatMonitor.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp#L640-L652) lines 640–652:

\lstset{language=C++}
```cpp
/// SIGCHLD signal handler
/** Ignores SIGCHLD signals while preventing zombies
  */
static void
hbm_sigchld_handler(int sig)
{
    static_cast<void>(sig);
    int saved_errno = errno;
    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}
    errno = saved_errno;
}
```

And the handler installation ([lines 659–677](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp#L659-L677)):

\lstset{language=C++}
```cpp
sa.sa_flags = SA_RESTART | SA_NOCLDSTOP | SA_NOCLDWAIT;
```

**Strength:** `SA_RESTART` — system calls interrupted by SIGCHLD are automatically restarted.

**Strength:** `SA_NOCLDWAIT` — prevents zombie accumulation even if `waitpid` misses one.

**Strength:** Saves/restores `errno` — proper async-signal-safe practice.

## Process Discovery via /proc

[`HexbeatMonitor.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp#L397-L460) lines 397–460 — `find_hexbeater_pid()`:

\lstset{language=C++}
```cpp
static int
find_hexbeater_pid(const std::string& argv0,
                   const std::string& hexbeater_name)
{
    // Open the /proc/ directory
    DIR* pdir;
    pdir = opendir("/proc");
    // Loop over entries in the /proc/ directory
    while ((de=readdir(pdir)) != NULL)
    {
        // Only look at entries that are directories
        // with names comprising digit characters
        if (de->d_type != DT_DIR) { continue; }
        // Open /proc/<pid>/cmdline file ...
        // Match argv0 and -n hexbeater_name
    }
}
```

**Reliability improvement:** On startup or SIGUSR2, the resurrector **discovers already-running processes** rather than blindly forking duplicates.

## Fork and Exec: Starting a Process

[`HexbeatMonitor.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp#L814-L868) lines 814–868 — `fork_hexbeater()`:

\lstset{language=C++}
```cpp
int
fork_hexbeater(int delay, bool check_existing)
{
    // If directed to check for a hexbeater ...
    // and such a hexbeater is found, return its PID
    int pid{check_existing ? find_hexbeater_pid() : 0};
    if (pid > 0) { return pid; };

    // ... Otherwise fork, ...
    pid = fork();
    if (pid != 0)
    {
        // Parent: initialize last hexbeat with delay
        if (pid > 0) {
            m_last_hb = time_to_hb(m_init_delay);
        }
        return pid;
    }

    // Child fork: set new process group
    int ipgstat = setpgid(0,0);
    // ... output redirection ...
    // ... And then exec
    int e = execlp(argv0, argv0, "-n", name, (char*)NULL);
}
```

**Strength:** `setpgid(0,0)` — child starts a **new process group**, preventing signal leakage from parent.

**Strength:** `check_existing` flag enables idempotent startup — won't duplicate a running process.

# The Resurrector: Orchestrating Recovery

## The Select/Read/Check/Restart Cycle

[`resurrector.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector.hpp#L80-L159) — `srcr_cycle()` (the core loop):

\lstset{language=C++}
```cpp
void srcr_cycle(struct timeval& tv)
{
    std::string hbnow{time_to_hb(0)};

    // Copy fd_set, call select(2) for ready FDs
    fd_set lcl_fdset{m_fdset_cpy};
    int iselect = select(m_nfds, &lcl_fdset,0,0, &tv);

    // Loop on the active FDs
    for (auto it = m_hbmarr.begin();
         it != m_hbmarr.end(); ++it)
    {
        it->read_hexbeater(lcl_fdset);
        bool islate = it->late_hexbeat(hbnow);
        if (!islate) { continue; }

        // Hexbeat expired => stop and restart
        if (it->update_restart_check_expiry())
        {
            // Restart limit exceeded: give up
            it->close_hexbeater(m_fdset_cpy, m_nfds);
            continue;
        }
        it->stop_hexbeater(m_fdset_cpy, m_nfds);
        // Brief pause
        struct timeval tv{0,99999};
        select(0, 0,0,0, &tv);
        it->start_hexbeater(m_fdset_cpy, m_nfds,
                            it->init_dly(), false);
    }
}
```

**Reliability improvement:** Single-threaded `select(2)` loop — no mutexes, no deadlocks, deterministic behavior.

## Restart Count Limiting

[`HexbeatMonitor.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp#L489-L493) lines 489–493:

\lstset{language=C++}
```cpp
bool
update_restart_check_expiry()
{
    // Increment restart parameter, return true
    // after Nth restart
    return ++m_restart > m_restart_max
        && m_restart_max != 0;
}
```

And the attributes ([lines 507–509](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp#L507-L509)):

\lstset{language=C++}
```cpp
int m_restart{0};     ///< Accumulated restart parameter
int m_restart_max{0}; ///< Max Arp; 0 => infinity
```

**Reliability improvement:** Directly addresses indiserver weakness #5 ("No restart count limit") from the predecessor analysis.

**Strength:** `m_restart_max == 0` means unlimited restarts — configurable per-process.

## Giving Up: Expiry Message

[`resurrector.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector.hpp#L134-L140) lines 134–140:

\lstset{language=C++}
```cpp
if (it->update_restart_check_expiry())
{
    // If the restart parameter has expired,
    // issue a message and close that hexbeater
    // FIFO, which also stops further restarts
    std::cerr << "[resurrector giving up on "
              << *it << " at "
              << time_to_hb(0).substr(0,9) << "]\n";
    it->close_hexbeater(m_fdset_cpy, m_nfds);
    continue;
}
```

**Strength:** Permanently failing processes don't consume resources indefinitely.

**Contrast with indiserver:** The C indiserver restarts forever. resurrector\_indi stops after N attempts.

# MagAO-X Integration: resurrector\_indi.cpp

## The Main Program Flow

[`resurrector_indi.cpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector_indi.cpp#L88-L108) lines 88–108:

\lstset{language=C++}
```cpp
int
main(int argc, char** argv)
{
    // Get MagAOX role and build process list path
    std::string proclist_role =
        get_magaox_proclist_role(argc, argv);

    resurrectorT<> resurr(
        nor ? nullptr : &stdout_stderr_redirect);

    if (logging) { resurr.set_resurr_logging(); }
    if (verbose) {
        resurr.set_resurr_verbose_logging();
    }

    setup_SIGUSR12_handler(SIGUSR1);
    setup_SIGUSR12_handler(SIGUSR2);

    do { /* main loop */ } while (no_SIGUSR1_yet);
}
```

**Strength:** Clean separation — the template `resurrectorT<>` is generic; MagAO-X specifics are in this file.

## SIGUSR2: Live Reconfiguration

[`resurrector_indi.cpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector_indi.cpp#L262-L273) lines 262–273:

\lstset{language=C++}
```cpp
// Run the select/read/check/restart cycle
// Exit loop when either SIGUSR1 or SIGUSR2 received,
// EITHER to re-read the proclist for SIGUSR2,
// OR to stop all children for SIGUSR1 and then exit
do
{
    struct timeval tv{1,0};
    resurr.srcr_cycle(tv);
} while (no_SIGUSR2_yet && no_SIGUSR1_yet);
```

From the [README](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/README.md):

> *"If the resurrector\_indi process receives a SIGUSR2 signal, it then re-parses the process list configuration file, kills any children processes that were previously started but are no longer in the configuration process list, starts any new children processes, and restarts any old children processes **that are not currently running**."*

**Reliability improvement:** Process list changes applied without stopping the supervisor — zero-downtime reconfiguration.

## Differential Reconfiguration: Three-Phase Close

[`resurrector_indi.cpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector_indi.cpp#L129-L152) lines 129–152:

\lstset{language=C++}
```cpp
// Close any HexbeatMonitors not in the proclist file

// (1) Assume all HBMs are to be closed
resurr.pending_close_all_set(true);

// (2) Parse proclist, reverse that assumption for
//     each HBM still in the proclist
while (EOF != (rnp=read_next_process(
                     f,driver_name,exec,dpfx)))
{
    if (2 != rnp) { continue; }
    argv0 = IRMAGAOX_bin + std::string("/") + exec;
    resurr.pending_close_all_set_on_match(
        false, argv0, driver_name);
}

// (3) Close any HBM that was not in the proclist
resurr.pending_close_all_close();
```

**Strength:** Mark-and-sweep pattern — clean, no races, O(N) for N processes.

**Reliability improvement:** Removed processes are stopped; added processes are started; unchanged processes are **left running**.

## Startup Ordering: sshDigger, Server, Drivers

[`resurrector_indi.cpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector_indi.cpp#L217-L252) lines 217–252:

\lstset{language=C++}
```cpp
// sshDigger (non-INDI) drivers: start immediately
if (exec=="sshDigger")
{
    resurr.start_hexbeater(newfd,10);
    sshDiggers = true;
    continue;
}

// INDI drivers: delayed start (accumulate FDs)
if (driver_name.substr(0,2)!="is")
{
    fd_indidrivers.push_back(newfd);
    continue;
}

// INDI server: start now, then 5s delay
resurr.start_hexbeater(isfd,10);
std::cerr << "Delay 5s after starting INDI server"
          << std::endl;
timeval tv = {5,0};
select(1,0,0,0,&tv);

// Then start all INDI drivers
for (auto fd : fd_indidrivers)
{
    resurr.start_hexbeater(fd,10);
}
```

**Reliability improvement:** Ordered startup prevents race conditions — network tunnels first, then server, then drivers.

## Process Prefix System

[`resurrector_indi.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector_indi.hpp#L51-L65) lines 51–65 — `read_next_process()`:

\lstset{language=C++}
```cpp
if ('-'==*argname)
{
    // prefix of "-" : do not manage heartbeats
    name = std::string(argname+1);
    prefix = std::string("-");
}
else if ((p=strchr(argname,':')))
{
    // prefix of "...:" e.g. "py:" or "nhb:"
    name = std::string(p);
    prefix = std::string(argname, (size_t)(p-argname));
}
```

And the delay logic ([`resurrector_indi.cpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector_indi.cpp#L196-L202) lines 196–202):

\lstset{language=C++}
```cpp
int init_delay = 10;
if      (dpfx=="-")    { init_delay = -1; }
else if (dpfx=="nhb:") { init_delay = -1; }
else if (dpfx=="py:")  { init_delay = -1; }
else                   { init_delay = 10; }
```

**Reliability improvement:** Processes that cannot send hexbeats (Python scripts, non-hexbeat apps) get `init_delay = -1` → `fffffffff` (never-expire sentinel), preventing false-positive restarts.

# Output Redirection and Logging

## stdout/stderr Capture

[`redirect_prototype.cpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/redirect_prototype.cpp#L161-L180) — `stdout_stderr_redirect()`:

\lstset{language=C++}
```cpp
void
stdout_stderr_redirect(std::string devicename)
{
    // Build /opt/MagAOX/sys/devicename dir path
    fullpath = MagAOXPathUtil()
               .make_sys_device_dirs(0644, devicename)
             + "/outputs";
    // Open file, redirect STDOUT and STDERR
    fstdxxx = open(fullpath.c_str(), fopts, 0644);
    dup2(fstdxxx, STDOUT_FILENO);
    dup2(fstdxxx, STDERR_FILENO);
    close(fstdxxx);
}
```

**Reliability improvement:** Each child's output is captured to `/opt/MagAOX/sys/<devicename>/outputs` — crash diagnostics are preserved even after restart.

## Output Redirect Integration

[`HexbeatMonitor.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp#L849-L852) lines 849–852, inside `fork_hexbeater()`:

\lstset{language=C++}
```cpp
if (m_output_redirect)
{
    errno = 0;
    m_output_redirect(m_hbname);
}
```

**Strength:** Redirect is a function pointer — pluggable and testable.

**Strength:** Can be disabled with `--no-output-redirect` for interactive debugging.

# The resuctrl Control Script

## Startup, Shutdown, Restart

[`resuctrl`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resuctrl) — Bash control script for the resurrector:

\lstset{language=bash}
```bash
### Startup: if resurrector_indi is not running,
### and config file is readable, then start
function verb_startup {
  ripid=$(resurrector_indi_pid) \
  && resu_failure "resurrector_indi already running
      PID{$ripid}; exiting ..." \
  && return 1

  proclist_txt="$(proclist_path readonly)" \
  || resu_failure "Readable process list file
      not found; exiting ..." \
  || return 1

  command="$(find_resurrector_indi_path)
           -r $ROLE $CACHESUFFIXargs" \
  $command &
}
```

\lstset{language=bash}
```bash
### Shutdown: signal via SIGUSR1 to trigger
### killing all INDI driver devices before exiting
function verb_shutdown {
  kill -USR1 $(resurrector_indi_pid)
}
```

**Reliability improvement:** Operator-friendly interface parallel to `xctrl` but controlling the compiled resurrector.

# Reliability Comparison: Before and After

## What XWCToolkit Had Before

| Capability | Status | Source |
|-----------|--------|--------|
| Heartbeat/watchdog | **Missing** (TODO) | `MagAOXApp.hpp:1536` |
| External process supervisor | **Missing** | (absent from repo) |
| Lockup detection | **Missing** | — |
| Restart count limit | **Missing** | `indiserver.c:164` |
| Exponential backoff | **Missing** | `indiserver.c:434` |
| Process discovery on start | **Missing** | — |
| Live reconfiguration | **Missing** | — |
| Per-process timeout | **Missing** | — |

## What resurrector\_indi Adds

| Capability | Status | Source |
|-----------|--------|--------|
| Heartbeat/watchdog | **Implemented** (Hexbeat) | `HexbeatMonitor.hpp:52–58` |
| External process supervisor | **Implemented** | `resurrector_indi.cpp` |
| Lockup detection | **Implemented** | `late_hexbeat()` at line 361 |
| Restart count limit | **Implemented** | `update_restart_check_expiry()` at line 489 |
| Process discovery on start | **Implemented** | `find_hexbeater_pid()` at line 397 |
| Live reconfiguration | **Implemented** (SIGUSR2) | `resurrector_indi.cpp:262` |
| Per-process timeout | **Implemented** | Hexbeat offset per process |
| Output capture | **Implemented** | `redirect_prototype.cpp` |

## Reliability Improvements Quantified

**Gap closure:**

1. **Heartbeat TODO → Hexbeat system** — The `\todo Need a heartbeat update here` at `MagAOXApp.hpp:1536` is resolved by the hexbeat FIFO protocol
2. **No supervisor → Full supervisor** — `resurrector_indi` is a dedicated process manager with fork/exec/monitor
3. **No lockup detection → Timeout-based detection** — `late_hexbeat()` catches both crashes and hangs
4. **Unlimited restarts → Bounded restarts** — `m_restart_max` prevents thrashing

. . .

**New capabilities not present in any XWCToolkit layer:**

5. **Live process list reconfiguration** via SIGUSR2
6. **Process discovery** — finds already-running instances via `/proc`
7. **Per-process timeout granularity** — each process sets its own deadline
8. **Ordered startup** — network → server → drivers

# Strengths of resurrector\_indi

## Architectural Strengths

1. **Single-threaded `select(2)` loop** — no mutexes, no deadlocks, predictable latency
2. **Template-based design** (`resurrectorT<>`) — generic core separates from MagAO-X specifics
3. **FIFO-based IPC** — simple, filesystem-visible, debuggable with `echo 000000000 >> name.hb`
4. **Process group isolation** — `setpgid(0,0)` in forked children prevents signal leakage
5. **Idempotent startup** — `check_existing` + `/proc` scanning prevents duplicate processes
6. **Differential reconfiguration** — mark-and-sweep pattern on SIGUSR2

## Implementation Strengths

7. **Async-signal-safe SIGCHLD handler** — saves/restores errno, uses `waitpid(WNOHANG)`
8. **SA\_RESTART flag** — interrupted system calls auto-retry
9. **Configurable restart limits** — 0 for unlimited, N for bounded
10. **Output capture per process** — crash logs preserved in `/opt/MagAOX/sys/<name>/outputs`
11. **Process name validation** — rejects driver names with invalid characters ([`resurrector_indi.hpp:74–83`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector_indi.hpp#L74-L83))
12. **Negative-delay sentinel** — processes that can't heartbeat (`py:`, `nhb:`, `-` prefix) never expire

# Weaknesses and Concerns

## No Exponential Backoff

The restart cycle in [`resurrector.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector.hpp#L143-L159) uses a fixed 0.1s pause:

\lstset{language=C++}
```cpp
it->stop_hexbeater(m_fdset_cpy, m_nfds);
struct timeval tv{0,99999};
select(0, 0,0,0, &tv);
it->start_hexbeater(m_fdset_cpy, m_nfds,
                    it->init_dly(), false);
```

**Weakness:** A rapidly-crashing process will be restarted every ~1 second (the `select` loop period plus 0.1s) until `m_restart_max` is hit. No backoff: 1s, 1s, 1s...

**Recommendation:** Add exponential backoff: 1s, 2s, 4s, ... capped at a configurable maximum.

## FD\_SETSIZE Limitation

[`resurrector.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector.hpp#L8) line 8:

\lstset{language=C++}
```cpp
template <bool _HBM_SIGCHLD_HNDLR = true,
          int HBR_FD_SETSIZE = MACRO_FD_SETSIZE>
class resurrectorT
```

The `m_hbmarr` vector is sized to `HBR_FD_SETSIZE` (typically 1024):

\lstset{language=C++}
```cpp
std::vector<HexbeatMonitor<_HBM_SIGCHLD_HNDLR>>
    m_hbmarr{std::vector<...>(HBR_FD_SETSIZE)};
```

**Weakness:** Pre-allocates 1024 `HexbeatMonitor` objects. On a system with large FD\_SETSIZE, this wastes significant memory.

**Weakness:** Uses `select(2)` instead of `poll(2)` or `epoll` — limited to FD\_SETSIZE file descriptors.

## No Health Check After Restart

After forking a new process, there is no verification that it actually starts sending hexbeats:

\lstset{language=C++}
```cpp
// In fork_hexbeater():
if (pid > 0) { m_last_hb = time_to_hb(m_init_delay); }
```

The initial delay (10 seconds) is the only grace period. If the process starts but cannot initialize within 10 seconds, it will be killed and restarted.

**Recommendation:** Add configurable initial delay per process based on expected startup time.

## Signal Handler Uses Non-Async-Signal-Safe Functions

[`resurrector_indi.cpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/resurrector_indi.cpp#L40-L49) lines 40–49:

\lstset{language=C++}
```cpp
void
sigusr12_handler(int sig, siginfo_t *si,
                 void *unused)
{
    if (logging)
    {
        std::cerr << "Received signal["
                  << strerror(sig) << "]"
                  << std::endl;
    }
    no_SIGUSR1_yet = sig==SIGUSR1 ? false
                                  : no_SIGUSR1_yet;
}
```

**Weakness:** `std::cerr`, `strerror()`, and `std::endl` are **not async-signal-safe** per POSIX.

**Weakness:** `strerror(sig)` is called with a signal number, but `strerror()` expects an `errno` value — `strsignal(sig)` should be used instead. This is a bug in the source code.

**Weakness:** `no_SIGUSR1_yet` and `no_SIGUSR2_yet` are plain `bool` — should be `volatile sig_atomic_t`.

## Stop Signal Is SIGUSR2, Not SIGTERM

[`HexbeatMonitor.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/HexbeatMonitor.hpp#L295-L300) lines 295–300:

\lstset{language=C++}
```cpp
int pid = find_hexbeater_pid();
// ...
int istatus = kill(pid, SIGUSR2);
```

**Weakness:** Stopping a child process sends `SIGUSR2` — but standard practice is `SIGTERM` then `SIGKILL`. A process that doesn't handle `SIGUSR2` will ignore it.

## No Graceful Shutdown Escalation

When `stop_hexbeater` sends `SIGUSR2`, there is no escalation to `SIGKILL` if the process doesn't exit within a timeout.

**Recommendation:** Send `SIGUSR2` → wait N seconds → send `SIGTERM` → wait → `SIGKILL`.

# Weakness Summary Table

## All Identified Weaknesses

| ID | Weakness | Severity | Location |
|----|----------|----------|----------|
| 1 | No exponential backoff | High | `resurrector.hpp:143–159` |
| 2 | `no_SIGUSR*_yet` not `volatile sig_atomic_t` | High | `resurrector_indi.cpp:34–35` |
| 3 | Signal handler uses non-async-safe functions | Medium | `resurrector_indi.cpp:40–49` |
| 4 | `strerror(sig)` instead of `strsignal(sig)` | Medium | `resurrector_indi.cpp:43` |
| 5 | Stop signal is SIGUSR2 not SIGTERM | Medium | `HexbeatMonitor.hpp:300` |
| 6 | No shutdown escalation (SIGTERM→SIGKILL) | Medium | `HexbeatMonitor.hpp:292–306` |
| 7 | FD\_SETSIZE memory waste / select(2) limit | Low | `resurrector.hpp:8` |
| 8 | No post-restart health check | Low | `HexbeatMonitor.hpp:829` |
| 9 | Fixed 10s initial delay for all C++ processes | Low | `resurrector_indi.cpp:196` |

# Comparison: resurrector\_indi vs xctrl vs indiserver

## Feature Matrix

| Feature | `xctrl` (Python) | `indiserver` (C) | `resurrector_indi` (C++) |
|---------|:-:|:-:|:-:|
| Process startup | ✓ | — | ✓ |
| Process shutdown | ✓ | — | ✓ |
| Heartbeat monitoring | — | — | **✓** |
| Lockup detection | — | — | **✓** |
| Crash detection | — | EOF/XML/write | **Hexbeat timeout** |
| Restart count limit | — | — | **✓** |
| Live reconfiguration | — | — | **✓** (SIGUSR2) |
| Process discovery | — | — | **✓** (`/proc` scan) |
| Output capture | — | pipe-based | **✓** (file redirect) |
| Per-process timeout | — | — | **✓** |
| Implementation | ~400 LOC Python | ~1400 LOC C | ~900 LOC C++ |

## What Remains Unaddressed

1. **systemd integration** — no `.service` file provided
2. **Hardware watchdog** — no `/dev/watchdog` support
3. **`m_shutdown` atomicity** — the `int` vs `std::atomic<int>` issue in `MagAOXApp.hpp` is unrelated to resurrector
4. **Fault isolation in indiserver** — the `exit(1)` on exec failure still exists

# Recommendations

## For Production Deployment

1. **Add exponential backoff** — restart delays of 1s, 2s, 4s, ..., cap at configurable max

2. **Fix signal safety** — use `volatile sig_atomic_t` for `no_SIGUSR*_yet`; remove `std::cerr` from signal handlers

3. **Use SIGTERM for stopping** — standard practice; escalate to SIGKILL after timeout

4. **Ship a systemd unit file** with `Restart=always` for the resurrector itself

5. **Replace `select(2)` with `poll(2)`** — removes FD\_SETSIZE limitation

6. **Add per-process initial delay** — allow proclist to specify startup timeout

## Summary

resurrector\_indi is a **significant reliability upgrade** for the MagAO-X software stack:

- **Closes the three largest gaps** identified in the XWCToolkit analysis
- **Adds capabilities** that no existing layer provides (lockup detection, restart limits, live reconfiguration)
- **Clean, single-threaded design** avoids the complexity of the multi-threaded indiserver restart path

. . .

The weaknesses identified are **addressable engineering items**, not architectural flaws. The hexbeat paradigm — where each process declares its own deadline — is an elegant solution that scales well to heterogeneous process populations.

# Appendix: Manual Hexbeat Injection

## Triggering a Restart from the Command Line

From the [README](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/README.md):

> *"Since each process's Hexbeat FIFO is available in the file system, it is possible to send an expired Hexbeat to resurrector\_indi to trigger a synthetic Hexbeat expiration:"*
>
> `% echo 000000000 >> /opt/MagAOX/drivers/fifos/drivername.hb`
>
> *"After receiving such a Hexbeat telling it that the corresponding driver/server expired several decades ago, resurrector\_indi should stop and restart that process."*

**Strength:** Filesystem-visible, scriptable interface for restart triggering — no special tools needed.

# Appendix: CRC Integrity Module

## generic\_crc.hpp

[`generic_crc.hpp`](https://github.com/magao-x/MagAOX/blob/dev-resurrector/utils/resurrector_indi/generic_crc.hpp) — a configurable CRC implementation:

\lstset{language=C++}
```cpp
class CRC
{
    int32_t poly{0xa001};
    int32_t width{16};
    // ...
    CRC(int32_t poly_arg, int32_t width_arg,
        bool ones, bool do_xorout)
};
```

Present in the source tree but **not yet integrated** into the hexbeat protocol. Could potentially be used for hexbeat integrity checking in future versions.

## Disclaimer

\begin{center}
\textbf{Important Notice}
\end{center}

This analysis was produced by an LLM (Large Language Model) agent by inspecting the source code at the URLs cited. All claims are grounded in specific file locations and line numbers in the `dev-resurrector` branch of `magao-x/MagAOX`.

**LLM agents may not have the full context** of the project's design intent, deployment environment, or institutional knowledge. All findings should be reviewed by domain experts before being acted upon.

Source repository: [`magao-x/MagAOX` `dev-resurrector` branch](https://github.com/magao-x/MagAOX/tree/dev-resurrector/utils/resurrector_indi)
