#include <iostream>
#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>

void foo(int a, int b)
{
	std::cout << a << ", " << b << std::endl;
}

int get_num() {
	static int t = 0;
	return ++t;
}

int main()
{
	/* Undefine behavor, also read in Meyers and standart */
	foo(get_num(), get_num());

	return 0;
}
