#include "./includes.cpp"

struct ListNode {
        int val;
        ListNode *next;
        ListNode() : val(0), next(nullptr) {}
        ListNode(int x) : val(x), next(nullptr) {}
        ListNode(int x, ListNode *next) : val(x), next(next) {}
    };

class Solution {
    public:
        ListNode* reverseList(ListNode* head) {
            if(head == nullptr)
                return head;
    
            if(head -> next == nullptr)
                return head;
    
            auto current = head;
            auto next = head -> next;
    
            while(next != nullptr) {
                auto second_next = next -> next;
                next -> next = current;
                current = next;
                next = second_next;
            }
            head -> next = nullptr;
            return current;
        }
    };