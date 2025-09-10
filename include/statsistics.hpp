#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <flat_map>
#include <string_view>

#include "book_database.hpp"

namespace bookdb {

using VectorBookRefs = std::vector<std::reference_wrapper<const Book>>;

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &db, Comparator comp = {}) {
    std::flat_map<std::string_view, size_t, Comparator> result;
    for (const auto &book : db) {
        ++result[std::ref(book.author)];
    }
    return result;
}

template <BookContainerLike T>
auto calculateGenreRatings(const BookDatabase<T> &db) {
    struct GenreInfo {
        double rating = 0;
        size_t counter = 0;
    };
    std::flat_map<Genre, GenreInfo> genre_info;
    std::flat_map<Genre, double> result;
    for (const auto &book : db) {
        Genre key = book.genre;
        GenreInfo &current_info = genre_info[key];
        current_info.rating += book.rating;
        ++current_info.counter;
    }
    std::transform(genre_info.begin(), genre_info.end(), std::inserter(result, result.begin()),
                   [](const auto &elem) -> decltype(result)::value_type {
                       return {elem.first, elem.second.rating / elem.second.counter};
                   });
    return result;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &db) {
    return db.empty() ? 0 : std::accumulate(db.begin(), db.end(), 0., [](double sum, const Book &book) {
                                return sum + book.rating_;
                            }) / db.size();
}

template <BookContainerLike T>
VectorBookRefs sampleRandomBooks(const BookDatabase<T> &db, size_t N) {
    VectorBookRefs result;
    std::sample(db.begin(), db.end(), std::back_inserter(result), N, std::mt19937{std::random_device{}()});
    return result;
}

template <BookContainerLike T>
VectorBookRefs getTopNBy(BookDatabase<T> &db, size_t N) {
    if (db.empty()) {
        return {};
    }
    auto result_end = db.begin() + std::min(N, db.size() - 1);
    std::nth_element(db.begin(), result_end, db.end(), comp::GreaterByRating());
    return {db.begin(), result_end};
}

}  // namespace bookdb