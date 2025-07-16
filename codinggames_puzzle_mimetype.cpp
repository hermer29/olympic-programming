#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cctype>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

struct TreeNode {
    unordered_map<char, TreeNode*> derived = unordered_map<char, TreeNode*>();
    string result = "";
};

int main()
{
    TreeNode nodeOuter;
    TreeNode* root = &nodeOuter;
    vector<TreeNode*> nodes;
    int n; // Number of elements which make up the association table.
    cin >> n; cin.ignore();
    int q; // Number Q of file names to be analyzed.
    cin >> q; cin.ignore();
    for (int i = 0; i < n; i++) {
        string ext; // file extension
        string mt; // MIME type.
        cin >> ext >> mt; cin.ignore();
        TreeNode* curr = root;
        for(int c = ext.size() - 1; c >= 0; c--) {
            char fname_char = std::tolower(ext[c]);
            TreeNode* next = new TreeNode();
            nodes.push_back(next);
            curr->derived[fname_char] = next;
            curr = next;
        }
        curr->result = mt;
    }
    for (int i = 0; i < q; i++) {
        string fname;
        getline(cin, fname); // One file name per line.
        int n = fname.size();
        TreeNode* curr = root;
        for(int c = n - 1;c >= 0; c--) {
            auto lowerVariant = std::tolower(fname[c]);
            if(fname[c] == '.') {
                if(curr->result == "") {
                    break;
                }
                cout << curr->result << endl;
                goto Exit;
            }
            if(curr->derived.find(lowerVariant) == curr->derived.end()) {
                break;
            }
            curr = curr->derived[lowerVariant];
        }
        cout << "UNKNOWN" << endl;
        Exit:;
    }
    for(TreeNode* node: nodes) {
        delete node;
    }

    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;


    // For each of the Q filenames, display on a line the corresponding MIME type. If there is no corresponding type, then display UNKNOWN.
}