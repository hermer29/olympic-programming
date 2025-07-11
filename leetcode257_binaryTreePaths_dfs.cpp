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
        vector<string> binaryTreePaths(TreeNode* root) {
            if(root == nullptr)
                return {};

            vector<string> results;
            dfs(root, "", results);
            return results;
        }

    private:
        void dfs(TreeNode* current, string currentPath, vector<string>& results) {
            if(currentPath != "")
                currentPath += "->";

            currentPath += to_string(current->val);
            
            if(current->left == nullptr && current->right == nullptr) {
                results.push_back(currentPath);
                return;
            }

            if(current->left != nullptr) {
                dfs(current->left, currentPath, results);
            } 
            
            if(current->right != nullptr) {
                dfs(current->right, currentPath, results);
            }
        }
    };