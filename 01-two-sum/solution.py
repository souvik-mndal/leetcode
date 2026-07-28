class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        mp = {}

        for i in range(len(nums)):
            rem = target - nums[i]

            if rem in mp:
                return [i, mp[rem]]

            mp[nums[i]] = i

        return []