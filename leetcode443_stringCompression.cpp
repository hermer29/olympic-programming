#include "./includes.cpp"

/*
    Problem link: https://leetcode.com/problems/string-compression/description/

    Pseudocode:
        foreach char
            is next char is not current or we're at end?
                yes: 
                    end counting, write result number
                no:
                    increment counter
    
    Thinking trials:
        ["a", "b", "c"]


                

*/

class Solution {
    public:
        int compress(vector<char>& chars) {
            int n = chars.size();
            int counter = 1;
            int write = 0;
            char current_char = chars[0];

            for(int i = 0; i < n; i++) {

                if(n - 1 == i || chars[i + 1] != current_char) {
                    chars[write++] = current_char;
                    if(counter > 1) {
                        write_number(chars, write, counter);
                    }
                    if(n - 1 != i) {
                        counter = 1;
                        current_char = chars[i + 1];
                    }   
                } else {
                    counter++;
                }
            }
            return write;
        }
    private:
        void write_number(vector<char>& chars, int& write, int number) {
            int amount = floor(log10(number)) + 1;
            int j = amount;
            while(j > 0) {
                int units = number % 10;
                chars[write + j-- - 1] = units + '0';
                number /= 10;
            }
            write += amount;
        }
    };