
// Copyright (C) 2008-2016 Lorenzo Caminiti
// Distributed under the Boost Software License, Version 1.0 (see accompanying
// file LICENSE_1_0.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).
// See: http://www.boost.org/doc/libs/release/libs/contract/doc/html/index.html

// Test only middle base class with exit invariants.

#define BOOST_CONTRACT_TEST_NO_A_INV
#undef BOOST_CONTRACT_TEST_NO_B_INV
#define BOOST_CONTRACT_TEST_NO_C_INV
#include "decl.hpp"

#include <boost/preprocessor/control/iif.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <sstream>
#include <string>

std::string ok_begin() {
    std::ostringstream ok; ok
        #ifndef BOOST_CONTRACT_NO_ENTRY_INVARIANTS
            << "c::static_inv" << std::endl
            << "b::static_inv" << std::endl
            << "b::inv" << std::endl
            << "a::static_inv" << std::endl
        #endif
        #ifndef BOOST_CONTRACT_NO_PRECONDITIONS
            << "c::f::pre" << std::endl
        #endif
        #ifndef BOOST_CONTRACT_NO_OLDS
            << "c::f::old" << std::endl
            << "b::f::old" << std::endl
            << "a::f::old" << std::endl
        #endif
        << "a::f::body" << std::endl
    ;
    return ok.str();
}

std::string ok_end() {
    std::ostringstream ok; ok << "" // Suppress a warning.
        #ifndef BOOST_CONTRACT_NO_POSTCONDITIONS
            << "c::f::old" << std::endl
            << "c::f::post" << std::endl
            << "b::f::old" << std::endl
            << "b::f::post" << std::endl
            << "a::f::post" << std::endl
        #endif
    ;
    return ok.str();
}

int main() {
    std::ostringstream ok;

    #ifdef BOOST_CONTRACT_NO_ENTRY_INVARIANTS
        #define BOOST_CONTRACT_TEST_entry_inv 0
    #else
        #define BOOST_CONTRACT_TEST_entry_inv 1
    #endif
    
    a aa;
    
    a_exit_inv = true;
    b_exit_inv = true;
    c_exit_inv = true;
    a_entering_inv = b_entering_inv = c_entering_inv =
            BOOST_PP_IIF(BOOST_CONTRACT_TEST_entry_inv, true, false);
    out.str("");
    aa.f();
    ok.str(""); ok // Test nothing failed.
        << ok_begin()
        #ifndef BOOST_CONTRACT_NO_EXIT_INVARIANTS
            << "c::static_inv" << std::endl
            << "b::static_inv" << std::endl
            << "b::inv" << std::endl
            << "a::static_inv" << std::endl
        #endif
        << ok_end()
    ;
    BOOST_TEST(out.eq(ok.str()));
    
    struct err {};
    boost::contract::set_exit_invariant_failure(
            [] (boost::contract::from) { throw err(); });

    a_exit_inv = false;
    b_exit_inv = true;
    c_exit_inv = true;
    a_entering_inv = b_entering_inv = c_entering_inv =
            BOOST_PP_IIF(BOOST_CONTRACT_TEST_entry_inv, true, false);
    out.str("");
    try {
        aa.f();
        ok.str(""); ok
            << ok_begin()
            #ifndef BOOST_CONTRACT_NO_EXIT_INVARIANTS
                << "c::static_inv" << std::endl
                << "b::static_inv" << std::endl
                << "b::inv" << std::endl
                << "a::static_inv" << std::endl
                // Test no failure here.
            #endif
            #ifndef BOOST_CONTRACT_NO_POSTCONDITIONS
                << ok_end()
            #endif
        ;
        BOOST_TEST(out.eq(ok.str()));
    } catch(...) { BOOST_TEST(false); }
    
    a_exit_inv = true;
    b_exit_inv = false;
    c_exit_inv = true;
    a_entering_inv = b_entering_inv = c_entering_inv =
            BOOST_PP_IIF(BOOST_CONTRACT_TEST_entry_inv, true, false);
    out.str("");
    try {
        aa.f();
        #ifndef BOOST_CONTRACT_NO_EXIT_INVARIANTS
                BOOST_TEST(false);
            } catch(err const&) {
        #endif
        ok.str(""); ok
            << ok_begin()
            #ifndef BOOST_CONTRACT_NO_EXIT_INVARIANTS
                << "c::static_inv" << std::endl
                << "b::static_inv" << std::endl
                << "b::inv" << std::endl // Test this failed.
            #elif !defined(BOOST_CONTRACT_NO_POSTCONDITIONS)
                << ok_end()
            #endif
        ;
        BOOST_TEST(out.eq(ok.str()));
    } catch(...) { BOOST_TEST(false); }
    
    a_exit_inv = true;
    b_exit_inv = true;
    c_exit_inv = false;
    a_entering_inv = b_entering_inv = c_entering_inv =
            BOOST_PP_IIF(BOOST_CONTRACT_TEST_entry_inv, true, false);
    out.str("");
    try {
        aa.f();
        ok.str(""); ok
            << ok_begin()
            #ifndef BOOST_CONTRACT_NO_EXIT_INVARIANTS
                << "c::static_inv" << std::endl
                << "b::static_inv" << std::endl
                << "b::inv" << std::endl
                << "a::static_inv" << std::endl
            #endif
            #ifndef BOOST_CONTRACT_NO_POSTCONDITIONS
                << ok_end()
            #endif
        ;
        BOOST_TEST(out.eq(ok.str()));
    } catch(...) { BOOST_TEST(false); }
    
    a_exit_inv = false;
    b_exit_inv = false;
    c_exit_inv = false;
    a_entering_inv = b_entering_inv = c_entering_inv =
            BOOST_PP_IIF(BOOST_CONTRACT_TEST_entry_inv, true, false);
    out.str("");
    try {
        aa.f();
        #ifndef BOOST_CONTRACT_NO_EXIT_INVARIANTS
                BOOST_TEST(false);
            } catch(err const&) {
        #endif
        ok.str(""); ok
            << ok_begin()
            #ifndef BOOST_CONTRACT_NO_EXIT_INVARIANTS
                << "c::static_inv" << std::endl
                << "b::static_inv" << std::endl
                << "b::inv" << std::endl // Test this failed (as all did).
            #elif !defined(BOOST_CONTRACT_NO_POSTCONDITIONS)
                << ok_end()
            #endif
        ;
        BOOST_TEST(out.eq(ok.str()));
    } catch(...) { BOOST_TEST(false); }

    return boost::report_errors();
}

