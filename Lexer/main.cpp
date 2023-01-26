#include <iostream>
#include <set>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <deque>

using namespace std;

map<string, string> keywords = {
	make_pair("int", "Type"),
	make_pair("using", "Keyword"),
	make_pair("namespace", "Keyword"),
	make_pair("std", "Keyword"),
	make_pair("void", "Type"),
	make_pair("string", "Type"),
	make_pair("for", "Keyword"),
	make_pair("while", "Keyword"),
	make_pair("false", "Keyword"),
	make_pair("true", "Keyword"),
	make_pair("vector", "Type"),
	make_pair("continue", "Keyword"),
	make_pair("break", "Keyword"),
	make_pair("return", "Keyword"),
	make_pair("if", "Keyword"),
	make_pair("else", "Keyword"),
	make_pair("char", "Type"),
	make_pair("bool", "Type"),
	make_pair("auto", "Type"),
	make_pair("const", "Keyword"),
	make_pair("cout", "Keyword"),
	make_pair("cin", "Keyword"),
	make_pair("struct", "Keyword"),
	make_pair("endl", "Keyword")
};

map<string, string> separators = {
	make_pair("(", "Separator"),
	make_pair(")", "Separator"),
	make_pair("[", "Separator"),
	make_pair("]", "Separator"),
	make_pair(";", "Separator"),
	make_pair(",", "Separator"),
	make_pair("^", "Arithmetic operator"),
	make_pair("/", "Arithmetic operator"),
	make_pair("*", "Arithmetic operator"),
	make_pair("+", "Arithmetic operator"),
	make_pair("-", "Arithmetic operator"),
	make_pair("=", "Assignment operator"),
	make_pair("&&", "Logical operation"),
	make_pair("||", "Logical operation"),
	make_pair(">", "Logical operation"),
	make_pair("<", "Logical operation"),
	make_pair(":", "Separator"),
	make_pair("{", "Separator"),
	make_pair("}", "Separator")
};

map<string, string> doubleSeparators = {
	make_pair("<<", "Operator output"),
	make_pair(">>", "Operator input"),
	make_pair("==", "Logical operation"),
	make_pair("<=", "Logical operation"),
	make_pair(">=", "Logical operation"),
	make_pair("=<", "Logical operation"),
	make_pair("=>", "Logical operation"),
	make_pair("!=", "Logical operation"),
};

string numbers = "1234567890.";

const string errorOutputStr = " **Error** ";

bool CreateToken(string& word, int indexStr, deque<string>& stringArray, ofstream& outputFile) {
	if (word == "")
		return false;

	for (auto& item : doubleSeparators) {
		size_t index = word.find(item.first);
		if (index != string::npos) {
			word.erase(index, item.first.size());
			string secondStr = word.substr(index, word.size() - index);
			word = word.substr(0, index);
			CreateToken(word, indexStr, stringArray, outputFile);
			outputFile << item.first << " ***" << item.second << "*** line " << indexStr << "\n";
			CreateToken(secondStr, indexStr, stringArray, outputFile);

			return true;
		}
	}

	for (auto& item : separators) {
		size_t index = word.find(item.first);
		if (index != string::npos) {
			word.erase(index, item.first.size());
			string secondStr = word.substr(index, word.size() - index);
			word = word.substr(0, index);
			CreateToken(word, indexStr, stringArray, outputFile);
			outputFile << item.first << " ***" << item.second << "*** line " << indexStr << "\n";
			CreateToken(secondStr, indexStr, stringArray, outputFile);

			return true;
		}
	}

	if (numbers.find(word[0]) != string::npos)
	{
		bool isPoint = false;
		bool isSecondPoint = false;
		for (auto i = 1; i < word.size(); i++)
		{
			if (isPoint && word[i] == '.')
				isSecondPoint = true;

			if (word[i] == '.')
				isPoint = true;

			if ((numbers.find(word[i]) == string::npos) || word[0] == '.' || word[word.size() - 1] == '.' || isSecondPoint) {
				outputFile << word << errorOutputStr << "line " << indexStr << "\n";

				return true;
			}
		}
		outputFile << word << " ***Number*** line " << indexStr << "\n";

		return true;
	}

	string output;
	if (keywords.find(word) != keywords.end()) {
		output = keywords[word];
		outputFile << word << " ***" << output << "*** line " << indexStr << "\n";

		return true;
	}


	if (separators.find(word) != separators.end()) {
		output = separators[word];
		outputFile << word << " ***" << output << "*** line " << indexStr << "\n";
		return true;
	}

	if (word == "\"" && !stringArray.empty())
	{
		word = stringArray[0];
		stringArray.pop_front();
		size_t i = word.find("\"") + 1;
		if (i < word.size() && word.find("\"", i) != string::npos)
			outputFile << word << " ***String*** line " << indexStr << "\n";
		else
			outputFile << word << errorOutputStr << indexStr << "\n";

		return true;
	}

	if (word == "\'" && !stringArray.empty())
	{
		word = stringArray[0];
		stringArray.pop_front();
		size_t i = word.find("\'") + 1;
		if (i < word.size() && word.find("\'", i) != string::npos && word.size() == 3)
			outputFile << word << " ***Char*** line " << indexStr << "\n";
		else
			outputFile << word << errorOutputStr << indexStr << "\n";

		return true;
	}

	if (isalpha(word[0]) || word[0] == '_') {
		for (auto i = 1; i < word.size(); i++)
			if (!isalpha(word[i]) && word[i] != '_' && (word[i] < '0' || word[i] > '9')) {
				outputFile << word << errorOutputStr << indexStr << "\n";

				return true;
			}
		outputFile << word << " ***Identifier*** line " << indexStr << "\n";

		return true;
	}
	outputFile << word << errorOutputStr << indexStr << "\n";

	return false;
}

