#include "../../../tests/catch2/catch.hpp"


#include "../ttyIOUtils.hpp"

namespace ttyIOUtils_test
{

SCENARIO( "A string needs to be telnet-ified", "[libXWCTk::tty]" ) 
{
   GIVEN("Strings in non-telnet format with single chars")
   {
      std::string telnetStr, inputStr;
      int rv;
            
      WHEN("A single \\r char at end")
      {
         inputStr = "test\r";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "test\r\n");
      }
      
      WHEN("A single \\n char at end")
      {
         inputStr = "test\n";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "test\r\n");
      }
      
      WHEN("A single \\r char in the middle")
      {
         inputStr = "test\rtest";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "test\r\ntest");
      }
      
      WHEN("A single \\n char in the middle")
      {
         inputStr = "test\ntest";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "test\r\ntest");
      }
      
      WHEN("A single \\r char at the beginning")
      {
         inputStr = "\rtest";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "\r\ntest");
      }
      
      WHEN("A single \\n char at the beginning")
      {
         inputStr = "\ntest";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "\r\ntest");
      }
   }
   
   GIVEN("Strings in non-telnet format with two split chars")
   {
      std::string telnetStr, inputStr;
      int rv;
            
      WHEN("A single \\r char at end, a \\n at beginning")
      {
         inputStr = "\ntest\r";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "\r\ntest\r\n");
      }
      
      WHEN("A single \\n char at end, a \\r at beginning")
      {
         inputStr = "\rtest\n";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "\r\ntest\r\n");
      }
      
      WHEN("A single \\r char in the middle, a \n at beginning")
      {
         inputStr = "\ntest\rtset";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "\r\ntest\r\ntset");
      }
      
      WHEN("A single \\n char in the middle, a \r at beginning")
      {
         inputStr = "\rtest\ntest";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "\r\ntest\r\ntest");
      }
      
      WHEN("A single \\r char at the beginning, a \\r at end")
      {
         inputStr = "\rtest\r";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "\r\ntest\r\n");
      }
      
      WHEN("A single \\n char at the beginning, a \\n at end")
      {
         inputStr = "\ntest\r\n";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "\r\ntest\r\n");
      }
   }
   
   GIVEN("Strings already in telnet format")
   {
      std::string telnetStr, inputStr;
      int rv;
            
      WHEN("A \\r\\n at end")
      {
         inputStr = "test\r\n";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "test\r\n");
      }
            
      WHEN("A \\r\\n char in the middle")
      {
         inputStr = "test\r\ntest";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "test\r\ntest");
      }
      
      WHEN("A \\r\\n char at the beginning")
      {
         inputStr = "\r\ntest";
         rv = XWCTk::tty::telnetCRLF(telnetStr, inputStr );
         REQUIRE(rv == 0);
         REQUIRE(telnetStr == "\r\ntest");
      }
      
   }
}

} //namespace ttyIOUtils_test
