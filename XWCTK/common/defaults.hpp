/** \file defaults.hpp
  * \brief Defaults for the XWCToolkit library
  * \author Jared R. Males (jaredmales@gmail.com)
  *
  * History:
  * - 2018-01-05 created by JRM
  */

#ifndef common_defaults_hpp
#define common_defaults_hpp


/** \defgroup default_app Default App Setup
  * \ingroup common 
  * @{
  */

#ifndef XWCTK_default_logExt
   /// The extension for XWCToolkit binary log files.
   /** Do not include period before name here.
     */
   #define XWCTK_default_logExt "binlog"
#endif

#ifndef XWCTK_default_writePause
   /// The default logger writePause
   /** Defines the default value of how long the logger write thread pauses after clearing the queue.  Default is 1 sec.
     *
     * Units: nanoseconds.
     */
   #define XWCTK_default_writePause (1000000000)
#endif

#ifndef XWCTK_default_max_logSize
   /// The default maximum log file size
   /** Defines the default maximum size in for a log file.  Default is 10 MB.
     *
     * Units: bytes
     */
   #define XWCTK_default_max_logSize (10485760)
#endif

#ifndef XWCTK_default_loopPause
   /// The default application loopPause
   /** Defines default value of how long the event loop in execute() pauses. Default is 1 sec.
     *
     * Units: nanoseconds.
     */
   #define XWCTK_default_loopPause (1000000000)
#endif

///@}

#endif //common_defaults_hpp
