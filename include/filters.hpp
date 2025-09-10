#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

using VectorBookRefs = std::vector<std::reference_wrapper<const Book>>;

constexpr auto YearBetween(const int year_left, const int year_right) {
    return [=](const Book &book) -> bool { return book.year_ >= year_left && book.year_ <= year_right; };
}
constexpr auto RatingAbove(const double rating) {
    return [=](const Book &book) -> bool { return book.rating_ >= rating; };
}
constexpr auto GenreIs(const Genre genre) {
    return [=](const Book &book) -> bool { return book.genre_ == genre; };
}

template <typename... Args>
auto all_of(Args... args) {
    return [... args = std::move(args)](const Book &book) -> bool { return (args(book) && ...); };
}

template <typename... Args>
auto any_of(Args... args) {
    return [... args = std::move(args)](const Book &book) -> bool { return (args(book) || ...); };
}

template <BookIterator T, BookPredicate P>
VectorBookRefs filterBooks(const T &begin, const T &end, const P &predicate) {
    VectorBookRefs result;
    for (T it = begin; it != end; ++it) {
        if (predicate(*it)) {
            result.push_back(*it);
        }
    }
    return result;
}

}  // namespace bookdb