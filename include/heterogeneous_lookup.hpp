#pragma once

#include <string>
#include <string_view>

namespace bookdb {


struct TransparentStringLess {
    using is_transparent = void;
    bool operator()(std::string_view lhs, std::string_view rhs) const {
        return std::ranges::lexicographical_compare(lhs, rhs);
    }

    bool operator()(const std::string &lhs, const std::string_view rhs) const {
        return operator()(std::string_view(lhs), rhs);
    }
    bool operator()(const std::string_view lhs, const std::string &rhs) const {
        return operator()(lhs, std::string_view(rhs));
    }
};

struct TransparentStringEqual {
    using is_transparent = void;
    bool operator()(const std::string_view lhs, const std::string_view rhs) const { return lhs == rhs; }

    bool operator()(const std::string &lhs, const std::string_view rhs) const {
        return operator()(std::string_view(lhs), rhs);
    }
    bool operator()(const std::string_view lhs, const std::string &rhs) const {
        return operator()(lhs, std::string_view(rhs));
    }
};

struct TransparentStringHash {
    using is_transparent = void;
    size_t operator()(const std::string &str) const { return std::hash<std::string>()(str); }
    size_t operator()(const std::string_view str) const { return std::hash<std::string_view>()(str); }
};

}  // namespace bookdb
