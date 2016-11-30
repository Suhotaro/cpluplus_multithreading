#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <exception>

class empty_stack : public std::exception {
	const char *what() const throw() { return "empty stack exeption"; }
};

template <typename T>
class threadsafe_stack {
private:
	std::stack<T> data;
	mutable std::mutex m;
public:
	threadsafe_stack() {}
	threadsafe_stack(const threadsafe_stack& other) {
		std::lock_guard<std::mutex> lock(m);
		data = other.data;
	}

	threadsafe_stack& operator=(const threadsafe_stack& other) = delete;

	void push(T new_value) {
		std::lock_guard<std::mutex> lock(m);
		data.push(new_value);
	}

	std::shared_ptr<T> pop() {
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
			throw empty_stack();

		/*XXX: if there will be thrown memory exception we do not pop a value from data */
		std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
		data.pop();
		return res;
	}

	void pop(T& value) {
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
			throw empty_stack();
		value = data.top();
		data.pop();
	}

	bool empty() const {
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
};

int main()
{

	return 0;
}
