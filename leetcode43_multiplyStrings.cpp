
#include "./includes.cpp"

/*

    Похоже на умножение в столбик
    Проходим по каждому числу, предположим из числа num1 (может быть и num2, порядок умножения роли не играет)

    Для каждого числа из num1 (идём с конца)
         
        Для каждого числа из num2 (идём с конца)
            умножаем на текущее число из num1
            полученное число превращаем в строку и записываем в список промежуточных результатов

*/

class Solution {
public:
    string multiply(string num1, string num2) {
        if (num1 == "0" || num2 == "0") return "0";
        int n1 = num1.size();
        int n2 = num2.size();
        
        vector<int> res(n1 + n2, 0);

        for(int i = n1 - 1; i >= 0; i--) {
            for(int j = n2 - 1; j >= 0; j--) {
                int product = (num1[i] - '0') * (num2[j] - '0');
                int sum = res[i + j + 1] + product;
                res[i + j + 1] = sum % 10;
                res[i + j] += sum / 10;
            }
        }
        
        int i = 0;
        while(i < res.size() && res[i] == 0) i++;
        
        string result;
        for(;i < res.size(); i++) {
            result += to_string(res[i]);
        }

        return result;
    }
};