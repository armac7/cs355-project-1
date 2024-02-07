#include <iostream>
#include <vector>
#include "../headers/BinarySearchTree.h"
using namespace std;

void PrintMenu();
string GetInput();
bool ExecuteOption(string input, BinarySearchTree<int> &tree);

int main()
{
    BinarySearchTree<int> tree;
    string input = "";
    bool execute = true;
    PrintMenu();
    while (execute)
    {
        input = GetInput();
        execute = ExecuteOption(input, tree);
        tree.PrintInOrder();
        cout << endl;
    }

    return 0;
}

void PrintMenu()
{
    cout << "[***************COMMANDS***************]" << endl;
    cout << "Insert:                    + <value>" << endl;
    cout << "Remove:                    - <value>" << endl;
    cout << "Search:                    ? <value>" << endl;
    cout << "Test Print:                p" << endl;
    cout << "AtCursor:                  @" << endl;
    cout << "GoToBeginning:             b" << endl;
    cout << "GoToEnd:                   e" << endl;
    cout << "GoToNext:                  >" << endl;
    cout << "GoToPrev:                  <" << endl;
    cout << "Test BST Constructor:      1" << endl;
    cout << "Test BST Copy Constructor: 2" << endl;
    cout << "Test Destructor:           3" << endl;
    cout << "Test Assignment Operator:  4" << endl;
    cout << "Test ClearList:            5" << endl;
    cout << "Test EmptyList:            6" << endl;
    cout << "Print All:                 7" << endl;
    cout << "Print Menu:                m" << endl;
    cout << "Quit:                      q" << endl;
    cout << "***************************************" << endl;
}
string GetInput()
{
    string input = "";
    cout << "Enter your desired operation(s): ";
    getline(cin, input);
    // cout << input << endl; // debug
    return input;
};
// string operationIdentifiers = "+-?p@be><1234567mq";
bool ExecuteOption(string input, BinarySearchTree<int> &tree)
{
    bool continueExecuting = true;
    char operation;
    string value = "";

    // variables for input processing
    bool inOperation = false;
    const string operations = "+-?p@be><1234567mq"; // redundant, but kept it anyways.
    const string tenaryOperations = "+-?";
    const string unaryOperations = "p@be><mq";
    const string numericOperations = "1234567";
    const string numbers = "0123456789"; // partly redundant, but helpful.
    for (int i = 0; i < input.length(); i++)
    {
        if (operations.find(input[i]) != string::npos) // if input[i] is an operation symbol
        {
            if (inOperation) // if in operation (+, -, or ?)
            {
                if (numericOperations.find(input[i]) != string::npos) // if the operation found is numeric
                {
                    value += input[i];           // adds input to value
                    input = input.substr(i + 1); // removes from input
                    i--;                         // downsize i
                    // continues looping
                }
                else if ('-' == input[i]) // if a minus is found while in operation
                {
                    if (value.find(input[i]) != string::npos || value != "") // if its already in value or value isn't empty, break
                        break;
                    else if (value == "") // if its not in value and value is empty, add it to value
                    {
                        value += input[i];           // add to value
                        input = input.substr(i + 1); // remove from input
                        i--;                         // downsize i
                    }
                }
                else
                    break; // if not special case, then break;
            }
            else // if not in operation
            {
                operation = input[i];                                                                                     // set operation
                input = input.substr(i + 1);                                                                              // remove from input
                i--;                                                                                                      // downsize i
                if (unaryOperations.find(operation) != string::npos || numericOperations.find(operation) != string::npos) // if operation only requires itself, break
                    break;
                else // else, set in operation to true and continue.
                    inOperation = true;
            }
        }
        else if (numbers.find(input[i]) != string::npos) // if input[i] is numeric, add to value
        {
            value += input[i];           // add to value
            input = input.substr(i + 1); // remove from input
            i--;                         // decrement
        }
        else if (value == "") // if value is empty;
            break;
        else // if outlier, overlook it.
        {
            input = input.substr(i + 1);
            i--;
            continue;
        }
    }

    // +-?p@be><1234567mq
    if (operation == '+' && value != "")
    {
        tree.Insert(stoi(value));
    }
    else if (operation == '-' && value != "")
    {
        tree.Remove(stoi(value));
    }
    else if (operation == '?' && value != "")
    {
        cout << tree.Search(stoi(value)) << endl;
    }
    else if (operation == 'p')
    {
        tree.PrintInOrder();
        cout << endl;
    }
    else if (operation == '@')
    {
        cout << tree.AtCursor() << endl;
    }
    else if (operation == 'b')
    {
        tree.GoToBeginning();
    }
    else if (operation == 'e')
    {
        tree.GoToEnd();
    }
    else if (operation == '>')
    {
        tree.GoToNext();
    }
    else if (operation == '<')
    {
        tree.GoToPrev();
    }
    else if (operation == '1')
    {
        BinarySearchTree<int> test;
    }
    else if (operation == '2')
    {
        BinarySearchTree<int> test(tree);
        cout << "Original: ";
        tree.PrintInOrder();
        cout << endl;

        cout << "Copy: ";
        test.PrintInOrder();
        cout << endl;
    }
    else if (operation == '3')
    {
        continueExecuting = false;
    }
    else if (operation == '4')
    {
        BinarySearchTree<int> test;
        test = tree;
        cout << "Original: ";
        tree.PrintInOrder();
        cout << endl;

        cout << "Copy: ";
        test.PrintInOrder();
        cout << endl;
    }
    else if (operation == '5')
    {
        tree.ClearList();
    }
    else if (operation == '6')
    {
        cout << "Empty List? ";
        if (tree.EmptyList())
            cout << "True" << endl;
        else
            cout << "False" << endl;
    }
    else if (operation == '7')
    {
        cout << "In-order: ";
        tree.PrintInOrder();
        cout << endl;
        cout << "Pre-order: ";
        tree.PrintPreOrder();
        cout << endl;
        cout << "Post-order: ";
        tree.PrintPostOrder();
        cout << endl;
    }
    else if (operation == 'm')
    {
        PrintMenu();
    }
    else if (operation == 'q')
    {
        continueExecuting = false;
    }

    if (input != "" && continueExecuting)
        continueExecuting = ExecuteOption(input, tree);
    return continueExecuting;
}
