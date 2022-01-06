#include <iostream>
#include <fstream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "btree.h"
#include "functions.h"


using namespace std ;
using namespace binTree_modul;
using namespace lab3_namespace;

int main () 
{
	string formula;
	binTree b;
	
	cout << "Please write formula :\n";
	b = FormToTree(cin);
	cout << "The formula tree is built:" << endl;
	formula = TreeToForm(b);
	cout << formula << endl;

	cout << "Transforming a formula tree..." << endl;
	ProcTree(b);
	formula = TreeToForm(b);
	cout << formula << endl;

	cout << "Prefix Formula Tree..." << endl;
	formula = Translate(b);
	cout << formula << endl;

	cout << "Formula tree visualization: " << endl;
	print(b, 0);

	destroy (b);
	cout << endl;
	return (0);
}