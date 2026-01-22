/** \file logPluginLoader.hpp
  * \brief Plugin loader for external telemetry schemas
  * \ingroup logger
  *
  * Discovers and loads telemetry schema plugins from standard paths.
  */

#ifndef XWCTK_LOGGER_PLUGINLOADER_HPP
#define XWCTK_LOGGER_PLUGINLOADER_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "plugin.hpp"

namespace MagAOX {
namespace logger {

/** \brief Plugin loader for discovering and managing telemetry schema plugins
  *
  * Scans known directories for .so files, loads compatible plugins,
  * and maintains a registry of all available telemetry types.
  *
  * \ingroup logger
  */
class LogPluginLoader
{
public:
    /** \brief Loaded plugin handle
      */
    struct LoadedPlugin
    {
        void* dl_handle;                      ///< dlopen handle
        LogSchemaPlugin* plugin;              ///< Plugin instance
        std::string path;                     ///< Full path to .so file
        
        LoadedPlugin() : dl_handle(nullptr), plugin(nullptr) {}
        ~LoadedPlugin();
        
        // Prevent copying (you can't duplicate a dlhandle)
        LoadedPlugin(const LoadedPlugin&) = delete;
        LoadedPlugin& operator=(const LoadedPlugin&) = delete;
        
        // Allow moving
        LoadedPlugin(LoadedPlugin&& other) noexcept;
        LoadedPlugin& operator=(LoadedPlugin&& other) noexcept;
    };
    
private:
    /// Map of log code -> (plugin, handler)
    std::map<uint32_t, std::pair<LoadedPlugin*, const LogSchemaTypeHandler*>> m_typeRegistry;
    
    /// All loaded plugins
    std::vector<std::unique_ptr<LoadedPlugin>> m_plugins;
    
    /// Search paths (in priority order)
    std::vector<std::string> m_searchPaths;
    
public:
    LogPluginLoader();
    ~LogPluginLoader();
    
    // Prevent copying
    LogPluginLoader(const LogPluginLoader&) = delete;
    LogPluginLoader& operator=(const LogPluginLoader&) = delete;
    
    /** \brief Add a directory to the plugin search path
      * 
      * \param path Directory containing .so plugin files
      */
    void addSearchPath(const std::string& path);
    
    /** \brief Discover and load all plugins from search paths
      * 
      * Scans directories in order, loads compatible .so files,
      * verifies version, and registers all types.
      * 
      * \returns Number of plugins successfully loaded
      */
    int loadPlugins();
    
    /** \brief Load a specific plugin file
      * 
      * \param path Full path to .so file
      * 
      * \returns 0 on success, -1 on failure
      */
    int loadPlugin(const std::string& path);
    
    /** \brief Get type handler by log code
      * 
      * \param code Log code to look up
      * 
      * \returns Pointer to handler, nullptr if not found
      */
    const LogSchemaTypeHandler* getTypeHandler(uint32_t code) const;
    
    /** \brief Check if a type is registered
      * 
      * \param code Log code to check
      * 
      * \returns true if type is registered
      */
    bool hasType(uint32_t code) const;
    
    /** \brief Get number of loaded plugins
      * 
      * \returns Plugin count
      */
    size_t numPlugins() const { return m_plugins.size(); }
    
    /** \brief Get number of registered types
      * 
      * \returns Type count
      */
    size_t numTypes() const { return m_typeRegistry.size(); }
    
    /** \brief Get list of all registered log codes
      * 
      * \returns Vector of log codes
      */
    std::vector<uint32_t> getRegisteredCodes() const;
    
    /** \brief Unload all plugins and clear registry
      */
    void unloadAll();

private:
    /** \brief Discover .so files in a directory
      * 
      * \param path Directory to scan
      * 
      * \returns Vector of full paths to .so files
      */
    std::vector<std::string> discoverPluginFiles(const std::string& path) const;
    
    /** \brief Register all types from a loaded plugin
      * 
      * \param loaded LoadedPlugin containing the plugin instance
      * 
      * \returns Number of types registered
      */
    int registerPluginTypes(LoadedPlugin* loaded);
};

} // namespace logger
} // namespace MagAOX

#endif // XWCTK_LOGGER_PLUGINLOADER_HPP
