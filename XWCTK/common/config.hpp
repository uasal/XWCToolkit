/** \file config.hpp
  * \brief Configuration defines for the XWCToolkit library
  * \author Jared R. Males (jaredmales@gmail.com)
  *
  * History:
  * - 2018-01-20 created by JRM
  */

#ifndef common_config_hpp
#define common_config_hpp

/** \defgroup config_defines Configuration Defines
  * \ingroup common
  *
  * @{
  */


#ifndef XWCTK_RT_SCHED_POLICY
   ///The real-time scheduling policy
   #define XWCTK_RT_SCHED_POLICY (SCHED_FIFO)
#endif

///@}

#endif //common_config_hpp
