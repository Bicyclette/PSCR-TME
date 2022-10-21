#pragma once

#include <mutex>
#include <condition_variable>

namespace pr {

class Barrier {
	int counter;
	const int N;
	std::mutex m;
	std::condition_variable cv;
public:
	Barrier(int max) : counter(0), N(max) {}
	void done() {
		std::unique_lock<std::mutex> ul(m);
		counter++;
		if(counter == N) {
			cv.notify_all();
		}
	}
	void waitFor() {
		std::unique_lock<std::mutex> ul(m);
		while(counter != N) {
			cv.wait(ul);
		}
	}
};

}
