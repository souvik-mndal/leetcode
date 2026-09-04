class Solution {
public:
    int reverse(int x) {
        bool flag=0;
        if(x<= INT_MIN ){
            return 0;
        }
        if( x<0 ){
            x*=-1;
            flag=1;
        }
        int ans=0;
        while(x!=0){
            int ld=x%10;
            
            if( ans>(INT_MAX/10)){
                return 0;
            }
            ans=ans*10+ld;
            x/=10;
        }
        if( flag ){
            return -ans;
        }
        else{
            return ans;
        }
    }
};