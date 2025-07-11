#include "./includes.cpp"

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
        void inorderTraversalInner(TreeNode* node, vector<int>& results) {
            if(node == nullptr)
                return;

            if(node -> left != nullptr)
                inorderTraversalInner(node->left, results);
            results.push_back(node->val);
            if(node -> right != nullptr)
                inorderTraversalInner(node->right, results);
        } 

        vector<int> inorderTraversal(TreeNode* root) {

            if(root == nullptr)
                return {};

            vector<int> results;
            inorderTraversalInner(root, results);
            
            return results;
        }
};