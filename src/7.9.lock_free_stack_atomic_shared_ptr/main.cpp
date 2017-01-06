#include <iostream>
#include <mutex>
#include <memory>
#include <atomic>
#include <thread>

using namespace std;

// For me it loocks strange that shared_ptr could work with atomic_* functions

#if 0
template <typename T>
class lock_free_stack
{
private:
	struct node
	{
		shared_ptr<T> data;
		shared_ptr<node> next;

		node(T const &data_) :
		data(make_shared<T>(data_)) {}
	};

	shared_ptr<node> head;
public:
	void push(T const& data)
	{
		shared_ptr<node> const new_node =
			make_shared<node> (data);
		new_node->next = atomic_load(&head);

		while(!atomic_compare_exchange_weak(
			&head, &new_node->next, new_node))
				;
	}

	shared_ptr<T> pop()
	{
		shared_ptr<node> old_head = atomic_load(&head);
		while(old_head &&
			atomic_compare_exchange_weak(&head,
				&old_head, old_head->next))
					;

		return old_head ? old_head->data : shared_ptr<T>();
	}


	void show() {
		node* n = head.load();

		while(n)
		{
			printf("%d \n", *n->data);
			n = n->next;
		}
	}
};
#endif

int main()
{
#if 0
	lock_free_stack<int> lfstak;

	lfstak.push(5);
	lfstak.push(8);
	lfstak.push(10);

	cout << "show\n";
	lfstak.show();
	cout << endl;

	cout << "pop: " << *lfstak.pop() << endl;
	cout << "pop: " << *lfstak.pop() << endl;
	cout << endl;

	cout << "show2\n";
	lfstak.show();
	cout << endl;

	lfstak.push(1);
	lfstak.push(2);

	cout << "show2\n";
	lfstak.show();
	cout << endl;
#endif
	return 0;
}


