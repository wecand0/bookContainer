#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь
// Конвертер строки в Genre
static constexpr std::array<std::pair<std::string_view, Genre>, 5> genres_arr{{{"Fiction", Genre::Fiction},
                                                                               {"NonFiction", Genre::NonFiction},
                                                                               {"SciFi", Genre::SciFi},
                                                                               {"Biography", Genre::Biography},
                                                                               {"Mystery", Genre::Mystery}}};

constexpr Genre GenreFromString(std::string_view s) {
    const auto it =
        std::ranges::find_if(genres_arr, [s](const std::pair<std::string_view, Genre> &el) { return el.first == s; });

    if (it != genres_arr.end()) {
        return it->second;
    }
    return Genre::Unknown;
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author_;
    std::string title_;

    int year_;
    Genre genre_;
    double rating_;
    int read_count_{};

    // Ваш код для конструкторов здесь
    constexpr Book(const std::string_view title, const std::string_view author, const uint16_t year, const Genre genre,
                   const float rating, const int read_count)
        : author_(author), title_(title), year_(year), genre_(genre), rating_(rating), read_count_(read_count) {}

    constexpr Book(const std::string_view title, const std::string_view author, const uint16_t year,
                   const std::string_view genre, const float rating, const int read_count)
        : author_(author), title_(title), year_(year), genre_(GenreFromString(genre)), rating_(rating),
          read_count_(read_count) {}
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
        return format_to(fc.out(), "{}", genre_str);
    }

    static constexpr auto parse(const format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

// Ваш код для std::formatter<Book> здесь
template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book &book, FormatContext &fc) const {
        return format_to(fc.out(),
                         "Book{{author: '{}', title: '{}', year: {}, genre: {}, rating: {:.1f}, read_count: {}}}",
                         book.author_, book.title_, book.year_, book.genre_, book.rating_, book.read_count_);
    }

    static constexpr auto parse(const format_parse_context &ctx) {
        return ctx.begin();  // Игнорируем пользовательский формат
    }
};
}  // namespace std
