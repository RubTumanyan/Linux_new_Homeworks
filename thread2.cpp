#include <iostream>
#include <thread>

int counter1 = 0;

void counter(){
	for (int i = 0 ; i < 10 ;i++) {
		counter1 += 1;
	}
}
int main(){
	std::thread t1(counter);
	std::thread t2(counter);
	std::thread t3(counter);

	t1.join();
	t2.join();
	t3.join();
        std::cout << counter1;
	return 0;
}
