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
	atomic<node*> to_be_deleted;
	atomic<unsigned> threads_in_pop;

	static void delete_nodes(node *nodes)
	{
		while(nodes)
		{
			node *next = nodes->next;
			delete nodes;
			nodes = next;
		}
	}

	void try_reclaim(node *old_head) {
		if (threads_in_pop == 1)
		{
			node *nodes_to_delete = to_be_deleted.exchange(nullptr);
			if (!--threads_in_pop)
				delete_nodes(nodes_to_delete);
			else if (nodes_to_delete)
				chain_pending_nodes(nodes_to_delete);
			delete old_head;
		}
		else
		{
			chain_pending_node(old_head);
			--threads_in_pop;
		}
	}

	void chain_pending_nodes(node* nodes) {
		node* last = nodes;

		while(node *const next=last->next)
			last=next;

		chain_pending_nodes(nodes, last);
	}

	void chain_pending_nodes(node* first, node* last) {
		last->next = to_be_deleted;
		while(!to_be_deleted.compare_exchange_weak(
				last->next, first))
			;
	}

	void chain_pending_node(node* n) {
		chain_pending_nodes(n, n);
	}

public:
	lock_free_stack () {
		head.store(nullptr);
	}

	void push (T const& data) {
		node *const new_node = new node(data);
		new_node->next = head.load();

		while(!head.compare_exchange_weak(new_node->next, new_node))
			;
	}

	shared_ptr<T> pop() {
		++threads_in_pop;
		node* old_head = head.load();

		while(old_head &&
			!head.compare_exchange_weak(old_head, old_head->next))
			;

		shared_ptr<T> res;

		if (old_head)
			res.swap(old_head->data);

		try_reclaim(old_head);

		return res;
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

int main()
{
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

	return 0;
}


