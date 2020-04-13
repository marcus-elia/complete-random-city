#ifndef COMPLETE_RANDOM_CITY_ROADPLOT_H
#define COMPLETE_RANDOM_CITY_ROADPLOT_H

#include "plot.h"
#include <experimental/optional>
#include <memory>
#include <vector>
#include <cmath>

class RoadPlot : public Plot
{
private:
    // Whether this roadplot has a road in each direction
    bool left, right, up, down;

    bool culDeSac; // If the road dead ends, make a circle

    // The actual road in each direction, or nullopt
    std::experimental::optional<RoadPlot*> leftRoad, rightRoad, upRoad, downRoad;

    // For drawing
    std::vector<Point> roadCorners;
    std::vector<Segment> yellowLines;
    std::vector<Point> circlePoints;
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
    std::experimental::optional<RoadPlot*> getLeftRoad() const;
    std::experimental::optional<RoadPlot*> getRightRoad() const;
    std::experimental::optional<RoadPlot*> getUpRoad() const;
    std::experimental::optional<RoadPlot*> getDownRoad() const;

    // Setters
    void setLeft(bool inputLeft);
    void setRight(bool inputRight);
    void setUp(bool inputUp);
    void setDown(bool inputDown);
    void setLeftRoad(std::experimental::optional<RoadPlot*> inputLeftRoad);
    void setRightRoad(std::experimental::optional<RoadPlot*> inputRightRoad);
    void setUpRoad(std::experimental::optional<RoadPlot*> inputUpRoad);
    void setDownRoad(std::experimental::optional<RoadPlot*> inputDownRoad);

    // For cars
    // Returns a valid direction to exit this intersection from
    DrivingDirection getRandomDirection() const;
    // Returns a direction which connects to another roadplot from this one, other
    // than the input direction (which a car presumably came from). If there is no
    // other direction (and this is a cul de sac), then it does return input.
    DrivingDirection getRandomDirectionExcept(DrivingDirection input) const;

    // Where certain edges are
    double getWestEdge() const;
    double getNorthEdge() const;
    double getEastEdge() const;
    double getSouthEdge() const;
    double getWestIntersectionEdge() const;
    double getNorthIntersectionEdge() const;
    double getEastIntersectionEdge() const;
    double getSouthIntersectionEdge() const;

    void draw();
};

#endif //COMPLETE_RANDOM_CITY_ROADPLOT_H
