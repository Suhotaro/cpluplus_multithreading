#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>

void job(int wait)
{
	std::cout << "wait for:" << wait << std::endl;
	sleep(wait);
}

std::thread returns_thread()
{
	std::cout << "create thread\n";
	std::thread t(job, 1);
	return t;
}

void accept_thread(std::thread t)
{
	std::cout << "join thread\n";
	if (t.joinable())
		t.join();
}

int main()
{
	std::thread t = returns_thread();
	accept_thread(std::move(t));

	return 0;
}
