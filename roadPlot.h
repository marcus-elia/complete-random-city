#ifndef COMPLETE_RANDOM_CITY_ROADPLOT_H
#define COMPLETE_RANDOM_CITY_ROADPLOT_H

#include "plot.h"
#include "player.h"
#include <experimental/optional>
#include <memory>
#include <vector>

// A line segment specified by two points
struct Segment
{
    Point p1;
    Point p2;
};

class RoadPlot : public Plot
{
private:
    // Whether this roadplot has a road in each direction
    bool left, right, up, down;

    // The actual road in each direction, or nullopt
    std::experimental::optional<std::shared_ptr<RoadPlot>> leftRoad, rightRoad, upRoad, downRoad;

    // For drawing
    std::vector<Point> roadCorners;
    std::vector<Segment> yellowLines;
public:
    RoadPlot();
    RoadPlot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength,
            bool inputLeft, bool inputRight, bool inputUp, bool inputDown);

    void initializeRoadCorners();
    void initializeYellowLines();

    // Getters
    bool getLeft() const;
    bool getRight() const;
    bool getUp() const;
    bool getDown() const;
    std::experimental::optional<std::shared_ptr<RoadPlot>> getLeftRoad() const;
    std::experimental::optional<std::shared_ptr<RoadPlot>> getRightRoad() const;
    std::experimental::optional<std::shared_ptr<RoadPlot>> getUpRoad() const;
    std::experimental::optional<std::shared_ptr<RoadPlot>> getDownRoad() const;

    // Setters
    void setLeft(bool inputLeft);
    void setRight(bool inputRight);
    void setUp(bool inputUp);
    void setDown(bool inputDown);
    void setLeftRoad(std::experimental::optional<std::shared_ptr<RoadPlot>> inputLeftRoad);
    void setRightRoad(std::experimental::optional<std::shared_ptr<RoadPlot>> inputRightRoad);
    void setUpRoad(std::experimental::optional<std::shared_ptr<RoadPlot>> inputUpRoad);
    void setDownRoad(std::experimental::optional<std::shared_ptr<RoadPlot>> inputDownRoad);

    void draw();
};

#endif //COMPLETE_RANDOM_CITY_ROADPLOT_H
