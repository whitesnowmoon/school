#ifdef _WIN32_
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS
#endif // _WIN32_

#ifndef SEVERTHREADPOOL_H
#define SEVERTHREADPOOL_H
#include<iostream>
#include<mutex>
#include<thread>
#include<queue>
#include<atomic>

class SeverTask
{
public:
	virtual void run() = 0;//线程使用函数
};
class SeverThreadPool
{
public:
	void push_task(SeverTask* task);
	static SeverThreadPool* Get_SeverThreadPool_Instance();
	int Get_CurTask_num();
private:
	std::atomic<int> thead_num_;
	static SeverThreadPool* instance;
	class clear
	{
	public:
		~clear() {
			if (instance)
				delete instance;
		}
	};
	SeverThreadPool();
	~SeverThreadPool();
	void work();
	std::vector<std::thread> threads;
	std::queue<SeverTask*> severtasks;
	std::vector<SeverTask*> start_severtasks;
	std::atomic_bool running_flag_;
	std::condition_variable cv_;
	std::mutex mtx_;
};






SeverThreadPool* SeverThreadPool::instance = nullptr;
SeverThreadPool::SeverThreadPool() :running_flag_(true) {
	int count = std::thread::hardware_concurrency();
	for (int i = 0; i < count; i++)
	{
		threads.emplace_back(&SeverThreadPool::work, this);
	}
	this->thead_num_ = this->threads.size();
	std::cout << "Pool thread sum=" << this->threads.size() << "\n";
}

SeverThreadPool::~SeverThreadPool() {
	running_flag_ = false;
	cv_.notify_all();
	for (auto& th : threads) {
		if (th.joinable()) {
			th.join();
		}
	}
}

void SeverThreadPool::push_task(SeverTask* task) {
	std::unique_lock<std::mutex> unlock(this->mtx_);
	this->severtasks.push(task);
	this->cv_.notify_one();
}

SeverThreadPool* SeverThreadPool::Get_SeverThreadPool_Instance() {
	static clear clr_SeverThreadPool;
	if (!instance)
		instance = new SeverThreadPool();
	return instance;
}

int SeverThreadPool::Get_CurTask_num() {
	return this->severtasks.size();
}

void SeverThreadPool::work() {
	while (true) {
		if (!running_flag_) {
			return;
		}
		SeverTask* task = nullptr;
		{
			std::unique_lock<std::mutex> unlock(this->mtx_);
			cv_.wait(unlock, [&] {
				return !severtasks.empty() || !running_flag_;
				});
			if (running_flag_) {
				if (this->severtasks.size() != 0)
					task = this->severtasks.front();
				this->severtasks.pop();
				start_severtasks.push_back(task);
			}
		}
		if (task != nullptr) {
			task->run();
		}
	}
}
#endif 
