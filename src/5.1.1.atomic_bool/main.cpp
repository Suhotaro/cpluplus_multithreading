#include <iostream>
#include <atomic>
#include <cassert>

using namespace std;

void ex1() {
	atomic<bool> b;
	cout << "1: b:" << b.load(std::memory_order_acquire) << endl;

	b.store(false);
	cout << "1: b:" << b.load(std::memory_order_acquire) << endl;

	b.store(true);
	cout << "1: b:" << b.load(std::memory_order_acquire) << endl;

	bool x = b.exchange(false, std::memory_order_acq_rel);
	cout << "1: b:" << b.load(std::memory_order_acquire) << ", x:" << x << endl;

	x = b.exchange(false, std::memory_order_acq_rel);
	cout << "1: b:" << b.load(std::memory_order_acquire) << ", x:" << x << endl;

	x = b.exchange(true, std::memory_order_acq_rel);
	cout << "1: b:" << b.load(std::memory_order_acquire) << ", x:" << x << endl;
}

void ex2() {
	atomic<int> a;
	int b = 4;
	int c = 5;
	bool ex = false;
	a = 3;

	auto handle = [&]() {
		cout << "ex: " << ex << ", "
		     << "a:" << a << ", "
			 << "b:" << b << ", "
			 << "c:" << c << ", "
			 << endl;
	};

	handle();

	ex = a.compare_exchange_strong(b, c);
	handle();

	ex = a.compare_exchange_strong(b, c);
	handle();

	c = 7;
	ex = a.compare_exchange_strong(b, c);
	handle();

	ex = a.compare_exchange_strong(b, c);
	handle();
}

void ex3() {
	class A {
	public:
		A() {}
	};

	A arr[5];
	std::atomic<A*> p(arr);

	A* x = p.fetch_add(2);

	assert(x == arr);
	assert(p.load() == &arr[2]);

	x = (p-=1);
	assert(x == &arr[1]);
	assert(p.load() == &arr[1]);
}

void ex4() {
	std::atomic<int> p(0);
	auto handle = [&]() { cout << "p: " << p << endl; };

	handle();

	p.fetch_sub(1);
	handle();

	p.fetch_sub(1);
	handle();



}

int main()
{
	ex1();
	ex2();
	ex3();
	ex4();

	return 0;
}
