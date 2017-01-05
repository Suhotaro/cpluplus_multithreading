#include <iostream>
#include <mutex>
#include <memory>
#include <atomic>
#include <thread>

using namespace std;

unsigned const max_hazard_pointers=100;

struct hazard_pointer
{
	atomic<thread::id> id;
	atomic<void*> pointer;
};

hazard_pointer hazard_pointers[max_hazard_pointers];

class hp_owner
{
	hazard_pointer *hp;

public:
	hp_owner(hp_owner const&) = delete;
	hp_owner operator=(hp_owner const&) = delete;
	hp_owner() : hp(nullptr) {
		for (unsigned i = 0; i < max_hazard_pointers; i++)
		{
			thread::id old_id;
			if (hazard_pointers[i].id.compare_exchange_weak(
					old_id, this_thread::get_id()))
			{
				hp = &hazard_pointers[i];
				break;
			}

			if (!hp)
				throw runtime_error("No hazard pointers available");
		}
	}

	atomic<void *>& get_pointer() {
		return hp->pointer;
	}

	~hp_owner() {
		hp->pointer.store(nullptr);
		hp->id.store(thread::id());
	}
};

atomic<void*> & get_hazard_pointer_for_current_thread() {
	thread_local static hp_owner hazard;
	return hazard.get_pointer();
}

bool outstanding_hazard_pointers_for(void *p) {
	for (unsigned i =0; i< max_hazard_pointers; i++)
		if (hazard_pointers[i].pointer.load()==p)
			return true;
	return false;
}

template <typename T>
void do_delete(void *p)
{
	delete static_cast<T*>(p);
}

struct data_to_reclaim
{
	void *data;
	function<void(void*)> deleter;
	data_to_reclaim *next;

	template <typename T>
	data_to_reclaim(T* p) :
	data(p),
	deleter(&do_delete<T>),
	next(0) {}

	~data_to_reclaim() {
		deleter(data);
	}
};

atomic<data_to_reclaim*> nodes_to_reclaim;

void add_to_reclaim_list(data_to_reclaim* node)
{
	node->next = nodes_to_reclaim.load();
	while (nodes_to_reclaim.compare_exchange_weak(node->next, node))
		;
}

template <typename T>
void reclaim_later(T* data)
{
	add_to_reclaim_list(new data_to_reclaim(data));
}

void delete_nodes_with_no_hazards()
{
	data_to_reclaim* current = nodes_to_reclaim.exchange(nullptr);
	while(current)
	{
		data_to_reclaim* const next = current->next;
		if (!outstanding_hazard_pointers_for(current->data))
			delete current;
		else
			add_to_reclaim_list(current);
		current=next;
	}
}

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
		atomic<void*> &hp = get_hazard_pointer_for_current_thread();
		node *old_head=head.load();

		do
		{
			node *temp;
			do
			{
				temp = old_head;
				hp.store(old_head);
				old_head = head.load();
			} while (old_head != temp)
				;
		} while (old_head &&
				!head.compare_exchange_strong(old_head, old_head->next))
			;

		hp.store(nullptr);
		shared_ptr<T> res;

		if (old_head) {
			res.swap(old_head->data);
			if (outstanding_hazard_pointers_for(old_head))
				reclaim_later(old_head);
			else
				delete old_head;

			delete_nodes_with_no_hazards();
		}

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


