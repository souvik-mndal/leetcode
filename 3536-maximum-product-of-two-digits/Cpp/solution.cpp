class Solution {
public:
    int maxProduct(int n) {
        int ans =1 ;
        vector<int>store;
        while ( n!= 0 ){
            store.push_back( n%10);
            n/=10;
        }
        int maxi = 0;
        for( int i=0 ; i<store.size() ; i++ ){
            for( int j=i+1 ; j<store.size() ; j++ ){
                maxi = max(maxi,store[i]*store[j]);
            }
        }
        return maxi;
    }
};