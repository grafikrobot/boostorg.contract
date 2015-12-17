
// Test with postconditions.

#undef BOOST_CONTRACT_AUX_TEST_NO_F_POST
#include "decl.hpp"

#include <boost/detail/lightweight_test.hpp>
#include <sstream>

int main() {
    std::ostringstream ok;

    f_post = true;
    out.str("");
    f();
    ok.str(""); ok
        << "f::pre" << std::endl
        << "f::old" << std::endl
        << "f::body" << std::endl
        << "f::post" << std::endl // Test no failure here.
    ;
    BOOST_TEST(out.eq(ok.str()));

    struct err {};
    boost::contract::set_postcondition_failure(
            [] (boost::contract::from) { throw err(); });

    f_post = false;
    out.str("");
    try {
        f();
        BOOST_TEST(false);
    } catch(err const&) {
        ok.str(""); ok
            << "f::pre" << std::endl
            << "f::old" << std::endl
            << "f::body" << std::endl
            << "f::post" << std::endl // Test this failed.
        ;
        BOOST_TEST(out.eq(ok.str()));
    } catch(...) { BOOST_TEST(false); }

    return boost::report_errors();
}

