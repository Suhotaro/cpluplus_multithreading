#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <string>
#include <cassert>

using namespace std;

struct X {
	int i;
	string s;
};

atomic<X*> p;
atomic<int> a;

void creat_x() {
	X *x = new X;
	x->i = 42;
	x->s = "hello";

	a.store(99, memory_order_relaxed);
	p.store(x, memory_order_relaxed);
}

void use_x() {
	X *x;
	while(!(x=p.load(std::memory_order_consume)))
		this_thread::sleep_for(chrono::microseconds(1));

	assert(x->i == 42);
	assert(x->s == "hello");
	/* XXX: on ARM SoCs it could false or on not sequential concistent socs */
	assert(a.load(memory_order_relaxed) == 99);
}

int main()
{
	std::thread a(creat_x);
	std::thread b(use_x);

	a.join();
	b.join();

	return 0;
}
