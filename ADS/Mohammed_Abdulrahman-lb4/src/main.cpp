#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

ostream& operator << ( ostream& out, const  vector<int>& vec) {
	out << '[';
	for (int i = 0; i < vec.size(); i++) {
		if (i == vec.size() - 1) {
			out << vec[i];
			break;
		}
		out << vec[i] << ", ";
	}
	out << ']';
	return out;
}

void select_sort( vector<int>& vec){
  int i,j,loc,temp,min;
  int n = vec.size();
  for(i=0;i<n-1;i++)
  {
    min=vec[i];
    loc=i;
    for(j=i+1;j<n;j++)
    {
      if(min>vec[j])
      {
        min=vec[j];
        loc=j;
      }
    }
      cout << "This is " << i+1 << " iteration of sorting" << std :: endl;
     cout << "We will swap these elements " << '(' << vec[i] << ')' << " and " << '(' << vec[loc]<< ')' <<  endl;
    temp=vec[i];
    vec[i]=vec[loc];
    vec[loc]=temp;

        
     cout << '[';
    for(int i = 0 ; i < n ; i++)
    {
      if(i == n-1)
      {
         cout << vec[i];
      }
      else
         cout << vec[i] << ',';
    }
     cout << ']' <<  endl;
  }
}


void check_string(string& str) {
	for (int i = 0; i < str.size(); i++) {
		if (!isdigit(str[i])&&str[i]!=' ') {
			str.erase(i, 1);
			i -= 1;
		}
	}
}


int main(int argc, char** argv) {
  cout << "____SELECTION SORT ALGORITHM___\n\n";
  cout << "Enter string of elements: ";
	string str1;
	vector<int> vec;
	vector<int> vecToCheck;
	int value;
 
	if (argc < 2)
	  getline( cin, str1);
	else
		str1 = argv[1];
 
	check_string(str1);
  stringstream ss(str1);
	while (ss >> value)
  {
		vec.push_back(value);
 
		if (ss.peek() == ' '){
			ss.ignore();
		}
	}
	vecToCheck = vec;
	select_sort(vec);
	 cout << "This is the sorted array" << '\n';
	 cout << vec;
	 cout << '\n';
	 sort(vecToCheck.begin(), vecToCheck.end());
	if (vec == vecToCheck) {
		 cout << "They are identical" <<  endl;
	}
	else {
		 cout << "Incorrect!" <<  endl;
	}
	return 0;
}