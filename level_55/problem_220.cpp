#include <math.h>
#include <array>
#include <iostream>



/*
Problem 220.

A D_n dragon will take 2^n steps in total. Using this fact and trig identities, to tell the dragon's position in step 1e12, we can fill the coordinates for a narrow range of steps between 
step 2^39 (even closer actually) and step 2^40, as 2^39 < 1e12 < 2^40.

Using blocks (two lines enclosing 45 degrees) as the smallest elements. The sizes of the lines (as well as the distance between the end points of the lines) depends on the step 
size n of dragon D_n, what's given initially is the size of the full dragon. The dragon fills itself between the input points (x1,y1) and (x2,y2), first one block (2^0), then one block out 
of both lines (2^1), then one block out of all four lines (2^2), etc. jolting out of straight line segments with 45 degree angles, drilling to 2^50 granularity in n=50 steps for the D_50.

*/

void dragon_func(const std::array<double, 2> &angles, double x1, double y1, double x2, double y2, int node, double sign, long long &step, double block_length);

const double get_pi{ 3.14159265358979323846264338327950288 };

class FoundStep1e12 : public std::exception {
public:
	FoundStep1e12() noexcept {}
	~FoundStep1e12() {}

	virtual const char* what() const noexcept override {
		return ".";

	}
};


void dragon_func(const std::array<double, 2> &angles, double x1, double y1, double x2, double y2, int node, double sign, long long &step, double block_length) {

	std::array<double, 2> x1_arr{ 0.0, 0.0 }, y1_arr{ 0.0, 0.0 }, x2_arr{ 0.0, 0.0 }, y2_arr{ 0.0, 0.0 };
	
	double full_distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	double edge_length = full_distance / block_length;
	
	double angle{0.0}; //radians
	double sign2{-1.0};
	node--;

	double dx{x2 - x1}, dy{y2 - y1};

	
	if (dx == 0 ) { 
		if (dy > 0.0)
			angle = get_pi / 2.0; //get the angle of the line connecting (x1, y1) and (x2, y2)
		else
			angle = get_pi + (get_pi / 2.0);
	}
	else
		angle = atan(dy / dx);
	if (dx < 0.0)
		angle += get_pi; //correct the arctan angle for negative dx


	//fill x1_arr, y1_arr, x2_arr, y2_arr with step coordinates, rotating the angle between the two (so the line between the two endpoints jolts out to a third midpoint giving the dragon dynamics)
	for (int j{ 0 }; j < angles.size(); j++) { 

		x1_arr[j] = x1;
		y1_arr[j] = y1;

		angle += sign * angles[j] * (get_pi / 180.0);
		double x2 = x1 + (cos(angle) * edge_length);
		double y2 = y1 + (sin(angle) * edge_length);

		if (x2 * x2 < 1e-8) 
			x2 = round(x2);
		if (y2 * y2 < 1e-8)
			y2 = round(y2);

		x2_arr[j] = x2;
		y2_arr[j] = y2;

		x1 = x2;
		y1 = y2;
	}

	//main node loop, parse the n nodes recursively, increment the steps and return the coordinates
	for (int j{ 0 }; j < angles.size(); j++) { 

		//skip as many irrelevant steps - out of the 2^50 total steps up to 1e12 - as possible
		{ 
			if (node >= 40 && j == 1) {
				step += pow(2, node);
				continue;
				//calculate the second half (j==1) of these nodes, but ignore j==0
			}
			if ((node == 39 || node == 38 || node == 37 || node == 35 || node == 31 || node == 30 || node == 28 || node == 26 || node == 23 || node == 21 || node == 18) && j == 0) {
				//want to approach 1e12 very closely from below: 2^39+2^38+2^37+(2^36) is above already, but 2^39+2^38+2^37+(2^35+2^31+2^30+2^28+2^26+2^23+2^21+2^18) is OK
				step += pow(2, node);
				sign2 *= -1;
				continue;
				//ignore the first half (j==0) of these nodes, but get j==1
			}
		}

		//increment at the smallest node=0
		if (node == 0) { 
			step++;
			if (step == 1e12) {
				std::cout << "edge length: " << edge_length << std::endl;
				std::cout << "step: " << step << ", x: " << y2_arr[j] << ", y: " << x2_arr[j] << std::endl;
				/*becauses 50 mod 8=2, the cursor starts out pointing at (1,0), instead of (0,1), plus our step 0 (or "head") is actually the "tail" of the dragon using the string 
				formulation ("D_0=Fa", etc.) given in the problem description. The difference between "head" an "tail" disappers for large n, but we have to flip the image on x and y.*/

				throw FoundStep1e12{};
			}
		}

		//drill to more granular node levels until reaching node=0
		else { 
			dragon_func(angles, x1_arr[j], y1_arr[j], x2_arr[j], y2_arr[j], node, sign2, step, block_length);
			sign2 *= -1;
			//update the rotation angle inside the block between the consecutive steps j==0 and j==1
		}
	}
}


int main() {

	long long step{ static_cast<long long>(0) };

	std::array<double, 2> angles{ 45.0, -90.0 }; //degrees
	double a{0.0}, block_length{ 0.0 };

	for (auto& j : angles) {
		a += j; 
		//toggles 45 and -45, then the axis distance after two block steps, i.e. block_length is |sin(a_) * 2|=|cos(a_) * 2|
		block_length += abs(sin(a * (get_pi / 180.0)));
	}
	
	double x1{ 0.0 }, y1{ 0.0 }, x2{ 1000000.0 / (0.02107342 * block_length) }, y2{ 0.0 };

	//set x2 end point coordinate to give unit-length edges for the steps (this is hard coded form a post-run adjustment) then xxxx76,xxxx04


	try {
		dragon_func(angles, x1, y1, x2, y2, 50, 1.0, step, block_length);
	}
	catch (const FoundStep1e12& ex) { std::cout << ex.what() << std::endl; }
	
	
	return 0;
}



//I would like to thank https://www.youtube.com/watch?v=WrQqN6itMjo&list=PL3WoIG-PLjSsUClVQ19kIuOStRQZnDZJX&index=6 for the SDL2 version