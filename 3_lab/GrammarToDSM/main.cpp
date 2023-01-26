#include <map>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Eclose {
	int column;
	string state;
	vector<string> eStates;
	bool fin;
};

struct State {
	string name;
	vector<string> arrOfStates;
	vector<string> transitionsName;
	vector<vector<string>> transitions;
	bool fin;
};

struct Position {
	string state;
	int column;
	int line;
};


void ReadRightTable(ifstream& input, vector<vector<string>>& inputTable) {
	string str;
	map<string, int> inputSymbols;
	map<string, bool> processedSymbols;
	int countOfInputSymbols = 2;
	vector<string> emptyTransitions;

	vector<string> states;
	states.push_back("");
	inputTable.push_back(states);
	inputTable.push_back(states);

	while (getline(input, str)) {
		str.erase(remove(str.begin(), str.end(), ' '), str.end());
		size_t indexState = str.find("->");
		std::string state = str.substr(0, indexState);

		emptyTransitions.push_back("-");
		inputTable[0].push_back("");
		inputTable[1].push_back(state);

		str.erase(0, indexState + 2);

		vector<string> arr;
		std::string item;
		std::istringstream strStream(str);
		while (getline(strStream, item, '|'))
			arr.push_back(item);

		for (auto i = 0; i < arr.size(); i++) {
			string inputSymbol(1, arr[i][0]);
			if (arr[i].size() == 1)
				arr[i] = "F";
			else
				arr[i].erase(0, 1);

			if (inputSymbols.find(inputSymbol) == inputSymbols.end()) {
				inputSymbols.insert(make_pair(inputSymbol, countOfInputSymbols));
				processedSymbols.insert(make_pair(inputSymbol, false));
				countOfInputSymbols += 1;
				emptyTransitions[0] = inputSymbol;
				inputTable.push_back(emptyTransitions);
			}
			int lineNumber = inputSymbols[inputSymbol];
			if (processedSymbols[inputSymbol] == false) {
				inputTable[lineNumber].push_back(arr[i]);
				processedSymbols[inputSymbol] = true;
			}
			else
				inputTable[lineNumber][inputTable[lineNumber].size() - 1] = inputTable[lineNumber][inputTable[lineNumber].size() - 1] + "," + arr[i];

		}

		for (auto& symbol : processedSymbols) {
			if (symbol.second == false) {
				int lineNumber = inputSymbols[symbol.first];
				inputTable[lineNumber].push_back("-");
			}
			if (symbol.second == true)
				symbol.second = false;
		}
	}
	inputTable[0].push_back("F");
	inputTable[1].push_back("F");
	for (auto i = 2; i < inputTable.size(); i++)
		inputTable[i].push_back("-");
	emptyTransitions.push_back("-");
	emptyTransitions.push_back("-");
	emptyTransitions[0] = "e";
	inputTable.push_back(emptyTransitions);
}

void ReadLeftTable(ifstream& input, vector<vector<string>>& inputTable) {
	string str;
	map<string, int> inputSymbols;
	map<string, int> inputStates;
	int countOfInputSymbols = 2;
	int countOfInputStates = 1;

	vector<string> states;
	states.push_back("");
	inputTable.push_back(states);
	inputTable.push_back(states);
	vector<Position> transitions;

	inputStates.insert(make_pair("H", countOfInputStates));
	inputTable[0].push_back("");
	countOfInputStates += 1;
	inputTable[1].push_back("H");

	while (getline(input, str)) {
		str.erase(remove(str.begin(), str.end(), ' '), str.end());
		size_t indexState = str.find("->");
		std::string state = str.substr(0, indexState);

		str.erase(0, indexState + 2);

		if (inputStates.find(state) == inputStates.end()) {
			inputStates.insert(make_pair(state, countOfInputStates));
			if (countOfInputStates == 2)
				inputTable[0].push_back("F");
			else
				inputTable[0].push_back("");
			countOfInputStates += 1;
			inputTable[1].push_back(state);
		}

		vector<string> arr;
		std::string item;
		std::istringstream strStream(str);
		while (getline(strStream, item, '|'))
			arr.push_back(item);

		for (auto i = 0; i < arr.size(); i++) {
			string inputSymbol(1, arr[i][arr[i].size() - 1]);
			if (arr[i].size() == 1)
				arr[i] = "H";
			else
				arr[i].erase(arr[i].size() - 1, 1);

			if (inputStates.find(arr[i]) == inputStates.end()) {
				inputStates.insert(make_pair(arr[i], countOfInputStates));
				inputTable[0].push_back("");
				countOfInputStates += 1;
				inputTable[1].push_back(arr[i]);
			}

			if (inputSymbols.find(inputSymbol) == inputSymbols.end()) {
				inputSymbols.insert(make_pair(inputSymbol, countOfInputSymbols));
				countOfInputSymbols += 1;
				states[0] = inputSymbol;
				inputTable.push_back(states);
			}
			int lineNumber = inputSymbols[inputSymbol];
			int columnNumber = inputStates[arr[i]];
			Position position;
			position.column = columnNumber;
			position.line = lineNumber;
			position.state = state;
			transitions.push_back(position);
		}
	}

	states[0] = "e";
	inputTable.push_back(states);
	countOfInputSymbols += 1;

	for (int i = 2; i < countOfInputSymbols; i++)
		for (int j = 1; j < countOfInputStates; j++)
			inputTable[i].push_back("-");

	for (auto i = 0; i < transitions.size(); i++) {
		if (inputTable[transitions[i].line][transitions[i].column] != "-")
			inputTable[transitions[i].line][transitions[i].column] = inputTable[transitions[i].line][transitions[i].column] + "," + transitions[i].state;
		else
			inputTable[transitions[i].line][transitions[i].column] = transitions[i].state;
	}
}


