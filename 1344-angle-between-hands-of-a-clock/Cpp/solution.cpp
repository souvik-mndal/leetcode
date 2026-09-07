class Solution {
public:
    double angleClock(int hour, int minutes) {
        double hr = 30.0 / ( 60.0 / minutes );
        double h_a = (hour%12) * 30;
        h_a += hr;
        double m_a = minutes * 6;
        double ans = abs( h_a - m_a);
        return min(ans, 360-ans);
    }
};