#pragma once
#include <list>
#include <functional>

template <class... ARGS>
class CDelegate
{
public:
	typedef typename std::list<std::function<void(ARGS...)>>::iterator iterator;

	void operator () (const ARGS... args)
	{
		for(auto & func : functions)
		{
			func(args...);
		}
	}

	CDelegate& operator += (std::function<void(ARGS...)> const& func)
	{
		functions.push_back(func);
		return *this;
	}

	CDelegate& operator -= (std::function<void(ARGS...)> const& func)
	{
		void(* const* func_ptr)(ARGS...) = func.template target<void(*)(ARGS...)>();
		const std::size_t func_hash = func.target_type().hash_code();

		if (nullptr == func_ptr)
		{
			for (auto iter = functions.begin(); iter != functions.end(); iter++)
			{
				if (func_hash == (*iter).target_type().hash_code())
				{
					functions.erase(iter);
					return *this;
				}
			}
		}

		else
		{
			for (auto iter = functions.begin(); iter != functions.end(); iter++)
			{
				void(* const* CDelegate_ptr)(ARGS...) = (*iter).template target<void(*)(ARGS...)>();
				if (nullptr != CDelegate_ptr && *func_ptr == *CDelegate_ptr)
				{
					functions.erase(iter);
					return *this;
				}
			}
		}

		return *this;
	}

	iterator begin()
	{
		return functions.begin();
	}

	iterator end()
	{
		return functions.end();
	}

	void Clear()
	{
		functions.clear();
	}


private:
	std::list<std::function<void(ARGS...)>> functions;
};