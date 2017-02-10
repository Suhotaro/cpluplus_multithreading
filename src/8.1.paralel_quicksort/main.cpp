#include <iostream>
#include <mutex>
#include <memory>
#include <atomic>
#include <thread>
#include <list>
#include <stack>
#include <exception>
#include <vector>
#include <future>
#include <boost/thread/shared_mutex.hpp>

using namespace std;

class empty_stack : public std::exception {
	const char *what() const throw() { return "empty stack exeption aahhhh"; }
};

template <typename T>
class thread_safe_stack {
private:
	std::stack<T> data;
	mutable std::mutex m;
public:
	thread_safe_stack() {}
	thread_safe_stack(const thread_safe_stack& other) {
		std::lock_guard<std::mutex> lock(m);
		data = other.data;
	}

	thread_safe_stack& operator=(const thread_safe_stack& other) = delete;

	void push(T new_value) {
		std::lock_guard<std::mutex> lock(m);
		data.push(new_value);
	}

	std::shared_ptr<T> pop() {
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
			return nullptr;

		/*XXX: if there will be thrown memory exception we do not pop a value from data */
		std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
		data.pop();
		return res;
	}

	void pop(T& value) {
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
			return nullptr;
		value = data.top();
		data.pop();
	}

	bool empty() const {
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
};

template <typename T>
struct sorter
{
	struct chunk_to_sort
	{
		list<T> data;
		promise<list<T> > promises;

		chunk_to_sort() {}

		chunk_to_sort(const chunk_to_sort &other)
			: data (other.data)
			{
			}
	};

	thread_safe_stack<chunk_to_sort> chunks;
	vector<thread> threads;

	unsigned const max_thread_count;
	atomic<bool> end_of_data;

	sorter() :
		max_thread_count(thread::hardware_concurrency()-1),
		end_of_data(false)
	{}

	sorter(sorter&) = default;

	~sorter() {
		end_of_data = true;

		for (unsigned i = 0; i < threads.size(); i++)
			threads[i].join();
	}

	void try_sort_chunk()
	{
		shared_ptr<chunk_to_sort > chunk = chunks.pop();
		if (chunk)
		{
			printf("2\n");
			sort_chunk(chunk);
			printf("3\n");
		}

	}

	list<T> do_sort(list<T> &chunk_data)
	{
		if (chunk_data.empty())
			return chunk_data;

		list<T> result;
		result.splice(result.begin(), chunk_data,
				chunk_data.begin());
		T const& partition_val = *result.begin();

		typename list<T>::iterator divide_point =
			partition(chunk_data.begin(), chunk_data.end(),
			[&](T const& val) {return val < partition_val;});

		chunk_to_sort new_lower_chunk;

		new_lower_chunk.data.splice(new_lower_chunk.data.end(),
			chunk_data, chunk_data.begin(),
			divide_point);

		future<list<T> > new_lower =
			new_lower_chunk.promises.get_future();

		printf("push\n");
		chunks.push(move(new_lower_chunk));
		if (threads.size() < max_thread_count)
			threads.push_back(thread(&sorter<T>::sort_thread, this));

		list<T> new_higher (do_sort(chunk_data));
		result.splice(result.end(), new_higher);

		while (new_lower.wait_for(chrono::seconds(0)) !=
				future_status::ready)
			try_sort_chunk();

		result.splice(result.begin(), new_lower.get());

		return result;
	}

	void sort_chunk(shared_ptr<chunk_to_sort > const & chunk) {
		chunk->promises.set_value(do_sort(chunk->data));
	}

	void sort_thread()
	{
		while(!end_of_data)
		{
			try_sort_chunk();
			this_thread::yield();
		}
	}
};

template <typename T>
list<T> parallel_quick_sort(list<T> input)
{
	if (input.empty())
		return input;

	sorter<T> s;
	return s.do_sort(input);
}

template <typename T>
void show_list(list<T>& l)
{
	printf("list: ");
	typename list<T>::const_iterator it;
	for (it = l.begin(); it != l.end(); it++)
		cout << " " << *it;
	printf("\n");
}

int main()
{
	list<int> l;

	for (int i = 0; i < 100; i++)
		l.push_back(rand() % 19);

	show_list(l);
	{
		printf("111\n");
		l = parallel_quick_sort(l);
		printf("222\n");
	}
	show_list(l);

	return 0;
}


