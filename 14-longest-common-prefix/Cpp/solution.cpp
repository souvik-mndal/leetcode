class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        string ans = "";
        int index = 0;
        if( strs.size() == 1 ){
            return strs[0];
        }
        while( true ){
            bool check = true;
            for( int i=1 ; i<strs.size() ; i++ ){
                if( index < strs[i].size() && strs[0][index] == strs[i][index] ){
                    check = 1;
                }
                else{
                    check = false;
                    break;
                }
            }
            if( check ){
                ans.push_back( strs[0][index++] );
            }
            else{
                break;
            }
        }
        return ans;
    }
};