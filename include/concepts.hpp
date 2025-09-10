#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = std::same_as<typename T::value_type, Book> && requires(T c) {
    { c.begin() } -> std::input_iterator;
    { c.end() } -> std::input_iterator;
    { c.size() } -> std::convertible_to<size_t>;
    { c.empty() } -> std::convertible_to<bool>;
    { c.operator[](size_t{}) } -> std::same_as<Book &>;
} && requires(T c, Book b) {
    { c.push_back(std::ref(b)) };
    { c.push_back(std::move(b)) };
    { c.emplace_back(b.author_, b.title_, b.year_, b.genre_, b.rating_, b.read_count_) } -> std::same_as<Book &>;
};

template <typename T>
concept BookIterator = std::is_same_v<Book, typename std::iterator_traits<T>::value_type> && std::input_iterator<T>;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I> && BookIterator<I>;

template <typename P>
concept BookPredicate = std::predicate<P, const Book &>;

template <typename C>
concept BookComparator = std::predicate<C, const Book &, const Book &>;

}  // namespace bookdb