class Solution {
public:
    int product( int n ){
        int total = 1;
        while( n!= 0 ){
            total = total * (n%10 );
            n/=10;
        }
        return total;
    }
    int smallestNumber(int n, int t) {
        while( true ){
            int prod = product( n );
            if( prod % t  == 0 ){
                return n;
            }
            else{
                n++;
            }
        }
    }
};