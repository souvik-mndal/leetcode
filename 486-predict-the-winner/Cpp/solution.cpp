class Solution {
public:
    bool predictTheWinner(vector<int>& nums) {
        int sum = 0;
        for (int num : nums)
            sum += num;

        int playerOneMax = getPlayer1Max(0, 0, nums.size() - 1, nums, true);

        if (sum % 2)
            return playerOneMax >= (sum + 1) / 2;
        return playerOneMax >= sum / 2;
    }

private:
    int getPlayer1Max(int player1, int start, int end,
                      vector<int>& nums, bool turn) {

        if (start > end)
            return player1;

        if (turn) {
            int left = getPlayer1Max(player1 + nums[start], start + 1, end, nums, false);
            int right = getPlayer1Max(player1 + nums[end], start, end - 1, nums, false);
            return max(left, right);
        } else {
            int left = getPlayer1Max(player1, start + 1, end, nums, true);
            int right = getPlayer1Max(player1, start, end - 1, nums, true);
            return min(left, right);
        }
    }
};