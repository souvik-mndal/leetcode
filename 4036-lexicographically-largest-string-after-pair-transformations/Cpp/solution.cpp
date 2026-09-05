class Solution {
public:
    vector<string> largestString(vector<int>& nums) {
        vector<int> calveroniq = nums;

        vector<string> ans;

        for (long long x : nums) {
            string s;

            // 'z' represents 2^25 a's.
            long long z = x / (1LL << 25);

            s.append(z, 'z');

            x %= (1LL << 25);

            // Represent the remaining value using
            // c,b,a... in descending order.
            for (int bit = 24; bit >= 0; --bit) {
                if (x & (1LL << bit)) {
                    s += char('a' + bit);
                }
            }

            ans.push_back(s);
        }

        return ans;
    }
};