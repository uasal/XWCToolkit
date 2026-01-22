/** \file logPluginFormat.hpp
  * \brief Plugin-aware log formatting functions
  * \ingroup logger
  *
  * Provides formatting functions that first try built-in types,
  * then fall back to plugins for external instrument telemetry.
  */

#ifndef XWCTK_LOGGER_LOGPLUGINFORMAT_HPP
#define XWCTK_LOGGER_LOGPLUGINFORMAT_HPP

#include <flatlogs/flatlogs.hpp>
#include "logPluginLoader.hpp"
#include "generated/logStdFormat.hpp"
#include "generated/logCodeValid.hpp"

namespace MagAOX
{
namespace logger
{

/** \brief Global plugin loader instance
  *
  * This singleton manages all loaded telemetry plugins.
  * Uses lazy initialization - plugins are loaded on first access.
  * 
  * \ingroup logger
  */
inline LogPluginLoader& getPluginLoader()
{
    static LogPluginLoader instance;
    static bool initialized = false;
    
    if (!initialized) {
        instance.loadPlugins();
        initialized = true;
    }
    
    return instance;
}

/** \brief Format plugin telemetry data to stream
  * 
  * Helper function to format plugin-handled telemetry.
  * 
  * \tparam iosT Output stream type
  * \param ios Output stream
  * \param buffer Log buffer
  * \param handler Plugin type handler
  * \returns Reference to the output stream
  */
template<class iosT>
iosT& formatPluginData(iosT& ios, flatlogs::bufferPtrT& buffer, 
                        const LogSchemaTypeHandler* handler)
{
    const uint8_t* msgData = reinterpret_cast<const uint8_t*>(
        flatlogs::logHeader::messageBuffer(buffer));
    flatlogs::msgLenT msgLen = flatlogs::logHeader::msgLen(buffer);
    
    flatlogs::timespecX ts = flatlogs::logHeader::timespec(buffer);
    flatlogs::logPrioT lvl = flatlogs::logHeader::logLevel(buffer);
    
    ios << ts.ISO8601DateTimeStrX() << " ";
    ios << flatlogs::priorityString(lvl) << " ";
    ios << "[" << handler->typeName() << "] ";
    
    void* obj = handler->deserialize(msgData, msgLen);
    if (obj) {
        ios << "(plugin data, " << msgLen << " bytes)";
        handler->deleteDeserialized(obj);
    } else {
        ios << "(failed to deserialize, " << msgLen << " bytes)";
    }
    
    return ios;
}

/** \brief Format a log entry using built-in types, falling back to plugins
  *
  * First checks if the event code is a known built-in type.
  * If so, uses the generated logStdFormat function.
  * If not, falls back to plugin handlers for external instrument telemetry.
  *
  * \tparam iosT Output stream type
  * \param ios Output stream
  * \param buffer Log buffer to format
  * \returns Reference to the output stream
  */
template<class iosT>
iosT& logPluginStdFormat(iosT& ios, flatlogs::bufferPtrT& buffer)
{
    flatlogs::eventCodeT ec = flatlogs::logHeader::eventCode(buffer);
    
    // Check if this is a known built-in type first
    if (logCodeValid(ec)) {
        return logStdFormat(ios, buffer);
    }
    
    // Not a built-in type - check plugins
    auto& loader = getPluginLoader();
    const LogSchemaTypeHandler* handler = loader.getTypeHandler(ec);
    
    if (handler) {
        return formatPluginData(ios, buffer, handler);
    }
    
    // Unknown type - neither built-in nor plugin
    ios << "Unknown log type: " << ec << "\n";
    return ios;
}

/** \brief Format a log entry in short format using built-in types or plugins
  *
  * First checks if the event code is a known built-in type.
  * If so, uses the generated logShortStdFormat function.
  * If not, falls back to plugin handlers.
  *
  * \tparam iosT Output stream type
  * \param ios Output stream
  * \param appName Application name to include
  * \param buffer Log buffer to format
  * \returns Reference to the output stream
  */
template<class iosT>
iosT& logPluginShortFormat(iosT& ios, const std::string& appName, 
                            flatlogs::bufferPtrT& buffer)
{
    flatlogs::eventCodeT ec = flatlogs::logHeader::eventCode(buffer);
    
    // Check if this is a known built-in type first
    if (logCodeValid(ec)) {
        return logShortStdFormat(ios, appName, buffer);
    }
    
    // Not a built-in type - check plugins
    auto& loader = getPluginLoader();
    const LogSchemaTypeHandler* handler = loader.getTypeHandler(ec);
    
    if (handler) {
        const uint8_t* msgData = reinterpret_cast<const uint8_t*>(
            flatlogs::logHeader::messageBuffer(buffer));
        flatlogs::msgLenT msgLen = flatlogs::logHeader::msgLen(buffer);
        flatlogs::timespecX ts = flatlogs::logHeader::timespec(buffer);
        
        ios << ts.ISO8601DateTimeStrX() << " ";
        ios << appName << " ";
        ios << handler->typeName();
        
        void* obj = handler->deserialize(msgData, msgLen);
        if (obj) {
            ios << " (ok)";
            handler->deleteDeserialized(obj);
        }
        
        return ios;
    }
    
    // Unknown type
    ios << "Unknown log type: " << ec << "\n";
    return ios;
}

/** \brief Format a log entry in minimal format using built-in types or plugins
  *
  * First checks if the event code is a known built-in type.
  * If so, uses the generated logMinStdFormat function.
  * If not, falls back to plugin handlers.
  *
  * \tparam iosT Output stream type
  * \param ios Output stream
  * \param buffer Log buffer to format
  * \returns Reference to the output stream
  */
template<class iosT>
iosT& logPluginMinFormat(iosT& ios, flatlogs::bufferPtrT& buffer)
{
    flatlogs::eventCodeT ec = flatlogs::logHeader::eventCode(buffer);
    
    // Check if this is a known built-in type first
    if (logCodeValid(ec)) {
        return logMinStdFormat(ios, buffer);
    }
    
    // Not a built-in type - check plugins
    auto& loader = getPluginLoader();
    const LogSchemaTypeHandler* handler = loader.getTypeHandler(ec);
    
    if (handler) {
        const uint8_t* msgData = reinterpret_cast<const uint8_t*>(
            flatlogs::logHeader::messageBuffer(buffer));
        flatlogs::msgLenT msgLen = flatlogs::logHeader::msgLen(buffer);
        
        ios << handler->typeName() << ": ";
        
        void* obj = handler->deserialize(msgData, msgLen);
        if (obj) {
            ios << msgLen << " bytes";
            handler->deleteDeserialized(obj);
        } else {
            ios << "error";
        }
        
        return ios;
    }
    
    // Unknown type
    ios << "Unknown log type: " << ec << "\n";
    return ios;
}

/** \brief Format a log entry as JSON using built-in types or plugins
  *
  * First checks if the event code is a known built-in type.
  * If so, uses the generated logJsonFormat function.
  * If not, falls back to plugin handlers.
  *
  * \tparam iosT Output stream type
  * \param ios Output stream
  * \param buffer Log buffer to format
  * \returns Reference to the output stream
  */
template<class iosT>
iosT& logPluginJsonFormat(iosT& ios, flatlogs::bufferPtrT& buffer)
{
    flatlogs::eventCodeT ec = flatlogs::logHeader::eventCode(buffer);
    
    // Check if this is a known built-in type first
    if (logCodeValid(ec)) {
        return logJsonFormat(ios, buffer);
    }
    
    // Not a built-in type - check plugins
    auto& loader = getPluginLoader();
    const LogSchemaTypeHandler* handler = loader.getTypeHandler(ec);
    
    if (handler) {
        const uint8_t* msgData = reinterpret_cast<const uint8_t*>(
            flatlogs::logHeader::messageBuffer(buffer));
        flatlogs::msgLenT msgLen = flatlogs::logHeader::msgLen(buffer);
        flatlogs::timespecX ts = flatlogs::logHeader::timespec(buffer);
        flatlogs::logPrioT lvl = flatlogs::logHeader::logLevel(buffer);
        
        // Output basic JSON structure for plugin types
        ios << "{";
        ios << "\"type\":\"" << handler->typeName() << "\",";
        ios << "\"code\":" << ec << ",";
        ios << "\"ts\":\"" << ts.ISO8601DateTimeStrX() << "\",";
        ios << "\"level\":\"" << flatlogs::priorityString(lvl) << "\",";
        
        void* obj = handler->deserialize(msgData, msgLen);
        if (obj) {
            ios << "\"size\":" << msgLen << ",";
            ios << "\"status\":\"ok\"";
            handler->deleteDeserialized(obj);
        } else {
            ios << "\"size\":" << msgLen << ",";
            ios << "\"status\":\"error\"";
        }
        
        ios << "}";
        return ios;
    }
    
    // Unknown type - output error JSON
    ios << "{\"error\":\"Unknown log type\",\"code\":" << ec << "}";
    return ios;
}

/** \brief Check if an event code is handled (built-in or plugin)
  *
  * \param ec Event code to check
  * \returns true if the code can be formatted
  */
inline bool logCodeValidWithPlugins(flatlogs::eventCodeT ec)
{
    // Check built-in first
    if (logCodeValid(ec)) {
        return true;
    }
    
    // Check plugins
    auto& loader = getPluginLoader();
    return loader.hasType(ec);
}

} // namespace logger
} // namespace MagAOX

#endif // XWCTK_LOGGER_LOGPLUGINFORMAT_HPP
