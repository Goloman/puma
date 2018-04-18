#include "Reader.h"
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <fstream>

Mesh readMesh(string file) {
	ifstream myfile;
	myfile.open(file);
	if (myfile.fail()) {
		throw runtime_error("Musisz sobie podmieniæ œcie¿ke do pliku");
		cout << "Problem with opening file " + file;
	}
	string line;

    Mesh ret;

	if (myfile.is_open())
	{
	    // load vertex positions
		getline(myfile, line);
        vector<glm::vec3> vertexPositions;
		int count = stoi(line);
        vertexPositions.reserve(count);
		for(int i = 0; i < count; i ++)
		{
			getline(myfile, line);
			istringstream iss(line);
			vector<string> tokens{ istream_iterator<string>{iss},
				istream_iterator<string>{} };
			vertexPositions.push_back(glm::vec3(stod(tokens[0]), stod(tokens[1]), stod(tokens[2])));
		}

		// load vertices
		getline(myfile, line);
		count = stoi(line);
		ret.positions.reserve(count);
		ret.normals.reserve(count);
		for(int i = 0; i < count; i ++)
		{
			getline(myfile, line);
			istringstream iss(line);
			vector<string> tokens{ istream_iterator<string>{iss},
				istream_iterator<string>{} };
			ret.positions.push_back(vertexPositions[stoi(tokens[0])]);
			ret.normals.push_back(glm::vec3(stod(tokens[1]), stod(tokens[2]), stod(tokens[3])));
		}

		//load triangles
		getline(myfile, line);
		count = stoi(line);
		ret.indices.reserve(count * 3);
		for(int i = 0; i < count; i ++)
		{
			getline(myfile, line);
			istringstream iss(line);
			vector<string> tokens{ istream_iterator<string>{iss},
				istream_iterator<string>{} };
			ret.indices.push_back(stoi(tokens[0]));
			ret.indices.push_back(stoi(tokens[1]));
			ret.indices.push_back(stoi(tokens[2]));
		}

		myfile.close();
	}
	return ret;
}
