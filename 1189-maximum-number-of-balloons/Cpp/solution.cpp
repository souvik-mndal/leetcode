class Solution {
public:
    int maxNumberOfBalloons(string text) {
        unordered_map<char,int>mp;
        char ch[] = {'b','a','l','o','n'};
        for( auto &it:text ){
            mp[it]++;
        }
        int count = 1e9;
        for( auto &it:ch ){
            if( mp.find(it) == mp.end() ){
                return 0;
            }
            else{
                if( it == 'l' || it == 'o' ){
                    count =  min( count , mp[it]/2 );
                }
                else{
                    count =  min( count , mp[it] );
                }
            }
        }
        return count;
    }
};