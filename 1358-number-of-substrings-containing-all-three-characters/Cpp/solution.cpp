class Solution {
public:
    int numberOfSubstrings(string s) {
        int count = 0;
            int a = 0;
            int b = 0;
            int c = 0;
            int st = 0 , e = 0;
            while( st < s.size()-2 ){
                if( a >= 1 && b >=1 && c >= 1 ){
                    count++;
                    count += (s.size() - e);
                    if( s[st] == 'a' ) a--;
                    else if( s[st] == 'b' ) b--;
                    else c--;
                    st++;
                }
                else if( e >= s.size() ) break;
                else if( s[e] == 'a' ){
                    a++;
                    e++;
                }
                else if( s[e] == 'b' ){
                    b++;
                    e++;
                }
                else if( s[e] == 'c' ){
                    c++;
                    e++;
                }
            }
        
        return count;
    }
};