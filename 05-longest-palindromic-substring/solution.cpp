class Solution {
public:
    string check( string s , int st , int end ){
        string store;
        while( st>=0 && end<s.size() ){
            if( s[st] == s[end] ){
                store = s.substr(st,(end-st)+1);
            }
            else{
                break;
            }
            st--;
            end++;
        }
        return store;
    }
    string longestPalindrome(string s) {
        string ans = "";
        for( int i=0 ; i<s.size() ; i++ ){
            string one = check(s,i,i);
            string two = check(s,i,i+1);
            string temp;
            if( one.size() > two.size() ){
                temp = one;
            }
            else{
                temp = two;
            }
            if( temp.size() > ans.size() ){
                ans = temp;
            }
        }
        return ans;
    }
};