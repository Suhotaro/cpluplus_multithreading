#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>
#include <thread>
#include <condition_variable>

template<typename T>
class threadsafe_queue {
private:
	std::mutex m;
	std::queue<T> data_queue;
	std::condition_variable data_cond;

public:
	void push(T new_value) {
		std::lock_guard<std::mutex> lk(m);
		data_queue.push(new_value);
		data_cond.notify_one();
	}

	void wait_and_pop(T& value) {
		std::unique_lock<std::mutex> lk(m);
		data_cond.wait(lk, [this]{ return !data_queue.empty(); });
		value = data_queue.front();
		data_queue.pop();
	}

};

threadsafe_queue<int> data_queue;

void data_processing_thread() {
	int tmp;

	while(true) {
		data_queue.wait_and_pop(tmp);
		std::cout << "get: " << tmp << "\n";
		if (tmp == 1)
			return;
	}
}

int main()
{
	std::vector<std::thread> v;
	v.push_back(std::thread(data_processing_thread));

	for (int i = 3; i > 0; i--)
		data_queue.push(i);

	if (v[0].joinable())
		v[0].join();

	return 0;
}
