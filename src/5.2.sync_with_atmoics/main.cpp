#include <iostream>
#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>

std::vector<int> data;
std::atomic<bool> data_ready(false);

using namespace std::chrono;

void reader_thread() {
	while(!data_ready.load())
	{
		std::cout << "wait...\n";
		std::this_thread::sleep_for(milliseconds(100));
	}

	std::cout << "Respond is: " << data[0] << std::endl;
}

void writer_thread() {
	std::this_thread::sleep_for(::seconds(1));
	data.push_back(42);
	data_ready = true;
}

int main()
{
	std::thread t(reader_thread);
	std::thread t2(writer_thread);
	t.join();
	t2.join();

	return 0;
}
