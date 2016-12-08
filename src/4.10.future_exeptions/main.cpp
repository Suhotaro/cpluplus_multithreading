#include <iostream>
#include <future>
#include <thread>
#include <exception>

void job(std::promise<int> &p, double numerator, double denominator) {

	if (denominator == 0) {
		p.set_exception(std::make_exception_ptr(std::logic_error("divide by 0")));
		return;
	}

	p.set_value(numerator/denominator);
}

void job2(std::promise<int> &p, int numerator, int denominator) {

	try {
		if (denominator == 0)
			throw std::overflow_error("Divide by zero exception");
		int tmp = numerator/denominator;

		p.set_value(tmp);
	}
	catch (...) {
		try {
			p.set_exception(std::current_exception());
		}
		catch (...) { /* set_exeption can throw exeptions too */ }
	}
}

int main()
{
	std::promise<int> p;
	std::future<int> f = p.get_future();

	std::thread t(job, std::ref(p), 16, 2);
	try {
		std::cout << "get future 1: " << f.get() << std::endl;
	}
	catch (std::logic_error& e) {
		std::cout << "exeption: " << e.what() << std::endl;
	}
	t.join();


	std::promise<int> p2;
	std::future<int> f2 = p2.get_future();
	std::thread t2(job, std::ref(p2), 16, 0);
	try {
		std::cout << "get future 2: " << f2.get() << std::endl;
	}
	catch (std::logic_error& e) {
		std::cout << "exeption 2: " << e.what() << std::endl;
	}
	t2.join();

	std::promise<int> p3;
	std::future<int> f3 = p3.get_future();
	std::thread t3(job2, std::ref(p3), 16, 0);
	try {
		std::cout << "get future 3: " << f3.get() << std::endl;
	}
	catch (...) {
		std::cout << "exeption 3: \n";
	}
	t3.join();

	return 0;
}
