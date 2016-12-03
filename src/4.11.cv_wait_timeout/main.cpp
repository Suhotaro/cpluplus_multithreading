#include <iostream>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::condition_variable cv;
bool done;
std::mutex m;

bool wait_loop() {
	auto const timeout = std::chrono::system_clock::now() +
		std::chrono::milliseconds(2000);

	std::unique_lock<std::mutex> lk(m);
	while (!done) {
		std::cout << "wait...\n";
		if (cv.wait_until(lk, timeout) == std::cv_status::timeout)
			break;
	}

	std::cout << "done\n";
	return done;
}

int main()
{
	bool f = wait_loop();
	(void) f;

	return 0;
}
