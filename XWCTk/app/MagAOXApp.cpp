/** \file magAOXApp.cpp
  * \brief Instantiation of the basic MagAO-X Application
  * \author Jared R. Males (jaredmales@gmail.com)
  *
  * History:
  * - 2021-01-05 created by JRM
  *
  * \ingroup app_files
  */

#include "MagAOXApp.hpp"

namespace MagAOX
{
namespace app
{

void sigUsr1Handler( int signum,
                     siginfo_t * siginf,
                     void *ucont
                   )
{
   static_cast<void>(signum);
   static_cast<void>(siginf);
   static_cast<void>(ucont);

   return;
}

template class MagAOXApp<true>;
template class MagAOXApp<false>;


}
}
