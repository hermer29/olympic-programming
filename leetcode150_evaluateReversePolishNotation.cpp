#include "includes.cpp"

/*
    ["2","1","+","3","*"]

    *(3, +(2,1))
*/

class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        stack<int> sk;
        int a = 0;
        int b = 0;

        for(string c : tokens) {
            if(c == "+") {
                pop_args(sk, a, b);
                sk.push(a + b);
            } else if(c == "-") {
                pop_args(sk, a, b);
                sk.push(a - b);
            } else if(c == "/") {
                pop_args(sk, a, b);
                sk.push(a / b);
            } else if(c == "*") {
                pop_args(sk, a, b);
                sk.push(a * b);
            } else {
                sk.push(stoi(c));
            }
        }
        return sk.top();
    }
private:
    void pop_args(stack<int>& sk, int& a, int& b) {
        b = sk.top();
        sk.pop();
        a = sk.top();
        sk.pop();
    }
};