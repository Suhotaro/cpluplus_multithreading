#include <iostream>
#include <future>

int find_the_answer_to_ltuae()
{
	std::cout << "find the answer" << std::endl;
	return 42;
}

void do_work(std::promise<void> barrier) {
	std::cout << "t2: working...\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "t2: signal\n";
	barrier.set_value();
}

void set_promise(std::promise<int> promise) {
	std::cout << " t4: working...\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << " t4: set value\n";
	promise.set_value(5);
}

void set_future(std::future<int>& future) {
	std::cout << "  t6: wait...\n";
	int tmp = future.get();
	std::cout << "  t6: get value:" << tmp << std::endl;
}

void ex1() {
	std::future<int> the_answer = std::async(find_the_answer_to_ltuae);
	std::cout << "the answer is:" << the_answer.get() << std::endl;
}

void ex2() {
	/* Example of signalling between thread with "promise<void>" */
	std::promise<void> barrier;
	std::future<void> barrier_future = barrier.get_future();
	std::thread t(do_work, std::move(barrier));
	std::cout << "t1: wait signal\n";
	barrier_future.wait();
	std::cout << "t1: received signal\n";
	t.join();
}

void ex3() {
	std::promise<int> prom;
	std::future<int> fut = prom.get_future();
	std::thread t(set_promise, std::move(prom));
	std::cout << " t3: received future:" << fut.get() << std::endl;
	t.join();
}

void ex4() {
	std::promise<int> promise;
	std::future<int> future = promise.get_future();
	std::thread t(set_future, std::ref(future));

	std::cout << "  t5: working...\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "  t5: set future\n";
	promise.set_value(10);
	t.join();
}


int main()
{
	ex1();
	ex2();
	ex3();
	ex4();

	return 0;
}


