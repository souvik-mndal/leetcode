![Runtime](https://img.shields.io/badge/Runtime-0%20ms%20(beats%20100.00%25)-brightgreen?style=for-the-badge)
![Memory](https://img.shields.io/badge/Memory-8.2%20MB%20(beats%2044.59%25)-yellow?style=for-the-badge)

---

## Problem Explained

Imagine looking at a standard analog clock with an hour hand and a minute hand. As time ticks forward, both hands sweep around the circular clock face at different speeds. 

This problem asks us to calculate the **smaller angle** between the two hands at any given hour and minute. A clock face has a total of 360 degrees. Because two hands always form two angles (one going clockwise, the other going the other way around), we want to find the smaller of those two values. 

For example, if the hour is 12 and the minutes are 30, the minute hand points straight down at the 6, while the hour hand has moved halfway between the 12 and the 1$. The resulting smaller angle between them is 165 degrees.

## Intuition

The core idea is to figure out the exact position of both the hour hand and the minute hand in degrees, measured from the very top (the 12 o'clock position, which is 0 degrees). 

A clock is a circle of 360 degrees split into 12 hour sections, meaning each hour mark covers 30 degrees (360 / 12 = 30). The minute hand moves much faster, covering a full 360 degrees every 60 minutes, which means each minute moves it by 6 degrees (360 / 60 = 6). 

The trickier part is that the hour hand does not just jump from number to number. It creeps forward gradually as the minutes tick by. If we calculate the exact degree position of both hands, we can subtract one from the other to find the distance between them. If that distance turns out to be bigger than 180 degrees, we just subtract it from 360 to get the smaller, opposite side of the circle.

## Approach

Here is how the code implements this step by step:

*   `double hr = 30.0 / ( 60.0 / minutes );` : This calculates the extra drift the hour hand gets from the passing minutes. Dividing 60.0 by minutes and dividing 30.0 by that result gives the fraction of an hour section the hour hand has crawled through.
*   `double h_a = (hour%12) * 30;` : This converts the hour into a base angle in degrees. Using `hour%12` correctly treats 12 o'clock as 0 degrees, and multiplying by 30 gives 30 degrees per hour.
*   `h_a += hr;` : This adds the minute-by-minute drift we calculated earlier to the hour hand's base angle, giving its final, precise position.
*   `double m_a = minutes * 6;` : This calculates the exact position of the minute hand in degrees by multiplying the minutes by 6 degrees per minute.
*   `double ans = abs( h_a - m_a);` : This finds the absolute (positive) difference between the hour hand position and the minute hand position.
*   `return min(ans, 360-ans);` : This compares the direct difference against its counterpart on the other side of the clock (360 minus ans) and returns the smaller of the two values.

## Dry Run

### Case 1: Typical case (hour = 12, minutes = 30)

| hr | h_a | m_a | ans | Action |
| :--- | :--- | :--- | :--- | :--- |
| 15.0 | 0.0 | 0.0 | 0.0 | Initial setup of variables |
| 15.0 | 15.0 | 0.0 | 0.0 | `hr` calculates extra hour hand drift (30 / (60/30) = 15) |
| 15.0 | 15.0 | 0.0 | 0.0 | `h_a` computes base hour angle ((12%12)*30 = 0) |
| 15.0 | 15.0 | 0.0 | 0.0 | `h_a` adds drift (`h_a += hr`, so `0 + 15 = 15`) |
| 15.0 | 15.0 | 180.0 | 0.0 | `m_a` computes minute hand angle (`30 * 6 = 180`) |
| 15.0 | 15.0 | 180.0 | 165.0 | `ans` finds absolute difference (`abs(15 - 180) = 165`) |
| 15.0 | 15.0 | 180.0 | 165.0 | Returns `min(165, 360 - 165)` which is `165` |

### Case 2: Edge case with equal hour and minute markers (hour = 3, minutes = 30)

| hr | h_a | m_a | ans | Action |
| :--- | :--- | :--- | :--- | :--- |
| 15.0 | 0.0 | 0.0 | 0.0 | Initial setup of variables |
| 15.0 | 15.0 | 0.0 | 0.0 | `hr` calculates extra hour hand drift (`30 / (60/30) = 15`) |
| 15.0 | 90.0 | 0.0 | 0.0 | `h_a` computes base hour angle (`(3%12)*30 = 90`) |
| 15.0 | 105.0 | 0.0 | 0.0 | `h_a` adds drift (`h_a += hr`, so `90 + 15 = 105`) |
| 15.0 | 105.0 | 180.0 | 0.0 | `m_a` computes minute hand angle (`30 * 6 = 180`) |
| 15.0 | 105.0 | 180.0 | 75.0 | `ans` finds absolute difference (`abs(105 - 180) = 75`) |
| 15.0 | 105.0 | 180.0 | 75.0 | Returns `min(75, 360 - 75)` which is `75` |

## Time & Space Complexity

**Time:** O(1) — the code performs a fixed number of basic arithmetic operations regardless of the input values. 
**Space:** O(1) — it only stores a few double-precision variables (`hr`, `h_a`, `m_a`, `ans`), using a constant amount of memory.

**Is this already the most optimal possible complexity, or can it be improved?**

This is already optimal. 

- **Can it be improved?** No. Because the solution runs in constant time O(1) and constant space O(1), it cannot be made faster or lighter. It uses a direct mathematical formula instead of loops or data structures.
- **Theoretical best possible complexity:** O(1) time and O(1) space, which this solution already achieves.

## Edge Cases Handled

*   **Hour 12 wrapping around:** The code uses `hour%12` so that an input hour of 12 correctly evaluates to 0 degrees instead of 360 degrees.
*   **Zero minutes:** When minutes equal 0, `hr` evaluates to 0 and minute hand position `m_a` evaluates to 0, correctly measuring exact hour marks.
*   **Maximum boundary values:** Handles upper constraints like hour = 12 and minutes = 59 smoothly without arithmetic overflow.
