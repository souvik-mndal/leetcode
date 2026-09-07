class Solution {
public:
    int maxBuilding(int n, vector<vector<int>>& restrictions) {
        restrictions.push_back({1,0});
        sort( restrictions.begin() , restrictions.end() );
        if( restrictions.back()[0] != n ){
            restrictions.push_back({n,n-1});
        }
        int m = restrictions.size();
        for( int i=1 ; i<m ; i++ ){
            int dist = abs(restrictions[i][0]-restrictions[i-1][0]);
            dist = min(dist+restrictions[i-1][1],restrictions[i][1]);
            restrictions[i][1] = dist;
        }
        for( int i=m-2 ; i>=0 ; i-- ){
            int dist = abs(restrictions[i][0]-restrictions[i+1][0]);
            dist = min(dist+restrictions[i+1][1],restrictions[i][1]);
            restrictions[i][1] = dist;
        }
        long long ht = 0;
        for( int i=1 ; i<m ; i++ ){
            int x1 = restrictions[i-1][0];
            int x2 = restrictions[i][0];
            int h1 = restrictions[i-1][1];
            int h2 = restrictions[i][1];
            long long d = abs( x1-x2 );
            long long peak = max(h1,h2) + (d - abs( h1 - h2 ))/2;
            ht = max( ht , peak );
        }
        return (int)ht;
    }
};