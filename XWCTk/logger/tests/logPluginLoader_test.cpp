/** \file logPluginLoader_test.cpp
  * \brief Unit tests for the plugin loader
  * \ingroup logger
  */

#include "../../tests/catch2/catch.hpp"

#include "../plugin.hpp"
#include "../logPluginLoader.hpp"

using namespace MagAOX::logger;

// ============================================================================
// Test Plugin Implementation (for testing only)
// ============================================================================

class TestHandler : public LogSchemaTypeHandler {
public:
    uint32_t logCode() const override { return 99999; }
    const char* typeName() const override { return "test_type"; }
    
    void* deserialize(const uint8_t* data, size_t length) override {
        if (length < 4) return nullptr;
        size_t* copy = new size_t(length);
        return copy;
    }
    
    void deleteDeserialized(void* obj) override {
        delete reinterpret_cast<size_t*>(obj);
    }
};

class TestPlugin : public LogSchemaPlugin {
    TestHandler m_handler;
    
public:
    const char* pluginName() const override { return "Test Plugin"; }
    const char* pluginVersion() const override { return "1.0.0"; }
    uint32_t numTypes() const override { return 1; }
    
    const LogSchemaTypeHandler* getType(uint32_t index) const override {
        if (index == 0) return &m_handler;
        return nullptr;
    }
    
    const LogSchemaTypeHandler* getTypeByCode(uint32_t code) const override {
        if (code == m_handler.logCode()) return &m_handler;
        return nullptr;
    }
};

// ============================================================================
// Unit Tests
// ============================================================================

TEST_CASE("LogSchemaPlugin interface", "[logger][plugin]")
{
    TestPlugin plugin;
    
    SECTION("Plugin metadata")
    {
        REQUIRE(std::string(plugin.pluginName()) == "Test Plugin");
        REQUIRE(std::string(plugin.pluginVersion()) == "1.0.0");
        REQUIRE(plugin.numTypes() == 1);
        REQUIRE(plugin.version == XWCTK_PLUGIN_VERSION);
    }
    
    SECTION("Type handler access")
    {
        const LogSchemaTypeHandler* handler = plugin.getType(0);
        REQUIRE(handler != nullptr);
        REQUIRE(handler->logCode() == 99999);
        REQUIRE(std::string(handler->typeName()) == "test_type");
        
        REQUIRE(plugin.getType(1) == nullptr);
    }
    
    SECTION("Type lookup by code")
    {
        const LogSchemaTypeHandler* handler = plugin.getTypeByCode(99999);
        REQUIRE(handler != nullptr);
        REQUIRE(std::string(handler->typeName()) == "test_type");
        
        REQUIRE(plugin.getTypeByCode(12345) == nullptr);
    }
}

TEST_CASE("LogSchemaTypeHandler deserialization", "[logger][plugin]")
{
    TestHandler handler;
    
    SECTION("Deserialize valid data")
    {
        uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
        void* obj = handler.deserialize(data, sizeof(data));
        REQUIRE(obj != nullptr);
        
        size_t* result = reinterpret_cast<size_t*>(obj);
        REQUIRE(*result == sizeof(data));
        
        handler.deleteDeserialized(obj);
    }
    
    SECTION("Deserialize too-small data returns nullptr")
    {
        uint8_t data[] = {0x01, 0x02};
        void* obj = handler.deserialize(data, sizeof(data));
        REQUIRE(obj == nullptr);
    }
    
    SECTION("Default validate returns success")
    {
        uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
        REQUIRE(handler.validate(data, sizeof(data)) == 0);
    }
}

TEST_CASE("LogPluginLoader basic operations", "[logger][plugin]")
{
    LogPluginLoader loader;
    
    SECTION("Initial state")
    {
        REQUIRE(loader.numPlugins() == 0);
        REQUIRE(loader.numTypes() == 0);
    }
    
    SECTION("Add search paths")
    {
        loader.addSearchPath("/test/path1");
        loader.addSearchPath("/test/path2");
        loader.addSearchPath("/test/path1");  // Duplicate - should be ignored
    }
    
    SECTION("Query non-existent type")
    {
        REQUIRE(loader.getTypeHandler(99999) == nullptr);
        REQUIRE(loader.hasType(99999) == false);
    }
    
    SECTION("Load from empty directory")
    {
        loader.addSearchPath("/nonexistent/path/that/does/not/exist");
        int loaded = loader.loadPlugins();
        REQUIRE(loaded == 0);
    }
    
    SECTION("Get registered codes from empty loader")
    {
        std::vector<uint32_t> codes = loader.getRegisteredCodes();
        REQUIRE(codes.empty());
    }
    
    SECTION("Unload all")
    {
        loader.unloadAll();
        REQUIRE(loader.numPlugins() == 0);
        REQUIRE(loader.numTypes() == 0);
    }
}

TEST_CASE("Plugin version compatibility", "[logger][plugin]")
{
    SECTION("Current version constant")
    {
        REQUIRE(XWCTK_PLUGIN_VERSION == 1);
    }
    
    SECTION("Plugin has correct version")
    {
        TestPlugin plugin;
        REQUIRE(plugin.version == XWCTK_PLUGIN_VERSION);
    }
}
