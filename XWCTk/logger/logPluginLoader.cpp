/** \file logPluginLoader.cpp
  * \brief Plugin loader implementation
  * \ingroup logger
  */

#include "logPluginLoader.hpp"
#include "../common/paths.hpp"

#include <dlfcn.h>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include <iostream>
#include <algorithm>

namespace MagAOX {
namespace logger {


LogPluginLoader::LoadedPlugin::~LoadedPlugin()
{
    if (plugin) {
        // Call plugin's destroy function if available
        void* sym = dlsym(dl_handle, "destroy_schema_plugin");
        if (sym) {
            auto destroy_fn = reinterpret_cast<void(*)(LogSchemaPlugin*)>(sym);
            destroy_fn(plugin);
        }
        plugin = nullptr;
    }
    
    if (dl_handle) {
        dlclose(dl_handle);
        dl_handle = nullptr;
    }
}

LogPluginLoader::LoadedPlugin::LoadedPlugin(LoadedPlugin&& other) noexcept :
    dl_handle(other.dl_handle),
    plugin(other.plugin),
    path(std::move(other.path))
{
    other.dl_handle = nullptr;
    other.plugin = nullptr;
}

LogPluginLoader::LoadedPlugin& LogPluginLoader::LoadedPlugin::operator=(LoadedPlugin&& other) noexcept
{
    if (this != &other) {
        // Clean up existing resources
        this->~LoadedPlugin();
        
        // Move from other
        dl_handle = other.dl_handle;
        plugin = other.plugin;
        path = std::move(other.path);
        
        other.dl_handle = nullptr;
        other.plugin = nullptr;
    }
    return *this;
}


LogPluginLoader::LogPluginLoader()
{
    // Add default search paths in priority order
    
    // 1. Environment variable override
    const char* env_path = getenv("XWCTK_PLUGIN_PATH");
    if (env_path) {
        addSearchPath(env_path);
    }
    
    // 2. Compile-time configured path (defaults to what's set in common/paths.hpp)
#ifdef XWCTK_PLUGIN_PATH
    addSearchPath(XWCTK_PLUGIN_PATH);
#else
    // 3. Default fallback path
    std::string default_path = std::string(MAGAOX_path) + "/" + MAGAOX_pluginsRelPath;
    addSearchPath(default_path);
#endif
}

LogPluginLoader::~LogPluginLoader()
{
    unloadAll();
}

void LogPluginLoader::addSearchPath(const std::string& path)
{
    // Check if path already exists
    if (std::find(m_searchPaths.begin(), m_searchPaths.end(), path) == m_searchPaths.end()) {
        m_searchPaths.push_back(path);
    }
}

int LogPluginLoader::loadPlugins()
{
    int loaded_count = 0;
    
    for (const auto& search_path : m_searchPaths) {
        std::vector<std::string> plugin_files = discoverPluginFiles(search_path);
        
        for (const auto& file_path : plugin_files) {
            if (loadPlugin(file_path) == 0) {
                loaded_count++;
            }
        }
    }
    
    return loaded_count;
}

int LogPluginLoader::loadPlugin(const std::string& path)
{
    // Open the shared library
    void* handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (!handle) {
        std::cerr << __FILE__ << " " << __LINE__ << " Failed to load plugin " << path << ": " << dlerror() << std::endl;
        return -1;
    }
    
    // Clear any existing error
    dlerror();
    
    // Look up the factory function
    void* create_sym = dlsym(handle, "create_schema_plugin");
    const char* dlsym_error = dlerror();
    if (dlsym_error || !create_sym) {
        std::cerr << __FILE__ << " " << __LINE__ << " Plugin " << path << " missing create_schema_plugin(): " 
                  << (dlsym_error ? dlsym_error : "symbol not found") << std::endl;
        dlclose(handle);
        return -1;
    }
    
    // Cast to function pointer and call
    auto create_fn = reinterpret_cast<LogSchemaPlugin*(*)()>(create_sym);
    LogSchemaPlugin* plugin = create_fn();
    
    if (!plugin) {
        std::cerr << __FILE__ << " " << __LINE__ << " Plugin " << path << " create_schema_plugin() returned nullptr" << std::endl;
        dlclose(handle);
        return -1;
    }
    
    // Verify version
    if (plugin->version != XWCTK_PLUGIN_VERSION) {
        std::cerr << __FILE__ << " " << __LINE__ << " Plugin " << path << " version mismatch: "
                  << "plugin=" << plugin->version 
                  << ", expected=" << XWCTK_PLUGIN_VERSION << std::endl;
        
        // Try to clean up
        void* destroy_sym = dlsym(handle, "destroy_schema_plugin");
        if (destroy_sym) {
            auto destroy_fn = reinterpret_cast<void(*)(LogSchemaPlugin*)>(destroy_sym);
            destroy_fn(plugin);
        }
        dlclose(handle);
        return -1;
    }
    
    // Create LoadedPlugin wrapper
    auto loaded = std::make_unique<LoadedPlugin>();
    loaded->dl_handle = handle;
    loaded->plugin = plugin;
    loaded->path = path;
    
    // Register all types from this plugin
    int type_count = registerPluginTypes(loaded.get());
    
    std::cout << "Loaded plugin: " << plugin->pluginName() 
              << " v" << plugin->pluginVersion()
              << " (" << type_count << " types)" << std::endl;
    
    m_plugins.push_back(std::move(loaded));
    return 0;
}

const LogSchemaTypeHandler* LogPluginLoader::getTypeHandler(uint32_t code) const
{
    auto it = m_typeRegistry.find(code);
    if (it != m_typeRegistry.end()) {
        return it->second.second;
    }
    return nullptr;
}

bool LogPluginLoader::hasType(uint32_t code) const
{
    return m_typeRegistry.find(code) != m_typeRegistry.end();
}

std::vector<uint32_t> LogPluginLoader::getRegisteredCodes() const
{
    std::vector<uint32_t> codes;
    codes.reserve(m_typeRegistry.size());
    
    for (const auto& entry : m_typeRegistry) {
        codes.push_back(entry.first);
    }
    
    return codes;
}

void LogPluginLoader::unloadAll()
{
    m_typeRegistry.clear();
    m_plugins.clear();
}

std::vector<std::string> LogPluginLoader::discoverPluginFiles(const std::string& path) const
{
    std::vector<std::string> files;
    
    DIR* dir = opendir(path.c_str());
    if (!dir) {
        std::cerr << __FILE__ << " " << __LINE__ 
             << " Failed to open plugin directory " << path 
             << ": " << strerror(errno) << std::endl;
        return files;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string filename(entry->d_name);
        
        // Skip . and ..
        if (filename == "." || filename == "..") {
            continue;
        }
        
        std::string full_path = path + "/" + filename;
        struct stat st;
        if (stat(full_path.c_str(), &st) != 0) {
            continue;
        }
        
        // Look for .so files in current directory
        if (S_ISREG(st.st_mode) && filename.size() > 3 && 
            filename.substr(filename.size() - 3) == ".so") {
            files.push_back(full_path);
        }
        // Also recurse into subdirectories (for instrument packages)
        // e.g., /opt/MagAOX/lib/xwctk-plugins/myinstrument/libmyinstrument_schemas.so
        else if (S_ISDIR(st.st_mode)) {
            auto subdir_files = discoverPluginFiles(full_path);
            files.insert(files.end(), subdir_files.begin(), subdir_files.end());
        }
    }
    
    closedir(dir);
    return files;
}

int LogPluginLoader::registerPluginTypes(LoadedPlugin* loaded)
{
    if (!loaded || !loaded->plugin) {
        return 0;
    }
    
    LogSchemaPlugin* plugin = loaded->plugin;
    uint32_t num_types = plugin->numTypes();
    int registered = 0;
    
    for (uint32_t i = 0; i < num_types; ++i) {
        const LogSchemaTypeHandler* handler = plugin->getType(i);
        if (!handler) {
            std::cerr << __FILE__ << " " << __LINE__ << " Warning: Plugin " << plugin->pluginName()
                      << " returned nullptr for type index " << i << std::endl;
            continue;
        }
        
        uint32_t code = handler->logCode();
        
        // Check for duplicate codes
        if (m_typeRegistry.find(code) != m_typeRegistry.end()) {
            std::cerr << __FILE__ << " " << __LINE__ << " Warning: Plugin " << plugin->pluginName()
                      << " type '" << handler->typeName() << "' (code " << code
                      << ") conflicts with existing registration. Skipping." << std::endl;
            continue;
        }
        
        // Register the type
        m_typeRegistry[code] = std::make_pair(loaded, handler);
        registered++;
    }
    
    return registered;
}

} // namespace logger
} // namespace MagAOX
