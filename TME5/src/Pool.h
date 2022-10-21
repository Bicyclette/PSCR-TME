#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>

namespace pr {

void poolWorker(Queue<Job> & queue) {
	while(true) {
		Job* job = queue.pop();
		if(job == nullptr) { return; }
		job->run();
		delete job;
	}
}

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
public:
	Pool(int qsize) : queue(qsize) {}
	void start (int nbthread) {
		for(int i = 0; i < nbthread; ++i) {
			threads.emplace_back(poolWorker, std::ref(queue));
		}
	}
	void submit (Job * job) {
		while(!queue.push(job)){}
	}
	void stop() {
		queue.setBlocking(false);
		for(auto & t : threads) {
			t.join();
		}
		threads.clear();
	}
	~Pool() {}
};

}
