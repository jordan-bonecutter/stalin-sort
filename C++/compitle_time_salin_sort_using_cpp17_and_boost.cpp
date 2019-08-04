#include <utility>
#include <boost/hana/tuple.hpp>
#include <boost/hana/functional/arg.hpp>
#include <boost/hana/equal.hpp>

namespace hana = boost::hana;

template <auto ... args, auto ...i>
inline constexpr auto make_tuple_helper(std::index_sequence<i...>) noexcept
{
    return hana::make_tuple(hana::arg<i>(args...)...);
}

template <auto itr, auto end, auto ... args, auto ... result_indexes>
inline constexpr decltype(auto) stalin_impl(std::index_sequence<result_indexes...>) noexcept
{   
    if constexpr (sizeof...(args) == 0) return std::index_sequence<>{};
    
    else if constexpr (itr == end) return std::index_sequence<result_indexes...>{};
    
    else if constexpr (hana::arg<itr>(args...) > hana::arg<itr + 1>(args...))
        return stalin_impl<itr + 1, end, args...>(std::index_sequence<result_indexes...>{});
    
    else return stalin_impl<itr + 1, end, args...>(std::index_sequence<result_indexes..., itr + 1>{});
}

template <auto ... args>
inline constexpr auto stalin_sort() noexcept
{
    return make_tuple_helper<args...>(stalin_impl<1, sizeof...(args), args...>(std::index_sequence<1>{}));
}

auto main() -> int
{
    static_assert(stalin_sort<>() == hana::make_tuple());
    static_assert(stalin_sort<0>() == hana::make_tuple(0));
    static_assert(stalin_sort<2,3>() == hana::make_tuple(2, 3));
    static_assert(stalin_sort<2,1,3>() == hana::make_tuple(2, 3));
    static_assert(stalin_sort<2,1,3,3,2,5,6>() == hana::make_tuple(2,3,3,5,6));
}
