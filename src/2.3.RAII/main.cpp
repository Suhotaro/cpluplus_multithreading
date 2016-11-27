#include <iostream>
#include <thread>

struct func
{
	int & i;
	func(int &i_) : i(i_) {}

	void operator() ()
	{
		std::cout << "operator start\n";

		for (unsigned int j = 0; j < 100; j++)
			std::cout << i << std::endl;

		std::cout << "operator end\n";
	}
};

class thread_guard
{
	std::thread &t;
public:
	explicit thread_guard(std::thread& t_) : t(t_) {}
	~thread_guard() {
		if (t.joinable())
			t.join();
	}

	thread_guard(thread_guard const& ) = delete;
	thread_guard operator=(thread_guard const& ) = delete;
};

void raii()
{
	std::cout << "raii start\n";

	int local_val = 0;
	func my_func(local_val);
	std::thread my_thread(my_func);
	thread_guard my_thread_guard(my_thread);

	/* do something */

	std::cout << "raii end\n";
}

int main()
{
	raii();
	
	return 0;
}
