#include <algorithm>

#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statsistics.hpp"

#include <print>

using namespace bookdb;

int main() {
    // Create a book database
    BookDatabase<> db;

    // Add some books
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
    db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
    db.EmplaceBack("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
    db.EmplaceBack("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
    db.EmplaceBack("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
    std::println("Books: {}", db);

    // Sorts
    std::ranges::sort(db, comp::LessByAuthor{});
    std::println("Books sorted by author: {}\n\n==================\n", db);

    std::ranges::sort(db, comp::LessByPopularity{});
    std::println("Books sorted by popularity: {}\n\n==================\n", db);
    //
    // // Author histogram
    auto histogram = buildAuthorHistogramFlat(db);
    // std::print("Author histogram: {}", histogram);
    //
    //  // Ratings
    auto genreRatings = calculateGenreRatings(db);
    // std::println("Average ratings by genres: {}", genreRatings);
    //
    auto avrRating = calculateAverageRating(db);
    std::println("Average books rating in library: {}\n", avrRating);
    //
    // // Filters
    auto filtered = filterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));
    std::print("\n\nBooks from the 20th century with rating ≥ 4.5:\n");
    std::for_each(filtered.cbegin(), filtered.cend(), [](const auto &v) { std::print("{}\n", v.get()); });
    //
    // // Top 3 books
    auto topBooks = getTopNBy(db, 3, comp::LessByRating{});
    std::print("\n\nTop 3 books by rating:\n");
    std::ranges::for_each(std::as_const(topBooks), [](const auto &v) { std::print("{}\n", v.get()); });
    //
    if (const auto orwellBookIt = std::ranges::find_if(db, [](const auto &v) { return v.author_ == "George Orwell"; });
        orwellBookIt != db.end()) {
        std::print("\n\nTransparent lookup by authors. Found Orwell's book: {}\n", *orwellBookIt);
    }

    return 0;
}