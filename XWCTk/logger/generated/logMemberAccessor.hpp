#ifndef logger_logMemberAccessor_hpp
#define logger_logMemberAccessor_hpp
#include <flatlogs/flatlogs.hpp>
#include "logTypes.hpp"
namespace MagAOX
{
namespace logger
{

inline
logMetaDetail logMemberAccessor( flatlogs::eventCodeT ec,
                                 const std::string & memberName
                               )
{
    switch(ec)
    {
        case eventCodes::GIT_STATE:
            return git_state::getAccessor( memberName );
        case eventCodes::TEXT_LOG:
            return text_log::getAccessor( memberName );
        case eventCodes::USER_LOG:
            return user_log::getAccessor( memberName );
        case eventCodes::STATE_CHANGE:
            return state_change::getAccessor( memberName );
        case eventCodes::SOFTWARE_LOG:
            return software_log::getAccessor( memberName );
        case eventCodes::CONFIG_LOG:
            return config_log::getAccessor( memberName );
        case eventCodes::INDIDRIVER_START:
            return indidriver_start::getAccessor( memberName );
        case eventCodes::INDIDRIVER_STOP:
            return indidriver_stop::getAccessor( memberName );
        case eventCodes::LOOP_CLOSED:
            return loop_closed::getAccessor( memberName );
        case eventCodes::LOOP_PAUSED:
            return loop_paused::getAccessor( memberName );
        case eventCodes::LOOP_OPEN:
            return loop_open::getAccessor( memberName );
        case eventCodes::OBSERVER:
            return observer::getAccessor( memberName );
        case eventCodes::AO_OPERATOR:
            return ao_operator::getAccessor( memberName );
        case eventCodes::PICO_CHANNEL:
            return pico_channel::getAccessor( memberName );
        case eventCodes::OUTLET_STATE:
            return outlet_state::getAccessor( memberName );
        case eventCodes::OUTLET_CHANNEL_STATE:
            return outlet_channel_state::getAccessor( memberName );
        case eventCodes::TELEM_SAVING_STATE:
            return telem_saving_state::getAccessor( memberName );
        case eventCodes::TELEM_FXNGEN:
            return telem_fxngen::getAccessor( memberName );
        case eventCodes::TTMMOD_PARAMS:
            return ttmmod_params::getAccessor( memberName );
        case eventCodes::OCAM_TEMPS:
            return ocam_temps::getAccessor( memberName );
        case eventCodes::SAVING_START:
            return saving_start::getAccessor( memberName );
        case eventCodes::SAVING_STOP:
            return saving_stop::getAccessor( memberName );
        case eventCodes::TELEM_SAVING:
            return telem_saving::getAccessor( memberName );
        case eventCodes::TELEM_TELPOS:
            return telem_telpos::getAccessor( memberName );
        case eventCodes::TELEM_TELDATA:
            return telem_teldata::getAccessor( memberName );
        case eventCodes::TELEM_TELVANE:
            return telem_telvane::getAccessor( memberName );
        case eventCodes::TELEM_TELENV:
            return telem_telenv::getAccessor( memberName );
        case eventCodes::TELEM_TELCAT:
            return telem_telcat::getAccessor( memberName );
        case eventCodes::TELEM_TELSEE:
            return telem_telsee::getAccessor( memberName );
        case eventCodes::TELEM_STAGE:
            return telem_stage::getAccessor( memberName );
        case eventCodes::TELEM_ZABER:
            return telem_zaber::getAccessor( memberName );
        case eventCodes::TELEM_PICO:
            return telem_pico::getAccessor( memberName );
        case eventCodes::TELEM_POSITION:
            return telem_position::getAccessor( memberName );
        case eventCodes::TELEM_POKECENTER:
            return telem_pokecenter::getAccessor( memberName );
        case eventCodes::TELEM_POKELOOP:
            return telem_pokeloop::getAccessor( memberName );
        case eventCodes::TELEM_OBSERVER:
            return telem_observer::getAccessor( memberName );
        case eventCodes::TELEM_RHUSB:
            return telem_rhusb::getAccessor( memberName );
        case eventCodes::TELEM_TEMPS:
            return telem_temps::getAccessor( memberName );
        case eventCodes::TELEM_STDCAM:
            return telem_stdcam::getAccessor( memberName );
        case eventCodes::TELEM_CORETEMPS:
            return telem_coretemps::getAccessor( memberName );
        case eventCodes::TELEM_CORELOADS:
            return telem_coreloads::getAccessor( memberName );
        case eventCodes::TELEM_DRIVETEMPS:
            return telem_drivetemps::getAccessor( memberName );
        case eventCodes::TELEM_USAGE:
            return telem_usage::getAccessor( memberName );
        case eventCodes::TELEM_COOLER:
            return telem_cooler::getAccessor( memberName );
        case eventCodes::TELEM_CHRONY_STATUS:
            return telem_chrony_status::getAccessor( memberName );
        case eventCodes::TELEM_CHRONY_STATS:
            return telem_chrony_stats::getAccessor( memberName );
        case eventCodes::TELEM_DMSPECK:
            return telem_dmspeck::getAccessor( memberName );
        case eventCodes::TELEM_FGTIMINGS:
            return telem_fgtimings::getAccessor( memberName );
        case eventCodes::TELEM_DMMODES:
            return telem_dmmodes::getAccessor( memberName );
        case eventCodes::TELEM_LOOPGAIN:
            return telem_loopgain::getAccessor( memberName );
        case eventCodes::TELEM_BLOCKGAINS:
            return telem_blockgains::getAccessor( memberName );
        case eventCodes::TELEM_OFFLOADING:
            return telem_offloading::getAccessor( memberName );
        case eventCodes::TELEM_PI335:
            return telem_pi335::getAccessor( memberName );
        case eventCodes::TELEM_SPARKLECLOCK:
            return telem_sparkleclock::getAccessor( memberName );
        case eventCodes::TELEM_POLTRACK:
            return telem_poltrack::getAccessor( memberName );
        default:
            std::cerr << "Missing logMemberAccessor case entry for " << ec << ": " << memberName << '\n';
            return logMetaDetail();
    }
}
}
}
#endif
