#include "Parser.h"

using namespace std;

Parser::Parser(string str) : tokenManager(CTokenManager(str))
{
}

string Parser::CreateErrorDescription(string expected, string found, size_t pos)
{
	return "Expected: " + expected + " found: '" + found + "' position: " + to_string(pos) + "\n";
}

void Parser::OpenBracket(Token token)
{
	if (token.type != Type::OpenBracket)
		throw CreateErrorDescription("'('", token.value, tokenManager.pos);
}

void Parser::ClosedBracket(Token token)
{
	if (token.type != Type::ClosedBracket)
		throw CreateErrorDescription("')'", token.value, tokenManager.pos);
}

void Parser::Semicolon(Token token)
{
	if (token.type != Type::Semicolon)
		throw CreateErrorDescription("';'", token.value, tokenManager.pos);
}

void Parser::Colon(Token token)
{
	if (token.type != Type::Colon)
		throw CreateErrorDescription("':'", token.value, tokenManager.pos);
}

void Parser::Equal(Token token)
{
	if (token.type != Type::Equal)
		throw CreateErrorDescription("'='", token.value, tokenManager.pos);
}

void Parser::Id(Token token, string name)
{
	if (name != "")
		if (token.type != Type::Identifier && token.value != name)
			throw CreateErrorDescription(name, token.value, tokenManager.pos);
		else
			if (token.type != Type::Identifier)
				throw CreateErrorDescription("Identifier", token.value, tokenManager.pos);
}

void Parser::Begin(Token token)
{
	if (token.type != Type::Begin)
		throw CreateErrorDescription("'begin'", token.value, tokenManager.pos);
}

void Parser::End(Token token)
{
	if (token.type != Type::End)
		throw CreateErrorDescription("'end'", token.value, tokenManager.pos);

	token = tokenManager.GetNextToken();
	if (token.type != Type::EndF)
		throw CreateErrorDescription("", token.value, tokenManager.pos);
}

void Parser::IDList(Token token)
{
	Id(token, "id");

	size_t pos = tokenManager.pos;

	token = tokenManager.GetNextToken();

	if (token.type == Type::Comma)
		IDList(tokenManager.GetNextToken());
	else
		tokenManager.pos = pos;
}

void Parser::DataType(Token token)
{
	if (token.type != Type::Type)
		throw CreateErrorDescription("Type", token.value, tokenManager.pos);
}

void Parser::Var(Token token)
{
	if (token.type != Type::Var)
		throw CreateErrorDescription("'var'", token.value, tokenManager.pos);

	IDList(tokenManager.GetNextToken());

	Colon(tokenManager.GetNextToken());

	DataType(tokenManager.GetNextToken());

	Semicolon(tokenManager.GetNextToken());
}

void Parser::Assign(Token token)
{
	Id(token, "id");
	Colon(tokenManager.GetNextToken());
	if (isspace(tokenManager.code[tokenManager.pos]))
		throw CreateErrorDescription("':='", ":", tokenManager.pos);
	Equal(tokenManager.GetNextToken());

	token = tokenManager.GetNextToken();
	switch (token.type)
	{
	case Type::OpenBracket:
	case Type::Identifier:
	case Type::Minus:
		Exp(token);
		break;
	default:
		throw CreateErrorDescription("Identifier or '(' or '-'", token.value, tokenManager.pos);
	}
}

void Parser::Exp(Token token)
{
	switch (token.type)
	{
	case Type::OpenBracket:
	case Type::Identifier:
	case Type::Minus:
		T(token);
		token = tokenManager.GetNextToken();
		switch (token.type)
		{
		case Type::Plus:
			Exp(tokenManager.GetNextToken());
			break;
		case Type::ClosedBracket:
		case Type::Semicolon:
			tokenManager.pos--;
			break;
		default:
			throw CreateErrorDescription("'+' or ')' or ';'", token.value, tokenManager.pos);
		}
		break;
	default:
		throw CreateErrorDescription("Identifier or '(' or '-'", token.value, tokenManager.pos);
	}
}

void Parser::T(Token token)
{
	switch (token.type)
	{
	case Type::OpenBracket:
	case Type::Identifier:
	case Type::Minus:
		F(token);
		token = tokenManager.GetNextToken();
		switch (token.type)
		{
		case Type::Star:
			T(tokenManager.GetNextToken());
			break;
		case Type::Plus:
		case Type::ClosedBracket:
		case Type::Semicolon:
			tokenManager.pos--;
			break;
		default:
			throw CreateErrorDescription("'*' or '+' or ')' or ';'", token.value, tokenManager.pos);
		}
		break;
	default:
		throw CreateErrorDescription("Identifier or '(' or '-'", token.value, tokenManager.pos);
	}
}

void Parser::F(Token token)
{
	switch (token.type)
	{
	case Type::Identifier:
		return;
	case Type::OpenBracket:
		Exp(tokenManager.GetNextToken());
		ClosedBracket(tokenManager.GetNextToken());
		break;
	case Type::Minus:
		F(tokenManager.GetNextToken());
		break;
	default:
		throw CreateErrorDescription("Identifier or '(' or '-'", token.value, tokenManager.pos);
	}
}

void Parser::IDListInBrackets(Token token)
{
	OpenBracket(token);

	IDList(tokenManager.GetNextToken());

	ClosedBracket(tokenManager.GetNextToken());
}

void Parser::St(Token token)
{
	if (token.type == Type::Read)
		IDListInBrackets(tokenManager.GetNextToken());
	else if (token.type == Type::Write)
		IDListInBrackets(tokenManager.GetNextToken());
	else if (token.type == Type::Identifier)
		Assign(token);
	else
		throw CreateErrorDescription("Statement", token.value, tokenManager.pos);
}

void Parser::ListSt(Token token)
{
	St(token);

	Semicolon(tokenManager.GetNextToken());

	size_t pos = tokenManager.pos;
	token = tokenManager.GetNextToken();

	if (token.type != Type::End && token.type != Type::EndF)
		ListSt(token);
	else
		tokenManager.pos = pos;
}

void Parser::Prog(Token token)
{
	if (token.type != Type::Prog)
		throw CreateErrorDescription("'prog'", token.value, tokenManager.pos);

	Id(tokenManager.GetNextToken(), "id");

	Var(tokenManager.GetNextToken());

	Begin(tokenManager.GetNextToken());

	ListSt(tokenManager.GetNextToken());

	End(tokenManager.GetNextToken());
}
