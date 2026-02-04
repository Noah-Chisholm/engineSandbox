#pragma once
#include <string>

class baseObject {
public:
	baseObject(std::string name);
	std::string toString();
protected:
	std::string name;
};