/** \file paths.hpp
  * \brief Defaults for the XWCToolkit library
  * \author Jared R. Males (jaredmales@gmail.com)
  *
  * History:
  * - 2018-07-13 created by JRM
  */

#ifndef common_paths_hpp
#define common_paths_hpp

/** \defgroup default_paths Default Paths
  * \ingroup common
  *
  * @{
  */

#ifndef XWCTK_path
   /// The path to the XWCToolkit system files
   /** This directory will have subdirectories including config, log, and sys directories.
     */
   #define XWCTK_path "/opt/XWCTk"
#endif

#ifndef XWCTK_configRelPath
   /// The relative path to the configuration files.
   /** This is the subdirectory for configuration files.
     */
   #define XWCTK_configRelPath "config"
#endif

#ifndef XWCTK_calibRelPath
   /// The relative path to the calibration files.
   /** This is the subdirectory for calibration files.
     */
   #define XWCTK_calibRelPath "calib"
#endif

#ifndef XWCTK_globalConfig
   /// The filename for the global configuration file.
   /** Will be looked for in the config/ subdirectory.
     */
   #define XWCTK_globalConfig "xwctk.conf"
#endif

#ifndef XWCTK_logRelPath
   /// The relative path to the log directory.
   /** This is the subdirectory for logs.
     */
   #define XWCTK_logRelPath "logs"
#endif

#ifndef XWCTK_telRelPath
   /// The relative path to the telemetry directory.
   /** This is the subdirectory for telemetry.
     */
   #define XWCTK_telRelPath "telem"
#endif

#ifndef XWCTK_sysRelPath
   /// The relative path to the system directory
   /** This is the subdirectory for the system status files.
     */
   #define XWCTK_sysRelPath "sys"
#endif

#ifndef XWCTK_secretsRelPath
   /// The relative path to the secrets directory. Used for storing passwords, etc.
   /** This is the subdirectory for secrets.
     */
   #define XWCTK_secretsRelPath "secrets"
#endif

#ifndef XWCTK_driverRelPath
   /// The relative path to the INDI drivers
   /** This is the subdirectory for the INDI drivers.
     */
   #define XWCTK_driverRelPath "drivers"
#endif

#ifndef XWCTK_driverFIFORelPath
   /// The relative path to the INDI driver FIFOs
   /** This is the subdirectory for the INDI driver FIFOs.
     */
   #define XWCTK_driverFIFORelPath "drivers/fifos"
#endif

#ifndef XWCTK_rawimageRelPath
   /// The relative path to the raw images directory.
   /** This is the subdirectory for raw images.
     */
   #define XWCTK_rawimageRelPath "rawimages"
#endif

#ifndef XWCTK_cpusetPath
   /// The absolute path the the cpuset mount point
   /**
     */
   #define XWCTK_cpusetPath "/opt/XWCTk/cpuset/"
#endif

///@}

#endif //common_paths_hpp
