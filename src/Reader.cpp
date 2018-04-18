#include "Reader.h"
#include <string>
#include <sstream>
#include <iterator>

vector<vec3> readMesh(string file) {
	ifstream myfile;
	myfile.open(file);
	if (myfile.fail()) {
		throw exception("Musisz sobie podmieniæ œcie¿ke do pliku");
		cout << "Problem with opening file " + file;
	}
	string line;
	vector<vec3> ret;
	if (myfile.is_open())
	{
		getline(myfile, line);
		int vertices = stoi(line);
		for(int i = 0; i < vertices; i ++)
		{
			getline(myfile, line);
			istringstream iss(line);
			vector<string> tokens{ istream_iterator<string>{iss},
				istream_iterator<string>{} };
			ret.push_back(vec3(stod(tokens[0]), stod(tokens[1]), stod(tokens[2])));
			//ret.push_back(vec3(stod(tokens[0]), stod(tokens[1]), stod(tokens[2])));
		}
		myfile.close();
	}
	return ret;
}
