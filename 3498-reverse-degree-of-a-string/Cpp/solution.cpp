class Solution {
public:
    int reverseDegree(string s) {
        int ans = 0;
        int i = 1;
        for( auto it: s ){
            char ch = it;
            int index = ch - 96;
            int rev = 27 - index;
            ans = ans + ( i++ * rev );
        }
        return ans;
    }
};