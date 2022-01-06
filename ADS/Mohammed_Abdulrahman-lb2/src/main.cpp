#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#include <fstream>

#define N 501

using namespace std;

typedef struct BinCor
{
    unsigned int length_left;
    unsigned int length_right;
    unsigned int weight_left;
    unsigned int weight_right;
    struct BinCor* cor_1;
    struct BinCor* cor_2;
} BinCor;



void errorMassage(int error_number, char* str, int index)
{
    cout << "\nError! You entered incorrect data:\n";
    switch (error_number)
    {
        case 1:
            cout << "Symbol № " << index + 1 << " - " << "'" << str[index] << "'.\n";
            cout << "Symbol expected - '('.\n";
            break;
        case 2:
            cout << "Symbol № " << index + 1 << " - " << "'" << str[index] << "'.\n";
            cout << "Symbol expected - ')'.\n";
            break;
        case 3:
            cout << "Symbol № " << index + 1 << " - " << "'" << str[index] << "'.\n";
            cout << "Expected a value between 1 and 9.\n";
            break;
        case 4:
            cout << "Symbol № " << index + 1 << " - " << "'" << str[index] << "'.\n";
            cout << "Gap expected.\n";
            break;
        case 5:
            cout << "Symbol № " << index + 1 << " - " << "'" << str[index] << "'.\n";
            cout << "Missing closing parenthesis.\n";
            break;
        case 6:
            cout << "Symbol № " << index + 1 << " - " << "'" << str[index] << "'.\n";
            cout << "value was expected between 1 and 9 or'('.\n";
            break;
        case 7:
            cout << "There are extra characters after the №" << index + 1 << " character.\n";
            break;
    }
}


BinCor* initBinCorElement()
{
    BinCor* element = new BinCor[sizeof(BinCor)];
    element->length_left = 0;
    element->length_right = 0;
    element->weight_left = 0;
    element->weight_right = 0;
    element->cor_1 = NULL;
    element->cor_2 = NULL;
    return element;
}


// read and create a binary rocker
// function returns 1 if an error occurred
int readBinCorElement(char* str, int index_1, int index_2, BinCor** element)
{
    *element = initBinCorElement();

    if (str[index_1++] != '(')
    {
        errorMassage(1, str, index_1 - 1);
        return 1;
    }
    if (str[index_2--] != ')')
    {
        errorMassage(2, str, index_2 + 1);
        return 1;
    }


    if (str[index_1++] != '(')
    {
        errorMassage(1, str, index_1 - 1);
        return 1;
    }

    if (!isdigit(str[index_1]) || str[index_1] == '0')
    {
        errorMassage(3, str, index_1);
        return 1;
    }

    while (1)
    {
        if (isdigit(str[index_1]))
        {
            (*element)->length_left = (*element)->length_left * 10 + str[index_1] - '0';
            index_1++;
        }
        else
            break;
    }

    if (str[index_1++] != ' ')
    {
        errorMassage(4, str, index_1 - 1);
        return 1;
    }

    if (isdigit(str[index_1]) && str[index_1] != 0)
    {
        while (1)
        {
            if (isdigit(str[index_1]))
            {
                (*element)->weight_left = (*element)->weight_left * 10 + str[index_1] - '0';
                index_1++;
            }
            else
                break;
        }
    }
    else
    if (str[index_1] == '(')
    {
        int bracket_count = 0;
        int index;

        for (index = index_1; index < index_2; index++)
        {
            if (str[index] == '(')
                bracket_count++;
            if (str[index] == ')')
                bracket_count--;

            if (bracket_count == 0)
            {
                if (readBinCorElement(str, index_1, index, &((*element)->cor_1)))
                    return 1;
                index_1 = index + 1;
                break;
            }
        }
        if (bracket_count != 0)
        {
            errorMassage(5, str, index_1);
            return 1;
        }
    }
    else
    {
        errorMassage(6, str, index_1);
        return 1;
    }

    if (str[index_1++] != ')')
    {
        errorMassage(2, str, index_1 - 1);
        return 1;
    }


    if (str[index_1++] != ' ')
    {
        errorMassage(4, str, index_1 - 1);
        return 1;
    }



    if (str[index_1++] != '(')
    {
        errorMassage(1, str, index_1 - 1);
        return 1;
    }


    if (!isdigit(str[index_1]) || str[index_1] == 0)
    {
        errorMassage(3, str, index_1);
        return 1;
    }
    while (1)
    {
        if (isdigit(str[index_1]))
        {
            (*element)->length_right = (*element)->length_right * 10 + str[index_1] - '0';
            index_1++;
        }
        else
            break;
    }

    if (str[index_1++] != ' ')
    {
        errorMassage(4, str, index_1 - 1);
        return 1;
    }

    if (isdigit(str[index_1]) && str[index_1] != '0')
    {
        while (1)
        {
            if (isdigit(str[index_1]))
            {
                (*element)->weight_right = (*element)->weight_right * 10 + str[index_1] - '0';
                index_1++;
            }
            else
                break;
        }
    }
    else
    if (str[index_1] == '(')
    {
        int bracket_count = 0;
        int index;

        for (index = index_1; index < index_2; index++)
        {
            if (str[index] == '(')
                bracket_count++;
            if (str[index] == ')')
                bracket_count--;

            if (bracket_count == 0) {
                if (readBinCorElement(str, index_1, index, &((*element)->cor_2)))
                    return 1;
                index_1 = index + 1;
                break;
            }
        }

        if (bracket_count != 0)
        {
            errorMassage(5, str, index_1);
            return 1;
        }
    }
    else
    {
        errorMassage(6, str, index_1);
        return 1;
    }
    if (str[index_1] != ')')
    {
        errorMassage(2, str, index_1);
        return 1;
    }


    if (index_2 != index_1)
    {
        errorMassage(7, str, index_1);
        return 1;
    }

    return 0;
}


