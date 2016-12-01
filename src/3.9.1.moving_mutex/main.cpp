#include <iostream>
#include <mutex>

std::unique_lock<std::mutex> return_lock() {
	std::mutex m;
	std::unique_lock<std::mutex> lk(m);

	std::cout << "own lock 1:" << lk.owns_lock() << std::endl;

	return lk;
}

void acquire_lock(std::unique_lock<std::mutex> lk) {
	std::cout << "own lock 3:" << lk.owns_lock() << std::endl;
	lk.unlock();
	std::cout << "own lock 4:" << lk.owns_lock() << std::endl;
}

int main()
{
	std::unique_lock<std::mutex> lk(return_lock());
	std::cout << "own lock 2:" << lk.owns_lock() << std::endl;
	acquire_lock(std::move(lk));

	return 0;
}
