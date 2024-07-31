#ifndef logger_logVerify_hpp
#define logger_logVerify_hpp
#include <flatlogs/flatlogs.hpp>
#include "logTypes.hpp"
namespace MagAOX
{
namespace logger
{
inline bool logVerify( flatlogs::eventCodeT ec,
                       flatlogs::bufferPtrT & buffer,
                       flatlogs::msgLenT len )
{
   switch(ec)
   {
      case 0:
         return git_state::verify(buffer, len);
      case 10:
         return text_log::verify(buffer, len);
      case 12:
         return user_log::verify(buffer, len);
      case 20:
         return state_change::verify(buffer, len);
      case 50:
         return software_log::verify(buffer, len);
      case 60:
         return config_log::verify(buffer, len);
      case 140:
         return indidriver_start::verify(buffer, len);
      case 141:
         return indidriver_stop::verify(buffer, len);
      case 1001:
         return loop_closed::verify(buffer, len);
      case 1002:
         return loop_paused::verify(buffer, len);
      case 1003:
         return loop_open::verify(buffer, len);
      case 1050:
         return observer::verify(buffer, len);
      case 11025:
         return pico_channel::verify(buffer, len);
      case 12001:
         return outlet_state::verify(buffer, len);
      case 12002:
         return outlet_channel_state::verify(buffer, len);
      case 12053:
         return telem_saving_state::verify(buffer, len);
      case 12100:
         return telem_fxngen::verify(buffer, len);
      case 12110:
         return ttmmod_params::verify(buffer, len);
      case 12120:
         return ocam_temps::verify(buffer, len);
      case 12500:
         return saving_start::verify(buffer, len);
      case 12501:
         return saving_stop::verify(buffer, len);
      case 12502:
         return telem_saving::verify(buffer, len);
      case 20000:
         return telem_telpos::verify(buffer, len);
      case 20001:
         return telem_teldata::verify(buffer, len);
      case 20002:
         return telem_telvane::verify(buffer, len);
      case 20003:
         return telem_telenv::verify(buffer, len);
      case 20004:
         return telem_telcat::verify(buffer, len);
      case 20005:
         return telem_telsee::verify(buffer, len);
      case 20050:
         return telem_stage::verify(buffer, len);
      case 20055:
         return telem_zaber::verify(buffer, len);
      case 20060:
         return telem_pico::verify(buffer, len);
      case 20065:
         return telem_position::verify(buffer, len);
      case 20067:
         return telem_pokecenter::verify(buffer, len);
      case 20068:
         return telem_pokeloop::verify(buffer, len);
      case 20070:
         return telem_observer::verify(buffer, len);
      case 20100:
         return telem_rhusb::verify(buffer, len);
      case 20250:
         return telem_temps::verify(buffer, len);
      case 20260:
         return telem_stdcam::verify(buffer, len);
      case 20825:
         return telem_coretemps::verify(buffer, len);
      case 20826:
         return telem_coreloads::verify(buffer, len);
      case 20827:
         return telem_drivetemps::verify(buffer, len);
      case 20828:
         return telem_usage::verify(buffer, len);
      case 20850:
         return telem_cooler::verify(buffer, len);
      case 20860:
         return telem_chrony_status::verify(buffer, len);
      case 20861:
         return telem_chrony_stats::verify(buffer, len);
      case 20890:
         return telem_dmspeck::verify(buffer, len);
      case 20905:
         return telem_fgtimings::verify(buffer, len);
      case 20910:
         return telem_dmmodes::verify(buffer, len);
      case 20915:
         return telem_loopgain::verify(buffer, len);
      case 20920:
         return telem_blockgains::verify(buffer, len);
      case 20930:
         return telem_pi335::verify(buffer, len);
      default:
         std::cerr << "Unknown log type: " << ec << "\n";
         return false;
   }
}
}
}
#endif
