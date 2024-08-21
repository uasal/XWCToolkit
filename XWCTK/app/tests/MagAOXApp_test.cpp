//#define CATCH_CONFIG_MAIN
#include "../../../tests/catch2/catch.hpp"

#include <mx/sys/timeUtils.hpp>

#include "../XWCTkApp.hpp"

namespace XWCTkApp_tests 
{
   
struct XWCTkApp_test : public XWCTk::app::XWCTkApp<true>
{
    XWCTkApp_test() : XWCTkApp("sha1",false){}
    
    virtual int appStartup(){return 0;}
    virtual int appLogic(){return 0;}
    virtual int appShutdown(){return 0;}

    std::string configName()
    {
        return XWCTk::app::XWCTkApp<true>::configName();
    }

    void configName(const std::string & cn)
    {
        m_configName = cn;

        m_indiDriver = new XWCTk::app::indiDriver<XWCTk::app::XWCTkApp<true>>(this, m_configName, "0", "0");
    }

    int called_back {0};
};

int callback( void * app, const pcf::IndiProperty &ipRecv)
{
    static_cast<void>(ipRecv); //be unused

    XWCTkApp_test * appt = static_cast<XWCTkApp_test*>(app);

    appt->called_back = 1;

    return 0;
}

SCENARIO( "XWCTkApp INDI NewProperty", "[XWCTkApp]" ) 
{
    GIVEN("a new property request")
    {
        WHEN("a wrong device name")
        {
            XWCTkApp_test app;

            app.configName("test");

            REQUIRE(app.configName() == "test");

            pcf::IndiProperty prop;
            app.registerIndiPropertyNew(prop, "nprop", pcf::IndiProperty::Number, pcf::IndiProperty::ReadWrite, pcf::IndiProperty::Idle, callback);

            pcf::IndiProperty nprop;

            //First test the right device name
            nprop.setDevice("test");
            nprop.setName("nprop");

            app.handleNewProperty(nprop);

            REQUIRE(app.called_back == 1);

            app.called_back = 0;

            //Now test the wrong device name
            nprop.setDevice("wrong");

            app.handleNewProperty(nprop);

            REQUIRE(app.called_back == 0);

        }
    }
}


} //namespace XWCTkApp_tests 
