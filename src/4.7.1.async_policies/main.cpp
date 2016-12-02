#include <iostream>
#include <future>
#include <string>

int main()
{
	auto f1 = std::async(std::launch::async, []{ std::cout << "job1\n"; }); /* exec in new thread */
	auto f2 = std::async(std::launch::deferred, []{ std::cout << "job2\n"; }); /* exec when wait() or get() called */
	auto f3 = std::async(std::launch::async | std::launch::deferred, []{ std::cout << "job3\n"; }); /* aync will be deside by async itself */
	auto f4 = std::async([]{ std::cout << "job4\n"; }); /* aync will be deside by async itself */

	f1.get();
	f3.get();
	f4.get();
	f2.get();

	return 0;
}
