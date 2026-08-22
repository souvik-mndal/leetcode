class Solution {
    public int[] twoSum(int[] nums, int target) {
        HashMap<Integer, Integer> map = new HashMap<>();

        for (int i = 0; i < nums.length; i++) {
            int remain = target - nums[i];

            if (map.containsKey(remain)) {
                return new int[] {i, map.get(remain)};
            }

            map.put(nums[i], i);
        }
        //return the value
        return new int[] {};
    }
}