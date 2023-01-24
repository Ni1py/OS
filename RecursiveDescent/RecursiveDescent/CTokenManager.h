#pragma once

#include <string>
#include <set>

enum class Type {
	Type, Identifier, OpenBracket, ClosedBracket, Semicolon, Colon, Comma,
	Prog, Begin, End, Var, Read, Write, EndF, Star, Plus, Minus, Equal
};

struct Token {
	Type type;
	std::string value;
};

class CTokenManager
{
public:
	CTokenManager(std::string str = "");

	Token GetNextToken();

	std::string code;
	size_t pos = 0;
private:
	Token CreateToken(const std::string& word);
};