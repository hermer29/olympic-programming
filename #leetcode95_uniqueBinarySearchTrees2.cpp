
#include "./includes.cpp"
/**
 * Definition for a binary tree node.
 * 
 */

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
    public:
        vector<TreeNode*> generateTrees(int n) {
            vector<TreeNode*> results;
            vector<int> results2;
            for(int i = 1; i <= n; i++) {
                results.push_back(i);    
                
            }
        }
    };
