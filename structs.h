#ifndef COMPLETE_RANDOM_CITY_STRUCTS_H
#define COMPLETE_RANDOM_CITY_STRUCTS_H


// int points used for the chunks
// in the xz plane
struct Point2D
{
    int x;
    int z;
    bool operator ==(const Point2D& p1)
    {
        return x == p1.x && z == p1.z;
    }
    bool operator !=(const Point2D& p1)
    {
        return x != p1.x || z != p1.z;
    }
};


struct Point
{
    double x;
    double y;
    double z;
};

// A line segment specified by two points
struct Segment
{
    Point p1;
    Point p2;
};


#endif //COMPLETE_RANDOM_CITY_STRUCTS_H
