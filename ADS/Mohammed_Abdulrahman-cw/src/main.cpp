
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <cctype>
using namespace std;

void pr_menu(){
	cout << "Enter the number corresponding to the type of task:" << endl;
    cout << "1 - input from the keyboard" << endl;
    cout << "2 - input from file" << endl;
    cout << "0 - exit" << endl;
}

void pr_menu2(){
    cout << "Enter the number corresponding to the type of task:" << endl;
    cout << "1 - add item" << endl;
    cout << "2 - remove item" << endl;
    cout << "3 - step back" << endl;
    cout << "0 - finish work with this tree" << endl;
    cout << endl;
}

struct elem{
    int val;
    int height;
    elem* left;
    elem* right;
};

struct trunk{
	trunk* prev;
	string str;
	trunk(trunk* prev, string str){
		this->prev = prev;
		this->str = str;	
	}
};

void show_tr(trunk* p, int &count){
	if(p == NULL)
		return;
	show_tr(p->prev, count);
	count++;
	cout << p->str;
}

void PR_b_tree(elem* tree, trunk* prev, bool is_right){
	if(tree == NULL)
		return;
	string prev_str = "    ";
	trunk* tmp = new trunk(prev, prev_str);
	PR_b_tree(tree->right, tmp, 1);
	if(!prev)
		tmp->str = "-->";
	else if(is_right){
		tmp->str = ".-->";
		prev_str = "   |";	
	} else {
		tmp->str = "`-->";
		prev->str = prev_str;	
	}
	int count = 0;
	show_tr(tmp, count);
	cout << tree->val << endl;

	if(prev)
		prev->str = prev_str;
	tmp->str = "   |";
	PR_b_tree(tree->left, tmp, 0);
}

int f_height(elem* root){
    int a = 0;
    int b = 0;
    if(root->left !=NULL)
        a = f_height(root->left);
    if(root->right != NULL)
        b = f_height(root->right);
    if(a<b)
        a = b;
    root->height = a+1;
    return root->height;
}

int check_tree(elem* root){
    int error = 0;
    int tmp = 0;
    int left = 0;
    int right = 0;
    if(root->left != NULL){
        error = check_tree(root->left);
        left = root->left->height;
    }
    if(error > 0)
        return error;
    if(root->right != NULL){
        error = check_tree(root->right);
        right = root->right->height;
    }
    if(error > 0)
        return error;
    if(abs(left - right) > 1)
        return root->val;
    else
        return 0;
}

void left_turn(elem* tmp, elem* root){
    tmp->right = root->left;
    root->left = tmp;
}

void right_turn(elem* tmp, elem* root){
    tmp->left = root->right;
    root->right = tmp;
}

void right_right_turn(elem* root, elem* tmp){
    tmp->left->right = root->left;
    root->left = tmp->left;
    tmp->left = root->right;
    root->right = tmp;
}

void left_left_turn(elem* root, elem* tmp){
    tmp->right->left = root->right;
    root->right = tmp->right;
    tmp->right = root->left;
    root->left = tmp;
}

elem* fix_tree(elem* root, int tmp){
    int left = 0;
    int right = 0;
    if(root->left != NULL)
        left = root->left->height;
    if(root->right != NULL)
        right = root->right->height;
    
    if(root->val == tmp){
        elem* tmp_elem = new elem;
        tmp_elem = root;
        if(right > left){
            root = tmp_elem->right;
            left_turn(tmp_elem, root);
            f_height(root);
            tmp = check_tree(root);
            if(tmp == root->val){
                tmp_elem = root;
                root = tmp_elem->left->right;
                right_right_turn(root, tmp_elem);    
            }
        } else {
            root = tmp_elem->left;
            right_turn(tmp_elem, root);
            f_height(root);
            tmp = check_tree(root);
            if(tmp == root->val){
                tmp_elem = root;
                root = tmp_elem->right->left;
                left_left_turn(root, tmp_elem);
            }
        }
    } else {
        if(tmp > root->val)
            root->right = fix_tree(root->right, tmp);
        else
            root->left = fix_tree(root->left, tmp);
    }
    return root;
}

void add_to_tree(elem* root, int tmp_val){
    if(root->val < tmp_val){
        if(root->right != NULL)
            add_to_tree(root->right, tmp_val);
        else{
            root->right = new elem;
            root->right->left = NULL;
            root->right->right = NULL;
            root->right->val = tmp_val;
        }
    } else if(root->val > tmp_val){
        if(root->left != NULL)
            add_to_tree(root->left, tmp_val);
        else{
            root->left = new elem;
            root->left->left = NULL;
            root->left->right = NULL;
            root->left->val = tmp_val;
        }
    }
}

