#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Core/Name.h"

namespace GameObjects {
	namespace CoreObjects {
		class BaseObject : public std::enable_shared_from_this<BaseObject > {
		public:
			BaseObject(Core::Name str);
			std::string toString();
			virtual void init();

			virtual ~BaseObject();

			void addChild(std::shared_ptr<BaseObject > child);

			virtual void destroy();

			std::shared_ptr<BaseObject > getSharedBaseObject() {
				return shared_from_this();
			}

			template<typename T>
			std::shared_ptr<T> getSharedAs() {
				return std::dynamic_pointer_cast<T>(shared_from_this());
			}
		protected:
			Core::Name _name;
		private:
			std::vector<std::shared_ptr<BaseObject >> children;
		};
	}
}