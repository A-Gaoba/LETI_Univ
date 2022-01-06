
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool Bracket_exp (string str, unsigned int cur = 0, bool flag = false, int obr = 0)
{
    if(cur < str.length())
    {
        if(str[cur]=='A' && flag==false)
        {
            flag = true;
        }
        else if(str[cur] == '(' && flag==true && str[cur-1] != ')')
        {
            flag = false;
            obr++;
        }
        else if(str[cur] == ';' && flag == true && obr > 0)
        {
            flag = false;
        }
        else if(str[cur] == ')' && flag == true && obr > 0)
        {
            obr--;
        }
        else if(obr != 0)
        {
            return flag = false;
        }
        else
        {
            return flag = false;
        }
        flag = Bracket_exp (str, cur+1, flag, obr);
    }
    return flag;
}

string delet(string str){
  int i = 0;
  while(str.length()>i){
    if(str[i] == ' ')
      str.erase(i,1);
    else  i++;
  }
  return str;
}
int print(bool res, int sw_var){
  if(res)
      cout << "True. This is the concept of brackets.\n";
  else
      cout << "False. This is not the concept of brackets.\n";
  return sw_var = 4;
}

int main()
{
    int sw_var;
    string str;
    bool res = false;
    string FileName;
    fstream file;
  
    cout <<"\033[0;31m"<< "0.Exit from the program"<< '\n';
    cout << "1.Input a string from the keyboard"<< '\n';
    cout << "2.Input a line from a file" << '\n'<<"\033[0m";
    cin >> sw_var;
    cin.ignore();
    while(sw_var)
    {
        switch(sw_var)
        {
        case 1:
            cout <<"\033[0;34m"<< "Enter the string:"<< '\n'<<"\033[0m";
            getline(cin, str);
            if(str != ""){
                res = Bracket_exp (delet(str));
              }
            sw_var=print(res, sw_var);
            break;
        case 2:
            cout <<"\033[0;34m"<< "Enter the name of the file:"<< '\n'<<"\033[0m";
            cin >> FileName;
            cin.ignore();
            file.open(FileName, fstream::in);
            if(!file.is_open()){
                cout << "Error opening file.\n";
                sw_var = 4;
              }
            else{
                getline(file, str);
                cout << delet(str) << endl;
                if(str != "")
                    res = Bracket_exp(delet(str));
                sw_var=print(res, sw_var);
                file.close();
              }
            break;
        case 0:
            break;
        default:
            cout <<"\033[0;1m"<< "Enter again.\n"<<"\033[0m";
            cin >> sw_var;
            cin.ignore();
            break;
        }
    }
    return 0;
}