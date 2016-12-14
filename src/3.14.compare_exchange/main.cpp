#include <iostream>
#include <atomic>

using namespace std;

int main()
{
	std::atomic<int> var(1);
	int t = 2, t2 = 3;
	int res = 0;

	auto handle = [&]{
		static int times = 1;
		printf("%d: res=%d  var=%d  t=%d  t2=%d\n", times, res, var.load(), t, t2);
		times += 1;
	};

	handle();

	res = var.compare_exchange_weak(t, t2);
	handle();

	res = var.compare_exchange_weak(t, t2);
	handle();

	res = var.compare_exchange_weak(t, t2);
	handle();

	res = var.compare_exchange_weak(t, t2);
	handle();

	res = var.compare_exchange_weak(t, t2);
	handle();

	return 0;
}

