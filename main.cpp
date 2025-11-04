//
// Created by Manju Muralidharan on 10/19/25.
//
#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include "heap.h"
using namespace std;

// Global arrays for node information
const int MAX_NODES = 64;
int weightArr[MAX_NODES];
int leftArr[MAX_NODES];
int rightArr[MAX_NODES];
char charArr[MAX_NODES];

// Function prototypes
void buildFrequencyTable(int freq[], const string& filename);
int createLeafNodes(int freq[]);
int buildEncodingTree(int nextFree);
void generateCodes(int root, string codes[]);
void encodeMessage(const string& filename, string codes[]);

int main() {
    int freq[26] = {0};

    // Step 1: Read file and count letter frequencies
    buildFrequencyTable(freq, "input.txt");

    // Step 2: Create leaf nodes for each character with nonzero frequency
    int nextFree = createLeafNodes(freq);

    // Step 3: Build encoding tree using your heap
    int root = buildEncodingTree(nextFree);

    // Step 4: Generate binary codes using an STL stack
    string codes[26];
    generateCodes(root, codes);

    // Step 5: Encode the message and print output
    encodeMessage("input.txt", codes);

    return 0;
}

/*------------------------------------------------------
    Function Definitions (Students will complete logic)
  ------------------------------------------------------*/

// Step 1: Read file and count frequencies
void buildFrequencyTable(int freq[], const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    char ch;
    while (file.get(ch)) {
        // Convert uppercase to lowercase
        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';

        // Count only lowercase letters
        if (ch >= 'a' && ch <= 'z')
            freq[ch - 'a']++;
    }
    file.close();

    cout << "Frequency table built successfully.\n";
}

// Step 2: Create leaf nodes for each character
int createLeafNodes(int freq[]) {
    int nextFree = 0;
    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0) {
            // Leaf node stores the character its weight and no children
            charArr[nextFree] = 'a' + i;
            weightArr[nextFree] = freq[i];
            leftArr[nextFree] = -1;
            rightArr[nextFree] = -1;
            nextFree++;
        }
    }
    cout << "Created " << nextFree << " leaf nodes.\n";
    return nextFree;
}

// Step 3: Build the encoding tree using heap operations
int buildEncodingTree(int nextFree) {
    //Creates a heap and push indices of all current leaves
    MinHeap heap;
    for (int i = 0; i < nextFree; ++i) {
        heap.push(i, weightArr);
    }
    if (heap.size == 1) {
        return heap.pop(weightArr);
    }
    //if more than one node remains, merge the two smallest subtrees.
    while (heap.size > 1) {
        int a = heap.pop(weightArr); // smallest
        int b = heap.pop(weightArr); // next smallest

        if (nextFree >= MAX_NODES) {
            cout << "Node capacity exceeded\n";
            exit(1);
        }

        // Creates a new internal parent node
        int parent = nextFree++;
        weightArr[parent] = weightArr[a] + weightArr[b];
        leftArr[parent]   = a;
        rightArr[parent]  = b;
        charArr[parent]   = '\0'; // internal marker

        heap.push(parent, weightArr);
    }
    int root = heap.pop(weightArr);
    return root;
}

// Step 4: Use an STL stack to generate codes
//Left edges append 0 and right edges append 1.
void generateCodes(int root, string codes[]) {
    stack<pair<int, string>> st;
    st.push({root, ""});

    while (!st.empty()) {
        auto [node, path] = st.top();
        st.pop();

        int L = leftArr[node];
        int R = rightArr[node];

        bool isLeaf = (L == -1 && R == -1);
        if (isLeaf) {
            char ch = charArr[node];
            if (ch >= 'a' && ch <= 'z') {
                // If path is empty fallback to 0
                codes[ch - 'a'] = path.empty() ? "0" : path;
            }
        } else {
            // Push right first so left is processed first
            if (R != -1) st.push({R, path + "1"});
            if (L != -1) st.push({L, path + "0"});
        }
    }
}

// Step 5: Print table and encoded message
void encodeMessage(const string& filename, string codes[]) {
    cout << "\nCharacter : Code\n";
    for (int i = 0; i < 26; ++i) {
        if (!codes[i].empty())
            cout << char('a' + i) << " : " << codes[i] << "\n";
    }

    cout << "\nEncoded message:\n";

    ifstream file(filename);
    char ch;
    while (file.get(ch)) {
        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';
        if (ch >= 'a' && ch <= 'z')
            cout << codes[ch - 'a'];
    }
    cout << "\n";
    file.close();
}