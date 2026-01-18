/** \file template_test.cpp
  * \brief Catch2 tests for the template app.
  *
  * History:
  */
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../modbus.hpp"
#include "../modbus_exception.hpp"

namespace template_test
{

SCENARIO( "xxxx", "[template]" )
{
   GIVEN("xxxxx")
   {
      int rv;

      WHEN("xxxx")
      {
         rv = 0;

         REQUIRE(rv == 0);
      }
   }
}
} //namespace template_test
