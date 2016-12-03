#include <iostream>
#include <mutex>
#include <list>
#include <algorithm>

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input) {
	if (input.empty())
		return input;

	std::list<T> result;
	result.splice(result.begin(), input, input.begin());

	T const& pivot = *result.begin();

	auto divide_point = std::partition(input.begin(), input.end(),
			[&](T const& t){ return t<pivot; });

	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

	auto new_lower(sequential_quick_sort(std::move(lower_part)));
	auto new_higher(sequential_quick_sort(std::move(input)));

	result.splice(result.end(), new_higher);
	result.splice(result.begin(), new_lower);

	return result;
}

void show(std::list<int>& l) {
	for (std::list<int>::iterator it = l.begin() ; it != l.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
}

int main()
{
	std::list<int> v;

	for (int i = 0; i < 100; i++)
		v.push_back(rand()%5);

	show(v);

	std::list<int> t = sequential_quick_sort(v);

	show(t);

	return 0;
}
