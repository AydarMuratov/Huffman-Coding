#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

// Структура узла дерева Хаффмана
struct HuffmanNode {
    char character;
    int frequency;
    HuffmanNode* leftChild;
    HuffmanNode* rightChild;
    HuffmanNode(char ch, int freq, HuffmanNode* left = nullptr, HuffmanNode* right = nullptr) : character(ch), frequency(freq), leftChild(left), rightChild(right) {}
    ~HuffmanNode() {
        delete leftChild;
        delete rightChild;
    }
};

// Функция для сравнения узлов по частоте
struct CompareNodes {
    bool operator()(const HuffmanNode* a, const HuffmanNode* b) {
        return a->frequency > b->frequency;
    }
};

// Функция для построения дерева Хаффмана
HuffmanNode* buildHuffmanTree(unordered_map<char, int>& frequencyTable) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> minHeap;

    // Добавляем узлы в очередь
    for (auto& item : frequencyTable) {
        minHeap.push(new HuffmanNode(item.first, item.second));
    }

    // Строим дерево Хаффмана
    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top(); minHeap.pop();
        HuffmanNode* right = minHeap.top(); minHeap.pop();
        minHeap.push(new HuffmanNode('\0', left->frequency + right->frequency, left, right));
    }

    return minHeap.top();
}

// Функция для кодирования сообщения
void encode(string inputFileName, string outputFileName) {
    // Читаем файл в строку
    string inputText;
    ifstream inputFile(inputFileName);
    if (inputFile.is_open()) {
        char c;
        while (inputFile.get(c)) {
            inputText += c;
        }
        inputFile.close();
    } else {
        cout << "Unable to open input file" << endl;
        return;
    }

    // Считаем частоту символов
    unordered_map<char, int> frequencyTable;
    for (char c : inputText) {
        frequencyTable[c]++;
    }

    // Строим дерево Хаффмана
    HuffmanNode* root = buildHuffmanTree(frequencyTable);

    // Создаем таблицу кодов символов
    unordered_map<char, string> codeTable;
    function<void(HuffmanNode*, string)> buildCodeTable = [&](HuffmanNode* node, string code) {
        if (node) {
            if (node->character) {
                codeTable[node->character] = code;
            } else {
                buildCodeTable(node->leftChild, code + '0');
                buildCodeTable(node->rightChild, code + '1');
            }
        }
};
buildCodeTable(root, "");

// Записываем закодированный текст в выходной файл
ofstream outputFile(outputFileName);
if (outputFile.is_open()) {
    for (char c : inputText) {
        outputFile << codeTable[c];
    }
    outputFile.close();
} else {
    cout << "Unable to open output file" << endl;
}

// Очищаем память, выделенную под дерево
delete root;
}

// Функция для декодирования сообщения
void decode(string inputFileName, string outputFileName) {
// Читаем файл в строку
string encodedText;
ifstream inputFile(inputFileName);
if (inputFile.is_open()) {
char c;
while (inputFile.get(c)) {
encodedText += c;
}
inputFile.close();
} else {
cout << "Unable to open input file" << endl;
return;
}
// Считаем частоту символов
unordered_map<char, int> frequencyTable;
for (char c : encodedText) {
    frequencyTable[c]++;
}

// Строим дерево Хаффмана
HuffmanNode* root = buildHuffmanTree(frequencyTable);

// Декодируем текст
string decodedText;
HuffmanNode* currentNode = root;
for (char c : encodedText) {
    if (c == '0') {
        currentNode = currentNode->leftChild;
    } else {
        currentNode = currentNode->rightChild;
    }
    if (currentNode->character) {
        decodedText += currentNode->character;
        currentNode = root;
    }
}

// Записываем раскодированный текст в выходной файл
ofstream outputFile(outputFileName);
if (outputFile.is_open()) {
    outputFile << decodedText;
    outputFile.close();
} else {
    cout << "Unable to open output file" << endl;
}

// Очищаем память, выделенную под дерево
delete root;
}
int main() {
cout << "Choose mode (1 - encode, 2 - decode): ";
int mode;
cin >> mode;
cout << "Enter input file path: ";
string inputFileName;
cin >> inputFileName;

cout << "Enter output file path: ";
string outputFileName;
cin >> outputFileName;

if (mode == 1) {
    encode(inputFileName, outputFileName);
} else if (mode == 2) {
    decode(inputFileName, outputFileName);
} else {
    cout << "Invalid mode" << endl;
}

return 0;
}