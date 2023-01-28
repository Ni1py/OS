#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <windows.h>
#include "Parser.h"

using namespace std;

Parser parser = Parser();

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	ifstream input("input.txt");

	string str{ istreambuf_iterator<char>(input), istreambuf_iterator<char>() };
	transform(str.begin(), str.end(), str.begin(), tolower);

	parser = Parser(str);

	try
	{
		parser.Prog(parser.tokenManager.GetNextToken());
		cout << "The code is correct!";
	}
	catch (const string msg)
	{
		cout << msg;
	}
}