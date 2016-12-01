#include <iostream>
#include <memory>
#include <mutex>

struct some_resource {
	int num;
	some_resource(int num_) : num(num_) {}
	void show() { std::cout << "num: " << num << std::endl; }
};

std::shared_ptr<some_resource> resource_ptr;
std::once_flag resource_flag;

void resource_init() {
	std::cout << "init resource\n";
	resource_ptr.reset(new some_resource(42));
}

void foo() {
	std::cout << "call foo\n";

	std::call_once(resource_flag, resource_init);
	resource_ptr->show();
}


int main()
{
	foo();
	foo();
	foo();

	return 0;
}
