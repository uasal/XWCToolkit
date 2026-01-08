/** \file environment.hpp
  * \brief Environment variables for the MagAO-X library
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

#ifndef MAGAOX_env_path
   /// Environment variable setting the MagAO-X path.
   #define MAGAOX_env_path "MAGAOX_PATH"
#endif

#ifndef MAGAOX_env_config
   /// Environment variable setting the relative config path.
   #define MAGAOX_env_config "MAGAOX_CONFIG_RPATH"
#endif

#ifndef MAGAOX_env_calib
   /// Environment variable setting the relative calib path.
   #define MAGAOX_env_calib "MAGAOX_CALIB_RPATH"
#endif

#ifndef MAGAOX_env_log
   /// Environment variable setting the relative log path.
   #define MAGAOX_env_log "MAGAOX_LOG_RPATH"
#endif

#ifndef MAGAOX_env_telem
   /// Environment variable setting the relative telem path.
   #define MAGAOX_env_telem "MAGAOX_TELEM_RPATH"
#endif

#ifndef MAGAOX_env_rawimage
   /// Environment variable setting the relative raw image path.
   #define MAGAOX_env_rawimage "MAGAOX_RAWIMAGE_RPATH"
#endif

#ifndef MAGAOX_env_sys
   /// Environment variable setting the relative system directory path.
   #define MAGAOX_env_sys "MAGAOX_SYS_RPATH"
#endif

#ifndef MAGAOX_env_secrets
   /// Environment variable setting the relative secrets path.
   #define MAGAOX_env_secrets "MAGAOX_SECRETS_RPATH"
#endif

#ifndef MAGAOX_env_cpuset
   /// Environment variable setting the cpu set path.
   #define MAGAOX_env_cpuset "CGROUPS1_CPUSET_MOUNTPOINT"
#endif


///@}


#endif //common_environment_hpp
