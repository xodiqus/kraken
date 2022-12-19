#include <boost/test/unit_test.hpp>
#include <string>
#include <array>

extern "C" {
    #include "parser/set.h"
}

BOOST_AUTO_TEST_SUITE(set_tests)

BOOST_AUTO_TEST_CASE(set_test)
{
    const char* err = nullptr;
    auto c = create_context(&err);

    BOOST_TEST(err == nullptr);

    for (auto item : std::array {"{}", " {}", "{} ", " {} ", "{ }"})
    {
        auto kset = parse_KSet(c, item, &err);

        BOOST_TEST(kset.length  == 0);
        BOOST_TEST(kset.items   == nullptr);
        BOOST_TEST(err == nullptr);
    }

    auto kset = parse_KSet(c, "{1,2,3,}", &err);

    BOOST_TEST(err == nullptr);
    BOOST_TEST(kset.length == 3);

    BOOST_TEST(kset.items[0].type == CType::CT_U8);
    BOOST_TEST(kset.items[1].type == CType::CT_U8);
    BOOST_TEST(kset.items[2].type == CType::CT_U8);

    free_context(c);
}

BOOST_AUTO_TEST_SUITE_END()