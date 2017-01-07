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
	struct node;

	struct counted_node_ptr
	{
		int external_count;
		node *ptr;
	};

	struct node
	{
		shared_ptr<T> data;
		atomic<int> interanl_count;
		counted_node_ptr *next;

		node(T const &data_) :
		data(make_shared<T>(data_)),
		interanl_count(0)
		{}
	};

	atomic<counted_node_ptr*> head;

	void increase_head_count(counted_node_ptr*
		old_counter)
	{
		counted_node_ptr *new_counter;

		do
		{
			new_counter = old_counter;
			++new_counter->external_count;

			printf("11: %d %d\n", old_counter->external_count, new_counter->external_count);

			printf("2: %p %p\n", old_counter, new_counter);
		}
		while (!head.compare_exchange_strong(
			old_counter, new_counter))
				;

		printf("3: %d %d\n", old_counter->external_count, new_counter->external_count);

		old_counter->external_count =
			new_counter->external_count;

		printf("4: %p %p\n", old_counter, new_counter);
	}

public:
	lock_free_stack() : head(nullptr) {}

	~lock_free_stack() {
#if 0
		while(pop())
			;
#endif
	}

	void push(T const& data)
	{
		counted_node_ptr *new_node = new counted_node_ptr;

		new_node->ptr = new node(data);
		new_node->external_count=1;
		new_node->ptr->next = head.load();
		while(!head.compare_exchange_weak(
			new_node->ptr->next, new_node))
				;
	}

	shared_ptr<T> pop()
	{
		counted_node_ptr *old_head = head.load();

		for(;;) {
			printf("1: %p\n", old_head);
			increase_head_count(old_head);
			node *const ptr = old_head->ptr;
			if (!ptr)
				return shared_ptr<T>();

			if (head.compare_exchange_strong(
					old_head, ptr->next))
			{
				shared_ptr<T> res;
				res.swap(ptr->data);
				int const count_increase = old_head->external_count - 2;
				int ress = ptr->interanl_count.fetch_add(count_increase);

				printf("5: %d %d %d\n", count_increase, -count_increase, ress);

				if (ress == -count_increase)
				{
					delete ptr;
					delete old_head;
				}
				return res;
			}
			else if (ptr->interanl_count.fetch_sub(1) == 1)
			{
				delete ptr;
				delete old_head;
			}
		}
	}

	void show() {
		counted_node_ptr *cnp = head.load();

		while(cnp)
		{
			node *n = cnp->ptr;
			printf("%d \n", *n->data);
			cnp = n->next;
		}
	}

};

int main()
{

	lock_free_stack<int> lfstak;

	lfstak.push(5);
	lfstak.push(8);
	lfstak.push(10);

	cout << "show\n";
	lfstak.show();
	cout << endl;

	cout << "done\n";

	cout << "pop: " << *lfstak.pop() << endl;
	//cout << "pop: " << *lfstak.pop() << endl;
	cout << endl;

	cout << "show2\n";
	lfstak.show();
	cout << endl;

	lfstak.push(1);
	lfstak.push(2);

	cout << "show3\n";
	lfstak.show();
	cout << endl;

	return 0;
}


