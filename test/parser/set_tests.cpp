#include <boost/test/unit_test.hpp>

#include <array> 
#include <string>
#include <string_view>

#include "utils.hpp"

extern "C" {
    #include "parser/kset.h"
    #include "parser/kcontext.h"
    #include "parser/kerror.h"
}

using std::operator""s;

BOOST_AUTO_TEST_SUITE(set_tests)

BOOST_AUTO_TEST_CASE(empty_set_test)
{
    KError err;
    auto ctx = create_KContext(&err);

    BOOST_TEST(err.num == EN_OK);

    for (auto item : std::array {"{}", " {}", "{} ", " {} ", "{ }"})
    {
        auto kset = parse_KSet(ctx, item, &err);

        BOOST_TEST(kset.length  == 0);
        BOOST_TEST(kset.items   == nullptr);
        BOOST_TEST(err.num      == EN_OK);
    }

    free_KContext(ctx);
}

BOOST_AUTO_TEST_CASE(numbers_set_test)    
{
    KError err;
    auto ctx = create_KContext(&err);
    BOOST_TEST(err.num == EN_OK);

    auto kset = parse_KSet(ctx, "{1,9,3,}", &err);
    BOOST_TEST(err.num == EN_OK);

    BOOST_TEST(kset.length == 3);

    BOOST_TEST(kset.items[0].type == CType::CT_U8);
    BOOST_TEST(kset.items[1].type == CType::CT_U8);
    BOOST_TEST(kset.items[2].type == CType::CT_U8);

    BOOST_TEST(kset.items[0].u64 == 1);
    BOOST_TEST(kset.items[1].u64 == 9);
    BOOST_TEST(kset.items[2].u64 == 3);

    free_KContext(ctx);
}

BOOST_AUTO_TEST_CASE(constant_set_test)
{
    KError err;
    auto ctx = create_KContext(&err);
    BOOST_TEST(err.num == EN_OK);

    auto kset = parse_KSet(ctx, "{x,3,y,z,}", &err);
    BOOST_TEST(err.num == EN_OK);
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

    free_KContext(ctx);
}

BOOST_AUTO_TEST_CASE(function_set_plus_test)
{
    const char* func = "plus x:i8 y:i8 = { return (+) x y }";

    KError err;
    auto ctx = create_KContext(&err);
    BOOST_TEST(err.num == EN_OK);

    auto kfunc = parse_KSet(ctx, func, NULL);

    BOOST_TEST(to_sv(kfunc.name) == "plus");
    BOOST_TEST(kfunc.args_count == 2);
    BOOST_TEST(to_sv(kfunc.args[0].name) == "x");
    BOOST_TEST(to_sv(kfunc.args[1].name) == "y");

    free_KContext(ctx);
}

BOOST_AUTO_TEST_SUITE_END()