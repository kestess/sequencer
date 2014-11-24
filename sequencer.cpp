#include <iostream>
#include <fstream>
#include <string>
#include <limits>

/* I passed my allotted time, so ... this works, but is ugly */

using namespace std;

int longestLength = 0;
int shortestLength = 0;

string longestWord = "";
string shortestWord = "";

int bestScore = std::numeric_limits<int>::min();
string bestWord = "";

void score(const string word) {
	int score = 0;
	bool indelStart = false;
	for (int i = 0; i < longestLength; i++) {
		if (longestWord[i] == word[i]) {
			score += 3;
			indelStart = false;
		} else  {
			if (word[i] == '-') {
				score -= (!indelStart) ? 8 : 1;
				if (!indelStart) indelStart = true;
			} else {
				score -= 3;
				indelStart = false;
			}
		}
	}
	if (score > bestScore) {
		bestScore = score;
		bestWord = word;
	}
}

void process(string soFar, int indexOfWord, int slop) {
	if (soFar.length() == longestLength) {
		//cout << soFar << endl;
		score(soFar);
	} else if (indexOfWord == shortestLength) {
		string s(longestLength - soFar.length(), '-');
		//cout << soFar + s << endl;
		score(soFar + s);
	} else {
		if (slop > 0) process(soFar + '-', indexOfWord, slop - 1);
		process(soFar + shortestWord[indexOfWord], indexOfWord + 1, slop);
	}
}

void start(const string a, const string b) {
	longestWord = (a.length() >= b.length()) ? a : b;
	shortestWord = (longestWord == a) ? b : a;
	longestLength = longestWord.length();
	shortestLength = shortestWord.length();
	process("", 0, longestLength - shortestLength);
}

void trim(string& word) {
    size_t first = word.find_first_not_of(' ');
    size_t last  = word.find_last_not_of(' ');
    word = word.substr(first, last - first + 1);
}

int main(int argc, char *argv[])
{
	string filename = "";
	string line;

	string delimiter = "|";
	string a;
	string b;

	if (argc == 2) {
		filename = argv[1];
	} else {
		cout << "Usage: " << argv[0] << " <input file>" << endl;
		exit(1);
	} 

	ifstream input(argv[1]);
	if (input.is_open()) {
		while (getline (input, line)) {

			if (line.length() == 0) {
				continue;
			}

			// Global state - bad idea, but this wasn't supposed to take more
			// than a few minutes, so the first correct (hopefully) progam is 
			// being submitted.
			longestLength = 0;
			shortestLength = 0;
			longestWord = "";
			shortestWord = "";
			bestScore = std::numeric_limits<int>::min();
			bestWord = "";

			a = "";
			b = "";
			
			a = line.substr(0, line.find(delimiter));
			b = line.substr(line.find(delimiter) + 1);

			trim(a);
			trim(b);

			if (a.length() == 0 || b.length() == 0) {
				continue;
			}

			start(a, b);
			cout << bestScore << endl;
		}
	} else {
		cerr << "Couldn't open file: " << filename << endl;
		exit(1);
	}

	input.close();

	return 0;
}
