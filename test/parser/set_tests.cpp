#include <boost/test/unit_test.hpp>

#include <array> 
#include <string>
#include <string_view>

extern "C" {
    #include "parser/set.h"
}

using std::operator""s;

BOOST_AUTO_TEST_SUITE(set_tests)

BOOST_AUTO_TEST_CASE(empty_set_test)
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

    free_context(c);
}

BOOST_AUTO_TEST_CASE(numbers_set_test)    
{
    const char* err = nullptr;
    auto c = create_context(&err);

    auto kset = parse_KSet(c, "{1,9,3,}", &err);

    BOOST_TEST(err == nullptr);
    BOOST_TEST(kset.length == 3);

    BOOST_TEST(kset.items[0].type == CType::CT_U8);
    BOOST_TEST(kset.items[1].type == CType::CT_U8);
    BOOST_TEST(kset.items[2].type == CType::CT_U8);

    BOOST_TEST(kset.items[0].u64 == 1);
    BOOST_TEST(kset.items[1].u64 == 9);
    BOOST_TEST(kset.items[2].u64 == 3);

    free_context(c);
}

BOOST_AUTO_TEST_CASE(constant_set_test)
{
    const char* err = nullptr;
    auto c = create_context(&err);

    auto kset = parse_KSet(c, "{x,3,y,z,}", &err);

    BOOST_TEST(err == nullptr);
    BOOST_TEST(kset.length == 4);

    BOOST_TEST(kset.items[0].type == CType::CT_Const);
    BOOST_TEST(kset.items[1].type == CType::CT_U8);
    BOOST_TEST(kset.items[2].type == CType::CT_Const);
    BOOST_TEST(kset.items[3].type == CType::CT_Const);
    
    std::string_view res = {
        kset.items[0].cnst->val, 
        kset.items[0].cnst->len 
    };
    BOOST_TEST(res == "x");
    BOOST_TEST(kset.items[0].cnst->owns== true);

    BOOST_TEST(kset.items[1].u64 == 3);
    
    res = std::string_view {
        kset.items[2].cnst->val, 
        kset.items[2].cnst->len 
    };
    BOOST_TEST(res == "y");
    BOOST_TEST(kset.items[2].cnst->owns== true);
    
    res = std::string_view {
        kset.items[3].cnst->val, 
        kset.items[3].cnst->len 
    };
    BOOST_TEST(res == "z");
    BOOST_TEST(kset.items[3].cnst->owns== true);

    free_context(c);
}

BOOST_AUTO_TEST_SUITE_END()