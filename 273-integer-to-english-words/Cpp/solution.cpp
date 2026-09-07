class Solution {
public:
    string solve( int n ){
        if( n == 0 ) return "Zero";
        vector<pair<int, string>> v = {
            {1000000000, "Billion"},
            {1000000,    "Million"},
            {1000,       "Thousand"},
            {100,        "Hundred"},
            {90,         "Ninety"},
            {80,         "Eighty"},
            {70,         "Seventy"},
            {60,         "Sixty"},
            {50,         "Fifty"},
            {40,         "Forty"},
            {30,         "Thirty"},
            {20,         "Twenty"},
            {19,         "Nineteen"},
            {18,         "Eighteen"},
            {17,         "Seventeen"},
            {16,         "Sixteen"},
            {15,         "Fifteen"},
            {14,         "Fourteen"},
            {13,         "Thirteen"},
            {12,         "Twelve"},
            {11,         "Eleven"},
            {10,         "Ten"},
            {9,          "Nine"},
            {8,          "Eight"},
            {7,          "Seven"},
            {6,          "Six"},
            {5,          "Five"},
            {4,          "Four"},
            {3,          "Three"},
            {2,          "Two"},
            {1,          "One"}
        };
        string ans = "";
        for( auto &it : v ){
            if( n >= it.first ){
                int temp = n / it.first;
                if( n>=100) ans += solve(temp);
                ans = ans + it.second + " ";
                n %= it.first;
                
            }
        }
        return ans;
    }
    string numberToWords(int num) {
        string ans = solve(num);
        if( ans.back() == ' ') ans.pop_back();
        return ans;
    }
};