#include "includes.cpp"

class Solution {
public:
    string simplifyPath(string path) {
        
        stringstream ss(path);
        string token;
        stack<string> sk;

        while(getline(ss, token, '/')) {
            if(token == "." || token.empty()) {
                continue;
            } 

            if(token == "..") {
                if(sk.empty())
                    continue;
                sk.pop();
                continue;
            }

            sk.push(token);
        }

        string result;
        while(!sk.empty()) {
            result = "/" + sk.top() + result;  
            sk.pop();
        }

        return result.empty() ? "/" : result;
    }
};