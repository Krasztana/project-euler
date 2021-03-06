#include <iostream>
#include <queue>

//Problem 3:
//The prime factors of 13195 are 5, 7, 13 and 29.
//What is the largest prime factor of the number 600851475143?

long long smallest_divisor(long long num); //returns the smallest divisor or 1 for primes

long long smallest_divisor(long long num) {
	long long i{ 3 }; //change to 2 if num is even

	while (i < num) {
		if (num % i++ == 0) {
			return i - 1;
		}
	}
	return 1;
}


int main() {

	std::priority_queue<long long> pq{};

	long long num{ 13195 };
	num = 600851475143;

	while (num != 1) {

		if (smallest_divisor(num) == 1) {
			pq.push(num);
			break;
		}

		pq.push(smallest_divisor(num));
		num = num/smallest_divisor(num);
	}

	std::cout << "largest prime factor is " << pq.top() << std::endl;

	return 0;
}