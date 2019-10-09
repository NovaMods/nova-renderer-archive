#pragma once

#include <functional>
#include <memory>
#include <string>

#include <fmt/format.h>

#include "utils.hpp"

namespace nova::renderer {
    struct NOVA_API NovaError {
        std::string message = "";

        std::unique_ptr<NovaError> cause;

        NovaError() = default;

        explicit NovaError(const std::string& message);

        NovaError(const std::string& message, NovaError cause);

        [[nodiscard]] std::string to_string() const;
    };

    inline NOVA_API NovaError operator""_err(const char* str, std::size_t size) { return NovaError(std::string(str, size)); }

    template <typename ValueType>
    struct NOVA_API Result {
        union {
            ValueType value;
            NovaError error;
        };

        bool has_value = false;

        explicit Result(ValueType&& value) : value(value), has_value(true) {}

        explicit Result(const ValueType& value) : value(value), has_value(true) {}

        explicit Result(NovaError error) : error(std::move(error)) {}

        Result(const Result<ValueType>& other) = delete;
        Result<ValueType>& operator=(const Result<ValueType>& other) = delete;

        Result(Result<ValueType>&& old) noexcept {
            if(old.has_value) {
                value = std::move(old.value);
                old.value = {};

                has_value = true;
            } else {
                error = std::move(old.error);
                old.error = {};
            }
        };

        Result<ValueType>& operator=(Result<ValueType>&& old) noexcept {
            if(old.has_value) {
                value = old.value;
                old.value = {};

                has_value = true;
            } else {
                error = old.error;
                old.error = {};
            }

            return *this;
        };

        ~Result() {
            if(has_value) {
                value.~ValueType();
            } else {
                error.~NovaError();
            }
        }

        template <typename FuncType>
        auto map(FuncType&& func) -> Result<decltype(func(value))> {
            using RetVal = decltype(func(value));

            if(has_value) {
                return Result<RetVal>(func(value));
            } else {
                return Result<RetVal>(std::move(error));
            }
        }

        template <typename FuncType>
        auto flat_map(FuncType&& func) -> Result<decltype(func(value).value)> {
            using RetVal = decltype(func(value).value);

            if(has_value) {
                return func(value);
            } else {
                return Result<RetVal>(std::move(error));
            }
        }

        template <typename FuncType>
        void if_present(FuncType&& func) {
            if(has_value) {
                func(value);
            }
        }

        void on_error(std::function<void(const NovaError&)> error_func) const {
            if(!has_value) {
                error_func(error);
            }
        }

        operator bool() const { return has_value; }

        ValueType operator*() { return value; }
    };

    template <typename ValueType>
    Result(ValueType value)->Result<ValueType>;

#define MAKE_ERROR(s, ...) NovaError(fmt::format(fmt(s), __VA_ARGS__).c_str())
} // namespace nova::renderer