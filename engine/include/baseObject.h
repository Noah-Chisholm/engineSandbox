#pragma once
#include <string>
#include <memory>
#include <vector>

class baseObject : public std::enable_shared_from_this<baseObject> {
public:
	baseObject(std::string name);
	std::string toString();
	virtual void init();

	virtual ~baseObject();

	void addChild(std::shared_ptr<baseObject> child);

	virtual void destroy();
protected:
	float HeavyComputations(int numIterations);

	std::string name;

private:
	std::vector<std::shared_ptr<baseObject>> children;
};