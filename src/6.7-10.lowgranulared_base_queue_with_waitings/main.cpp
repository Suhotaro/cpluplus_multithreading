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

	mutex head_mutex;
	unique_ptr<node> head;
	mutex tail_mutex;
	node *tail;
	condition_variable data_cond;

	node *get_tail() {
		lock_guard<mutex> tail_lock(tail_mutex);
		return tail;
	}

	unique_ptr<node> pop_head() {
		unique_ptr<node> old_head = move(head);
		head = move(old_head->next);
		return old_head;
	}

	unique_ptr<node> try_pop_head() {
		lock_guard<mutex> head_lock(head_mutex);
		if (head.get() == get_tail())
			return unique_ptr<node>();
		return pop_head();
	}

	unique_ptr<node> try_pop_head(T& value) {
		lock_guard<mutex> head_lock(head_mutex);
		if (head.get() == get_tail())
			return unique_ptr<node>();
		value = move(*head->data);
		return pop_head();
	}

	unique_lock<mutex> wait_for_data() {
		unique_lock<mutex>  head_lock(head_mutex);
		data_cond.wait(head_lock, [&]{
			return head.get() != get_tail();
		});

		return move(head_lock);
	}

	unique_ptr<node> wait_pop_head() {
		unique_lock<mutex>  head_lock(wait_for_data());
		return pop_head();
	}

	unique_ptr<node> wait_pop_head(T &value) {
		unique_lock<mutex>  head_lock(wait_for_data());
		value = move(*head->data);
		return pop_head();
	}

public:
	queue () : head(new node), tail(head.get()) {}
	queue (const queue& other) = delete;
	queue& operator=(const queue& other) = delete;

	shared_ptr<T> try_pop() {
		unique_ptr<node> old_head = try_pop_head();
		return old_head?old_head->data:shared_ptr<T>();
	}

	bool try_pop(T &value) {
		unique_ptr<node> old_head = try_pop_head(value);
		return old_head;
	}

	shared_ptr<T> wait_and_pop() {
		unique_ptr<node> const old_head = wait_pop_head();
		return old_head->data;
	}

	void wait_and_pop(T &value) {
		unique_ptr<node> const old_head = wait_pop_head();
	}

	void push(T new_value) {
	    std::shared_ptr<T> new_data = std::make_shared<T>(std::move(new_value));
		unique_ptr<node> p(new node);
		{
			lock_guard<mutex> tail_lock(tail_mutex);

			node* const new_tail = p.get();

			tail->data = new_data;
			tail->next = move(p);
			tail = new_tail;
		}
		data_cond.notify_one();
	}

	bool empty() {
		lock_guard<mutex> head_lock(head_mutex);
		return (head.get() == get_tail());
	}

	template <typename Function>
	void do_job(Function f) {
		for (node * t = head.get(); t != tail; t = t->next.get())
			f(*t->data);
	}
};

int main()
{
	queue<int> q;
	q.push(5);
	q.push(6);
	q.push(7);

	shared_ptr<int> a = q.try_pop();

	q.do_job(show);

	return 0;
}
