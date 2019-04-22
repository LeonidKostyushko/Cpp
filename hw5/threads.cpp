#include <iostream>
#include <condition_variable>
#include <thread>

using namespace std;
condition_variable cond;
mutex mut;
bool done = true;
const int N = 500000;

void ping() {
	for (int i = 0; i < N; i++)
	{
		std::unique_lock<mutex> locker(mut);
		cond.wait(locker, [] {return done; });
		cout << "ping" << "\n";
		done = false;
		cond.notify_one();
	}
}

void pong() {
	for (int i = 0; i < N; i++) 
	{
		unique_lock<mutex> locker(mut);
		cond.wait(locker, [] {return !done; });
		cout << "pong" << "\n";
		done = true;
		cond.notify_one();
	}
}

int main() {
	thread t1(ping);
	thread t2(pong);
	t1.join();
	t2.join();
	return 0;
}