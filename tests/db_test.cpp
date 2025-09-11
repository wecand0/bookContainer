#include <gtest/gtest.h>

#include "book_database.hpp"

using namespace bookdb;

auto makeBookDb() {
    return BookDatabase{{"HighAndLow", "Yandex", 2008, Genre::SciFi, 5.2, 1000},
                        {"OnTheGround", "Google", 2020, "NonFiction", 5.7, 1000}};
}

TEST(BookDatabase, addBook) {
    // initializer_list
    BookDatabase db = makeBookDb();
    EXPECT_EQ(db.Size(), 2);

    // push_back
    db.PushBack({"aaa", "pipupu", 2099, Genre::SciFi, 5.4, 1000});
    EXPECT_EQ(db.Size(), 3);
    EXPECT_EQ(db.at(0).title_, "HighAndLow");
    EXPECT_EQ(db.at(2).author_, "pipupu");

    // emplace_back
    const auto last_book = db.EmplaceBack("kiki", "toto", 1911, "NonFiction", 5.8, 1000);
    EXPECT_EQ(db.Size(), 4);
    EXPECT_EQ(last_book.title_, "kiki");
    EXPECT_EQ(last_book.author_, "toto");
    EXPECT_EQ(last_book.genre_, Genre::NonFiction);
}

TEST(BookDatabase, iterators) {
    BookDatabase db = makeBookDb();
    EXPECT_EQ(db.begin()->title_, "HighAndLow");
    EXPECT_EQ((db.begin() + 1)->title_, "OnTheGround");

    const std::vector db_copy(db.begin(), db.end());
    EXPECT_EQ(db_copy.size(), 2);
    EXPECT_EQ(db_copy.at(0).title_, "HighAndLow");
    EXPECT_EQ(db_copy[1].title_, "OnTheGround");

    db.Clear();
    EXPECT_EQ(std::distance(db.begin(), db.end()), 0);
}

TEST(BookDatabase, emptyDb) {
    BookDatabase db{};
    EXPECT_EQ(db.Empty(), true);
    EXPECT_EQ(db.Size(), 0);
    EXPECT_EQ(db.GetBooks().empty(), true);
    EXPECT_EQ(db.GetAuthors().empty(), true);
    EXPECT_EQ(std::distance(db.begin(), db.end()), 0);
}

TEST(BookDatabase, badInput) {
    BookDatabase db = makeBookDb();
    EXPECT_NE(db.at(0).author_, "noooo");
}