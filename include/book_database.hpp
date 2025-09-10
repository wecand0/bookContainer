#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases
    using value_type = BookContainer::value_type;
    using reference = BookContainer::reference;
    using pointer = BookContainer::pointer;
    using iterator = BookContainer::iterator;
    using const_iterator = BookContainer::const_iterator;
    using difference_type = BookContainer::difference_type;
    using size_type = BookContainer::size_type;
    // Ваш код здесь

    using AuthorContainer = std::unordered_set<std::string, TransparentStringHash>;

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book> books) : books_(books) {
        for (Book &book : books_) {
            extractAuthor(book);
        }
    }

    iterator begin() { return books_.begin(); }
    iterator end() { return books_.end(); }
    const_iterator begin() const { return books_.begin(); }
    const_iterator end() const { return books_.end(); }

    // Standard container interface methods
    bool Empty() const { return books_.empty(); }
    void Clear() {
        books_.clear();
        authors_.clear();
    }
    size_type Size() const { return books_.size(); }
    reference operator[](size_type i) { return books_[i]; }
    reference operator[](size_type i) const { return books_[i]; }
    void PushBack(Book &book) {
        extractAuthor(book);
        books_.push_back(book);
    }
    void PushBack(Book &&book) {
        PushBack(book);
    }
    template <typename... Args>
    reference Emplace_back(Args &&...args) {
        reference result = books_.emplace_back(std::forward<Args>(args)...);
        extractAuthor(result);
        return result;
    }
    const BookContainer &GetBooks() const { return books_; }
    const AuthorContainer &GetAuthors() const { return authors_; }

private:
    BookContainer books_;
    AuthorContainer authors_;

    void extractAuthor(Book &book) {
        auto [iter, success] = authors_.emplace(book.author_);
        book.author_ = *iter;
    }
};

}  // namespace bookdb

template <>
struct std::formatter<bookdb::BookDatabase<>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<> &db, FormatContext &fc) const {
        format_to(fc.out(), "BookDatabase (size = {}): ", db.Size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    static constexpr auto parse(const format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};  // namespace std
