#include <iostream>
#include <mutex>
#include <memory>
#include <atomic>
#include <thread>

using namespace std;

template <typename T>
class lock_free_stack
{
private:
	struct node {
		shared_ptr<T> data;
		node *next;
		node(T const& data_) :
			data(make_shared<T>(data_)), next(nullptr) {}
	};

	atomic<node*> head;

public:
	void push (T const& data) {
		node *const new_node = new node(data);
		new_node->next = head.load();

		while(!head.compare_exchange_weak(new_node->next, new_node))
			;
	}

	shared_ptr<T> pop() {
		node* old_head = head.load();

		while(old_head &&
			!head.compare_exchange_weak(old_head, old_head->next) )
			;

		return old_head ? old_head->data : shared_ptr<T>();
	}

	void show() {
		node* n = head.load();

		while(n->next)
		{
			printf("%d \n", *n->data);
			n = n->next;
		}
	}
};

int main()
{
	lock_free_stack<int> lfstak;

	lfstak.push(5);
	lfstak.push(8);
	lfstak.push(10);

	lfstak.show();

	return 0;
}


