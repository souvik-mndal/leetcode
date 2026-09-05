class Solution {
public:
    int minimumDeletions(vector<int>& nums) {
        int miniInd = 0 , maxInd = 0;
        int eleMin = nums[0] , eleMax = nums[0];
        for( int i=1 ; i<nums.size() ; i++ ){
            if( eleMin > nums[i] ){
                miniInd = i;
                eleMin = nums[i];
            }
            if( eleMax < nums[i] ){
                maxInd = i;
                eleMax = nums[i];
            }
        }
        int count = 0;
        int Lmin = 0, Rmin = 0 , Lmax = 0 , Rmax = 0;
        Lmin = miniInd+1;
        Rmin = nums.size() - miniInd;
        Lmax = maxInd+1;
        Rmax = nums.size() - maxInd;
        if( min(Lmin,Rmin) < min(Lmax,Rmax) ){
            count = count + min(Lmin,Rmin);
            if( Lmin<Rmin ){
                Lmax -= Lmin;
            }
            else{
                Rmax -= Rmin;
            }
            count = count + min(Rmax,Lmax);
        }
        else{
            count = count + min(Rmax,Lmax);
            if( Lmax<Rmax ){
                Lmin -= Lmax;
            }
            else{
                Rmin -= Rmax;
            }
            count = count + min(Lmin,Rmin);
        }
        return count;
    }
};