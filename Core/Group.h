#pragma once
#include <algorithm>
#include <vector>
#include <queue>
#include <memory>
#include "Object.h"
#include "MemPool.h"

namespace Snowing::Scene
{
	template <typename TBaseObject = Object>
	class[[nodiscard]] Group : public Object
	{
	private:
		std::vector<std::unique_ptr<TBaseObject>> objs_;
		std::queue<std::unique_ptr<TBaseObject>> newObjs_;
	public:
		bool Update() override
		{
			objs_.erase(
				std::remove_if(
					objs_.begin(),
					objs_.end(),
					[](auto& p) { return !p->Update(); }),
				objs_.end());

			while (!newObjs_.empty())
			{
				objs_.emplace_back(std::move(newObjs_.front()));
				newObjs_.pop();
			}

			return !objs_.empty();
		}

		template <typename TObj = TBaseObject,typename ... TArgs>
		TObj* Emplace(TArgs&&... args)
		{
			auto p = new TObj{ std::forward<TArgs>(args)... };
			newObjs_.emplace(std::unique_ptr<TObj>(p));
			return p;
		}

		template <typename TObj>
		TObj* PushBack(std::unique_ptr<TObj>&& obj)
		{
			auto p = obj.get();
			newObjs_.emplace(std::move(obj));
			return p;
		}

		template <typename TFunc>
		void Iter(TFunc& f)
		{
			for (auto& p : objs_)
				f(*p);
		}

		template <typename TObjectType, typename TFunc>
		void IterType(TFunc& f)
		{
			Iter([this,&f](TBaseObject& obj) {
				try
				{
					f(dynamic_cast<TObjectType&>(obj));
				}
				catch(std::bad_cast)
				{ }
			});
		}
		
		template <typename TObjectType>
		TObjectType * FindFirst()
		{
			for(auto& p : objs_)
			{
				try
				{
					return dynamic_cast<TObjectType*>(p.get());
				}
				catch(std::bad_cast)
				{ }
			}
			return nullptr;
		}

		void Clear()
		{
			objs_.clear();
		}

		size_t Count() const
		{
			return objs_.size();
		}

		size_t CountWithNewObjects() const
		{
			return objs_.size() + newObjs_.size();
		}

		TBaseObject* operator [] (size_t i) const
		{
			return objs_[i].get();
		}

		bool Exist(TBaseObject* pObject) const
		{
			return std::any_of(objs_.begin(), objs_.end(), [pObject](const auto& pObjectInList)
			{
				return pObject == pObjectInList.get();
			});
		}
	};

	template <typename TBaseObject = Object>
	class FinalGroup : public Group<TBaseObject>, public MemPool<FinalGroup<TBaseObject>>
	{
	};
}
