#pragma once

#include <functional>
#include <optional>
#include <string>
#include <string_view>

#include "string_format.hpp"

class SErr {
  public:
    static SErr &instance() {
        static SErr err{};
        return err;
    }

    template <typename... Args>
    static SErr &set(const std::string_view format, Args const &...args) {
        auto &err  = instance();
        err.lambda = [=]() {
            return std::string{fmt(format, args...)};
        };
        return err;
    }

    static std::string get() {
        auto &err = instance();
        if (err.lambda) {
            const auto msg = (*(err.lambda))();
            err.lambda     = {};
            return msg;
        } else {
            return "";
        }
    }

  private:
    std::optional<std::function<std::string()>> lambda;
};