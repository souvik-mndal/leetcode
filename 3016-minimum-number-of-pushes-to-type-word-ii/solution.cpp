class Solution {
public:
    int minimumPushes(string word) {
        vector<int>store(26);
        for( int i=0 ; i<word.size() ; i++ ){
            store[word[i]-'a']++;
        }
        sort(store.begin() , store.end());
        int j=0;
        int ans=0;
        for( int i=25 ; i>=0 ; i-- ){
            if( store[i] == 0 ) continue;
            ans += store[i]*( (j/8) + 1 );
            j++;
        }
        return ans;
    }
};