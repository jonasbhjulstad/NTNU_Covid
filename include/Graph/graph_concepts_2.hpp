#ifndef GRAPH_CONCEPTS_HPP
#define GRAPH_CONCEPTS_HPP

#include <concepts>
#include <ranges>

template <class T>
class graph_traits
{
    using vertex_id_type = typename T::vertex_id_type;
};

template <typename G>
using vertex_id_t = typename graph_traits<G>::vertex_id_type;

template <typename G>
concept graph = std::semiregular<G> && requires(G g)
{
    typename vertex_id_t<G>;
};

template <typename G>
using inner_range_t = std::ranges::range_value_t<G>;

template <typename G>
using inner_value_t = std::ranges::range_value_t<inner_range_t<G>>;



#endif