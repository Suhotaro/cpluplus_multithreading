#include <iostream>
#include <future>
#include <string>

struct X
{
	void foo(int num, std::string str) {
		std::cout << "foo: num:" << num << " str:" << str << std::endl;
	}

	std::string bar(std::string& str) {
			std::cout << "bar: str:" << str << std::endl;
			std::string ret("world");
			return ret;
		}
};

struct Y
{
	double operator() (double val) {
		std::cout << "operator: val:" << val<< std::endl;
		return 3.1415;
	}
};

X baz(X&) {return X();}

int main()
{
	X x;
	auto f1 = std::async(&X::foo, &x, 42, "hello"); /* called p->foo where p=&x */
	f1.get();
	std::string str = "goodbay";
	auto f2 = std::async(&X::bar, x, std::ref(str)); /* called tmpx.bar */
	std::cout << "returned string: " << f2.get() << std::endl;

	Y y;
	auto f3 = std::async(Y(), 3.1415); /* tmpy(3.1415) tmpy is copy of Y maden by mode constructor */
	auto f4 = std::async(std::ref(y), 2.718);  /* y(2.178) */
	f3.get();
	f4.get();

	X baz(X&);
	auto f5 = std::async(baz, std::ref(x));  /* baz(x) */
	f5.get();

	/* In case of only movable objects  will be called std::move(Obj()) */

	return 0;
}


