#include <iostream>
#include <glm/glm.hpp>
#include <json/json.h>
#include <fstream>

using namespace std;


int main(int argc, char* argv[])
{
	glm::vec3 a(1.0f, 2.0f, 3.0f);
	glm::vec3 b(4.0f, 5.0f, 6.0f);
 
	Json::Value root = "Jason Test";



	auto sum = a + b;

	
	cout << "Hello World!" << endl;
	cout << "a + b = (" << sum.x << ", " << sum.y << ", " << sum.z << ")" << endl;
	cout << "Json: " << root << endl;

	ifstream file("JSON_test.json");
	json data;
	file >> data;
	return EXIT_SUCCESS;
}