class Solution {
public:
    vector<int> lexicographicallySmallestArray(vector<int>& nums, int limit) {
        vector<pair<int, int>> a;
        int n = nums.size();

        for (int i = 0; i < n; i++)
            a.push_back(make_pair(nums[i], i));

        sort(a.begin(), a.end());

        vector<int> tempVal, tempInd;
        tempVal.push_back(a[0].first);
        tempInd.push_back(a[0].second);

        for (int i = 1; i <= n; i++) {
            if (i == n || a[i].first - a[i - 1].first > limit) {
                sort(tempInd.begin(), tempInd.end());
                int m = tempInd.size();

                for (int j = 0; j < m; j++)
                    nums[tempInd[j]] = tempVal[j];
                
                tempVal.clear();
                tempInd.clear();
            }

            if (i < n) {
                tempVal.push_back(a[i].first);
                tempInd.push_back(a[i].second);
            }
        }

        return nums;
    }
};