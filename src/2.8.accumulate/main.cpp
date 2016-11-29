#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

template <typename Iterator, typename T>
struct accumulate_block {
	void operator()(Iterator first, Iterator last, T& result) {
		result = std::accumulate(first, last, result);
	}
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
	unsigned int const length = std::distance(first, last);
	if (!length)
		return init;

	std::cout << "length: " << length << std::endl;

	unsigned long const min_per_thread = 25;
	/* XXX: why adds min_per_thread and subs 1??? */
	/* Get max_threads our task could be divided and run on */
	unsigned long const max_threads = (length+min_per_thread-1)/min_per_thread;

	/* get num threads which can be run simultaneously */
	unsigned long const hardware_threads = std::thread::hardware_concurrency();

	/* chose minimum among max_threads and hardware_threads to avoid overhead of procceses's switching issue */
	unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads: 2, max_threads);

	unsigned long const block_size = length / num_threads;

	std::cout << "max_threads: " << max_threads << ", num_threads: " << num_threads << std::endl;
	std::cout << "num threads to be used: " << num_threads << std::endl;

	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads - 1);

	Iterator block_start = first;
	for (unsigned long i = 0; i < (num_threads - 1); ++i) {
		Iterator block_end = block_start;
		std::advance(block_end, block_size);
		threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));
		block_start = block_end;
	}

	accumulate_block<Iterator, T>()(block_start, last, results[num_threads-1]);
	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

	return std::accumulate(results.begin(), results.end(), init);
}

int main()
{
	std::vector<int> v;
	for (int i = 0; i < 500; i++)
		v.push_back(i+1);

	int init = 0;
	int result =  parallel_accumulate(v.begin(), v.end(), init);

	std::cout << "result: " << result  << std::endl;

	return 0;
}
