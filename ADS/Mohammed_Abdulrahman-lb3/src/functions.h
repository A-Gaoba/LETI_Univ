#include <iostream>
#include "btree.h"

using namespace std;
using namespace binTree_modul;

namespace lab3_namespace
{
	bool isTerminal( const char c );
	bool isSign( const char c );
	binTree MakeNode( base c );
	void PrintBt(binTree b);
	void SkipSpaces(istream &in);
	string TreeToForm(binTree b);
	binTree FormToTree(istream &in);
	void Transform(binTree b);
	void ProcTree(binTree b);
	string Translate(binTree b);
	void print(binTree f, int l);
}