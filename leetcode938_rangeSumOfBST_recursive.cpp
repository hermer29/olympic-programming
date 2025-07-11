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
private:
    void rangeSumBST(TreeNode* node, const int low, const int high, int& sum) {
        if(node == nullptr)
            return;
        
        if(node->val >= low && node->val <= high)
            sum += node->val;

        rangeSumBST(node->left, low, high, sum);
        rangeSumBST(node->right, low, high, sum);
    }

public:
    int rangeSumBST(TreeNode* root, int low, int high) {
        int sum = 0;
        rangeSumBST(root, low, high, sum);
        return sum;
    }
};