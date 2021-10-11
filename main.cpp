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

void Eat(unsigned int phil_position, unsigned int eat_time) {
	//cout << eat_time << endl;


	SignalEating(phil_position);
	this_thread::sleep_for(chrono::seconds(eat_time));

}

int main(){
	srand(time(0));

	unsigned int phil_count = 3;

	vector<Fork> forks(phil_count);
	vector<Philosopher> philosophers(phil_count);

	for (unsigned int i = 0; i < phil_count; i++) {
		philosophers[i] = Philosopher(Eat, i, 1 + rand() % 4);
	}

	for (unsigned int i = 0; i < phil_count; i++) {
		philosophers[i].join();
	}

	getchar();
}