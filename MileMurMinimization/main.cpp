#include <map>
#include <algorithm>
#include <set>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct State {
	string firstName;
	string name;
	vector<string> transitions;
	vector<string> outputSymbols;
	vector<string> firstTransitions;
};

void ReadTable(ifstream& input, vector<vector<string>>& inputTable) {
	string str;
	int indexStr = 0;
	while (getline(input, str)) {
		indexStr += 1;
		vector<string> arr;
		std::string item;
		if (str[str.size() - 1] == ';')
			str = str + " ";
		std::istringstream strStream(str);
		while (getline(strStream, item, ';'))
			arr.push_back(item);
		inputTable.push_back(arr);
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

bool InitializingMealy(vector<vector<string>>& inputTable, vector<State>& inputStates) {
	if (inputTable.size() < 2)
		return false;
	if (inputTable[0].size() < 2)
		return false;
	set<string> achievableStates;
	for (auto i = 1; i < inputTable[0].size(); i++) {
		State state;
		state.name = inputTable[0][i];
		for (auto j = 1; j < inputTable.size(); j++) {
			size_t index = inputTable[j][i].find("/");
			string newState = inputTable[j][i].substr(0, index);
			string outputSymbol = inputTable[j][i].substr(index + 1, inputTable[j][i].size() - 1 - index);
			state.outputSymbols.push_back(outputSymbol);
			state.transitions.push_back(newState);
			if (newState != state.name)
				achievableStates.insert(newState);
		}
		state.firstName = state.name;
		state.firstTransitions = state.transitions;
		inputStates.push_back(state);
	}
	for (auto i = 0; i < inputStates.size(); i++)
		if (achievableStates.find(inputStates[i].name) == achievableStates.end()) {
			inputStates.erase(inputStates.begin() + i);
			i -= 1;
		}

	return true;
}

bool InitializingMoore(vector<vector<string>>& inputTable, vector<State>& inputStates) {
	if (inputTable.size() < 3)
		return false;
	if (inputTable[0].size() < 2)
		return false;
	set<string> achievableStates;
	for (auto i = 1; i < inputTable[0].size(); i++) {
		State state;
		state.name = inputTable[1][i];
		for (auto j = 2; j < inputTable.size(); j++) {
			string newState = inputTable[j][i];
			string outputSymbol = inputTable[0][i];
			state.outputSymbols.push_back(outputSymbol);
			state.transitions.push_back(newState);
			if (newState != state.name)
				achievableStates.insert(newState);
		}
		state.firstName = state.name;
		state.firstTransitions = state.transitions;
		inputStates.push_back(state);
	}
	for (auto i = 0; i < inputStates.size(); i++)
		if (achievableStates.find(inputStates[i].name) == achievableStates.end()) {
			inputStates.erase(inputStates.begin() + i);
			i -= 1;
		}

	return true;
}

bool HandleMachineF(vector<State>& processStates, vector<State>& outputStates, size_t& countOfStates) {
	vector<vector<State>> newStates;
	bool notFound = true;
	map<string, string> dict;
	for (auto i = 0; i < processStates.size(); i++) {
		notFound = true;
		for (auto j = 0; j < newStates.size(); j++)
			if (processStates[i].outputSymbols == newStates[j][0].outputSymbols) {
				notFound = false;
				newStates[j].push_back(processStates[i]);
				string newName = "S" + to_string(j);
				dict.insert(make_pair(processStates[i].firstName, newName));
			}
		if (notFound) {
			vector<State> vr;
			vr.push_back(processStates[i]);
			newStates.push_back(vr);
			string newName = "S" + to_string(newStates.size() - 1);
			dict.insert(make_pair(processStates[i].firstName, newName));
			notFound = false;
			continue;
		}
	}

	if (countOfStates == newStates.size())
		return true;

	for (auto i = 0; i < processStates.size(); i++) {
		State newState;
		newState.name = dict[processStates[i].firstName];
		newState.outputSymbols = processStates[i].outputSymbols;
		newState.firstName = processStates[i].firstName;
		newState.firstTransitions = processStates[i].firstTransitions;
		outputStates.push_back(newState);
		for (auto j = 0; j < outputStates[i].outputSymbols.size(); j++)
			outputStates[i].transitions.push_back(dict[processStates[i].firstTransitions[j]]);
	}
	countOfStates = newStates.size();

	return false;
}

bool HandleMachine(vector<State>& processStates, vector<State>& outputStates, size_t& countOfStates) {
	vector<vector<State>> newStates;
	bool notFound = true;
	map<string, string> dict;
	for (auto i = 0; i < processStates.size(); i++) {
		notFound = true;
		for (auto j = 0; j < newStates.size(); j++)
			if ((processStates[i].transitions == newStates[j][0].transitions) && (processStates[i].outputSymbols == newStates[j][0].outputSymbols)) {
				notFound = false;
				newStates[j].push_back(processStates[i]);
				string newName = "S" + to_string(j);
				dict.insert(make_pair(processStates[i].firstName, newName));
			}
		if (notFound) {
			vector<State> vr;
			vr.push_back(processStates[i]);
			newStates.push_back(vr);
			string newName = "S" + to_string(newStates.size() - 1);
			dict.insert(make_pair(processStates[i].firstName, newName));
		}
	}

	if (countOfStates == newStates.size())
		return true;

	for (auto i = 0; i < processStates.size(); i++) {
		State newState;
		newState.name = dict[processStates[i].firstName];
		newState.outputSymbols = processStates[i].outputSymbols;
		newState.firstName = processStates[i].firstName;
		newState.firstTransitions = processStates[i].firstTransitions;
		outputStates.push_back(newState);
		for (auto j = 0; j < outputStates[i].outputSymbols.size(); j++)
			outputStates[i].transitions.push_back(dict[processStates[i].firstTransitions[j]]);

	}
	countOfStates = newStates.size();
	processStates.clear();

	return false;
}

void Minimization(vector<State>& inputStates, vector<State>& outputStates) {
	size_t countOfStates = inputStates.size();
	if (HandleMachineF(inputStates, outputStates, countOfStates)) {
		outputStates = inputStates;

		return;
	}
	vector<State> processStates;
	bool end = false;

	while (!end) {
		processStates.clear();
		processStates = outputStates;
		outputStates.clear();
		end = HandleMachine(processStates, outputStates, countOfStates);
	}

	outputStates = processStates;

	return;
}

void FillOutputMealyTable(vector<State>& outputStates, vector<vector<string>>& outputTable, vector<vector<string>>& inputTable) {
	for (auto i = 0; i < inputTable.size(); i++) {
		vector<string> vr;
		vr.push_back(inputTable[i][0]);
		outputTable.push_back(vr);
	}
	set<string> thisStatesAre;
	for (auto i = 0; i < outputStates.size(); i++) {
		if (thisStatesAre.find(outputStates[i].name) != thisStatesAre.end())
			continue;
		thisStatesAre.insert(outputStates[i].name);
		outputTable[0].push_back(outputStates[i].name);
		for (auto j = 1; j < outputTable.size(); j++) {
			string str = outputStates[i].transitions[j - 1] + "/" + outputStates[i].outputSymbols[j - 1];
			outputTable[j].push_back(str);
		}
	}
}

void FillOutputMooreTable(vector<State>& outputStates, vector<vector<string>>& outputTable, vector<vector<string>>& inputTable) {
	for (auto i = 0; i < inputTable.size(); i++) {
		vector<string> vr;
		vr.push_back(inputTable[i][0]);
		outputTable.push_back(vr);
	}
	set<string> thisStatesAre;
	for (auto i = 0; i < outputStates.size(); i++) {
		if (thisStatesAre.find(outputStates[i].name) != thisStatesAre.end())
			continue;
		thisStatesAre.insert(outputStates[i].name);
		outputTable[1].push_back(outputStates[i].name);
		outputTable[0].push_back(outputStates[i].outputSymbols[0]);
		for (auto j = 2; j < outputTable.size(); j++)
			outputTable[j].push_back(outputStates[i].transitions[j - 2]);
	}
}

int main(int argc, char* argv[]) {
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
	if (type != "mealy" && type != "moore") {
		cerr << "You need to enter type of automaton: mealy or moore\n";

		return 1;
	}

	vector<vector<string>> inputTable;
	vector<vector<string>> outputTable;
	ReadTable(input, inputTable);
	vector<State> inputStates;
	vector<State> outputStates;

	bool isNotError = false;
	if (type == "mealy")
		isNotError = InitializingMealy(inputTable, inputStates);
	if (type == "moore")
		isNotError = InitializingMoore(inputTable, inputStates);
	if (!isNotError) {
		cerr << "Error table!\n";

		return 1;
	}
	Minimization(inputStates, outputStates);
	if (type == "mealy")
		FillOutputMealyTable(outputStates, outputTable, inputTable);
	if (type == "moore")
		FillOutputMooreTable(outputStates, outputTable, inputTable);

	WriteTable(output, outputTable);

	input.close();
	output.close();

	return 0;
}
