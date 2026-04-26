#pragma once
#include <string>
#include <memory>
#include <vector>
#include "name.h"

class baseObject : public std::enable_shared_from_this<baseObject> {
public:
	baseObject(name str);
	std::string toString();
	virtual void init();

	virtual ~baseObject();

	void addChild(std::shared_ptr<baseObject> child);

	virtual void destroy();

	std::shared_ptr<baseObject> getSharedBaseObject() {
		return shared_from_this();
	}

	template<typename T>
	std::shared_ptr<T> getSharedAs() {
		return std::dynamic_pointer_cast<T>(shared_from_this());
	}
protected:
	float HeavyComputations(int numIterations);

	name _name;
private:
	std::vector<std::shared_ptr<baseObject>> children;
};