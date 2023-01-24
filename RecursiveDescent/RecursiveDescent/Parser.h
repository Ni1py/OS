#pragma once

#include <string>
#include "CTokenManager.h"

class Parser
{
private:
	std::string CreateMsg(std::string expected, std::string found, size_t pos);

	void OpenBracket(Token token);

	void ClosedBracket(Token token);

	void Semicolon(Token token);

	void Colon(Token token);

	void Equal(Token token);

	void Id(Token token, std::string name = "");

	void Begin(Token token);

	void End(Token token);

	void IDList(Token token);

	void DataType(Token token);

	void Var(Token token);

	void Assign(Token token);

	void Exp(Token token);

	void T(Token token);

	void F(Token token);

	void IDListInBrackets(Token token);

	void St(Token token);

	void ListSt(Token token);
public:
	CTokenManager tokenManager;

	void Prog(Token token);

	Parser(std::string str = "");
};