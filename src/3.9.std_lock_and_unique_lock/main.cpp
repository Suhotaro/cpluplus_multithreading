#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <exception>

class some_big_object {
public:
	some_big_object() {}
};

void swap(some_big_object &lhs, some_big_object &rhs) { std::cout << "swap some_big_objects\n"; }

class X {
private:
	some_big_object some_detail;
	std::mutex m;

public:
	X(some_big_object const& sd) : some_detail(sd) {}

	friend void swap(X& lhs, X& rhs) {
		if (&lhs == &rhs)
			return;

		std::unique_lock<std::mutex> lock_a(lhs.m, std::defer_lock);
		std::unique_lock<std::mutex> lock_b(rhs.m, std::defer_lock);
		std::lock(lhs.m, rhs.m);

		swap(lhs.some_detail, rhs.some_detail);
	}
};

int main()
{
	some_big_object some_a;
	some_big_object some_b;

	X x(some_a);
	X y(some_b);

	swap(x, y);

	return 0;
}
