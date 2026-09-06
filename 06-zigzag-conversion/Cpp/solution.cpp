class Solution {
public:
    string convert(string s, int numRows) {
        vector<string>store(numRows);
        int i=0,n=s.size();
        while( i<n ){
            for( int j=0 ; j<numRows && i<n ; j++ ){
                store[j] += s[i++];
            }
            for( int k=numRows-2 ; k>0 && i<n ; k-- ){
                store[k] += s[i++];
            }
        }
        string ans;
        for( auto ii:store ){
                ans+=ii;
        }
        return ans;
    }
};