#include "includes.cpp"

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if(lists.size() == 0)
            return nullptr;

        ListNode* result = nullptr;
        ListNode* root;
        while(true) {
            int min = NULL;
            int index = NULL;
            for(int i = 0; i < lists.size(); i++) {
                if(lists[i] == nullptr)
                    continue;
        
                if(lists[i]->val < min || min == NULL) {
                    min = lists[i]->val;
                    index = i;
                }
            }
            if(min == NULL)
                break;
            if(result == nullptr) {
                result = lists[index];
                root = result;
                lists[index] = lists[index]->next;
            } else {
                cout << lists[index]->val << endl;
                result->next = lists[index];
                result = result->next;
                lists[index] = lists[index]->next;
            }

        }
        return root;
    }
};