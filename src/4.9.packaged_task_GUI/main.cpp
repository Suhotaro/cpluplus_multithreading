#include <iostream>
#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include <utility>

typedef int (*foo)(int);

std::mutex m;
std::deque<std::packaged_task<int(int)> > tasks;

int job(int num) { return num*2; }

void gui_thread() {
	while (1) {
		/* do_something(); */
		{
			std::lock_guard<std::mutex> lk(m);
			if (tasks.empty())
				continue;
			std::packaged_task<int(int)> task(std::move(tasks.front()));
			tasks.pop_front();

			task(5);
			return;
		}
	}
}

std::future <int> post_task_for_gui_thread(foo f) {
	std::packaged_task<int(int)> task(f);
	std::future<int> res = task.get_future();

	std::lock_guard<std::mutex> lk(m);
	tasks.push_back(std::move(task));

	return res;
}

int main()
{
	std::thread t(gui_thread);
	std::future <int> ret = post_task_for_gui_thread(job);
	std::cout << "async job's res: " << ret.get() << std::endl;
	t.join();

	return 0;
}
