#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    using is_transparent = void;

    bool operator()(const Book &lhs, const Book &rhs) {
        return std::lexicographical_compare(lhs.author_.begin(), lhs.author_.end(), rhs.author_.begin(),
                                            rhs.author_.end());
    }
    bool operator()(const Book &lhs, std::string_view rhs) {
        return std::lexicographical_compare(lhs.author_.begin(), lhs.author_.end(), rhs.begin(), rhs.end());
    }
    bool operator()(std::string_view lhs, const Book &rhs) {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.author_.begin(), rhs.author_.end());
    }
};
// LessByPopularity

// не знаю, что такое read_count, количество чтений, уже есть рейтинг, в main yandex предложил некикий компаратор
// LessByPopularity
struct LessByPopularity {
    using is_transparent = void;

    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.read_count_ < rhs.read_count_; }
    bool operator()(const Book &lhs, const double rhs) const { return lhs.read_count_ < rhs; }
    bool operator()(const double lhs, const Book &rhs) const { return lhs < rhs.read_count_; }
};

struct GreaterByRating {
    using is_transparent = void;

    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.rating_ > rhs.rating_; }
    bool operator()(const Book &lhs, const double rhs) const { return lhs.rating_ > rhs; }
    bool operator()(const double lhs, const Book &rhs) const { return lhs > rhs.rating_; }
};

struct LessByRating {
    using is_transparent = void;

    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.rating_ < rhs.rating_; }
    bool operator()(const Book &lhs, const double rhs) const { return lhs.rating_ < rhs; }
    bool operator()(const double lhs, const Book &rhs) const { return lhs < rhs.rating_; }
};

}  // namespace bookdb::comp