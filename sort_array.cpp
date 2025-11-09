#include <iostream>
#include <thread>
#include <algorithm>
void partial_sum(const std::vector<int>& arr, int start, int end, long long& result){
	std::sort(arr.begin() + start , arr.begin() + end);
}

int main() {
	std::vector<int> numbers = {4,5,2,6,3,5,2,4,8,5,4,5,8,7,4,5,8,4,5};
	int N = numbers.size();

	int part = N/3;

	std::thread t1(partial_sum , std::ref(numbers) , 0 , part);
	std::thread t2(partial_sum , std::ref(numbers) , part , 2 * part);
	std::thread t3(partial_sum , std::ref(numbers) , 2*part , N);

	t1.join();
	t2.join();
	t3.join();


	return 0;
}
