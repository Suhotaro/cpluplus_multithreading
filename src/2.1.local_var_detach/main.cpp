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

void oops()
{
	std::cout << "ooops start\n";

	int local_val = 0;
	func my_func(local_val);
	std::thread my_thread(my_func);
	my_thread.detach();

	std::cout << "ooops end\n";
}

int main()
{
	oops();
	
	return 0;
}
