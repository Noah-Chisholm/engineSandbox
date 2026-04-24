#include "logging.h"

//void log(std::string_view fmt) {
//	std::cout << fmt;
//}

std::string logging::Narrow(const std::wstring& ws) {
    // Simple/OK for paths on Windows most of the time:
    // If you want fully correct Unicode conversion later, we can do WideCharToMultiByte.
    return std::string(ws.begin(), ws.end());
}