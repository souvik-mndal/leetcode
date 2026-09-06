class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string,vector<string>>mp;
        for( auto &it:strs ){
            string temp = it;
            sort(temp.begin() , temp.end());
            mp[temp].push_back(it);
        }
        vector<vector<string>>str;
        for( auto &it:mp ){
            str.push_back(it.second);
        }
        return str;
        //returns
    }
};