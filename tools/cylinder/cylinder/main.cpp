#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

#define M_PI 3.14159265358979323846

using namespace std;

int main() {

	ofstream cylinderFile;
	cylinderFile.open("cylinder.txt");
	cylinderFile << "202\n";
	cylinderFile << "0.0 0.0 0.5\n";
	//cylinderFile << "0.0 0.0 -0.5\n";
	for (int i = 0; i < 100; i++) {
			cylinderFile <<to_string( 0.2 * cos(((float)i / 100.0f) * 2 * M_PI)) + ' ' + to_string(0.2f * sin(((float)i / 100.0f) * 2 * M_PI)) + ' ' + "0.5\n";
			//cylinderFile << to_string(0.5 * cos((i / 100) * M_PI)) + ' ' + to_string(0.2 * sin((i / 100) * M_PI)) + ' ' + "-0.5\n";
	}
	cylinderFile << "0.0 0.0 -0.5\n";
	for (int i = 0; i < 100; i++) {
		cylinderFile << to_string(0.2 * cos(((float)i / 100.0f) * 2 * M_PI)) + ' ' + to_string(0.2f * sin(((float)i / 100.0f) * 2 * M_PI)) + ' ' + "-0.5\n";
		//cylinderFile << to_string(0.5 * cos((i / 100) * M_PI)) + ' ' + to_string(0.2 * sin((i / 100) * M_PI)) + ' ' + "-0.5\n";
	}
	cylinderFile << "402\n";
	for (int i = 0; i < 101; i++) {
		cylinderFile << to_string(i) + " " + to_string(0) + " " + to_string(0) + " " + to_string(1) +"\n";
	}
	for (int i = 101; i < 202; i++) {
		cylinderFile << to_string(i) + " " + to_string(0) + " " + to_string(0) + " " + to_string(-1) + "\n";
	}
	for (int i = 1; i < 101; i++) { // 202 -> 301
		cylinderFile << to_string(i) + " " + to_string(cos(((float)(i - 1) / 100.0f) * 2 * M_PI)) + " " + to_string(sin(((float)(i - 1) / 100.0f) * 2 * M_PI)) + " " + to_string(0) + "\n";
	}
	for (int i = 102; i < 202; i++) { // 302 -> 401
		cylinderFile << to_string(i) + " " + to_string(cos(((float)(i - 102 - 1) / 100.0f) * 2 * M_PI)) + " " + to_string(sin(((float)(i - 102 - 1) / 100.0f) * 2 * M_PI)) + " " + to_string(0) + "\n";
	}
	//cylinderFile << "2\n";
	cylinderFile << "402\n";
	for (int i = 1; i < 100; i++) {
		cylinderFile << to_string(i) + " " + to_string(i + 1) + " " + to_string(0) + "\n";
	}
	cylinderFile << to_string(100) + " " + to_string(1) + " " + to_string(0) + "\n";
	for (int i = 103; i < 202; i++) {
		cylinderFile << to_string(i + 1) + " " + to_string(i) + " " + to_string(102) + "\n";
	}
	cylinderFile << to_string(103) + " " + to_string(201) + " " + to_string(102) + "\n";
	for (int i = 202; i < 302; i++) {
		cylinderFile << to_string(i + 1) + " " + to_string(i) + " " + to_string(100 + i) + "\n";
		//cylinderFile << to_string(i) + " " + to_string(i + 1) + " " + to_string(102 + i) + "\n";
	}
	for (int i = 202; i < 302; i++) {
		cylinderFile << to_string(100 + i) + " " + to_string(101 + i) + " " + to_string(i + 1) + "\n";
	}
	cylinderFile << to_string(401) + " " + to_string(302) + " " + to_string(202) + "\n";
	cylinderFile << to_string(202) + " " + to_string(301) + " " + to_string(401) + "\n";
	cylinderFile.close();
	return 0;
}