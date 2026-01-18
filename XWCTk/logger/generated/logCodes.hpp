#ifndef logger_logCodes_hpp
#define logger_logCodes_hpp
#include <flatlogs/flatlogs.hpp>
namespace MagAOX
{
namespace logger
{
namespace eventCodes
{
   constexpr static flatlogs::eventCodeT GIT_STATE = 0;
   constexpr static flatlogs::eventCodeT TEXT_LOG = 10;
   constexpr static flatlogs::eventCodeT USER_LOG = 12;
   constexpr static flatlogs::eventCodeT STATE_CHANGE = 20;
   constexpr static flatlogs::eventCodeT SOFTWARE_LOG = 50;
   constexpr static flatlogs::eventCodeT CONFIG_LOG = 60;
   constexpr static flatlogs::eventCodeT INDIDRIVER_START = 140;
   constexpr static flatlogs::eventCodeT INDIDRIVER_STOP = 141;
   constexpr static flatlogs::eventCodeT LOOP_CLOSED = 1001;
   constexpr static flatlogs::eventCodeT LOOP_PAUSED = 1002;
   constexpr static flatlogs::eventCodeT LOOP_OPEN = 1003;
   constexpr static flatlogs::eventCodeT OBSERVER = 1050;
   constexpr static flatlogs::eventCodeT AO_OPERATOR = 1051;
   constexpr static flatlogs::eventCodeT PICO_CHANNEL = 11025;
   constexpr static flatlogs::eventCodeT OUTLET_STATE = 12001;
   constexpr static flatlogs::eventCodeT OUTLET_CHANNEL_STATE = 12002;
   constexpr static flatlogs::eventCodeT TELEM_SAVING_STATE = 12053;
   constexpr static flatlogs::eventCodeT TELEM_FXNGEN = 12100;
   constexpr static flatlogs::eventCodeT TTMMOD_PARAMS = 12110;
   constexpr static flatlogs::eventCodeT OCAM_TEMPS = 12120;
   constexpr static flatlogs::eventCodeT SAVING_START = 12500;
   constexpr static flatlogs::eventCodeT SAVING_STOP = 12501;
   constexpr static flatlogs::eventCodeT TELEM_SAVING = 12502;
   constexpr static flatlogs::eventCodeT TELEM_TELPOS = 20000;
   constexpr static flatlogs::eventCodeT TELEM_TELDATA = 20001;
   constexpr static flatlogs::eventCodeT TELEM_TELVANE = 20002;
   constexpr static flatlogs::eventCodeT TELEM_TELENV = 20003;
   constexpr static flatlogs::eventCodeT TELEM_TELCAT = 20004;
   constexpr static flatlogs::eventCodeT TELEM_TELSEE = 20005;
   constexpr static flatlogs::eventCodeT TELEM_STAGE = 20050;
   constexpr static flatlogs::eventCodeT TELEM_ZABER = 20055;
   constexpr static flatlogs::eventCodeT TELEM_PICO = 20060;
   constexpr static flatlogs::eventCodeT TELEM_POSITION = 20065;
   constexpr static flatlogs::eventCodeT TELEM_POKECENTER = 20067;
   constexpr static flatlogs::eventCodeT TELEM_POKELOOP = 20068;
   constexpr static flatlogs::eventCodeT TELEM_OBSERVER = 20070;
   constexpr static flatlogs::eventCodeT TELEM_RHUSB = 20100;
   constexpr static flatlogs::eventCodeT TELEM_TEMPS = 20250;
   constexpr static flatlogs::eventCodeT TELEM_STDCAM = 20260;
   constexpr static flatlogs::eventCodeT TELEM_CORETEMPS = 20825;
   constexpr static flatlogs::eventCodeT TELEM_CORELOADS = 20826;
   constexpr static flatlogs::eventCodeT TELEM_DRIVETEMPS = 20827;
   constexpr static flatlogs::eventCodeT TELEM_USAGE = 20828;
   constexpr static flatlogs::eventCodeT TELEM_COOLER = 20850;
   constexpr static flatlogs::eventCodeT TELEM_CHRONY_STATUS = 20860;
   constexpr static flatlogs::eventCodeT TELEM_CHRONY_STATS = 20861;
   constexpr static flatlogs::eventCodeT TELEM_DMSPECK = 20890;
   constexpr static flatlogs::eventCodeT TELEM_FGTIMINGS = 20905;
   constexpr static flatlogs::eventCodeT TELEM_DMMODES = 20910;
   constexpr static flatlogs::eventCodeT TELEM_LOOPGAIN = 20915;
   constexpr static flatlogs::eventCodeT TELEM_BLOCKGAINS = 20920;
   constexpr static flatlogs::eventCodeT TELEM_OFFLOADING = 20923;
   constexpr static flatlogs::eventCodeT TELEM_PI335 = 20930;
   constexpr static flatlogs::eventCodeT TELEM_SPARKLECLOCK = 20940;
   constexpr static flatlogs::eventCodeT TELEM_POLTRACK = 20945;
   constexpr static flatlogs::eventCodeT UNKNOWN = 65535;
}

inline
flatlogs::eventCodeT eventCode( const std::string & cn /**< [in] the code name to convert */)
{
    if(cn == "git_state" )
    {
        return eventCodes::GIT_STATE;
    }
    else if(cn == "text_log" )
    {
        return eventCodes::TEXT_LOG;
    }
    else if(cn == "user_log" )
    {
        return eventCodes::USER_LOG;
    }
    else if(cn == "state_change" )
    {
        return eventCodes::STATE_CHANGE;
    }
    else if(cn == "software_log" )
    {
        return eventCodes::SOFTWARE_LOG;
    }
    else if(cn == "config_log" )
    {
        return eventCodes::CONFIG_LOG;
    }
    else if(cn == "indidriver_start" )
    {
        return eventCodes::INDIDRIVER_START;
    }
    else if(cn == "indidriver_stop" )
    {
        return eventCodes::INDIDRIVER_STOP;
    }
    else if(cn == "loop_closed" )
    {
        return eventCodes::LOOP_CLOSED;
    }
    else if(cn == "loop_paused" )
    {
        return eventCodes::LOOP_PAUSED;
    }
    else if(cn == "loop_open" )
    {
        return eventCodes::LOOP_OPEN;
    }
    else if(cn == "observer" )
    {
        return eventCodes::OBSERVER;
    }
    else if(cn == "ao_operator" )
    {
        return eventCodes::AO_OPERATOR;
    }
    else if(cn == "pico_channel" )
    {
        return eventCodes::PICO_CHANNEL;
    }
    else if(cn == "outlet_state" )
    {
        return eventCodes::OUTLET_STATE;
    }
    else if(cn == "outlet_channel_state" )
    {
        return eventCodes::OUTLET_CHANNEL_STATE;
    }
    else if(cn == "telem_saving_state" )
    {
        return eventCodes::TELEM_SAVING_STATE;
    }
    else if(cn == "telem_fxngen" )
    {
        return eventCodes::TELEM_FXNGEN;
    }
    else if(cn == "ttmmod_params" )
    {
        return eventCodes::TTMMOD_PARAMS;
    }
    else if(cn == "ocam_temps" )
    {
        return eventCodes::OCAM_TEMPS;
    }
    else if(cn == "saving_start" )
    {
        return eventCodes::SAVING_START;
    }
    else if(cn == "saving_stop" )
    {
        return eventCodes::SAVING_STOP;
    }
    else if(cn == "telem_saving" )
    {
        return eventCodes::TELEM_SAVING;
    }
    else if(cn == "telem_telpos" )
    {
        return eventCodes::TELEM_TELPOS;
    }
    else if(cn == "telem_teldata" )
    {
        return eventCodes::TELEM_TELDATA;
    }
    else if(cn == "telem_telvane" )
    {
        return eventCodes::TELEM_TELVANE;
    }
    else if(cn == "telem_telenv" )
    {
        return eventCodes::TELEM_TELENV;
    }
    else if(cn == "telem_telcat" )
    {
        return eventCodes::TELEM_TELCAT;
    }
    else if(cn == "telem_telsee" )
    {
        return eventCodes::TELEM_TELSEE;
    }
    else if(cn == "telem_stage" )
    {
        return eventCodes::TELEM_STAGE;
    }
    else if(cn == "telem_zaber" )
    {
        return eventCodes::TELEM_ZABER;
    }
    else if(cn == "telem_pico" )
    {
        return eventCodes::TELEM_PICO;
    }
    else if(cn == "telem_position" )
    {
        return eventCodes::TELEM_POSITION;
    }
    else if(cn == "telem_pokecenter" )
    {
        return eventCodes::TELEM_POKECENTER;
    }
    else if(cn == "telem_pokeloop" )
    {
        return eventCodes::TELEM_POKELOOP;
    }
    else if(cn == "telem_observer" )
    {
        return eventCodes::TELEM_OBSERVER;
    }
    else if(cn == "telem_rhusb" )
    {
        return eventCodes::TELEM_RHUSB;
    }
    else if(cn == "telem_temps" )
    {
        return eventCodes::TELEM_TEMPS;
    }
    else if(cn == "telem_stdcam" )
    {
        return eventCodes::TELEM_STDCAM;
    }
    else if(cn == "telem_coretemps" )
    {
        return eventCodes::TELEM_CORETEMPS;
    }
    else if(cn == "telem_coreloads" )
    {
        return eventCodes::TELEM_CORELOADS;
    }
    else if(cn == "telem_drivetemps" )
    {
        return eventCodes::TELEM_DRIVETEMPS;
    }
    else if(cn == "telem_usage" )
    {
        return eventCodes::TELEM_USAGE;
    }
    else if(cn == "telem_cooler" )
    {
        return eventCodes::TELEM_COOLER;
    }
    else if(cn == "telem_chrony_status" )
    {
        return eventCodes::TELEM_CHRONY_STATUS;
    }
    else if(cn == "telem_chrony_stats" )
    {
        return eventCodes::TELEM_CHRONY_STATS;
    }
    else if(cn == "telem_dmspeck" )
    {
        return eventCodes::TELEM_DMSPECK;
    }
    else if(cn == "telem_fgtimings" )
    {
        return eventCodes::TELEM_FGTIMINGS;
    }
    else if(cn == "telem_dmmodes" )
    {
        return eventCodes::TELEM_DMMODES;
    }
    else if(cn == "telem_loopgain" )
    {
        return eventCodes::TELEM_LOOPGAIN;
    }
    else if(cn == "telem_blockgains" )
    {
        return eventCodes::TELEM_BLOCKGAINS;
    }
    else if(cn == "telem_offloading" )
    {
        return eventCodes::TELEM_OFFLOADING;
    }
    else if(cn == "telem_pi335" )
    {
        return eventCodes::TELEM_PI335;
    }
    else if(cn == "telem_sparkleclock" )
    {
        return eventCodes::TELEM_SPARKLECLOCK;
    }
    else if(cn == "telem_poltrack" )
    {
        return eventCodes::TELEM_POLTRACK;
    }
    else
    {
        return eventCodes::UNKNOWN;
    }
} // flatlogs::eventCodeT eventCode(const std::string &)

inline
std::string eventCodeName( flatlogs::eventCodeT ec/**< [in] the code to convert to its name */)
{
    if(ec == eventCodes::GIT_STATE )
    {
        return "git_state";
    }
    else if(ec == eventCodes::TEXT_LOG )
    {
        return "text_log";
    }
    else if(ec == eventCodes::USER_LOG )
    {
        return "user_log";
    }
    else if(ec == eventCodes::STATE_CHANGE )
    {
        return "state_change";
    }
    else if(ec == eventCodes::SOFTWARE_LOG )
    {
        return "software_log";
    }
    else if(ec == eventCodes::CONFIG_LOG )
    {
        return "config_log";
    }
    else if(ec == eventCodes::INDIDRIVER_START )
    {
        return "indidriver_start";
    }
    else if(ec == eventCodes::INDIDRIVER_STOP )
    {
        return "indidriver_stop";
    }
    else if(ec == eventCodes::LOOP_CLOSED )
    {
        return "loop_closed";
    }
    else if(ec == eventCodes::LOOP_PAUSED )
    {
        return "loop_paused";
    }
    else if(ec == eventCodes::LOOP_OPEN )
    {
        return "loop_open";
    }
    else if(ec == eventCodes::OBSERVER )
    {
        return "observer";
    }
    else if(ec == eventCodes::AO_OPERATOR )
    {
        return "ao_operator";
    }
    else if(ec == eventCodes::PICO_CHANNEL )
    {
        return "pico_channel";
    }
    else if(ec == eventCodes::OUTLET_STATE )
    {
        return "outlet_state";
    }
    else if(ec == eventCodes::OUTLET_CHANNEL_STATE )
    {
        return "outlet_channel_state";
    }
    else if(ec == eventCodes::TELEM_SAVING_STATE )
    {
        return "telem_saving_state";
    }
    else if(ec == eventCodes::TELEM_FXNGEN )
    {
        return "telem_fxngen";
    }
    else if(ec == eventCodes::TTMMOD_PARAMS )
    {
        return "ttmmod_params";
    }
    else if(ec == eventCodes::OCAM_TEMPS )
    {
        return "ocam_temps";
    }
    else if(ec == eventCodes::SAVING_START )
    {
        return "saving_start";
    }
    else if(ec == eventCodes::SAVING_STOP )
    {
        return "saving_stop";
    }
    else if(ec == eventCodes::TELEM_SAVING )
    {
        return "telem_saving";
    }
    else if(ec == eventCodes::TELEM_TELPOS )
    {
        return "telem_telpos";
    }
    else if(ec == eventCodes::TELEM_TELDATA )
    {
        return "telem_teldata";
    }
    else if(ec == eventCodes::TELEM_TELVANE )
    {
        return "telem_telvane";
    }
    else if(ec == eventCodes::TELEM_TELENV )
    {
        return "telem_telenv";
    }
    else if(ec == eventCodes::TELEM_TELCAT )
    {
        return "telem_telcat";
    }
    else if(ec == eventCodes::TELEM_TELSEE )
    {
        return "telem_telsee";
    }
    else if(ec == eventCodes::TELEM_STAGE )
    {
        return "telem_stage";
    }
    else if(ec == eventCodes::TELEM_ZABER )
    {
        return "telem_zaber";
    }
    else if(ec == eventCodes::TELEM_PICO )
    {
        return "telem_pico";
    }
    else if(ec == eventCodes::TELEM_POSITION )
    {
        return "telem_position";
    }
    else if(ec == eventCodes::TELEM_POKECENTER )
    {
        return "telem_pokecenter";
    }
    else if(ec == eventCodes::TELEM_POKELOOP )
    {
        return "telem_pokeloop";
    }
    else if(ec == eventCodes::TELEM_OBSERVER )
    {
        return "telem_observer";
    }
    else if(ec == eventCodes::TELEM_RHUSB )
    {
        return "telem_rhusb";
    }
    else if(ec == eventCodes::TELEM_TEMPS )
    {
        return "telem_temps";
    }
    else if(ec == eventCodes::TELEM_STDCAM )
    {
        return "telem_stdcam";
    }
    else if(ec == eventCodes::TELEM_CORETEMPS )
    {
        return "telem_coretemps";
    }
    else if(ec == eventCodes::TELEM_CORELOADS )
    {
        return "telem_coreloads";
    }
    else if(ec == eventCodes::TELEM_DRIVETEMPS )
    {
        return "telem_drivetemps";
    }
    else if(ec == eventCodes::TELEM_USAGE )
    {
        return "telem_usage";
    }
    else if(ec == eventCodes::TELEM_COOLER )
    {
        return "telem_cooler";
    }
    else if(ec == eventCodes::TELEM_CHRONY_STATUS )
    {
        return "telem_chrony_status";
    }
    else if(ec == eventCodes::TELEM_CHRONY_STATS )
    {
        return "telem_chrony_stats";
    }
    else if(ec == eventCodes::TELEM_DMSPECK )
    {
        return "telem_dmspeck";
    }
    else if(ec == eventCodes::TELEM_FGTIMINGS )
    {
        return "telem_fgtimings";
    }
    else if(ec == eventCodes::TELEM_DMMODES )
    {
        return "telem_dmmodes";
    }
    else if(ec == eventCodes::TELEM_LOOPGAIN )
    {
        return "telem_loopgain";
    }
    else if(ec == eventCodes::TELEM_BLOCKGAINS )
    {
        return "telem_blockgains";
    }
    else if(ec == eventCodes::TELEM_OFFLOADING )
    {
        return "telem_offloading";
    }
    else if(ec == eventCodes::TELEM_PI335 )
    {
        return "telem_pi335";
    }
    else if(ec == eventCodes::TELEM_SPARKLECLOCK )
    {
        return "telem_sparkleclock";
    }
    else if(ec == eventCodes::TELEM_POLTRACK )
    {
        return "telem_poltrack";
    }
    else
    {
        return "unknown event code";
    }
} // std::string eventCodeName(flatlogs::eventCodeT)

}
}
#endif
