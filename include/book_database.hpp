#pragma once

#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

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
    using const_reference = const Book &;
    using pointer = BookContainer::pointer;
    using iterator = BookContainer::iterator;
    using const_iterator = BookContainer::const_iterator;
    using reverse_iterator = std::vector<Book>::reverse_iterator;
    using const_reverse_iterator = std::vector<Book>::const_reverse_iterator;
    using difference_type = BookContainer::difference_type;
    using size_type = BookContainer::size_type;

    using AuthorContainer = std::unordered_set<std::string, TransparentStringHash>;

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book> books) : books_(books) {
        for (Book &book : books_) {
            extractAuthor(book);
        }
    }

    [[nodiscard]] iterator begin() noexcept { return books_.begin(); }
    [[nodiscard]] const_iterator begin() const noexcept { return books_.begin(); }
    [[nodiscard]] const_iterator cbegin() const noexcept { return books_.cbegin(); }

    [[nodiscard]] iterator end() noexcept { return books_.end(); }
    [[nodiscard]] const_iterator end() const noexcept { return books_.end(); }
    [[nodiscard]] const_iterator cend() const noexcept { return books_.cend(); }

    [[nodiscard]] reverse_iterator rbegin() noexcept { return books_.rbegin(); }
    [[nodiscard]] const_reverse_iterator rbegin() const noexcept { return books_.rbegin(); }
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept { return books_.crbegin(); }

    [[nodiscard]] reverse_iterator rend() noexcept { return books_.rend(); }
    [[nodiscard]] const_reverse_iterator rend() const noexcept { return books_.rend(); }
    [[nodiscard]] const_reverse_iterator crend() const noexcept { return books_.crend(); }

    // Standard container interface methods
    bool Empty() const noexcept { return books_.empty(); }
    void Clear() {
        books_.clear();
        authors_.clear();
    }
    size_type Size() const noexcept { return books_.size(); }

    [[nodiscard]] reference operator[](size_type pos) { return books_[pos]; }
    [[nodiscard]] const_reference operator[](size_type pos) const { return books_[pos]; }

    [[nodiscard]] reference at(size_type pos) { return books_.at(pos); }
    [[nodiscard]] const_reference at(size_type pos) const { return books_.at(pos); }

    [[nodiscard]] reference front() { return books_.front(); }
    [[nodiscard]] const_reference front() const { return books_.front(); }

    [[nodiscard]] reference back() { return books_.back(); }
    [[nodiscard]] const_reference back() const { return books_.back(); }

    void Reserve(size_type newC) { books_.reserve(newC); }
    [[nodiscard]] size_type Capacity() const noexcept { return books_.capacity(); }
    void Shrink_to_fit() { books_.shrink_to_fit(); }

    iterator erase(const_iterator pos) { return books_.erase(pos); }
    iterator erase(const_iterator first, const_iterator last) { return books_.erase(first, last); }
    void PushBack(const Book &book) {
        extractAuthor(book);
        books_.push_back(book);
    }
    void PushBack(Book &&book) {
        extractAuthor(book);
        books_.push_back(std::move(book));
    }
    template <typename... Args>
    reference EmplaceBack(Args &&...args) {
        reference result = books_.emplace_back(std::forward<Args>(args)...);
        extractAuthor(result);
        return result;
    }
    [[nodiscard]] std::span<const Book> GetBooks() const noexcept { return books_; }
    [[nodiscard]] std::span<Book> GetBooks() noexcept { return books_; }
    const AuthorContainer &GetAuthors() const noexcept { return authors_; }

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
