#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/symbol.hpp>
#include <eosio/system.hpp>
#include <experimental/type_traits>
#include <string>

/*
 * Helper functions to build more helpful error and debug messages:
 */

/* Helpers to detect if a type has the "to_string" method */
template <typename T>
using to_string_t = decltype(std::declval<T>().to_string());

template <typename T>
using has_to_string = std::experimental::is_detected<to_string_t, T>;

/*
 * Polymorphic helper to convert common EOS.IO types to string
 */
template <typename T>
inline std::string toString(const T &x) {
    if constexpr (std::is_same<T, std::string>::value) {
        return x;
    } else if constexpr (std::is_same<T, std::string_view>::value) {
        return std::string(x);
    } else if constexpr (std::is_same<T, eosio::symbol>::value) {
        return std::to_string(x.precision()) + "," + x.code().to_string();
    } else if constexpr (has_to_string<T>::value) {
        return x.to_string();
    } else {
        return std::to_string(x);
    }
}

/*
 * C++'s missing format string function :-)
 */
template <typename... Args>
inline char *fmt(const std::string_view format, Args const &...args) {
    static char buf[512];
    snprintf(buf, sizeof(buf), format.data(), toString(args).c_str()...);
    return buf;
}

/**
 * @brief Splits a string into a vector of tokens delimited by a a delimiter string.
 *
 * @param string to be split
 * @param delimiter
 * @return vector<string>
 */
vector<string> split(string s, string delimiter) {
    size_t         pos_start = 0, pos_end, delim_len = delimiter.length();
    vector<string> result;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        const auto item = s.substr(pos_start, pos_end - pos_start);
        pos_start       = pos_end + delim_len;
        result.push_back(item);
    }

    // push the remainder to the result
    result.push_back(s.substr(pos_start));
    return result;
}