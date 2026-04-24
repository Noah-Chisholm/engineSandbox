#pragma once
#include "baseObject.h"

class tickableObject : public baseObject {
public:
	tickableObject(std::string name, bool isActive = true);
	void setActive(bool newActive);
	virtual void tick(float dt);
	virtual void init() override;

	~tickableObject();
	virtual void destroy() override;
private:
protected:
	bool isActive = true;
};