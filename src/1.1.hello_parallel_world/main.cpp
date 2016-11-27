#include <iostream>
#include <thread>

void hello()
{
	std::cout << "Hello parallel world\n";
}

int main()
{
	std::thread t(hello);
	t.join();
	
	return 0;
}
