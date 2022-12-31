#include <boost/test/unit_test.hpp>

#include "utils.hpp"

extern "C" {
    #include "parser/kset.h"
    #include "parser/kcontext.h"
    #include "parser/kerror.h"
    #include "parser/kany.h"
}

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

    auto kset = parse_KSet(ctx, "{1,9,3}", &err);
    BOOST_TEST(err.num == EN_OK);

    BOOST_TEST(kset.length == 3);

    BOOST_TEST(kset.items[0]->type == CType::CT_U8);
    BOOST_TEST(kset.items[1]->type == CType::CT_U8);
    BOOST_TEST(kset.items[2]->type == CType::CT_U8);

    BOOST_TEST(kset.items[0]->as_u64 == 1);
    BOOST_TEST(kset.items[1]->as_u64 == 9);
    BOOST_TEST(kset.items[2]->as_u64 == 3);

    free_KContext(ctx);
}

BOOST_AUTO_TEST_CASE(constant_set_test)
{
    KError err;
    auto ctx = create_KContext(&err);
    BOOST_TEST(err.num == EN_OK);

    auto kset = parse_KSet(ctx, "{x,3,y,z}", &err);
    BOOST_TEST(err.num == EN_OK);
    BOOST_TEST(kset.length == 4);

    BOOST_TEST(kset.items[0]->type == CType::CT_Const);
    BOOST_TEST(kset.items[1]->type == CType::CT_U8);
    BOOST_TEST(kset.items[2]->type == CType::CT_Const);
    BOOST_TEST(kset.items[3]->type == CType::CT_Const);
    
    BOOST_TEST(to_sv(kset.items[0]->as_const) == "x");
    BOOST_TEST(kset.items[0]->as_const->owns== true);

    BOOST_TEST(kset.items[1]->as_u64 == 3);
    
    BOOST_TEST(to_sv(kset.items[2]->as_const) == "y");
    BOOST_TEST(kset.items[2]->as_const->owns== true);
    
    BOOST_TEST(to_sv(kset.items[3]->as_const) == "z");
    BOOST_TEST(kset.items[3]->as_const->owns== true);

    free_KContext(ctx);
}

BOOST_AUTO_TEST_CASE(function_set_plus_test)
{
    // const char* func = "{ x:i8 = 2, y=3, return (+) x y }";

    // KError err;
    // auto ctx = create_KContext(&err);
    // BOOST_TEST(err.num == EN_OK);

    // auto kfunc = parse_KSet(ctx, func, &err);
    // BOOST_TEST(err.num == EN_OK);

    // BOOST_TEST(kfunc.length == 3);
    // BOOST_TEST(kfunc.items[0].type == CT_InitVar);
    // BOOST_TEST(kfunc.items[1].type == CT_InitCst);
    // BOOST_TEST(kfunc.items[2].type == CT_Return);




    // free_KContext(ctx);
}

BOOST_AUTO_TEST_SUITE_END()