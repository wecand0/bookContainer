#include <gtest/gtest.h>

#include "book.hpp"
#include "book_database.hpp"
#include "filters.hpp"

using namespace bookdb;

namespace {
auto makeBookDb() {
    return BookDatabase{{"HighAndLow", "Yandex", 2008, Genre::SciFi, 5.2, 1000},
                        {"OnTheGround", "Google", 2020, "NonFiction", 5.7, 1000},
                        {"Daniel AC", "DangaDanga", 947, "NonFiction", 5.6, 1000},
                        {"HighAndLow2", "YandexAndCo", 2011, Genre::SciFi, 5.2, 1000}};
}
}  // namespace

TEST(Filters, YearBetween) {
    BookDatabase db = makeBookDb();
    const VectorBookRefs result = filterBooks(db.begin(), db.end(), YearBetween(1000, 2010));
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].get().author_, "Yandex");

    const VectorBookRefs empty_result = filterBooks(db.begin(), db.end(), YearBetween(2021, 2030));
    EXPECT_EQ(empty_result.size(), 0);

    BookDatabase empty_db{};
    const auto empty_db_result = filterBooks(empty_db.begin(), empty_db.end(), YearBetween(1920, 2080));
    EXPECT_EQ(empty_db_result.empty(), true);
}

TEST(Filters, RatingAbove) {
    BookDatabase db = makeBookDb();
    const VectorBookRefs result = filterBooks(db.begin(), db.end(), RatingAbove(5.5));
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].get().author_, "Google");
    EXPECT_EQ(result[1].get().author_, "DangaDanga");

    BookDatabase empty_db{};
    const auto empty_db_result = filterBooks(empty_db.begin(), empty_db.end(), RatingAbove(5.5));
    EXPECT_EQ(empty_db_result.empty(), true);
}

TEST(Filters, GenreIs) {
    BookDatabase db = makeBookDb();
    VectorBookRefs result = filterBooks(db.begin(), db.end(), GenreIs(Genre::NonFiction));
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].get().author_, "Google");
    EXPECT_EQ(result[1].get().author_, "DangaDanga");

    BookDatabase empty_db{};
    auto empty_db_result = filterBooks(empty_db.begin(), empty_db.end(), GenreIs(Genre::NonFiction));
    EXPECT_EQ(empty_db_result.empty(), true);
}

TEST(Filters, all_of) {
    BookDatabase db = makeBookDb();
    VectorBookRefs result = filterBooks(db.begin(), db.end(), all_of(GenreIs(Genre::NonFiction), RatingAbove(5.6)));
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].get().author_, "Google");
}