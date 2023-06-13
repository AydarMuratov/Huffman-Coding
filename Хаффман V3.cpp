#include <iostream>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

class Node
{
public:
    int count;    
    char symbol;  
    Node* left;   
    Node* right;  

    Node(char __symbol, int __count)
    {
        symbol = __symbol;
        count = __count;
        left = nullptr;
        right = nullptr;
    }

    Node(Node* l, Node* r)
    {
        symbol = 0;
        count = l->count + r->count;
        left = l;
        right = r;
    }

    static void Print(Node* root, int depth = 0)
    {
        if (!root) return;

        if (root->symbol)
        {
            for (int i = 0; i < depth; i++)
                cout << ".";
            cout << root->symbol << endl;
        }
        else
        {
            depth++;
        }

        Print(root->left, depth);
        Print(root->right, depth);
    }
};

void BuildTable(Node* root, vector<bool>& code, map<char, vector<bool>>& table)
{
    if (root->left)
    {
        code.push_back(0);
        BuildTable(root->left, code, table);
        code.pop_back();
    }

    if (root->right)
    {
        code.push_back(1);
        BuildTable(root->right, code, table);
        code.pop_back();
    }

    if (root->symbol)
    {
        table[root->symbol] = code;
    }
}

string Decode(string& str, map<vector<bool>, char>& table)
{
    string out = "";
    vector<bool> code;
    for (int i = 0; i < str.length(); i++)
    {
        code.push_back(str[i] == '0' ? false : true);
        if (table[code])
        {
            out += table[code];
            code.clear();
        }
    }
    return out;
}

bool SortNode(const Node* a, const Node* b)
{
    return a->count < b->count;
}

int main()
{
    ifstream inputFile("input.txt", ios::binary);  
    ofstream outputFile("output.txt", ios::binary);  

    if (!inputFile)
    {
        cout << "Failed to open input file." << endl;
        return 0;
    }

    map<char, int> symbols;  

    while (!inputFile.eof())
    {
        char c = inputFile.get();
        symbols[c]++;
    }

    if (symbols.empty())
    {
        cout << "No input data." << endl;
        return 0;
    }

    vector<Node*> trees;
    for (auto itr = symbols.begin(); itr != symbols.end(); itr++)
    {
        Node* p = new Node(itr->first, itr->second);
        trees.push_back(p);
    }

    while (trees.size() > 1)
    {
        sort(trees.begin(), trees.end(), SortNode);

        Node* l = trees[0];
        Node* r = trees[1];

        Node* parent = new Node(l, r);
        trees.erase(trees.begin(), trees.begin() + 2);
        trees.push_back(parent);
    }

    Node* root = trees[0];

    vector<bool> code;
    map<char, vector<bool>> table;
    BuildTable(root, code, table);

    inputFile.clear();
    inputFile.seekg(0);

    int count = 0;
    char buf = 0;
    while (!inputFile.eof())
    {
        char c = inputFile.get();
        vector<bool> x = table[c];
        for (int n = 0; n < x.size(); n++)
        {
            buf = buf | x[n] << (7 - count);
            count++;
            if (count == 8)
            {
                count = 0;
                outputFile << buf;
                buf = 0;
            }
        }
    }

    inputFile.close();
    outputFile.close();

    ifstream encodedFile("output.txt", ios::binary);
    if (!encodedFile)
    {
        cout << "Failed to open encoded file." << endl;
        return 0;
    }

    Node* p = root;
    count = 0;
    char byte;
    byte = encodedFile.get();
    while (!encodedFile.eof())
    {
        bool b = byte & (1 << (7 - count));
        if (b)
        {
            p = p->right;
        }
        else
        {
            p = p->left;
        }

        if (p->left == nullptr && p->right == nullptr)
        {
            cout << p->symbol;
            p = root;
        }

        count++;
        if (count == 8)
        {
            count = 0;
            byte = encodedFile.get();
        }
    }

    encodedFile.close();

    cout << endl;

    for (auto itr = symbols.begin(); itr != symbols.end(); itr++)
    {
        cout << itr->first << " - ";
        for (int j = 0; j < table[itr->first].size(); j++)
        {
            cout << table[itr->first][j];
        }
        cout << endl;
    }

    string inputStr;
    cout << "Enter the text to encode: ";
    getline(cin, inputStr);

    string encodedStr = "";
    for (int i = 0; i < inputStr.length(); i++)
    {
        for (int j = 0; j < table[inputStr[i]].size(); j++)
        {
            encodedStr += table[inputStr[i]][j] ? '1' : '0';
        }
    }

    cout << "Encoded string: " << encodedStr << endl;

    map<vector<bool>, char> reverseTable;
    for (auto itr = table.begin(); itr != table.end(); itr++)
    {
        reverseTable[itr->second] = itr->first;
    }

    string decodedStr = Decode(encodedStr, reverseTable);
    cout << "Decoded string: " << decodedStr << endl;

    return 0;
}
