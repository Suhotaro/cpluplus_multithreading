#include <iostream>
#include <thread>
#include <list>
#include <mutex>
#include <algorithm>

class some_data {
	int a;
	std::string b;
public:
	void do_something() { std::cout << "do something\n"; }
};

class data_wrapper {
	some_data data;
	std::mutex m;
public:
	template<typename Function>
	void process_data(Function func) {
		std::lock_guard<std::mutex> l(m);
		func(data);
	}
};

some_data *unprotected;

void malicious_function(some_data &protected_data) {
	unprotected = &protected_data;
}

int main()
{
	data_wrapper x;
	x.process_data(malicious_function);
	unprotected->do_something();

	return 0;
}
