#include <bits/stdc++.h>

using namespace std;

class Node {
public:
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char ch, int freq) : ch(ch), freq(freq), left(nullptr), right(nullptr) {}
};

class CompareNodes {
public:
    bool operator()(const Node* a, const Node* b) {
        return a->freq > b->freq;
    }
};

// Function to generate Huffman codes by traversing the Huffman tree
void generateHuffmanCodes(Node* root, unordered_map<char, string>& code) {
    if (!root) {
        return;
    }

    stack<pair<Node*, string>> st;
    st.push({root, ""});

    while (!st.empty()) {
        Node* node = st.top().first;
        string codes = st.top().second;
        st.pop();

        if (node->ch != '$') {
            code[node->ch] = codes;
        }

        if (node->left) {
            st.push({node->left, codes + '0'});
        }
        if (node->right) {
            st.push({node->right, codes + '1'});
        }
    }
}

// Function to decompress the encoded data using the Huffman tree
string decompress(Node* root, string& res) {
    string ans = "";
    Node* node = root;

    for (int i = 0; i < res.size(); i++) {
        if (res[i] == '1') {
            node = node->right;
        } else {
            node = node->left;
        }

        if (node->ch != '$') {
            ans += node->ch;
            node = root;
        }
    }

    return ans;
}

int main() {
    string s;
    cout << "Enter the input string: "<<endl;
    cin >> s;

    // Creating a frequency table for the characters of the input string
    unordered_map<char, int> freqTable;

    for (char c : s) {
        freqTable[c]++;
    }

    // Creating a priority queue to simulate min-heap
    priority_queue<Node*, vector<Node*>, CompareNodes> pq;

    for (auto& it : freqTable) {
        pq.push(new Node(it.first, it.second));
    }

    while (pq.size() > 1) {
        Node* first = pq.top();
        pq.pop();
        Node* second = pq.top();
        pq.pop();
        int combinedFreq = first->freq + second->freq;
        Node* newNode = new Node('$', combinedFreq);
        newNode->left = first;
        newNode->right = second;
        pq.push(newNode);
    }

    // Traversing the Huffman tree and generating the codes
    unordered_map<char, string> code;
    Node* root = pq.top();
    pq.pop();
    generateHuffmanCodes(root, code);

    // Compressing the input data using the Huffman codes
    string encodedData = "";

    for (char c : s) {
        encodedData += code[c];
    }

    cout << "Encoded data: " << encodedData << endl;

    // Decompressing the encoded data using the Huffman tree
    string ans = decompress(root, encodedData);
    cout << "Decoded data: " << ans << endl;

    return 0;
}