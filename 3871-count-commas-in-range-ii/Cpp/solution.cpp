// 1 , 000 , 000 , 000 , 000 , 000
// 100 , 000 , 000 , 000 , 000
// 10 , 000 , 000 , 000 , 000
// 1 , 000 , 000 , 000 , 000
// 100 , 000 , 000 , 000
// 10 , 000 , 000 , 000
// 1 , 000 , 000 , 000
// 100 , 000 , 000
// 10 , 000 , 000
// 1 , 000 , 000
// 100 , 000
// 10 , 000
// 1 , 000
class Solution {
public:
    long long countCommas(long long n) {
        long long ans = 0;
        long long thresh = 1000;
        while( thresh <= n ){
            ans = ans + n - thresh + 1 ;
            thresh *= 1000;
        }
        return ans;
    }
};