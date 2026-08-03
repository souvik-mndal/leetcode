class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int i=0 , j=0;
        int n=nums1.size();
        int m=nums2.size();
        int cnt  = 0;
        int val1=0,val2=0;
        int t=n+m;
        int find1=0,find2=0;
        if( t%2 == 1 ){
            find2 = t/2;
            find1=-1;
        }
        else{
            find2 = t/2;
            find1 = t/2-1;
        }
        while( i<n && j<m ){
            if( nums1[i]<=nums2[j] ){
                if( cnt == find1 ){
                    val1 = nums1[i];
                }
                if( cnt == find2 ){
                    val2 = nums1[i];
                }
                cnt++;
                i++;
            }
            else{
                if( cnt == find1 ){
                    val1 = nums2[j];
                }
                if( cnt == find2 ){
                    val2 = nums2[j];
                }
                cnt++;
                j++;
            }
        }
        while( i<n  ){
            if( cnt == find1 ){
                    val1 = nums1[i];
                }
                if( cnt == find2 ){
                    val2 = nums1[i];
                }
                cnt++;
                i++;
            
        }
        while(  j<m ){
                if( cnt == find1 ){
                    val1 = nums2[j];
                }
                if( cnt == find2 ){
                    val2 = nums2[j];
                }
                cnt++;
                j++;
        }
            // return (double)(val1+val2)/2.0;
        if( find1 == -1 ){
            return (float)val2;
        }
        else{
            return (val1+val2)/2.0;        

        }
    }
};