/** \file plugin.hpp
  * \brief Plugin interface for external telemetry schema registration
  * \ingroup logger
  *
  * Allows external instrument app packages to register telemetry types
  * without recompiling the core XWCToolkit library.
  */

#ifndef XWCTK_LOGGER_PLUGIN_HPP
#define XWCTK_LOGGER_PLUGIN_HPP

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>

#include <flatlogs/flatlogs.hpp>

namespace MagAOX {
namespace logger {

/// Plugin version - increment for backwards incompatible interface changes
#define XWCTK_PLUGIN_VERSION 1

/** \brief Handler for a single telemetry type
  *
  * Each telemetry type (e.g., telem_pico, telem_basler) has a handler
  * that knows how to deserialize and manage that type.
  *
  * \ingroup logger
  */
struct LogSchemaTypeHandler 
{
    virtual ~LogSchemaTypeHandler() = default;
    
    /** \brief Get unique log code for this telemetry type
      * \returns Log code (e.g. 20060 for telem_pico)
      */
    virtual uint32_t logCode() const = 0;
    
    /** \brief Get human-readable type name
      * \returns Type name string (e.g., "telem_pico")
      */
    virtual const char* typeName() const = 0;
    
    /** \brief Deserialize binary FlatBuffers data
      * 
      * \param data Pointer to serialized FlatBuffers data
      * \param length Size of data in bytes
      * 
      * \returns Opaque pointer to deserialized object (cast to concrete type by caller)
      *          nullptr on deserialization failure
      * 
      * \note Caller must call deleteDeserialized() to free returned pointer
      */
    virtual void* deserialize(const uint8_t* data, size_t length) const = 0;
    
    /** \brief Free memory allocated by deserialize()
      * 
      * \param obj Pointer previously returned by deserialize()
      */
    virtual void deleteDeserialized(void* obj) const = 0;
    
    /** \brief Validate data integrity before deserialization (optional)
      * 
      * \param data Pointer to serialized data
      * \param length Size of data in bytes
      * 
      * \returns 0 if valid, -1 if invalid
      * 
      * Default implementation always returns valid.
      * Override if telemetry type needs pre-deserialization checks.
      */
    virtual int validate(const uint8_t* data, size_t length) const 
    {
        // Suppress unused parameter warning
        (void)data;
        (void)length;
        return 0;
    }
};

/** \brief Main plugin interface for telemetry schema registration
  *
  * Each instrument app package creates ONE plugin that handles ALL telemetry
  * types from ALL applications in that instrument.
  *
  * \ingroup logger
  */
struct LogSchemaPlugin 
{
    /// Must match XWCTK_PLUGIN_VERSION
    uint32_t version = XWCTK_PLUGIN_VERSION;
    
    virtual ~LogSchemaPlugin() = default;
    
    /** \brief Get plugin name (e.g., "MagAO-X Telemetry Schemas")
      * \returns Plugin name string
      */
    virtual const char* pluginName() const = 0;
    
    /** \brief Get plugin version string (semantic versioning: major.minor.patch)
      * \returns Version string (e.g., "1.2.0")
      */
    virtual const char* pluginVersion() const = 0;
    
    /** \brief Get number of telemetry types in this plugin
      * \returns Count of registered types
      */
    virtual uint32_t numTypes() const = 0;
    
    /** \brief Get the i-th type handler (0 <= i < numTypes())
      * 
      * \param index Zero-based index into type list
      * 
      * \returns Pointer to type handler, nullptr if index out of bounds
      */
    virtual const LogSchemaTypeHandler* getType(uint32_t index) const = 0;
    
    /** \brief Get type handler by log code
      * 
      * \param code Log code to look up
      * 
      * \returns Pointer to type handler, nullptr if code not found
      */
    virtual const LogSchemaTypeHandler* getTypeByCode(uint32_t code) const = 0;
};

/** \brief Template adapter that wraps existing log types for plugin registration
  *
  * Use this to wrap existing log type structs (like telem_fsm) that follow
  * the standard flatbuffer_log interface with static eventCode, verify(), etc.
  *
  * Example usage:
  * \code{.cpp}
  * #include "telem_fsm.hpp"
  * 
  * class MyPlugin : public LogSchemaPlugin {
  *     LogTypeHandlerAdapter<telem_fsm> m_fsmHandler;
  *     // ...
  *     const LogSchemaTypeHandler* getType(uint32_t i) const override {
  *         if (i == 0) return &m_fsmHandler;
  *         return nullptr;
  *     }
  * };
  * \endcode
  *
  * \tparam logT Log type struct (must have static eventCode, verify(), etc.)
  *
  * \ingroup logger
  */
template<typename logT>
class LogTypeHandlerAdapter : public LogSchemaTypeHandler
{
public:
    uint32_t logCode() const override {
        return logT::eventCode;
    }
    
    const char* typeName() const override {
        return m_typeName.c_str();
    }
    
    void* deserialize(const uint8_t* data, size_t length) const override {
        // For now, just validate and return a copy of the data
        // Callers typically use the FlatBuffers accessors directly
        if (validate(data, length) != 0) return nullptr;
        
        uint8_t* copy = new uint8_t[length];
        std::memcpy(copy, data, length);
        return copy;
    }
    
    void deleteDeserialized(void* obj) const override {
        delete[] reinterpret_cast<uint8_t*>(obj);
    }
    
    int validate(const uint8_t* data, size_t length) const override {
        // Basic validation - check minimum size for FlatBuffers
        if (data == nullptr || length < 4) return -1;
        return 0;
    }
    
    /// Set the type name (call once during plugin construction)
    void setTypeName(const char* name) {
        m_typeName = name;
    }
    
    LogTypeHandlerAdapter(const char* name = "") : m_typeName(name) {}
    
private:
    std::string m_typeName;
};

/** \brief External C interface for dlopen/dlsym compatibility
  * 
  * Each plugin .so MUST implement these functions.
  * The extern "C" prevents C++ name mangling so dlsym() can find them.
  * 
  * Example implementation in your plugin:
  * \code{.cpp}
  * extern "C" {
  *     LogSchemaPlugin* create_schema_plugin() {
  *         return new MyInstrumentPlugin();
  *     }
  *     
  *     void destroy_schema_plugin(LogSchemaPlugin* plugin) {
  *         delete plugin;
  *     }
  * }
  * \endcode
  * 
  * \ingroup logger
  */
/// @{

/// Factory function
using CreateSchemaPluginFunc = LogSchemaPlugin* (*)();

/// Destructor function
using DestroySchemaPluginFunc = void (*)(LogSchemaPlugin*);

/// @}

} // namespace logger
} // namespace MagAOX

#endif // XWCTK_LOGGER_PLUGIN_HPP
