#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

void ex1() {
	[] { std::cout << "ex1: Hello lambda world\n"; }();
}

int global = 1;

void ex2() {
	[] { cout << "ex2: global val:" << global << endl; }();
}

void ex3() {
	int tmp = [] { return 2; }();
	cout << "ex3: return val:" << tmp << endl;
}

void ex4() {
	bool tmp = []()->bool { return 2; }();
	cout << "ex4: return first bool val:" << tmp << endl;

	tmp = []()->bool { return 0; }();
		cout << "ex4: return second bool val:" << tmp << endl;
}

void ex5() {
	int local = 3;

	[=] { cout << "ex5: captured local val:" << local << endl; }();
}

void ex6() {
	int local = 3;

	local = [=]()->int { return local + 1; }();
	cout << "ex6: captured returned local val:" << local << endl;
}

void ex7() {
	int local = 4;
	int local2 = 5;

	[=] { cout << "ex7: captured local vals:" << local << ", "<< local2 << endl; }();
}

void ex8() {
	int local = 6;

	[&] { local += 4; }();

	cout << "ex8: referenced local vals:" << local << endl;
}

void ex9() {
	int i = 10, j = 20, k = 30;

	std::function<int()> f = [=, &j, &k] {return i+j+k;};

	i = 1;
	j = 2;
	k = 3;

	cout << "ex9: mutual example of capturing all local variable by copies and j,k by references: " << 	f() << endl;
}

void ex10() {
	int i = 10, j = 20, k = 30;

	std::function<int()> f = [&, j, k] {return i+j+k;};

	i = 1;
	j = 2;
	k = 3;

	cout << "ex10: mutual example of capturing all local variable by copies and j,k by references: " << f() << endl;
}

void ex11() {
	int i = 10, j = 20, k = 30;

	std::function<int()> f = [&i, j, &k] {return i+j+k;};

	i = 1;
	j = 2;
	k = 3;

	cout << "ex12: chose only required variables: " << f() << endl;
}

struct X
{
	int some_data;

	X(int some_data_) : some_data(some_data_) {}

	void foo(vector<int>& v) {
		for_each(v.begin(), v.end(), [this](int& i){i+=some_data;});
	}
};

void ex12() {
	cout << "ex13: this in lambda" << endl;

	X x(10);
	vector<int> v = {1, 2, 3, 4, 5};

	for_each(v.begin(), v.end(), [](int& i){cout << i << " ";});
	cout << endl;

	x.foo(v);

	for_each(v.begin(), v.end(), [](int& i){cout << i << " ";});
	cout << endl;
}

int main() 
{
	ex1();
	ex2();
	ex3();
	ex4();
	ex5();
	ex6();
	ex7();
	ex8();
	ex9();
	ex10();
	ex11();
	ex12();

	return 0;
}








