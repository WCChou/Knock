#include <Config/Config.hpp>

#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

#include <Util/Hash.hpp>

BOOST_AUTO_TEST_CASE(my_test)
{
    BOOST_TEST(
        "7f83b1657ff1fc53b92dc18148a1d65dfc2d4b1fa3d677284addd200126d9069",
        Knock::Hash::stringSha256( "Hello World!" ) );
}