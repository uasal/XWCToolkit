/** \file environment.hpp 
  * \brief Environment variables for the XWCToolkit library
  * \author Jared R. Males (jaredmales@gmail.com)
  *
  * History:
  * - 2018-01-05 created by JRM
  */ 

#ifndef common_environment_hpp
#define common_environment_hpp

/** \defgroup env_var Environment Variables
  * \ingroup common 
  * 
  * @{
  */

#ifndef XWCTK_env_path
   /// Environment variable setting the XWCToolkit path.
   #define XWCTK_env_path "XWCTk_PATH"
#endif 

#ifndef XWCTK_env_config
   /// Environment variable setting the relative config path.
   #define XWCTK_env_config "XWCTk_CONFIG"
#endif

#ifndef XWCTK_env_calib
   /// Environment variable setting the relative calib path.
   #define XWCTK_env_calib "XWCTk_CALIB"
#endif

#ifndef XWCTK_env_cpuset
   /// Environment variable setting the relative calib path.
   #define XWCTK_env_cpuset "CGROUPS1_CPUSET_MOUNTPOINT"
#endif


///@}


#endif //common_environment_hpp
