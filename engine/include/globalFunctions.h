#pragma once

#include <math.h>
#include <iostream>
#include <format>
#include <string_view>

float HeavyComputations(int numIterations);

template <typename... Args>
void log(std::string_view fmt, Args&&... args) {
	std::cout << std::vformat(fmt, std::make_format_args(args...));
}