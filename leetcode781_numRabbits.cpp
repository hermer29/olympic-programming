#include "./includes.cpp"

class Solution {
    public:
        int numRabbits(std::vector<int>& answers) {
            std::unordered_map<int, int> colors;
            
            for(int i = 0; i < answers.size(); i++)
            {
                if(colors.find(answers[i]) == colors.end()) {
                    colors[answers[i]] = 1;
                } else {
                    colors[answers[i]] += 1;
                }
            }
            int sum = 0;
            for(auto color : colors)
            {
                int k = (int)ceil(((float)color.second) / (color.first + 1));
                sum += ((color.first + 1) * k);
            }
            return sum;
        }
    };