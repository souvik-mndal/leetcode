class Solution {
public:
    int myAtoi(string s) {
        bool sign = false;
        int i=0;
        int ans = 0;
        int n = s.size();
        while( s[i] == ' ' && i<n ){
            i++;
        }
        if( (s[i] == '-' || s[i] == '+') && i<n ){
            if( s[i] == '-' ){
                sign = true;
            }
            i++;
        }
        
        while( i<n && isdigit(s[i]) ){
            if( ans > INT_MAX/10 || (ans == INT_MAX/10 && s[i]>'7') ){
                return sign?INT_MIN :INT_MAX;
            }
            ans = ans * 10 + ( s[i] - '0' );
            i++;
        }
        if( sign ){
            return -ans;
        }
        return ans;

    }
};