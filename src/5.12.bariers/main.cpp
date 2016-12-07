#include <iostream>
#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

atomic<bool> x,y;
atomic<int> z;

void write_x_then_y() {
	x.store(true, memory_order_relaxed);
	atomic_thread_fence(memory_order_relaxed);
	y.store(true, memory_order_relaxed);
}

void read_y_then_x() {
	while(!y.load(memory_order_relaxed))
		;
	atomic_thread_fence(memory_order_acquire);
	if (x.load(memory_order_relaxed))
		++z;
}

int main()
{
	x = false;
	y = false;
	z = 0;

	thread a(write_x_then_y);
	thread b(read_y_then_x);
	a.join();
	b.join();

	/* XXX: It always more then 0 due to using fences */
	cout << z.load() << endl;

	return 0;
}
