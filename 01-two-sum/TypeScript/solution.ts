function twoSum(nums: number[], target: number): number[] {
    const mp = new Map<number, number>();

    for (let i = 0; i < nums.length; i++) {
        const rem = target - nums[i];

        if (mp.has(rem)) {
            return [i, mp.get(rem)!];
        }

        mp.set(nums[i], i);
    }

    return [];
}