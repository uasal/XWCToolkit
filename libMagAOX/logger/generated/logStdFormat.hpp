#ifndef logger_logStdFormat_hpp
#define logger_logStdFormat_hpp
#include <flatlogs/flatlogs.hpp>
#include "logTypes.hpp"
namespace MagAOX
{
namespace logger
{
template<class iosT>
iosT & logStdFormat( iosT & ios,
                     flatlogs::bufferPtrT & buffer )
{
   flatlogs::eventCodeT ec;
   ec = flatlogs::logHeader::eventCode(buffer);
   switch(ec)
   {
      case 0:
         return flatlogs::stdFormat<git_state>(ios, buffer);
      case 10:
         return flatlogs::stdFormat<text_log>(ios, buffer);
      case 12:
         return flatlogs::stdFormat<user_log>(ios, buffer);
      case 20:
         return flatlogs::stdFormat<state_change>(ios, buffer);
      case 50:
         return flatlogs::stdFormat<software_log>(ios, buffer);
      case 60:
         return flatlogs::stdFormat<config_log>(ios, buffer);
      case 140:
         return flatlogs::stdFormat<indidriver_start>(ios, buffer);
      case 141:
         return flatlogs::stdFormat<indidriver_stop>(ios, buffer);
      case 1001:
         return flatlogs::stdFormat<loop_closed>(ios, buffer);
      case 1002:
         return flatlogs::stdFormat<loop_paused>(ios, buffer);
      case 1003:
         return flatlogs::stdFormat<loop_open>(ios, buffer);
      case 1050:
         return flatlogs::stdFormat<observer>(ios, buffer);
      case 11025:
         return flatlogs::stdFormat<pico_channel>(ios, buffer);
      case 12001:
         return flatlogs::stdFormat<outlet_state>(ios, buffer);
      case 12002:
         return flatlogs::stdFormat<outlet_channel_state>(ios, buffer);
      case 12053:
         return flatlogs::stdFormat<telem_saving_state>(ios, buffer);
      case 12100:
         return flatlogs::stdFormat<telem_fxngen>(ios, buffer);
      case 12110:
         return flatlogs::stdFormat<ttmmod_params>(ios, buffer);
      case 12120:
         return flatlogs::stdFormat<ocam_temps>(ios, buffer);
      case 12500:
         return flatlogs::stdFormat<saving_start>(ios, buffer);
      case 12501:
         return flatlogs::stdFormat<saving_stop>(ios, buffer);
      case 12502:
         return flatlogs::stdFormat<telem_saving>(ios, buffer);
      case 20000:
         return flatlogs::stdFormat<telem_telpos>(ios, buffer);
      case 20001:
         return flatlogs::stdFormat<telem_teldata>(ios, buffer);
      case 20002:
         return flatlogs::stdFormat<telem_telvane>(ios, buffer);
      case 20003:
         return flatlogs::stdFormat<telem_telenv>(ios, buffer);
      case 20004:
         return flatlogs::stdFormat<telem_telcat>(ios, buffer);
      case 20005:
         return flatlogs::stdFormat<telem_telsee>(ios, buffer);
      case 20050:
         return flatlogs::stdFormat<telem_stage>(ios, buffer);
      case 20055:
         return flatlogs::stdFormat<telem_zaber>(ios, buffer);
      case 20060:
         return flatlogs::stdFormat<telem_pico>(ios, buffer);
      case 20065:
         return flatlogs::stdFormat<telem_position>(ios, buffer);
      case 20067:
         return flatlogs::stdFormat<telem_pokecenter>(ios, buffer);
      case 20068:
         return flatlogs::stdFormat<telem_pokeloop>(ios, buffer);
      case 20070:
         return flatlogs::stdFormat<telem_observer>(ios, buffer);
      case 20100:
         return flatlogs::stdFormat<telem_rhusb>(ios, buffer);
      case 20250:
         return flatlogs::stdFormat<telem_temps>(ios, buffer);
      case 20260:
         return flatlogs::stdFormat<telem_stdcam>(ios, buffer);
      case 20825:
         return flatlogs::stdFormat<telem_coretemps>(ios, buffer);
      case 20826:
         return flatlogs::stdFormat<telem_coreloads>(ios, buffer);
      case 20827:
         return flatlogs::stdFormat<telem_drivetemps>(ios, buffer);
      case 20828:
         return flatlogs::stdFormat<telem_usage>(ios, buffer);
      case 20850:
         return flatlogs::stdFormat<telem_cooler>(ios, buffer);
      case 20860:
         return flatlogs::stdFormat<telem_chrony_status>(ios, buffer);
      case 20861:
         return flatlogs::stdFormat<telem_chrony_stats>(ios, buffer);
      case 20890:
         return flatlogs::stdFormat<telem_dmspeck>(ios, buffer);
      case 20905:
         return flatlogs::stdFormat<telem_fgtimings>(ios, buffer);
      case 20910:
         return flatlogs::stdFormat<telem_dmmodes>(ios, buffer);
      case 20915:
         return flatlogs::stdFormat<telem_loopgain>(ios, buffer);
      case 20920:
         return flatlogs::stdFormat<telem_blockgains>(ios, buffer);
      case 20930:
         return flatlogs::stdFormat<telem_pi335>(ios, buffer);
      default:
         ios << "Unknown log type: " << ec << "\n";
         return ios;
   }
}
template<class iosT>
iosT & logShortStdFormat( iosT & ios,
                          const std::string & appName,
                          flatlogs::bufferPtrT & buffer )
{
   flatlogs::eventCodeT ec;
   ec = flatlogs::logHeader::eventCode(buffer);
   switch(ec)
   {
      case 0:
         return flatlogs::stdShortFormat<git_state>(ios, appName, buffer);
      case 10:
         return flatlogs::stdShortFormat<text_log>(ios, appName, buffer);
      case 12:
         return flatlogs::stdShortFormat<user_log>(ios, appName, buffer);
      case 20:
         return flatlogs::stdShortFormat<state_change>(ios, appName, buffer);
      case 50:
         return flatlogs::stdShortFormat<software_log>(ios, appName, buffer);
      case 60:
         return flatlogs::stdShortFormat<config_log>(ios, appName, buffer);
      case 140:
         return flatlogs::stdShortFormat<indidriver_start>(ios, appName, buffer);
      case 141:
         return flatlogs::stdShortFormat<indidriver_stop>(ios, appName, buffer);
      case 1001:
         return flatlogs::stdShortFormat<loop_closed>(ios, appName, buffer);
      case 1002:
         return flatlogs::stdShortFormat<loop_paused>(ios, appName, buffer);
      case 1003:
         return flatlogs::stdShortFormat<loop_open>(ios, appName, buffer);
      case 1050:
         return flatlogs::stdShortFormat<observer>(ios, appName, buffer);
      case 11025:
         return flatlogs::stdShortFormat<pico_channel>(ios, appName, buffer);
      case 12001:
         return flatlogs::stdShortFormat<outlet_state>(ios, appName, buffer);
      case 12002:
         return flatlogs::stdShortFormat<outlet_channel_state>(ios, appName, buffer);
      case 12053:
         return flatlogs::stdShortFormat<telem_saving_state>(ios, appName, buffer);
      case 12100:
         return flatlogs::stdShortFormat<telem_fxngen>(ios, appName, buffer);
      case 12110:
         return flatlogs::stdShortFormat<ttmmod_params>(ios, appName, buffer);
      case 12120:
         return flatlogs::stdShortFormat<ocam_temps>(ios, appName, buffer);
      case 12500:
         return flatlogs::stdShortFormat<saving_start>(ios, appName, buffer);
      case 12501:
         return flatlogs::stdShortFormat<saving_stop>(ios, appName, buffer);
      case 12502:
         return flatlogs::stdShortFormat<telem_saving>(ios, appName, buffer);
      case 20000:
         return flatlogs::stdShortFormat<telem_telpos>(ios, appName, buffer);
      case 20001:
         return flatlogs::stdShortFormat<telem_teldata>(ios, appName, buffer);
      case 20002:
         return flatlogs::stdShortFormat<telem_telvane>(ios, appName, buffer);
      case 20003:
         return flatlogs::stdShortFormat<telem_telenv>(ios, appName, buffer);
      case 20004:
         return flatlogs::stdShortFormat<telem_telcat>(ios, appName, buffer);
      case 20005:
         return flatlogs::stdShortFormat<telem_telsee>(ios, appName, buffer);
      case 20050:
         return flatlogs::stdShortFormat<telem_stage>(ios, appName, buffer);
      case 20055:
         return flatlogs::stdShortFormat<telem_zaber>(ios, appName, buffer);
      case 20060:
         return flatlogs::stdShortFormat<telem_pico>(ios, appName, buffer);
      case 20065:
         return flatlogs::stdShortFormat<telem_position>(ios, appName, buffer);
      case 20067:
         return flatlogs::stdShortFormat<telem_pokecenter>(ios, appName, buffer);
      case 20068:
         return flatlogs::stdShortFormat<telem_pokeloop>(ios, appName, buffer);
      case 20070:
         return flatlogs::stdShortFormat<telem_observer>(ios, appName, buffer);
      case 20100:
         return flatlogs::stdShortFormat<telem_rhusb>(ios, appName, buffer);
      case 20250:
         return flatlogs::stdShortFormat<telem_temps>(ios, appName, buffer);
      case 20260:
         return flatlogs::stdShortFormat<telem_stdcam>(ios, appName, buffer);
      case 20825:
         return flatlogs::stdShortFormat<telem_coretemps>(ios, appName, buffer);
      case 20826:
         return flatlogs::stdShortFormat<telem_coreloads>(ios, appName, buffer);
      case 20827:
         return flatlogs::stdShortFormat<telem_drivetemps>(ios, appName, buffer);
      case 20828:
         return flatlogs::stdShortFormat<telem_usage>(ios, appName, buffer);
      case 20850:
         return flatlogs::stdShortFormat<telem_cooler>(ios, appName, buffer);
      case 20860:
         return flatlogs::stdShortFormat<telem_chrony_status>(ios, appName, buffer);
      case 20861:
         return flatlogs::stdShortFormat<telem_chrony_stats>(ios, appName, buffer);
      case 20890:
         return flatlogs::stdShortFormat<telem_dmspeck>(ios, appName, buffer);
      case 20905:
         return flatlogs::stdShortFormat<telem_fgtimings>(ios, appName, buffer);
      case 20910:
         return flatlogs::stdShortFormat<telem_dmmodes>(ios, appName, buffer);
      case 20915:
         return flatlogs::stdShortFormat<telem_loopgain>(ios, appName, buffer);
      case 20920:
         return flatlogs::stdShortFormat<telem_blockgains>(ios, appName, buffer);
      case 20930:
         return flatlogs::stdShortFormat<telem_pi335>(ios, appName, buffer);
      default:
         ios << "Unknown log type: " << ec << "\n";
         return ios;
   }
}
template<class iosT>
iosT & logMinStdFormat( iosT & ios,
                        flatlogs::bufferPtrT & buffer )
{
   flatlogs::eventCodeT ec;
   ec = flatlogs::logHeader::eventCode(buffer);
   switch(ec)
   {
      case 0:
         return flatlogs::minFormat<git_state>(ios, buffer);
      case 10:
         return flatlogs::minFormat<text_log>(ios, buffer);
      case 12:
         return flatlogs::minFormat<user_log>(ios, buffer);
      case 20:
         return flatlogs::minFormat<state_change>(ios, buffer);
      case 50:
         return flatlogs::minFormat<software_log>(ios, buffer);
      case 60:
         return flatlogs::minFormat<config_log>(ios, buffer);
      case 140:
         return flatlogs::minFormat<indidriver_start>(ios, buffer);
      case 141:
         return flatlogs::minFormat<indidriver_stop>(ios, buffer);
      case 1001:
         return flatlogs::minFormat<loop_closed>(ios, buffer);
      case 1002:
         return flatlogs::minFormat<loop_paused>(ios, buffer);
      case 1003:
         return flatlogs::minFormat<loop_open>(ios, buffer);
      case 1050:
         return flatlogs::minFormat<observer>(ios, buffer);
      case 11025:
         return flatlogs::minFormat<pico_channel>(ios, buffer);
      case 12001:
         return flatlogs::minFormat<outlet_state>(ios, buffer);
      case 12002:
         return flatlogs::minFormat<outlet_channel_state>(ios, buffer);
      case 12053:
         return flatlogs::minFormat<telem_saving_state>(ios, buffer);
      case 12100:
         return flatlogs::minFormat<telem_fxngen>(ios, buffer);
      case 12110:
         return flatlogs::minFormat<ttmmod_params>(ios, buffer);
      case 12120:
         return flatlogs::minFormat<ocam_temps>(ios, buffer);
      case 12500:
         return flatlogs::minFormat<saving_start>(ios, buffer);
      case 12501:
         return flatlogs::minFormat<saving_stop>(ios, buffer);
      case 12502:
         return flatlogs::minFormat<telem_saving>(ios, buffer);
      case 20000:
         return flatlogs::minFormat<telem_telpos>(ios, buffer);
      case 20001:
         return flatlogs::minFormat<telem_teldata>(ios, buffer);
      case 20002:
         return flatlogs::minFormat<telem_telvane>(ios, buffer);
      case 20003:
         return flatlogs::minFormat<telem_telenv>(ios, buffer);
      case 20004:
         return flatlogs::minFormat<telem_telcat>(ios, buffer);
      case 20005:
         return flatlogs::minFormat<telem_telsee>(ios, buffer);
      case 20050:
         return flatlogs::minFormat<telem_stage>(ios, buffer);
      case 20055:
         return flatlogs::minFormat<telem_zaber>(ios, buffer);
      case 20060:
         return flatlogs::minFormat<telem_pico>(ios, buffer);
      case 20065:
         return flatlogs::minFormat<telem_position>(ios, buffer);
      case 20067:
         return flatlogs::minFormat<telem_pokecenter>(ios, buffer);
      case 20068:
         return flatlogs::minFormat<telem_pokeloop>(ios, buffer);
      case 20070:
         return flatlogs::minFormat<telem_observer>(ios, buffer);
      case 20100:
         return flatlogs::minFormat<telem_rhusb>(ios, buffer);
      case 20250:
         return flatlogs::minFormat<telem_temps>(ios, buffer);
      case 20260:
         return flatlogs::minFormat<telem_stdcam>(ios, buffer);
      case 20825:
         return flatlogs::minFormat<telem_coretemps>(ios, buffer);
      case 20826:
         return flatlogs::minFormat<telem_coreloads>(ios, buffer);
      case 20827:
         return flatlogs::minFormat<telem_drivetemps>(ios, buffer);
      case 20828:
         return flatlogs::minFormat<telem_usage>(ios, buffer);
      case 20850:
         return flatlogs::minFormat<telem_cooler>(ios, buffer);
      case 20860:
         return flatlogs::minFormat<telem_chrony_status>(ios, buffer);
      case 20861:
         return flatlogs::minFormat<telem_chrony_stats>(ios, buffer);
      case 20890:
         return flatlogs::minFormat<telem_dmspeck>(ios, buffer);
      case 20905:
         return flatlogs::minFormat<telem_fgtimings>(ios, buffer);
      case 20910:
         return flatlogs::minFormat<telem_dmmodes>(ios, buffer);
      case 20915:
         return flatlogs::minFormat<telem_loopgain>(ios, buffer);
      case 20920:
         return flatlogs::minFormat<telem_blockgains>(ios, buffer);
      case 20930:
         return flatlogs::minFormat<telem_pi335>(ios, buffer);
      default:
         ios << "Unknown log type: " << ec << "\n";
         return ios;
   }
}
template<class iosT>
iosT & logJsonFormat( iosT & ios,
                        flatlogs::bufferPtrT & buffer )
{
#include "binarySchemata.inc"
   flatlogs::eventCodeT ec;
   ec = flatlogs::logHeader::eventCode(buffer);
   switch(ec)
   {
      case 0:
         return flatlogs::jsonFormat<git_state>(ios, buffer, "git_state", reinterpret_cast<const uint8_t *>(git_state_bfbs), git_state_bfbs_len);
      case 10:
         return flatlogs::jsonFormat<text_log>(ios, buffer, "text_log", reinterpret_cast<const uint8_t *>(string_log_bfbs), string_log_bfbs_len);
      case 12:
         return flatlogs::jsonFormat<user_log>(ios, buffer, "user_log", reinterpret_cast<const uint8_t *>(user_log_bfbs), user_log_bfbs_len);
      case 20:
         return flatlogs::jsonFormat<state_change>(ios, buffer, "state_change", reinterpret_cast<const uint8_t *>(state_change_bfbs), state_change_bfbs_len);
      case 50:
         return flatlogs::jsonFormat<software_log>(ios, buffer, "software_log", reinterpret_cast<const uint8_t *>(software_log_bfbs), software_log_bfbs_len);
      case 60:
         return flatlogs::jsonFormat<config_log>(ios, buffer, "config_log", reinterpret_cast<const uint8_t *>(config_log_bfbs), config_log_bfbs_len);
      case 140:
         return flatlogs::jsonFormat<indidriver_start>(ios, buffer, "indidriver_start", nullptr, 0);
      case 141:
         return flatlogs::jsonFormat<indidriver_stop>(ios, buffer, "indidriver_stop", nullptr, 0);
      case 1001:
         return flatlogs::jsonFormat<loop_closed>(ios, buffer, "loop_closed", nullptr, 0);
      case 1002:
         return flatlogs::jsonFormat<loop_paused>(ios, buffer, "loop_paused", nullptr, 0);
      case 1003:
         return flatlogs::jsonFormat<loop_open>(ios, buffer, "loop_open", nullptr, 0);
      case 1050:
         return flatlogs::jsonFormat<observer>(ios, buffer, "observer", reinterpret_cast<const uint8_t *>(observer_bfbs), observer_bfbs_len);
      case 11025:
         return flatlogs::jsonFormat<pico_channel>(ios, buffer, "pico_channel", reinterpret_cast<const uint8_t *>(pico_channel_bfbs), pico_channel_bfbs_len);
      case 12001:
         return flatlogs::jsonFormat<outlet_state>(ios, buffer, "outlet_state", reinterpret_cast<const uint8_t *>(outlet_state_bfbs), outlet_state_bfbs_len);
      case 12002:
         return flatlogs::jsonFormat<outlet_channel_state>(ios, buffer, "outlet_channel_state", reinterpret_cast<const uint8_t *>(outlet_channel_state_bfbs), outlet_channel_state_bfbs_len);
      case 12053:
         return flatlogs::jsonFormat<telem_saving_state>(ios, buffer, "telem_saving_state", reinterpret_cast<const uint8_t *>(saving_state_change_bfbs), saving_state_change_bfbs_len);
      case 12100:
         return flatlogs::jsonFormat<telem_fxngen>(ios, buffer, "telem_fxngen", reinterpret_cast<const uint8_t *>(telem_fxngen_bfbs), telem_fxngen_bfbs_len);
      case 12110:
         return flatlogs::jsonFormat<ttmmod_params>(ios, buffer, "ttmmod_params", reinterpret_cast<const uint8_t *>(ttmmod_params_bfbs), ttmmod_params_bfbs_len);
      case 12120:
         return flatlogs::jsonFormat<ocam_temps>(ios, buffer, "ocam_temps", reinterpret_cast<const uint8_t *>(ocam_temps_bfbs), ocam_temps_bfbs_len);
      case 12500:
         return flatlogs::jsonFormat<saving_start>(ios, buffer, "saving_start", reinterpret_cast<const uint8_t *>(saving_state_change_bfbs), saving_state_change_bfbs_len);
      case 12501:
         return flatlogs::jsonFormat<saving_stop>(ios, buffer, "saving_stop", reinterpret_cast<const uint8_t *>(saving_state_change_bfbs), saving_state_change_bfbs_len);
      case 12502:
         return flatlogs::jsonFormat<telem_saving>(ios, buffer, "telem_saving", reinterpret_cast<const uint8_t *>(telem_saving_bfbs), telem_saving_bfbs_len);
      case 20000:
         return flatlogs::jsonFormat<telem_telpos>(ios, buffer, "telem_telpos", reinterpret_cast<const uint8_t *>(telem_telpos_bfbs), telem_telpos_bfbs_len);
      case 20001:
         return flatlogs::jsonFormat<telem_teldata>(ios, buffer, "telem_teldata", reinterpret_cast<const uint8_t *>(telem_teldata_bfbs), telem_teldata_bfbs_len);
      case 20002:
         return flatlogs::jsonFormat<telem_telvane>(ios, buffer, "telem_telvane", reinterpret_cast<const uint8_t *>(telem_telvane_bfbs), telem_telvane_bfbs_len);
      case 20003:
         return flatlogs::jsonFormat<telem_telenv>(ios, buffer, "telem_telenv", reinterpret_cast<const uint8_t *>(telem_telenv_bfbs), telem_telenv_bfbs_len);
      case 20004:
         return flatlogs::jsonFormat<telem_telcat>(ios, buffer, "telem_telcat", reinterpret_cast<const uint8_t *>(telem_telcat_bfbs), telem_telcat_bfbs_len);
      case 20005:
         return flatlogs::jsonFormat<telem_telsee>(ios, buffer, "telem_telsee", reinterpret_cast<const uint8_t *>(telem_telsee_bfbs), telem_telsee_bfbs_len);
      case 20050:
         return flatlogs::jsonFormat<telem_stage>(ios, buffer, "telem_stage", reinterpret_cast<const uint8_t *>(telem_stage_bfbs), telem_stage_bfbs_len);
      case 20055:
         return flatlogs::jsonFormat<telem_zaber>(ios, buffer, "telem_zaber", reinterpret_cast<const uint8_t *>(telem_zaber_bfbs), telem_zaber_bfbs_len);
      case 20060:
         return flatlogs::jsonFormat<telem_pico>(ios, buffer, "telem_pico", reinterpret_cast<const uint8_t *>(telem_pico_bfbs), telem_pico_bfbs_len);
      case 20065:
         return flatlogs::jsonFormat<telem_position>(ios, buffer, "telem_position", reinterpret_cast<const uint8_t *>(telem_position_bfbs), telem_position_bfbs_len);
      case 20067:
         return flatlogs::jsonFormat<telem_pokecenter>(ios, buffer, "telem_pokecenter", reinterpret_cast<const uint8_t *>(telem_pokecenter_bfbs), telem_pokecenter_bfbs_len);
      case 20068:
         return flatlogs::jsonFormat<telem_pokeloop>(ios, buffer, "telem_pokeloop", reinterpret_cast<const uint8_t *>(telem_pokeloop_bfbs), telem_pokeloop_bfbs_len);
      case 20070:
         return flatlogs::jsonFormat<telem_observer>(ios, buffer, "telem_observer", reinterpret_cast<const uint8_t *>(telem_observer_bfbs), telem_observer_bfbs_len);
      case 20100:
         return flatlogs::jsonFormat<telem_rhusb>(ios, buffer, "telem_rhusb", reinterpret_cast<const uint8_t *>(telem_rhusb_bfbs), telem_rhusb_bfbs_len);
      case 20250:
         return flatlogs::jsonFormat<telem_temps>(ios, buffer, "telem_temps", reinterpret_cast<const uint8_t *>(telem_temps_bfbs), telem_temps_bfbs_len);
      case 20260:
         return flatlogs::jsonFormat<telem_stdcam>(ios, buffer, "telem_stdcam", reinterpret_cast<const uint8_t *>(telem_stdcam_bfbs), telem_stdcam_bfbs_len);
      case 20825:
         return flatlogs::jsonFormat<telem_coretemps>(ios, buffer, "telem_coretemps", reinterpret_cast<const uint8_t *>(telem_coretemps_bfbs), telem_coretemps_bfbs_len);
      case 20826:
         return flatlogs::jsonFormat<telem_coreloads>(ios, buffer, "telem_coreloads", reinterpret_cast<const uint8_t *>(telem_coreloads_bfbs), telem_coreloads_bfbs_len);
      case 20827:
         return flatlogs::jsonFormat<telem_drivetemps>(ios, buffer, "telem_drivetemps", reinterpret_cast<const uint8_t *>(telem_drivetemps_bfbs), telem_drivetemps_bfbs_len);
      case 20828:
         return flatlogs::jsonFormat<telem_usage>(ios, buffer, "telem_usage", reinterpret_cast<const uint8_t *>(telem_usage_bfbs), telem_usage_bfbs_len);
      case 20850:
         return flatlogs::jsonFormat<telem_cooler>(ios, buffer, "telem_cooler", reinterpret_cast<const uint8_t *>(telem_cooler_bfbs), telem_cooler_bfbs_len);
      case 20860:
         return flatlogs::jsonFormat<telem_chrony_status>(ios, buffer, "telem_chrony_status", reinterpret_cast<const uint8_t *>(telem_chrony_status_bfbs), telem_chrony_status_bfbs_len);
      case 20861:
         return flatlogs::jsonFormat<telem_chrony_stats>(ios, buffer, "telem_chrony_stats", reinterpret_cast<const uint8_t *>(telem_chrony_stats_bfbs), telem_chrony_stats_bfbs_len);
      case 20890:
         return flatlogs::jsonFormat<telem_dmspeck>(ios, buffer, "telem_dmspeck", reinterpret_cast<const uint8_t *>(telem_dmspeck_bfbs), telem_dmspeck_bfbs_len);
      case 20905:
         return flatlogs::jsonFormat<telem_fgtimings>(ios, buffer, "telem_fgtimings", reinterpret_cast<const uint8_t *>(telem_fgtimings_bfbs), telem_fgtimings_bfbs_len);
      case 20910:
         return flatlogs::jsonFormat<telem_dmmodes>(ios, buffer, "telem_dmmodes", reinterpret_cast<const uint8_t *>(telem_dmmodes_bfbs), telem_dmmodes_bfbs_len);
      case 20915:
         return flatlogs::jsonFormat<telem_loopgain>(ios, buffer, "telem_loopgain", reinterpret_cast<const uint8_t *>(telem_loopgain_bfbs), telem_loopgain_bfbs_len);
      case 20920:
         return flatlogs::jsonFormat<telem_blockgains>(ios, buffer, "telem_blockgains", reinterpret_cast<const uint8_t *>(telem_blockgains_bfbs), telem_blockgains_bfbs_len);
      case 20930:
         return flatlogs::jsonFormat<telem_pi335>(ios, buffer, "telem_pi335", reinterpret_cast<const uint8_t *>(telem_pi335_bfbs), telem_pi335_bfbs_len);
      default:
         ios << "Unknown log type: " << ec << "\n";
         return ios;
   }
}
}
}
#endif
