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
   constexpr static flatlogs::eventCodeT TELEM_PI335 = 20930;
}
}
}
#endif
