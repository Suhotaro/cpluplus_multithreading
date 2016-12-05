#include <iostream>
#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

vector<int> queue_data;
atomic<int> count;

void populate_queue() {
	unsigned int numver_of_items = 20;

//	this_thread::sleep_for(chrono::seconds(1));

	queue_data.clear();
	for (unsigned int i = 0; i < numver_of_items; i++)
		queue_data.push_back(i);

	/* XXX: it writes 20 at once */
	count.store(numver_of_items, std::memory_order_release);
}

void consume_queue_items(int t){
	while(true) {
		int item_index;
		/* XXX: we do not care if count is minus alot =) */
		if ((item_index = count.fetch_sub(1, std::memory_order_acquire)) <= 0) {
			continue;
		}

		cout << "t" << t << ": " << queue_data[item_index - 1] << endl;

		if(queue_data[item_index - 1] == 0)
			return;
	}
}

void good() {
	std::thread t(populate_queue);
	std::thread t2(consume_queue_items, 1);

	t.join();
	t2.join();
}

void bad() {
	std::thread t(populate_queue);
	std::thread t2(consume_queue_items, 1);
	std::thread t3(consume_queue_items, 2);

	t.join();
	t2.join();
	t3.join();
}

int main()
{
	good();
	//bad();

	return 0;
}