void WriteTable(ofstream& output, vector<vector<string>>& outputTable) {
	for (auto i = 0; i < outputTable.size(); i++) {
		for (auto j = 0; j < outputTable[i].size(); j++) {
			output << outputTable[i][j];
			if (j + 1 != outputTable[i].size())
				output << ";";
		}
		output << "\n";
	}
}

bool IsInVector(map<vector<string>, string>& allStatesVector, vector<string>& newVector, map<string, Eclose>& ecloses, string& name) {
	if (allStatesVector.empty())
		return false;

	for (auto i = 0; i < newVector.size(); i++) {
		if (newVector[i] == "-") {
			newVector.erase(newVector.begin() + i + 1);
			i -= 1;
			continue;
		}
		if (ecloses.find(newVector[i]) == ecloses.end())
			continue;
		for (auto j = 0; j < ecloses[newVector[i]].eStates.size(); j++)
			if (std::find(newVector.begin(), newVector.end(), ecloses[newVector[i]].eStates[j]) == newVector.end()) {
				newVector.push_back(ecloses[newVector[i]].eStates[j]);
				name = name + ecloses[newVector[i]].eStates[j];
			}
	}

	for (auto& item : allStatesVector)
		if (item.first.size() == newVector.size() && is_permutation(item.first.begin(), item.first.end(), newVector.begin())) {
			newVector.clear();
			newVector = item.first;

			return true;
		}

	return false;
}

void CreateTransitions(map<string, Eclose>& ecloses, vector<vector<string>>& inputTable,
	vector<vector<string>>& outputTable, int eStr, int line, int column, State& outputState) {
	if (outputState.transitions.empty())
		for (auto i = 2; i < inputTable.size() - 1; i++) {
			vector<string> strVector;
			if (inputTable[i][column] == "-") {
				outputState.transitions.push_back(strVector);
				outputState.transitionsName.push_back("");
				continue;
			}

			string item;
			istringstream strStream(inputTable[i][column]);
			outputState.transitionsName.push_back("");

			while (getline(strStream, item, ','))
				if ((find(strVector.begin(), strVector.end(), item) == strVector.end())) {
					strVector.push_back(item);
					outputState.transitionsName[i - 2] = outputState.transitionsName[i - 2] + item;
				}
			outputState.transitions.push_back(strVector);
		}
	else
		for (auto i = 2; i < inputTable.size() - 1; i++) {
			if (inputTable[i][column] == "-")
				continue;
			string item;
			istringstream strStream(inputTable[i][column]);
			while (getline(strStream, item, ','))
				if ((find(outputState.transitions[i - 2].begin(), outputState.transitions[i - 2].end(), item) == outputState.transitions[i - 2].end())) {
					outputState.transitions[i - 2].push_back(item);
					outputState.transitionsName[i - 2] = outputState.transitionsName[i - 2] + item;
				}
		}
}

void CreateState(map<string, Eclose>& ecloses, vector<vector<string>>& inputTable,
	vector<vector<string>>& outputTable, int eStr, int line, int column, State& outputState) {
	string newState = inputTable[line][column];
	if (std::count(outputState.arrOfStates.begin(), outputState.arrOfStates.end(), newState))
		return;
	outputState.arrOfStates.push_back(newState);
	outputState.name = outputState.name + newState;
	if (ecloses[newState].fin)
		outputState.fin = true;

	CreateTransitions(ecloses, inputTable, outputTable, eStr, line, column, outputState);

	if (ecloses[newState].eStates.empty())
		return;

	for (auto i = 0; i < ecloses[newState].eStates.size(); i++)
		CreateState(ecloses, inputTable, outputTable, eStr, 1, ecloses[ecloses[newState].eStates[i]].column, outputState);

	return;
}

