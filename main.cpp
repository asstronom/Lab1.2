#include <iostream>
#include <thread>
#include <chrono>
#include <semaphore>
#include <vector>
#include <ctime>
#include <Windows.h>

using namespace std;

binary_semaphore cout_semaphore(1);

using Philosopher = thread;

class Fork : public binary_semaphore {
public:
	Fork() : binary_semaphore(1) {}
};

void SignalEating(int pos) {
	cout_semaphore.acquire();
	cout << "Philosopher " << pos << " is eating\n";
	cout_semaphore.release();
}

void SignalFinishEating(int pos) {
	cout_semaphore.acquire();
	cout << "Philosopher " << pos << " finished eating\n";
	cout_semaphore.release();
}

void Eat(unsigned int phil_position, unsigned int eat_time, Fork* left, Fork* right) {
	//cout << eat_time << endl;
	while (true) {
		if (left->try_acquire()) {
			if (right->try_acquire()) {
				SignalEating(phil_position);
				this_thread::sleep_for(chrono::seconds(eat_time));
				SignalFinishEating(phil_position);
				break;
			}
			else {
				left->release();
			}
		}
	}
	left->release();
	right->release();
}

int main(){
	srand(time(0));

	unsigned int phil_count = 5;

	vector<Fork> forks(phil_count);
	vector<Philosopher> philosophers(phil_count);

	for (unsigned int i = 0; i < phil_count; i++) {
		philosophers[i] = Philosopher(Eat, i, 1 + rand() % 4, &forks[i % phil_count], &forks[(i + 1) % phil_count]);
	}

	for (unsigned int i = 0; i < phil_count; i++) {
		philosophers[i].join();
	}

	cout << "\nDone\n";

}