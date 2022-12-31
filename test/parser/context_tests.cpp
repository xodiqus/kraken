#include <boost/test/unit_test.hpp>

#include "utils.hpp"

extern "C" {
    #include "parser/kfuncall.h"
    #include "parser/kcontext.h"
    #include "parser/kerror.h"
    #include "parser/kset.h"
    #include "parser/kany.h"
}

BOOST_AUTO_TEST_SUITE(context_tests)

BOOST_AUTO_TEST_CASE(get_from_context_test)
{
    // KError err;
    // auto ctx = create_KContext(&err);
    // BOOST_TEST(err.num == EN_OK);

    // parse_KSet(ctx, "a = {5,9}", &err);
    // BOOST_TEST(err.num == EN_OK);
    
    // parse_KSet(ctx, "b = {9}", &err);
    // BOOST_TEST(err.num == EN_OK);

    // const KAny* a = get_from_KContext(ctx, "a");
    // const KAny* b = get_from_KContext(ctx, "b");

    // BOOST_TEST(a != nullptr);
    // BOOST_TEST(b != nullptr);

    // BOOST_TEST(a->type == CType::CT_KSet);
    // BOOST_TEST(b->type == CType::CT_KSet);

    // const KAny* _5 = get_from_KContext(ctx, "5");
    // const KAny* _9 = get_from_KContext(ctx, "9");

    // BOOST_TEST(_5 != nullptr);
    // BOOST_TEST(_9 != nullptr);

    // BOOST_TEST(a->type == CType::CT_U8);
    // BOOST_TEST(b->type == CType::CT_U8);

    // BOOST_TEST(_5->sets.length == 1);
    // // BOOST_CHECK(eq_KAny(_5_sets.items[0], a));

    // BOOST_TEST(_9->sets.length == 2);
    // // BOOST_CHECK(eq_KAny(_9_sets.items[0], a) || eq_KAny(_9_sets.items[1], a));
    // // BOOST_CHECK(eq_KAny(_9_sets.items[0], b) || eq_KAny(_9_sets.items[1], b));

    // free_KContext(ctx);
}

BOOST_AUTO_TEST_SUITE_END()