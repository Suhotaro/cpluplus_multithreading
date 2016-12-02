#include <iostream>
#include <memory>
#include <mutex>

std::recursive_mutex rm;

void bar()
{
	rm.lock();
	std::cout << "call bar\n";
	rm.unlock();
}

void foo()
{
	rm.lock();
	std::cout << "call foo\n";
	bar();
	rm.unlock();
}

int main()
{
	foo();
	foo();

	return 0;
}
