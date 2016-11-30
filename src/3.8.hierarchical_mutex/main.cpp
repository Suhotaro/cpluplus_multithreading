#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <exception>
#include <climits>

class hierarchical_mutex {
	std::mutex internal_mutex;
	unsigned long const hierarchical_value;
	unsigned long previous_hierarchical_value;
	static thread_local unsigned long this_thread_hierarchical_value;

	void check_for_hierarchical_violation() {
		if (this_thread_hierarchical_value <= hierarchical_value) {
			throw std::logic_error("mutex hierarchy violated");
		}
	}

	void update_hierarchical_value() {
		previous_hierarchical_value = this_thread_hierarchical_value;
		this_thread_hierarchical_value = hierarchical_value;
	}

public:
	explicit hierarchical_mutex(unsigned long value) :
	hierarchical_value(value),
	previous_hierarchical_value(0)
	{}

	void lock() {
		check_for_hierarchical_violation();
		internal_mutex.lock();
		update_hierarchical_value();
	}

	void unlock() {
		this_thread_hierarchical_value = previous_hierarchical_value;
		internal_mutex.unlock();
	}

	bool try_lock() {
		check_for_hierarchical_violation();
		if (!internal_mutex.try_lock())
			return false;
		update_hierarchical_value();
		return true;
	}
};

thread_local unsigned long hierarchical_mutex::this_thread_hierarchical_value(ULONG_MAX);

int main()
{
	hierarchical_mutex m10(10);
	hierarchical_mutex m5(5);
	hierarchical_mutex m2(2);

	/* Correct using */
	try {
		m10.lock();
		m5.lock();
		m2.lock();

		m2.unlock();
		m5.unlock();
		m10.unlock();
	}
	catch (std::logic_error& e) {
		std::cout << "got exception 1: " << e.what();
	}

	/* Incorrect using */
	try {
		m10.lock();
		m2.lock();
		m5.lock();

		m5.unlock();
		m2.unlock();
		m10.unlock();
	}
	catch (std::logic_error& e) {
		std::cout << "got exception 2: " << e.what();
	}

	return 0;
}
