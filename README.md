# XWCToolkit library and utilities

This is the core library extracted from [MagAOX](https://github.com/magao-x/MagAOX), the software which runs the MagAO-X ExAO system.

## 1. Dependencies

1. milk & ImageStreamIO (https://github.com/milk-org/milk)
2. mxlib (https://github.com/jaredmales/mxlib).
   The flag `MXLIB_MILK` needs to be passed to enable `milk` for `mxlib` (accounted for in CMakeLists.txt)
3. libudev (for introspective device discovery). Get from package manager.
4. zlib (compression for INDI). Get from package manager:
    - zlib1g-dev [ubuntu]
5. pthread (standards based thread API included in [ubuntu]; for indiserver)
6. eigen (also requirement for mxlib)
    - library for linear algebra: matrices, vectors, numerical solvers, and related algorithms
    - [clone](https://eigen.tuxfamily.org/index.php?title=Main_Page) and install with cmake 
7. cfitsio (also requirement for mxlib)
    - library of subroutines for reading and writing data files in FITS data format
    - [download](https://heasarc.gsfc.nasa.gov/fitsio/), untar & make
8. xrif
    - a fast, high performance, lossless compression system.
    - [clone](https://github.com/jaredmales/xrif) and follow [instructions](https://github.com/jaredmales/xrif?tab=readme-ov-file#configure-make-and-install)
9. openmp (multi-platform shared-memory multiprocessing programming interface included in [ubuntu])
10. flatbuffers (https://google.github.io/flatbuffers/flatbuffers_guide_building.html)
   To build and install the flatc compiler and install the include files in /usr/local:
   ```
   $ cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
   $ make
   $ sudo make install
   ```
11. instGraph
    - [clone](https://github.com/jaredmales/instGraph.git) and follow [instructions](https://github.com/jaredmales/instGraph/tree/main?tab=readme-ov-file#building)

## 2. Software Configuration

For a standard build and install:
```
mkdir build
cd build
cmake ..
cmake --build .
cmake --install
```

However, various defaults can be changed with build options.

#### 2.1 Options

The following options are avialable on build. They do not need to be set, but can be used to override relevant defaults.

| Name                        | Default               | Description |
|-----------------------------|-----------------------|-------------|
| `BUILD_SHARED_LIBS`         | ON                    | Build all libraries as shared libraries |
| `MAGAOX_NOEDT`              | ON                    | Disable EDT driver support |
| `NOINSTGRAPH`               | ON                    | Does not build instGraph utility (which depends on the instGraph repo) |
| `MXLIB_MILK`                | TRUE                  | Enable milk (required by mxlib) |
| `XWCTK_DISABLE_USER_CHECK`  | ON                    | Disable app user check |
| `USE_APPS_LOGSCHEMAS`       | TRUE                  | Compile log schemas in apps dirs with the library. |
|                             |                       | Assumes dir structure ${XWCTK_APPS_PATH}/appdir1/logger/types/schemas/ |
|                             |                       | and ${XWCTK_APPS_PATH}/appdir1/logger/logCodes.dat. |
|                             |                       | If FALSE, only templates included in this base library are compiled. |
| `GIT_HEADER`                | `xwctk_git_version.h` | Name of required git version tracker file. |

#### 2.2 Paths

The following options are avialable on build. They do not need to be set, but can be used to override relevant defaults.

| Name                        | Default                  | Description |
|-----------------------------|--------------------------|-------------|
| `XWCTK_PREFIX`              | `/opt/MagAOX`            | Base directory for MagAOX vendor, packages, configs, drivers, apps & logs |
| `PREFIX`                    | `/usr/local`             | Default system installation path |
| `XWCTK_APPS_PATH`           | `/opt/MagAOX/apps`       | Path to XWCTk apps directory |
| `MILK_PATH`                 | `${PREFIX}/milk-1.03.00` | Path to the milk directory |
| `MXLIB_PATH`                | `${PREFIX}/lib`          | Path to the mxlib lib directory |
| `INGR_LIB_PATH`             | `${PREFIX}/lib`          | Path to the instGraph lib |
| `INGR_HEADER_PATH`          | `${PREFIX}/include`      | Path to the instGraph headers directory |


## 3. Building

The system builds and installs:
    - the core `XWCTK` header-only library, 
as well as dependecy libraries:
    - `flatlogslib` (based on `flatbuffers`),
    - `libtelnet` (elnet protocol allowing for programmatic access to a telnet server),
    - `libcommon` (LBTI INDI application framework), and
    - `liblilxml` (LBTI library to handle parsing and processing an XML file)

The system also builds and installs the following executables:
    - `flatlogcodes` (program to parse a log type to a code file)
    - `indiserver` (TCP server that provides network access to any number of local INDI Driver programs or INDI Devices running on other indiservers in a chained fashion; see man page for details)
    - `getINDI` (connects to an indiserver and reports the current value of one or more properties; see man page for details)
    - `setINDI` (connects to an indiserver and sends commands to set new values for specified properties; see man page for details)
    - `evalINDI` (connects to an indiserver and listens for the values of properties to evaluate an arithmetic expression; see man page for details)
    - `xindidriver` (acts as the INDI driver for indiserver, but pass the STDIN and STDOUT to/from FIFOs exposed by a device controller with INDI driver functionality)
    - `cursesINDI` (utility for curses-type INDI parameter control)
    - `instGraph` (utility for mapping the path of light across the instrument)
    - `logdump` (utility to dump MagAO-X binary logs to stdout)
    - `logstream` (utility to stream MagAO-X binary logs to stdout)
    - `logsurgeon` (utility to fix corrupted MagAO-X binary logs)
    - `xrif2fits` (utility to read images from an xrif archive and write to FITS)
    - `xrif2shmim` (utility to stream images from an xrif archive to shared memory)
    - `xindiserver` (XWCTk app that wraps INDI Server)

Note the difference between the `apps` and `utils` components, is that upon install MagAO-X Applications get setuid, whereas the utilities do not.

## 4. Adding apps

Copy the template repository [XWCTkapps](https://github.com/uasal/XWCTkapps) as a new repository. To add a new app to this new repository, copy the templateCtrl folder and follow the instructions in the templateCtrl's README.md. Each app folder implements a single app. See [ESCapps](https://github.com/uasal/ESCapps/) for an example.

<b>IMPORTANT:</b> The XWCTK library expects to find apps repositories in the dir pointed to by `XWCTK_APPS_PATH` (/opt/MagAOX/apps by default). If new telemetry types are implemented in the apps repository, the apps repositories need to be copied at `XWCTK_APPS_PATH` before building the apps and the XWCTK library. In this case the correct successions of steps is:
- copy all apps repos at `XWCTK_APPS_PATH` (or set `XWCTK_APPS_PATH` to where the apps repos are)
- build and install XWCTK
- build and install the apps repos 

## 5. Software Install

Install requires root privileges.

### Directory structure

| Directory                   | Description |
|-----------------------------|-------------|
| `/opt/MagAOX`               | MagAOX system directory |
| `/opt/MagAOX/bin`           | Contains all applications |
| `/opt/MagAOX/calib`         | [magao-x/calib](https://github.com/magao-x/calib) repo for instrument calibration files |
| `/opt/MagAOX/config`        | [magao-x/config](https://github.com/magao-x/config) repo for instrument config files |
| `/opt/MagAOX/drivers`       | Symlinks for INDI |
| `/opt/MagAOX/drivers/fifos` | FIFOs for INDI |
| `/opt/MagAOX/logs`          | Directory where logs are written by the applications |
| `/opt/MagAOX/rawimages`     | Directory where raw images are written by the applications |
| `/opt/MagAOX/secrets`       | Directory containing device passwords, etc. |
| `/opt/MagAOX/source`        | Directory containing clones of this repo, [cacao-org/cacao](https://github.com/cacao-org/cacao), [jaredmales/mxlib](https://github.com/jaredmales/mxlib)                    , [jaredmales/milkzmq](https://github.com/jaredmales/milkzmq) |
| `/opt/MagAOX/sys`           | Directory for application status files, e.g. PID lock-files |
| `/opt/MagAOX/vendor`        | Vendor software packages (anything we aren't frequent committers to ourselves) |

This directory structure is #define-ed in [libMagAOX/common/defaults.hpp](https://github.com/magao-x/MagAOX/blob/master/libMagAOX/common/paths.hpp).
System directories are created and permissions are set by [setup/steps/ensure_dirs_and_perms.sh](https://github.com/magao-x/MagAOX/blob/master/setup/steps/ensure_dirs_and_perms.sh). If permissions get out of whack, it is safe to re-run that script.

## 5 Documentation

The code is more-or-less carefully documented with doxygen.

## 6 To-Do

To-do items are listed in the above sections.  Also see the Todo page in the doxygen html.  Other misc. items below:

- [] split base INDI off into separate repo, which will be the minimum someone needs to have INDI utils for interacting with MagAO-X without installing the whole tree.
- [] create indiserver startup script which takes a list of drivers from a config file, creates symlinks to xindidriver as needed, and then starts indiserver itself.
- [] start issue tracking