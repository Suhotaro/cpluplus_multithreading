#include <iostream>
#include <mutex>
#include <list>
#include <algorithm>
#include <thread>
#include <future>


template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
	if (input.empty())
		return input;

	/* check if it is new thread */
	std::cout << std::this_thread::get_id() << std::endl;

	std::list<T> result;
	result.splice(result.begin(), input, input.begin());

	T const& pivot = *result.begin();

	auto divide_point = std::partition(input.begin(), input.end(),
			[&](T const& t){ return t<pivot; });

	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

	std::future<std::list<T> > new_lower(std::async(std::launch::async, &parallel_quick_sort<T>, std::move(lower_part)));
	/* it was in the book
	 * std::future<std::list<T> > new_lower(std::async(&parallel_quick_sort<T>, std::move(lower_part)));
	 */
	auto new_higher(parallel_quick_sort(std::move(input)));

	result.splice(result.end(), new_higher);
	result.splice(result.begin(), new_lower.get());

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

	std::list<int> t = parallel_quick_sort(v);

	show(t);

	return 0;
}