void CreateAllStates(map<string, Eclose>& ecloses, vector<vector<string>>& inputTable,
	vector<vector<string>>& outputTable, int eStr, vector<State>& allStates, map<vector<string>, string>& allStatesVector) {
	State outputState = {};
	outputState.fin = false;
	CreateState(ecloses, inputTable, outputTable, eStr, 1, 1, outputState);
	allStates.push_back(outputState);
	allStatesVector.insert(make_pair(outputState.arrOfStates, "S" + to_string(allStatesVector.size())));

	for (auto i = 0; i < allStates.size(); i++)
		for (auto j = 0; j < allStates[i].transitions.size(); j++)
			if (IsInVector(allStatesVector, allStates[i].transitions[j], ecloses, allStates[i].transitionsName[j]) == false && !allStates[i].transitions[j].empty()) {
				State newState;
				newState.fin = false;

				for (auto z = 0; z < allStates[i].transitions[j].size(); z++) {
					int column = ecloses[allStates[i].transitions[j][z]].column;
					CreateState(ecloses, inputTable, outputTable, eStr, 1, column, newState);
				}
				allStatesVector.insert(make_pair(newState.arrOfStates, "S" + to_string(allStatesVector.size())));
				allStates.push_back(newState);
			}

	for (auto item : allStatesVector) {
		for (auto j = 0; j < item.first.size(); j++)
			std::cout << item.first[j] << " ";
		std::cout << "-" << item.second << "\n";
	}
}

void HandleMachine(map<string, Eclose>& ecloses, vector<vector<string>>& inputTable, vector<vector<string>>& outputTable, int eStr) {
	vector<string> line;
	line.push_back("");
	outputTable.push_back(line);
	outputTable.push_back(line);
	for (auto i = 2; i < inputTable.size() - 1; i++) {
		line.pop_back();
		line.push_back(inputTable[i][0]);
		outputTable.push_back(line);
	}

	bool end = false;

	vector<State> allStates;
	map<vector<string>, string> allStatesVector;
	CreateAllStates(ecloses, inputTable, outputTable, eStr, allStates, allStatesVector);

	for (auto st = 0; st < allStates.size(); st++) {
		if (allStates[st].fin)
			outputTable[0].push_back("F");
		else
			outputTable[0].push_back("");

		outputTable[1].push_back(allStatesVector[allStates[st].arrOfStates]);

		for (auto i = 2; i < outputTable.size(); i++) {
			if (allStates[st].transitionsName[i - 2] == "")
				outputTable[i].push_back("-");
			else
				outputTable[i].push_back(allStatesVector[allStates[st].transitions[i - 2]]);
		}
	}
};

bool CreateEcloses(map<string, Eclose>& ecloses, vector<vector<string>>& inputTable, int eStr) {
	if (inputTable[inputTable.size() - 1][0] == "e")
		eStr = inputTable.size() - 1;
	else
		return false;
	for (auto i = 1; i < inputTable[1].size(); i++) {
		bool fin = false;
		if (inputTable[0][i] == "F")
			fin = true;
		Eclose eclose = {};
		eclose.state = inputTable[1][i];
		eclose.column = i;
		eclose.fin = fin;

		if (inputTable[eStr][i] != "" && inputTable[eStr][i] != "-") {
			std::string item;
			std::istringstream strStream(inputTable[eStr][i]);
			while (getline(strStream, item, ','))
				eclose.eStates.push_back(item);
		}
		ecloses.insert(make_pair(inputTable[1][i], eclose));
	}

	return true;
}

int main(int argc, char* argv[])
{
	if (argc != 4) {
		std::cout << "Invalid input format";

		return 0;
	}

	std::string type = argv[1];
	std::string file = argv[2];
	std::ifstream input;
	input.open(file);
	file = argv[3];
	std::ofstream output;
	output.open(file);

	if (!(input.is_open() && output.is_open())) {
		cerr << "Error open files!\n";

		return 1;
	}
	if (type != "left" && type != "right") {
		cerr << "You need to enter type of grammar: left or right\n";

		return 1;
	}
	int eStr = 0;
	vector<vector<string>> inputTable;
	vector<vector<string>> outputTable;
	map<string, Eclose> ecloses;
	if (type == "left")
		ReadLeftTable(input, inputTable);
	if (type == "right")
		ReadRightTable(input, inputTable);
	if (!CreateEcloses(ecloses, inputTable, eStr)) {
		cerr << "Error file format. There isn't empty symbols!\n";

		return 1;
	};
	HandleMachine(ecloses, inputTable, outputTable, eStr);
	WriteTable(output, outputTable);

	input.close();
	output.close();

	return 0;
}