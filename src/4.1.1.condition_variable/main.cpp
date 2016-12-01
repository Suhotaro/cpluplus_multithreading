#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>
#include <thread>
#include <condition_variable>

std::mutex m;
std::queue<int> data_queue;
std::condition_variable data_cond;

void data_processing_thread() {
	while(true) {
		std::unique_lock<std::mutex> lk(m);

		data_cond.wait(lk, []{ return !data_queue.empty(); });

		int tmp = data_queue.front();
		data_queue.pop();
		lk.unlock();

		std::cout << "get: " << tmp << "\n";

		if (tmp == 3)
			return;
	}
}

int main()
{
	std::vector<std::thread> v;
	v.push_back(std::thread(data_processing_thread));

	for (int i = 0; i < 3; i++) {
		std::lock_guard<std::mutex> ld(m);
		data_queue.push(i+1);
		data_cond.notify_one();
	}

	for (unsigned int i = 0; i < v.size(); i++)
		if (v[i].joinable())
			v[i].join();


	return 0;
}
