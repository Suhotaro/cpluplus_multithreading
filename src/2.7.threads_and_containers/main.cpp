#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <unistd.h>

void do_work(int i) {
	std::cout << "thread: " << i << " go to sleep\n";
	sleep(1);
}

void foo()
{
	std::vector<std::thread> threads;
	for (unsigned int i = 0; i < 20; i++)
		threads.push_back(std::thread(do_work, i+1));

	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

int main()
{
	foo();

	return 0;
}
