#pragma once

#include <math.h>
#include <iostream>
#include <format>
#include <string_view>

namespace logging {
    template <typename... Args>
    void log(std::string_view fmt, Args&&... args) {
        std::cout << std::vformat(fmt, std::make_format_args(args...));
    }

    std::string Narrow(const std::wstring& ws);
}
//void log(std::string_view fmt);