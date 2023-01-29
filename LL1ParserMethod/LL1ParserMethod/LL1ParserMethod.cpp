#include <iostream>
#include <set>
#include <windows.h>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

struct Row {
	string name;
	set<char> guidingSymbols;
	bool shift;
	bool error;
	int indicator;
	bool nextOnStack;
	bool end;
};

const vector<Row> Table = {
	{"EXP", {'-', '(', 'a', 'b', '8', '3'}, FALSE, TRUE, 1, FALSE, FALSE},			//0
	{"T", {'-', '(', 'a', 'b', '8', '3'}, FALSE, TRUE, 13, TRUE, FALSE},			//1
	{"EXPR", {'+', '\n', '*', ')'}, FALSE, TRUE, 3, FALSE, FALSE},					//2
	{"EXPR", {'*'}, FALSE, FALSE, 7, FALSE, FALSE},									//3
	{"EXPR", {')'}, FALSE, FALSE, 8, FALSE, FALSE},									//4
	{"EXPR", {'+'}, FALSE, FALSE, 9, FALSE, FALSE},									//5
	{"EXPR", {'\n'}, FALSE, TRUE, 12, FALSE, FALSE},								//6
	{"*", {'*'}, FALSE, TRUE, -1, FALSE, FALSE},									//7
	{")", {')'}, FALSE, TRUE, -1, FALSE, FALSE},									//8
	{"+", {'+'}, TRUE, TRUE, 10, FALSE, FALSE},										//9
	{"T", {'-', '(', 'a', 'b', '8', '3'}, FALSE, TRUE, 13, TRUE, FALSE},			//10
	{"EXPR", {'+', '\n', '*', ')'}, FALSE, TRUE, 3, FALSE, FALSE},					//11
	{"\n", {'\n'}, TRUE, FALSE, -1, FALSE, TRUE},									//12
	{"T", {'-', '(', 'a', 'b', '8', '3'}, FALSE, TRUE, 14, FALSE, FALSE},			//13
	{"F", {'-', '(', 'a', 'b', '8', '3'}, FALSE, TRUE, 26, TRUE, FALSE},			//14
	{"TR", {'*', '\n', '+', ')'}, FALSE, TRUE, 16, FALSE, FALSE},					//15
	{"TR", {'*'}, FALSE, FALSE, 20, FALSE, FALSE},									//16
	{"TR", {'+'}, FALSE, FALSE, 21, FALSE, FALSE},									//17
	{"TR", {')'}, FALSE, FALSE, 22, FALSE, FALSE},									//18
	{"TR", {'\n'}, FALSE, TRUE, 25, FALSE, FALSE},									//19
	{"*", {'*'}, TRUE, TRUE, 23, FALSE, FALSE},										//20
	{"+", {'+'}, FALSE, TRUE, -1, FALSE, FALSE},									//21
	{")", {')'}, FALSE, TRUE, -1, FALSE, FALSE},									//22
	{"F", {'-', '(', 'a', 'b', '8', '3'}, FALSE, TRUE, 26, TRUE, FALSE},			//23
	{"TR", {'*', '\n', '+', ')'}, FALSE, TRUE, 16, FALSE, FALSE},					//24
	{"\n", {'\n'}, TRUE, FALSE, -1, FALSE, TRUE},									//25
	{"F", {'-'}, FALSE, FALSE, 32, FALSE, FALSE},									//26
	{"F", {'('}, FALSE, FALSE, 34, FALSE, FALSE},									//27
	{"F", {'a'}, FALSE, FALSE, 37, FALSE, FALSE},									//28
	{"F", {'b'}, FALSE, FALSE, 38, FALSE, FALSE},									//29
	{"F", {'8'}, FALSE, FALSE, 39, FALSE, FALSE},									//30
	{"F", {'3'}, FALSE, TRUE, 40, FALSE, FALSE},									//31
	{"-", {'-'}, TRUE, TRUE, 33, FALSE, FALSE},										//32
	{"F", {'-', '(', 'a', 'b', '8', '3'}, FALSE, TRUE, 26, FALSE, FALSE},			//33
	{"(", {'('}, TRUE, TRUE, 35, FALSE, FALSE},										//34
	{"EXP", {'-', '(', 'a', 'b', '8', '3'}, FALSE, TRUE, 0, TRUE, FALSE},			//35
	{")", {')'}, TRUE, TRUE, -1, FALSE, FALSE},										//36
	{"a", {'a'}, TRUE, TRUE, -1, FALSE, FALSE},										//37
	{"b", {'b'}, TRUE, TRUE, -1, FALSE, FALSE},										//38
	{"8", {'8'}, TRUE, TRUE, -1, FALSE, FALSE},										//39
	{"3", {'3'}, TRUE, TRUE, -1, FALSE, FALSE}										//40
};

bool Analyse(string line) {
	stack<int> stack;
	int index = 0;
	int chIndex = 0;

	while (true) {
		auto currentRule = Table[index];
		auto symbol = (chIndex >= line.size()) ? '\n' : line[chIndex];
		auto found = std::find(Table[index].guidingSymbols.begin(), Table[index].guidingSymbols.end(), symbol) != Table[index].guidingSymbols.end();

		std::cout << symbol << ": " << (found ? "true" : "false") << " : " << index << " | ";
		auto newQ = stack;
		while (!newQ.empty()) {
			std::cout << newQ.top() << " ";
			newQ.pop();
		}
		std::cout << " | " << line << std::endl;

		if (!found) {
			if (currentRule.error) {
				cout << "Expected: ";
				for (auto ch : currentRule.guidingSymbols) {
					cout << ch << " ";
				}
				cout << "; Found: " << symbol << endl;
				return false;
			}
			index++;
			continue;
		}

		if (currentRule.shift) {
			chIndex++;
		}

		if (currentRule.nextOnStack) {
			stack.push(index + 1);
		}

		if (currentRule.indicator != -1) {
			index = currentRule.indicator;
		}
		else {
			if (stack.empty() && currentRule.end) {
				return true;
			}
			if (stack.empty()) {
				cout << "Stack is empty!" << endl;
				return false;
			}
			else {
				index = stack.top();
				stack.pop();
			}
		}
	}

	return true;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	ifstream input("input.txt");

	string str{ istreambuf_iterator<char>(input), istreambuf_iterator<char>() };
	transform(str.begin(), str.end(), str.begin(), tolower);

	int position = 0;
	int index = 0;
	stack<int> stack;

	Analyse(str) ?
		cout << "true" << endl :
		cout << "false" << endl;
}
