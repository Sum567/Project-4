#include "SymbolTable.h"

#include <string>
#include <list>
#include <stack>
#include <vector>
#include <iostream>

using namespace std;

// This class does the real work of the implementation.

class SymbolTableImpl
{
public:
	SymbolTableImpl();
	~SymbolTableImpl();
	void enterScope();
	bool exitScope();
	bool declare(const string& id, int lineNum);
	int find(const string& id) const;
private:
	struct var {
		string name;
		int line;
		int depth;
	};

	int hash(string name) const
	{
		unsigned long id = 0;
		int length = name.length();
		for (int i = 0; i < length; i++) {
			id = id * 34 + name[i];
		}

		return id % 19997;
	}

	int curr_depth = 0;

	list<var> hashTable[19997];
	
	stack <var> ids;

};

SymbolTableImpl::SymbolTableImpl()
{

}

SymbolTableImpl::~SymbolTableImpl()
{

}

//O(1) each time it is called
void SymbolTableImpl::enterScope()
{
	curr_depth++;
}

bool SymbolTableImpl::exitScope()
{
	int index = 0;
	if (curr_depth == 0)
		return false;

	while (!ids.empty()) {
		var temp = ids.top();
		if (temp.depth < curr_depth)
			break;
		ids.pop();
		
		int index = hash(temp.name);

		list<var>::iterator it = hashTable[index].begin();

		while (it != hashTable[index].end()) {
			if (it->name == temp.name && it->depth == curr_depth) {
				it = hashTable[index].erase(it);
			}
			else {
				++it;
			}

		}
	}

	curr_depth--;


	return true;
}

bool SymbolTableImpl::declare(const string& id, int lineNum)
{
	if (id.empty())
		return false;

	int index = hash(id);


	list<var>::iterator it = hashTable[index].begin();

	while (it != hashTable[index].end()) {
		if (it->name == id && it->depth == curr_depth) {
			return false;
		}
		else {
			++it;
		}

	}

	var temp;
	temp.depth = curr_depth;
	temp.line = lineNum;
	temp.name = id;
	hashTable[index].push_front(temp);

	ids.push(temp);

	return true;

}

int SymbolTableImpl::find(const string& id) const
{
	if (id.empty())
		return -1;

	if (ids.empty())
		return -1;
	int index = hash(id);

	if (hashTable[index].empty())
		return -1;
	list<var>::const_iterator it = hashTable[index].begin();

	while (it != hashTable[index].end()) {
		if (it->name == id) {
			return it->line;
		}
		else {
			++it;
		}

	}

	return -1;
}

//*********** SymbolTable functions **************

// For the most part, these functions simply delegate to SymbolTableImpl's
// functions.

SymbolTable::SymbolTable()
{
	m_impl = new SymbolTableImpl;
}

SymbolTable::~SymbolTable()
{
	delete m_impl;
}

void SymbolTable::enterScope()
{
	m_impl->enterScope();
}

bool SymbolTable::exitScope()
{
	return m_impl->exitScope();
}

bool SymbolTable::declare(const string& id, int lineNum)
{
	return m_impl->declare(id, lineNum);
}

int SymbolTable::find(const string& id) const
{
	return m_impl->find(id);
}


