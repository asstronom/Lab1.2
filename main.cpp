#include <iostream>
#include <thread>
#include <chrono>
#include <semaphore>
#include <vector>
#include <ctime>
#include <Windows.h>

using namespace std;

//Philosophers represent threads
using Philosopher = thread;

//Forks represent critical sections
class Fork : public binary_semaphore {
public:
	Fork() : binary_semaphore(1) {}
};

void SignalEating(int pos) {
	cout << ("Philosopher " + to_string(pos) +" is eating\n");
}

void SignalFinishEating(int pos) {
	cout << ("Philosopher " + to_string(pos) + " finished eating\n");
}


void Eat(unsigned int phil_position, unsigned int eat_time, Fork* left, Fork* right) {
	//cout << eat_time << endl;

	while (true) {
		//trying to take left fork
		left->acquire();
			//trying to take right fork
			if (right->try_acquire()) {
				//signal user that philosopher started eating
				SignalEating(phil_position);
				//waiting for philosopher to finish eating
				this_thread::sleep_for(chrono::seconds(eat_time));
				//signal user that philosopher finished eating
				SignalFinishEating(phil_position);
				break;
			}
			//if failed to take right fork we release left so other philosopher could take it
			else {
				left->release();
			}
	}
	//after philosopher finished eating we release forks
	left->release();
	right->release();
}

int main(){
	srand(time(0));

	//choose amount of philosophers
	unsigned int phil_count;
	cout << "Enter amount of philosophers: ";
	cin >> phil_count;

	//create forks and philosophers
	vector<Fork> forks(phil_count);
	vector<Philosopher> philosophers(phil_count);

	//initialize threads with function Eat and its arguments
	for (unsigned int i = 0; i < phil_count; i++) {
		philosophers[i] = Philosopher(Eat, i, 1 + rand() % 6, &forks[i % phil_count], &forks[(i + 1) % phil_count]);
	}

	//block execution of main() until all philosophers finish their meals
	for (unsigned int i = 0; i < phil_count; i++) {
		philosophers[i].join();
	}

	cout << "\nDone\n";

}