void del_b_tree(elem* root){
    if(root->left != NULL){
        del_b_tree(root->left);
        delete(root->left);
    }
    if(root->right != NULL){
        del_b_tree(root->right);
        delete(root->right);
    }
}

int check_str(string str){
    for(int i = 0; i < str.length(); i++){
        if(!isdigit(str[i]))
            return 0;
    }
    return 1;
} 

void out_elem(ofstream& f2, elem* root){
    f2 << '(' << root->val;
    if(root->left != NULL)
        out_elem(f2, root->left);
    else if(root->right != NULL)
        f2 << '#';
    if(root->right != NULL)
        out_elem(f2, root->right);
    f2 << ')';
}

int work_with_console(){
    int tmp = 0;
    int variants = 0;
    int complexity = 0;
    char str[256];
    string str1;
    int flag = 0;
    int current_s = 0;
    int current_c = 0;
    int is_right = 0;

    while(!is_right){
        is_right = 1;
        cout << "Enter the number of options:" << endl;
        getline(cin, str1);
        while(!flag){
            if(str1[current_s] == ' ')
                current_s++;
            else
                flag = 1;
            if(current_s >= str1.length()){
                is_right = 0;
                break;
            }
        }
        while(flag){
            if(current_s < str1.length())
                if(isdigit(str1[current_s])){
                    str[current_c] = str1[current_s];    
                    current_s++;
                    current_c++;
                } else{
                    flag = 0;
                }
            else{
                break;
            }
        }
        while(!flag){
            if(current_s == str1.length())
                break;
            if(str1[current_s] == ' ')
                current_s++;
            else{
                is_right = 0;
                break;
            }
        }
        if(!is_right){
            cout << "wrong expression" << endl;
            for(int i = 0; i < current_c; i++)
                str[i] = 0;
            flag = 0;
            current_s = 0;
            current_c = 0;
            continue;
        }
        variants = atoi(str);
        for(int i = 0; i < current_c; i++)
            str[i] = 0;
        flag = 0;
        current_s = 0;
        current_c = 0;
    }
    is_right = 0;
    while(!is_right){
        is_right = 1;
        cout << "Enter the number of items in the tree :" << endl;
        getline(cin, str1);
        while(!flag){
            if(str1[current_s] == ' ')
                current_s++;
            else
                flag = 1;
            if(current_s >= str1.length()){
                is_right = 0;
                break;
            }
        }
        while(flag){
            if(current_s < str1.length())
                if(isdigit(str1[current_s])){
                    str[current_c] = str1[current_s];    
                    current_s++;
                    current_c++;
                } else{
                    flag = 0;
                }
            else{
                break;
            }
        }
        while(!flag){
            if(current_s == str1.length())
                break;
            if(str1[current_s] == ' ')
                current_s++;
            else{
                is_right = 0;
                break;
            }
        }
        if(!is_right){
            cout << "wrong expression" << endl;
            for(int i = 0; i < current_c; i++)
                str[i] = 0;
            flag = 0;
            current_s = 0;
            current_c = 0;
            continue;
        }
        complexity = atoi(str);
        for(int i = 0; i < current_c; i++)
            str[i] = 0;
        flag = 0;
        current_s = 0;
        current_c = 0;
    }
    ofstream f1;
    f1.open("output_task.txt");
    ofstream f2;
    f2.open("output_ans.txt");
    int max = variants;
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    int tmp_comp = 0;
    while(variants){
        if(complexity == 0)
            break;
        tmp_comp = complexity;
        elem* root = new elem;
        root->left = NULL;
        root->right = NULL;
        
        tmp = rand();
        f1 << max-variants+1 << ')' << tmp << "  ";
        root->val = tmp;
        f_height(root);
        tmp_comp--;
        while(tmp_comp){
            tmp = rand();
            f1 << tmp << "  ";
            add_to_tree(root, tmp);
            f_height(root);
            tmp = check_tree(root);
            while(tmp){
                root = fix_tree(root, tmp);
                f_height(root);
                tmp = check_tree(root);
            }
            tmp_comp--;
        }
        cout << "Binary tree:" << endl;
        PR_b_tree(root, NULL, 0);
        f1 << "\\\\" << '\n';
        f2 << max-variants+1 << ')';
        out_elem(f2, root);
        f2 << "\\\\" << '\n';
        del_b_tree(root);
        delete(root);
        variants--;
    }
    f1.close();
    f2.close();
    return 0;
}

int main(){
    work_with_console();
    return 0;
}
