#ifndef RAYTRACINGFROMSCRATCH_INTERVAL_H
#define RAYTRACINGFROMSCRATCH_INTERVAL_H

class interval
{
public:
    double min;
    double max;

    interval() : min(+infinity), max(-infinity) {}
    interval(double min, double max) : min(min), max(max) {}

    double size() const
    {
        return max-min;
    }

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds (double x) const {
        return min < x && x < max;
    }

    static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);


#endif //RAYTRACINGFROMSCRATCH_INTERVAL_H
