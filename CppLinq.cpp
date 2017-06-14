// CppLinq.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>


template<class T>
class me_vector : public std::vector<T>
{
public:
	me_vector Select()
	{
		return *this;
	}

	me_vector Select(std::function<bool(const T&)> predicate)
	{
		return this->Where(predicate);
	}

	template<class TOut>
	me_vector<TOut> Select(std::function<TOut(const T&)> predicate)
	{
		if (this->size() == 0)
		{
			return me_vector<TOut>();
		}

		me_vector<TOut> output;

		auto predicateFunc = [&output, predicate](const T& x)
		{
			TOut outItem = predicate(x);
			output.push_back(outItem);
		};

		std::for_each(this->begin(), this->end(), predicateFunc);

		return output;
	}

	T First()
	{
		me_vector_const_iterator it = this->begin();
		if (this->size() == 0 || it == this->end())
		{
			throw std::string("No elements found in sequence.");
		}

		return *it;
	}

	T First(std::function<bool(const T&)> predicate)
	{
		return this->Where(predicate).First();
	}

	me_vector Skip(unsigned int count)
	{
		if (count > this->size() - 1)
		{
			return me_vector();
		}

		me_vector output = this->IterateOver(this->begin() + count, this->end());
		return output;
	}

	me_vector Take(unsigned int count)
	{
		if (count > this->size() - 1)
		{
			return *this;
		}

		me_vector output = this->IterateOver(this->begin(), this->begin() + count);
		return output;
	}

	me_vector Where(std::function<bool(const T&)> predicate)
	{
		if (this->size() == 0)
		{
			return *this;
		}

		me_vector output;
		auto predicateFunc = [&output, predicate](const T& x)
		{
			if (predicate(x))
			{
				output.push_back(x);
			}
		};

		std::for_each(this->begin(), this->end(), predicateFunc);

		return output;
	}

private:
	typedef typename me_vector<T>::const_iterator me_vector_const_iterator;

	me_vector IterateOver(me_vector_const_iterator start, me_vector_const_iterator finish)
	{
		me_vector output;

		for (auto it = start; it != finish; ++it)
		{
			output.push_back(*it);
		}

		return output;
	}
};


class ProjectionTarget
{
public:
	bool isEven;
	int value;
};


void PrintNumbers(std::string pre, me_vector<int> numbers)
{
	std::cout << pre.c_str();
	std::for_each(numbers.begin(), numbers.end(), [](int x) { std::cout << x << " "; });
}

void PrintProjectionTargets(std::string pre, me_vector<ProjectionTarget> projectionTargets)
{
	std::cout << pre.c_str() << std::endl;
	std::for_each(projectionTargets.begin(), projectionTargets.end(), [](ProjectionTarget projectionTarget)
	{
		std::cout << "{ value: " << projectionTarget.value << ", isEven: " << (projectionTarget.isEven ? "yes" : "no") << " }" << std::endl;
	});
}


int _tmain(int argc, _TCHAR* argv[])
{
	me_vector<int> numbers;
	for (int x = 1; x <= 30; x++)
	{
		numbers.push_back(x);
	}

	/* ===== EVEN & ODD ===== */
	auto evenPredicate = [](int x) { return x % 2 == 0; };
	auto evenNumbers = numbers.Where(evenPredicate);
	PrintNumbers("Even numbers: ", evenNumbers);

	std::cout << std::endl;

	auto oddPredicate = [](int x) { return x % 2 != 0; };
	auto oddNumbers = numbers.Where(oddPredicate);
	PrintNumbers("Odd numbers: ", oddNumbers);

	std::cout << std::endl << std::endl;


	/* ===== SKIP & TAKE ===== */

	auto skipTake = numbers.Skip(10).Take(15);
	PrintNumbers("Skip 10, Take 15: ", skipTake);

	std::cout << std::endl << std::endl;


	/* ===== CONDITIONAL SELECT ===== */

	auto conditionalSelectPredicate = [](int x) { return x >= 20; };
	auto greaterThan20 = numbers.Select(conditionalSelectPredicate);
	PrintNumbers("Greater Than 20: ", greaterThan20);

	std::cout << std::endl << std::endl;


	/* ===== SELECT PROJECTION ===== */

	auto projectionPredicate = [](int x)
	{
		ProjectionTarget target;
		target.value = x;
		target.isEven = x % 2 == 0;
		return target;
	};

	auto projections = numbers.Select<ProjectionTarget>(projectionPredicate);
	PrintProjectionTargets("Projections: ", projections);

	std::cout << std::endl << std::endl << std::endl << std::endl;
	return 0;
}
