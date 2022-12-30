#include <boost/test/unit_test.hpp>

#include <array> 

#include "utils.hpp"

extern "C" {
    #include "parser/kfuncall.h"
    #include "parser/kcontext.h"
    #include "parser/kerror.h"
}

using std::operator""s;

BOOST_AUTO_TEST_SUITE(funcall_tests)

BOOST_AUTO_TEST_CASE(parse_test)
{
    KError err;
    auto ctx = create_KContext(&err);
    BOOST_TEST(err.num == EN_OK);

    const char* srs = "sum 1 2";

    auto funcall = parse_KFuncall(ctx, srs, &err);

    BOOST_TEST(to_sv(funcall.name) == "sum");
    BOOST_TEST(funcall.args_count == 2);
    BOOST_TEST(to_sv(funcall.args[0]) == "1");
    BOOST_TEST(to_sv(funcall.args[1]) == "2");

    free_KContext(ctx);
}

BOOST_AUTO_TEST_SUITE_END();