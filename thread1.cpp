#include <iostream>
#include <thread>

void numbers(int id) {
	for (int i = 0 ; i < 5 ; i++){
		std::cout << id << " " << i;
	}
	std::cout << std::endl;
}

int main(){
	std::thread t1(numbers,1);
	std::thread t2(numbers,2);

	t1.join();
	t2.join();

	return 0;
}
