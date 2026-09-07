class Solution {
public:
    string change( string s ){
        char ch = 'a';
        unordered_map<char,char>mp;
        for( auto it:s ){
            if( mp.find(it) == mp.end()){
            mp[it] = ch++;

            }
        }
        for( int i=0 ; i<s.size() ; i++ ){
            s[i] = mp[s[i]];
        }
        return s;
    }
    vector<string> findAndReplacePattern(vector<string>& words, string pattern) {
        
        pattern = change(pattern);
        vector<string>chk;
        for( auto &it:words ){
            string temp = change(it);
            chk.push_back(temp);
        }
        vector<string>ans;
        for( int i=0 ; i<chk.size() ;i++ ){
            if( chk[i].compare(pattern) == 0 ){
                ans.push_back(words[i]);
            }
        }
        return ans;
    }
};