#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <exception>

class data_holder {
private:
	int some_detail;
	mutable std::mutex m;

	int get_detail() const {
		std::lock_guard<std::mutex> lk(m);
		return some_detail;
	}
public:

	data_holder(int sd) : some_detail(sd) {}

	friend bool operator==(data_holder& lhs, data_holder& rhs) {
		if (&lhs == &rhs)
			return true;

		int const lhs_value = lhs.get_detail();
		int const rhs_value = rhs.get_detail();

		return lhs_value == rhs_value;
	}
};

int main()
{
	data_holder a(5);
	data_holder b(4);
	std::cout << "check a and b on equality: " << (a == b) << std::endl;

	std::cout << "check a and a on equality: " << (a == a) << std::endl;

	data_holder c(5);
	std::cout << "check a and c on equality: " << (a == c) << std::endl;

	return 0;
}