// create a binary rocker
// function returns 1 if an error occurred
int createBinCor(char* str, BinCor** binCor)
{
    int index_1 = 0;
    int index_2 = strlen(str) - 2;

    return readBinCorElement(str, index_1, index_2, binCor);
}


void print_bin_kor(BinCor* binCor, int deep_of_recursion)
{
    for (int i = 0; i < deep_of_recursion; i++)
        cout << "\t";
    cout << binCor->length_right << "  ";

    if (binCor->cor_2 == NULL)
    {
        cout << binCor->weight_right << "(weight)\n\n";
    }
    else
    {
        cout << "\t\n";
        print_bin_kor(binCor->cor_2, deep_of_recursion + 1);
    }

    for (int i = 0; i < deep_of_recursion; i++)
        cout << "\t";
    cout << binCor->length_left << "  ";

    if (binCor->cor_1 == NULL)
    {
        cout << binCor->weight_left << "(weight)\n\n";
    }
    else
    {
        cout << "\t\n";
        print_bin_kor(binCor->cor_1, deep_of_recursion + 1);
    }
}


// The return value is equal to the length of all arms
// in the given binary rocker
short length(const BinCor binCor, int deep_of_recursion)
{
    int result = 0;

    for (int i = 0; i < deep_of_recursion; i++)
        cout << "     ";
    cout << "left shoulder(+" << binCor.length_left << "): ";
    result += binCor.length_left;

    if (binCor.cor_1 == NULL)
    {
        cout << "weight.\n";
    }
    else
    {
        cout << "rocker: \n";
        result += length(*(binCor.cor_1), deep_of_recursion + 1);
    }

    for (int i = 0; i < deep_of_recursion; i++)
        cout << "     ";
    cout << "right shoulder(+" << binCor.length_right << "): ";
    result += binCor.length_right;

    if (binCor.cor_2 == NULL)
    {
        cout << "weight.\n";
    }
    else
    {
        cout << "rocker: \n";
        result += length(*(binCor.cor_2), deep_of_recursion + 1);
    }

    return result;
}



void free_memory(BinCor* binCor)
{
    if (binCor != NULL)
    {
        delete binCor->cor_1;
        delete binCor->cor_2;
    }
    delete binCor;
}



int main()
{
    char str[N];
    BinCor* binCor = NULL;
    int in = 0;


    cout << "\nThe program displays the total length of all arms in the specified binary rocker.\n";
    cout << "\nThe binary rocker is written as:\n";
    cout << "(SHOULDER SHOULDER)\n";
    cout << "The leverage is as follows:\n";
    cout << "(LENGTH OF CARGO)\n";
    cout << "Another binary rocker or weight (number) can act as a weight.\n";
    cout << "\nSelect the input method for the binary rocker (no more than 500 characters): \n";
    cout << "\n1. Entering a rocker arm from a file.";
    cout << "\n2. Entering the rocker arm from the console.\n";
    cin >> in;

    if (in == 1)
    {
        FILE* file = fopen("input.txt", "r");
        if (file == NULL)
            perror("File open error");
        fgets(str, N, file);
        fclose(file);
    }
    if (in == 2)
    {
        cout << "\nEnter a binary rocker (max. 500 characters):";
        fgets(str, 2, stdin);
        fgets(str, N, stdin);
    }


    if (createBinCor(str, &binCor))
    {
        cout << "The program has ended.\n\n";
        free_memory(binCor);
        return 0;
    }
    cout << "\nThe entered data is correct.\n\n";

    print_bin_kor(binCor, 0);

    cout << "\n\n\n";


    cout << "Algorithm progress:\n\n";
    cout << "\nShoulder total length: " << length(*binCor, 1) << ".\n\n";

    free_memory(binCor);

    return 0;
}