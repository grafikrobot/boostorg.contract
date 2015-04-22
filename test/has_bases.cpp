
#include <boost/contract/aux_/type_traits/bases.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/detail/lightweight_test.hpp>

struct y {};
struct z {};

struct x : y, z {
    typedef boost::mpl::vector<y, z> base_types;
};

int main() {
    BOOST_TEST(!boost::contract::aux::type_traits::has_bases<y>::value);
    BOOST_TEST(boost::contract::aux::type_traits::has_bases<x>::value);
    BOOST_TEST((boost::is_same<boost::contract::aux::type_traits::bases_of<x>::
            type, boost::mpl::vector<y, z> >::value));
    return boost::report_errors();
}

