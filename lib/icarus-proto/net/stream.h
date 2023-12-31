/* Copyright (C) 2023 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/daedalus-proto-lib
 */
#pragma once

#include <memory>
#include <cassert>
#include <string>
#include <type_traits>

namespace icarus::io {

    class stream {
    public:
        virtual ~stream() = default;

        virtual void connect(std::string_view ip, std::string_view port) = 0;
        virtual void disconnect() = 0;

        virtual void write(const void *data, std::size_t length) = 0;
        virtual void read(void *data, std::size_t length) = 0;

        template<typename TValue>
        void write(const TValue &val) {
            static_assert(std::is_trivial_v<std::decay_t<TValue>>,
                          "write(const TValue&) is only available for trivial types");
            write(&val, sizeof(val));
        }

        template<typename TValue>
        void read(TValue& val) {
            static_assert(std::is_trivial_v <std::decay_t<TValue>> ,
                          "read() is only available for trivial types");
            read(&val, sizeof(val));
        }

        template<typename TValue>
        TValue read() {
            TValue val;
            read(&val, sizeof(val));
            return val;
        }
    };

    template <>
    inline void stream::read<std::string>(std::string& val) {
        if (const auto size = read<std::size_t>(); size > 0) {
            auto* buffer = new char [size];
            read(buffer, size);
            val = std::string(buffer, size);
        }
    }

    template <>
    inline void stream::write<std::string>(const std::string& val) {
        write(val.size());
        write(val.c_str(), val.size());
    }
}
