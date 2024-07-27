#ifndef logger_logCodeValid_hpp
#define logger_logCodeValid_hpp
#include <flatlogs/flatlogs.hpp>
#include "logTypes.hpp"
namespace MagAOX
{
namespace logger
{
inline bool logCodeValid( flatlogs::eventCodeT ec)
{
   switch(ec)
   {
      case 0:
         return true;
      case 10:
         return true;
      case 12:
         return true;
      case 20:
         return true;
      case 50:
         return true;
      case 60:
         return true;
      case 140:
         return true;
      case 141:
         return true;
      case 1001:
         return true;
      case 1002:
         return true;
      case 1003:
         return true;
      case 1050:
         return true;
      case 11025:
         return true;
      case 12001:
         return true;
      case 12002:
         return true;
      case 12053:
         return true;
      case 12100:
         return true;
      case 12110:
         return true;
      case 12120:
         return true;
      case 12500:
         return true;
      case 12501:
         return true;
      case 12502:
         return true;
      case 20000:
         return true;
      case 20001:
         return true;
      case 20002:
         return true;
      case 20003:
         return true;
      case 20004:
         return true;
      case 20005:
         return true;
      case 20050:
         return true;
      case 20055:
         return true;
      case 20060:
         return true;
      case 20065:
         return true;
      case 20067:
         return true;
      case 20068:
         return true;
      case 20070:
         return true;
      case 20100:
         return true;
      case 20250:
         return true;
      case 20260:
         return true;
      case 20825:
         return true;
      case 20826:
         return true;
      case 20827:
         return true;
      case 20828:
         return true;
      case 20850:
         return true;
      case 20860:
         return true;
      case 20861:
         return true;
      case 20890:
         return true;
      case 20905:
         return true;
      case 20910:
         return true;
      case 20915:
         return true;
      case 20920:
         return true;
      case 20930:
         return true;
      default:
         return false;
   }
}
}
}
#endif
