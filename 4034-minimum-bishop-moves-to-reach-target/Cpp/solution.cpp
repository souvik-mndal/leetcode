class Solution {
public:
    int minBishopMoves(vector<int>& source, vector<int>& target) {
        // Already at target
        if (source == target)
            return 0;

        // Bishop can only stay on the same color
        if ((source[0] + source[1]) % 2 !=
            (target[0] + target[1]) % 2)
            return -1;

        // Same diagonal
        if (source[0] + source[1] == target[0] + target[1] ||
            source[0] - source[1] == target[0] - target[1])
            return 1;

        // Same color, but not on the same diagonal
        return 2;
    }
};