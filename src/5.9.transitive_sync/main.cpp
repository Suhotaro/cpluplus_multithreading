#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <string>
#include <cassert>

using namespace std;

atomic<int> data[5];
atomic<bool> sync1(false), sync2(false);

void thread_1() {
	data[0].store(42, memory_order_relaxed);
	data[1].store(97, memory_order_relaxed);
	data[2].store(17, memory_order_relaxed);
	data[3].store(-141, memory_order_relaxed);
	data[4].store(2016, memory_order_relaxed);
	/* XXX: above instructions are synced by bellow barier */
	sync1.store(true, memory_order_release);
}

void thread_2() {
	while(!sync1.load(memory_order_acquire))
		;
	sync2.store(true, memory_order_release);
}

void thread_3() {
	while(!sync2.load(memory_order_acquire))
	std::cout << data[0].load(memory_order_relaxed) << " == " << 42 << endl;
	std::cout << data[1].load(memory_order_relaxed) << " == " << 97 << endl;
	std::cout << data[2].load(memory_order_relaxed) << " == " << 17 << endl;
	std::cout << data[3].load(memory_order_relaxed) << " == " << -141 << endl;
	std::cout << data[4].load(memory_order_relaxed) << " == " << 2016 << endl;
}

int main()
{
	thread a(thread_1);
	thread b(thread_2);
	thread c(thread_3);

	a.join();
	b.join();
	c.join();

	return 0;
}
