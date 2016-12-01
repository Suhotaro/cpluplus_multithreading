#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <exception>

int data_read() {
	return 42;
}

void data_write(int num) {
	(void)num;
}

int process(int num) {
	return num + 1;
}

std::mutex m;

void data_process() {
	std::unique_lock<std::mutex> lk(m);
	int num = data_read();
	lk.unlock();

	/* If process takes a lot of time to return result it is
	 * resonable to unlcok our mutex since other threads might
	 * use data without waiting until data_process finishes
	 */

	int result = process(num);

	lk.lock();
	data_write(result);
}

int main()
{
	data_process();
	return 0;
}
