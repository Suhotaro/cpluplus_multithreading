#include <iostream>
#include <mutex>
#include <memory>
#include <thread>
#include <condition_variable>

using namespace std;

void show(int num) {
	std::cout << "num:" << num << endl;
}

template<typename T>
class queue {
private:
	struct node {
		shared_ptr<T> data;
		unique_ptr<node> next;
	};

	unique_ptr<node> head;
	node *tail;

public:
	queue () : head(new node), tail(head.get()) {}
	queue (const queue& other) = delete;
	queue& operator=(const queue& other) = delete;

	shared_ptr<T> try_pop() {
		if (head.get() == tail)
			return shared_ptr<T>();

		shared_ptr<T> const res(head->data);
		unique_ptr<node> const old_head = move(head);
		head = move(old_head->next);

		return res;
	}

	void push(T new_value) {
	    std::shared_ptr<T> new_data = std::make_shared<T>(std::move(new_value));

		unique_ptr<node> p(new node);
		tail->data = new_data;

		node *const new_tail = p.get();
		tail->next = move(p);
		tail = new_tail;
	}

	template <typename Function>
	void do_job(Function f) {
		for (node * t = head->next.get(); t != tail; t = t->next.get())
			f(*t->data);
	}
};

int main()
{
	queue<int> q;
	q.push(5);
	q.push(6);
	q.push(7);

	//shared_ptr<int> a = q.try_pop();

	//cout << *a << endl;

	q.do_job(show);

	return 0;
}
