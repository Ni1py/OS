#include "CTokenManager.h"
#include <set>

using namespace std;

set<string> separators = { ";", ":", ",", "(", ")", "*", "+", "-", "=" };

set<string> identifiers = { "id", "num" };

set<string> types = { "int", "float", "bool", "string" };

CTokenManager::CTokenManager(string str) : code(str) {}

Token CTokenManager::GetNextToken()
{
	while (isspace(code[pos]))
		if ((pos + 1) <= code.length())
			pos++;

	string word = "";

	if (separators.count({ code[pos] }))
	{
		word += code[pos];
		pos++;
	}
	else
		for (size_t i = pos; i < code.length(); i++)
		{
			if (isspace(code[i]) || separators.count({ code[i] }))
				break;

			pos++;
			word += code[i];
		}

	return CreateToken(word);
}

Token CTokenManager::CreateToken(const string& word)
{
	if (types.count(word))
		return { Type::Type, word };

	else if (identifiers.count(word))
		return { Type::Identifier, word };

	else if (word == "(")
		return { Type::OpenBracket, word };

	else if (word == ")")
		return { Type::ClosedBracket, word };

	else if (word == ";")
		return { Type::Semicolon, word };

	else if (word == ":")
		return { Type::Colon, word };

	else if (word == ",")
		return { Type::Comma, word };

	else if (word == "prog")
		return { Type::Prog, word };

	else if (word == "begin")
		return { Type::Begin, word };

	else if (word == "end")
		return { Type::End, word };

	else if (word == "var")
		return { Type::Var, word };

	else if (word == "read")
		return { Type::Read, word };

	else if (word == "write")
		return { Type::Write, word };

	else if (word == "")
		return { Type::EndF, "" };

	else if (word == "*")
		return { Type::Star, word };

	else if (word == "+")
		return { Type::Plus, word };

	else if (word == "-")
		return { Type::Minus, word };

	else if (word == "=")
		return { Type::Equal, word };

	else
		throw "Unknown word: '" + word + "' pos: " + to_string(pos);
}
