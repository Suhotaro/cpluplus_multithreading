#include <iostream>
#include <thread>

#include "../include/func.h"

void join()
{
	std::cout << "join start\n";

	int local_val = 0;
	func my_func(local_val);
	std::thread my_thread(my_func);

	try
	{
		;
		/* do something */
	}
	catch (...)
	{
		my_thread.join();
		throw;
	}

	my_thread.join();

	std::cout << "join end\n";
}

int main()
{
	join();
	
	return 0;
}
