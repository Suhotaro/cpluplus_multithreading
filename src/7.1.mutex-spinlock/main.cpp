#include <iostream>
#include <mutex>
#include <atomic>
#include <thread>

using namespace std;

class spinlock_mutex
{
	atomic_flag flag;

public:
	spinlock_mutex() : flag(ATOMIC_FLAG_INIT) {}

	void lock() {
		while (flag.test_and_set(memory_order_acquire))
			;
	}

	void unlock() {
		flag.clear(memory_order_release);
	}
};

spinlock_mutex spin_mutex;

int main()
{

	thread t([]() {
		printf("t1: lock\n");
		spin_mutex.lock();
		printf("t1: locked\n");
		std::this_thread::sleep_for (std::chrono::seconds(3));
		spin_mutex.unlock();
		printf("t1: unlock\n");
	});

	thread t2([]() {
		std::this_thread::sleep_for (std::chrono::seconds(1));
		printf("t2: lock\n");
		spin_mutex.lock();
		printf("t2: locked\n");
		spin_mutex.unlock();
		printf("t2: unlock\n");
	});


	t.join();
	t2.join();

	return 0;
}