void MechanismStringAndChar(deque<string>& stringArray, string& inputStr, size_t indexStart) {
	if (inputStr.find("\"", indexStart) != string::npos) {
		string str;
		size_t start = inputStr.find("\"", indexStart);
		size_t end = inputStr.size() - 1;
		if (start + 1 < inputStr.size() && inputStr.find("\"", start + 1) != string::npos)
			end = inputStr.find("\"", start + 1);
		str = inputStr.substr(start, end - start + 1);
		stringArray.push_back(str);
		inputStr.erase(start + 1, end - start);

		MechanismStringAndChar(stringArray, inputStr, start + 1);
	}
	if (inputStr.find("\'", indexStart) != string::npos) {
		string str;
		size_t start = inputStr.find("\'", indexStart);
		size_t end = inputStr.size() - 1;
		if (start + 1 < inputStr.size() && inputStr.find("\'", start + 1) != string::npos)
			end = inputStr.find("\'", start + 1);
		str = inputStr.substr(start, end - start + 1);
		stringArray.push_back(str);
		inputStr.erase(start + 1, end - start);

		MechanismStringAndChar(stringArray, inputStr, start + 1);
	}
}

void MechanismStr(string& inputStr, int indexStr, ofstream& outputFile) {
	int commentsIndex = 0;
	if ((commentsIndex = inputStr.find("//", commentsIndex)) != string::npos) {
		string comment = inputStr.substr(commentsIndex, inputStr.size() - commentsIndex);
		inputStr = inputStr.substr(0, commentsIndex);
		outputFile << comment << " ***Comment*** line " << indexStr << "\n";
	}
	deque<string> stringArray;
	MechanismStringAndChar(stringArray, inputStr, 0);
	if (!inputStr.empty()) {
		vector<string> words;
		string item;
		istringstream strStream(inputStr);
		while (getline(strStream, item, ' '))
			words.push_back(item);
		for (auto i = 0; i < words.size(); i++)
			CreateToken(words[i], indexStr, stringArray, outputFile);
	}
}

void ReadAndRunMechanism(ifstream& input, ofstream& output) {
	string inputStr;
	int indexStr = 0;
	while (getline(input, inputStr)) {
		indexStr++;
		if (inputStr.empty())
			continue;
		MechanismStr(inputStr, indexStr, output);
	}
}

int main(int args, char* arsv[]) {
	ifstream input(arsv[1]);
	ofstream output(arsv[2]);
	ReadAndRunMechanism(input, output);
}
