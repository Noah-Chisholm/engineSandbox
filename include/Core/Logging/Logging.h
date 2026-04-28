#pragma once

#include <iostream>
#include <format>
#include <string_view>

#include "Core/Logging/Formatters.h"

namespace Core {
    namespace Logging {
        template <typename... Args>
        void log(std::string_view fmt, Args&&... args) {
            std::cout << std::vformat(fmt, std::make_format_args(args...));
        }

        inline std::string Narrow(const std::wstring& ws) {
            return std::string(ws.begin(), ws.end());
        }
    }
}