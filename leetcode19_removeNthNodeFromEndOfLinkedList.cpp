#include "./includes.cpp"

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

/*
    cases

    [] - [], n = ??

    [1] - [], n = 1
    
    [1, 2], n = 1 => [1]

    [1, 2], n = 2 => [2]
*/

class Solution {
    public:
        ListNode *removeNthFromEnd(ListNode *head, int n) {
            if(head == nullptr)
                return nullptr;
            
            if(head->next == nullptr)
                return nullptr;
    
            ListNode* before_detaching = nullptr;
            ListNode* detaching = head;
            ListNode* fast = head;
            ListNode* after_detaching = head->next;
            int fast_steps_amount = 1;
    
            while(fast != nullptr) {
                if(fast_steps_amount > n) {
                    before_detaching = detaching;
                    detaching = detaching->next;
                    after_detaching = detaching->next;
                }   
                fast_steps_amount++; // [1..n - 1]
                fast = fast->next;
            }

            if(before_detaching != nullptr) {
                before_detaching->next = after_detaching;
            } else {
                head = after_detaching;
            }

            return head;
        }
    };