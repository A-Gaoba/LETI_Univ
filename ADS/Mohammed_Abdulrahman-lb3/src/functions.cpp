#include <iostream>
#include <fstream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "btree.h"

using namespace std;
using namespace binTree_modul;

// display the tree as a tree
// more informative errors

namespace lab3_namespace
{
	bool isTerminal( const char c )
	{
		return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
	}

	bool isSign( const char c )
	{
		return c == '-' || c == '+' || c == '*';
	}

	binTree MakeLeaf( base c )
	{
		return ConsBT(c, nullptr, nullptr);
	}

	void PrintBt(binTree b)
	{
		if (b == NULL)
			return; 

		if (b->lt == NULL && b->rt == NULL){
			cout << ' ' << b->info << ' ';
			return;
		}

		cout << "( ";
		PrintBt(b->lt);
		cout << ' ' << b->info << ' ';
		PrintBt(b->rt);
		cout << " )";
	}

	void SkipSpaces(istream &in)
	{
		base c;
		do 
		{
			c = in.peek();
			if (c == ' ')
				c = in.get();
		} while (c == ' ');
	}

	string TreeToForm(binTree b)
	{
		string str = "";
		if (b == NULL)
			return str; 

		if (b->lt == NULL && b->rt == NULL)
    {
			str += ' ';
			str += b->info;
			str += ' ';
			return str;
		}

		str += "( ";
		str += TreeToForm(b->lt);
		str += ' ';
		str += b->info;
		str += ' ';
		str += TreeToForm(b->rt);
		str += " )";
    return str;
  }
  
	binTree FormToTree(istream &in)
	{
		base c, sign;
		binTree left, right;

		SkipSpaces(in);
		c = in.get();

		if (isTerminal(c)) // if the formula is a terminal
			return MakeLeaf(c); 

		if (c == '('){ // if the formula has the form (<term> <sign> <term>)
			SkipSpaces(in);
			left = FormToTree(in); // first formula 

			SkipSpaces(in);
			c = in.get();
			if (isSign(c))
				sign = c; // sign
			else { cerr << "error: sign expected" << endl; return NULL; }
			// if there is no sign after the first formula

			SkipSpaces(in);
			right = FormToTree(in); // second formula 

			SkipSpaces(in); // checking brackets
			c = in.get(); 
			if (c != ')') { cerr << "error: ')' expected" << endl; return NULL; }
      //formula of the form (<term> <sign> <term>) ends with a parenthesis

			left = ConsBT(sign, left, right); // we make from the sign and two formulas the formula
			return left;
		}
		else {
			if (isSign(c)) cerr << "error: extra sign in formula" << endl;
			else cerr << "error: external symbol if formula" << endl;
			return FormToTree(in);
		 	//return NULL;
		 } // the formula is either a term or has the form (<term> <sign> <term>) and must begin with a parenthesis
	}


	void Transform(binTree b)
	{	// ((f1 * f2) + (f1 * f3)) -> (f1 * (f2 + f3))
		if (isEqual(b->lt->lt, b->rt->lt)){
			binTree f1, f2, f3;

			f1 = b->lt->lt;
			f2 = b->lt->rt;
			f3 = b->rt->rt;

			b->info = '*';
			b->lt = f1;
			b->rt->info = '+';
			b->rt->lt = f2;
			b->rt->rt = f3;

			return;
		}
		// ((f1 * f2) + (f3 * f1)) -> (f1 * (f2 + f3))
		if (isEqual(b->lt->lt, b->rt->rt)){
			binTree f1, f2, f3;

			f1 = b->lt->lt;
			f2 = b->lt->rt;
			f3 = b->rt->lt;

			b->info = '*';
			b->lt = f1;
			b->rt->info = '+';
			b->rt->lt = f2;
			b->rt->rt = f3;

			return;
		}
		// ((f2 * f1) + (f1 * f3)) -> (f1 * (f2 + f3))
		if (isEqual(b->lt->rt, b->rt->lt)){
			binTree f1, f2, f3;

			f1 = b->lt->rt;
			f2 = b->lt->lt;
			f3 = b->rt->rt;

			b->info = '*';
			b->lt = f1;
			b->rt->info = '+';
			b->rt->lt = f2;
			b->rt->rt = f3;

			return;
		}
		// ((f2 * f1) + (f3 * f1)) -> (f1 * (f2 + f3))
		if (isEqual(b->lt->rt, b->rt->rt)){
			binTree f1, f2, f3;

			f1 = b->lt->rt;
			f2 = b->lt->lt;
			f3 = b->rt->lt;

			b->info = '*';
			b->lt = f1;
			b->rt->info = '+';
			b->rt->lt = f2;
			b->rt->rt = f3;

			return;
		}
		
	}


	void ProcTree(binTree b)
	{
		if (b == NULL)
			return;
		ProcTree(b->lt);
		ProcTree(b->rt);

		if (b == NULL || 
			b->lt == NULL || b->lt->lt == NULL || b->lt->rt == NULL ||
			b->rt == NULL || b->rt->lt == NULL || b->rt->rt == NULL)
			return; // check enough node descendants

		if (b->lt->info != '*' || b->rt->info != '*' || b->info != '+')
			return; // checking signs

		// don't check that there is a factor that can be taken out
  // Transform will check for it
		Transform(b);

	}

	string Translate(binTree b)
	{
		string str = "";

		if (b == NULL)
			return str;
		str += ' ';
		str += b->info;
		str += ' ';
		if (b->lt != NULL){
			str += ' ';
			str += Translate(b->lt);
			str += ' ';
		}
		if (b->rt != NULL){
			str += ' ';
			str += Translate(b->rt);
			str += ' ';
		}
    return str;
	}

	void print(binTree f, int l)
	{
		if(f == nullptr){
			for(int i = 0; i < l; i++)
				cout << "\t";
			cout << '#' << endl;
			return;
		}

		print(f->rt, l+1);

		for(int i = 0; i < l; i++)
			cout << "\t";

		cout << f->info << endl;

		print(f->lt, l+1);
    }
}