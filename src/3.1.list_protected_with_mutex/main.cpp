#include <iostream>
#include <thread>
#include <list>
#include <mutex>
#include <algorithm>

std::list<int> some_list;
std::mutex some_mutex;

void list_add(int new_value) {
	std::lock_guard<std::mutex> guard(some_mutex);
	some_list.push_back(new_value);
}

bool list_contains(int value_to_find) {
	std::lock_guard<std::mutex> guard(some_mutex);
	return (std::find(some_list.begin(), some_list.end(), value_to_find)
	!= some_list.end());
}

void list_show() {
	for (auto i : some_list) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

void job(int n) {
	for (int i = 0; i < n; i++ )
		list_add(i);
}

int main()
{
	std::thread t[2];

	for (int i = 0; i < 2; i++)
		t[i] = std::thread(job, 100);

	for (int i = 0; i < 2; i++)
		t[i].join();

	list_show();

	return 0;
}
