class Solution {
public:
    bool checkDivisibility(int n) {
        int sum = 0 , prod = 1 , m=n;
        while( m!=0 ){
            sum += (m%10);
            prod *= (m%10);
            m/=10;
        }
        int total = sum+prod;
        if( n%total == 0 ){
            return true;
        }
        return false;
    }
};