#include <iostream>
#include <thread>
#include <unistd.h>

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

class scoped_thread {
	std::thread t;
public:
	explicit scoped_thread(std::thread t_) : t(std::move(t_)) {
		if (!t.joinable())
			throw std::logic_error("No thread");
	}

	~scoped_thread() { t.join(); }

	scoped_thread(scoped_thread const&) = delete;
	scoped_thread& operator=(scoped_thread const&) = delete;
};

void foo() {
	std::cout << "foo start\n";

	int local_var = 1;
	scoped_thread t(std::move(std::thread(func(local_var))));
	(void)local_var;

	sleep(1);

	std::cout << "foo end\n";
}

int main()
{
	foo();

	return 0;
}
