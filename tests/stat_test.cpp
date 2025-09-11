#include <gtest/gtest.h>

#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "statsistics.hpp"

using namespace bookdb;
using namespace std::string_view_literals;

namespace {
auto makeBookDb() {
    return BookDatabase{{"HighAndLow", "Yandex", 2008, Genre::SciFi, 5.2, 1000},
                        {"OnTheGround", "Google", 2020, "NonFiction", 5.7, 1000},
                        {"Daniel AC", "DangaDanga", 947, "NonFiction", 5.6, 1000}};
}
}  // namespace

TEST(Statistics, buildAuthorHistogramFlat) {
    BookDatabase db = makeBookDb();
    auto result = buildAuthorHistogramFlat(db);
    EXPECT_EQ(result["DangaDanga"sv], 1);
    EXPECT_EQ(result["Google"sv], 1);
    EXPECT_EQ(result["Yandex"sv], 1);

    BookDatabase empty_db{};
    auto empty_result = buildAuthorHistogramFlat(empty_db);
    EXPECT_EQ(empty_result.empty(), true);
}

TEST(Statistics, calculateGenreRatings) {
    BookDatabase db = makeBookDb();
    auto result = calculateGenreRatings(db);
    EXPECT_NEAR(result[Genre::SciFi], 5.19, 0.2);
    EXPECT_NEAR(result[Genre::NonFiction], 5.7, 0.3);

    BookDatabase empty_db{};
    auto empty_result = calculateGenreRatings(empty_db);
    EXPECT_EQ(empty_result.empty(), true);
}

TEST(Statistics, calculateAverageRating) {
    BookDatabase db = makeBookDb();
    double result = calculateAverageRating(db);
    EXPECT_NEAR(result, 5.5, 0.5);

    BookDatabase empty_db{};
    double empty_result = calculateAverageRating(empty_db);
    EXPECT_EQ(empty_result, 0);
}

TEST(Statistics, sampleRandomBooks) {
    BookDatabase db = makeBookDb();
    VectorBookRefs result = sampleRandomBooks(db, 1);
    EXPECT_EQ(result.size(), 1);

    BookDatabase empty_db{};
    auto empty_result = sampleRandomBooks(empty_db, 2);
    EXPECT_EQ(empty_result.empty(), true);
}

TEST(Statistics, getTopNBy) {
    BookDatabase db = makeBookDb();
    VectorBookRefs result = getTopNBy(db, 2, comp::GreaterByRating{});
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].get().author_, "Google");
    EXPECT_EQ(result[1].get().author_, "DangaDanga");

    BookDatabase empty_db{};
    auto empty_result = getTopNBy(empty_db, 2, comp::LessByRating{});
    EXPECT_EQ(empty_result.empty(), true);
}