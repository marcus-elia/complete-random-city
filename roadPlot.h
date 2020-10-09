#ifndef COMPLETE_RANDOM_CITY_ROADPLOT_H
#define COMPLETE_RANDOM_CITY_ROADPLOT_H

#include "plot.h"
#include "streetLamp.h"
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

    // A streetlamp if conditions are met
    std::experimental::optional<StreetLamp> streetLamp;

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
    void initializeStreetLamp();

    // Getters
    bool getLeft() const;
    bool getRight() const;
    bool getUp() const;
    bool getDown() const;
    std::experimental::optional<RoadPlot*> getLeftRoad() const;
    std::experimental::optional<RoadPlot*> getRightRoad() const;
    std::experimental::optional<RoadPlot*> getUpRoad() const;
    std::experimental::optional<RoadPlot*> getDownRoad() const;
    int getNumRoads() const; // The number of directions that go into this plot

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

    // Where certain things are
    double getLaneWidth() const;
    double getWestEdge() const;
    double getNorthEdge() const;
    double getEastEdge() const;
    double getSouthEdge() const;
    double getWestIntersectionEdge() const;
    double getNorthIntersectionEdge() const;
    double getEastIntersectionEdge() const;
    double getSouthIntersectionEdge() const;

    // Get the physical points cars will go through on the perimeter of the intersection
    Point2D getUpExitPoint() const;
    Point2D getUpEntrancePoint() const;
    Point2D getRightExitPoint() const;
    Point2D getRightEntrancePoint() const;
    Point2D getDownExitPoint() const;
    Point2D getDownEntrancePoint() const;
    Point2D getLeftExitPoint() const;
    Point2D getLeftEntrancePoint() const;

    // Return a vector points for the car to drive on while turning through the intersection
    // For a car driving north (approaching from the south) and turning left
    std::vector<Point2D> getTurnPointsLeftNorth(double carSpeed) const;
    // Same, but turning right
    std::vector<Point2D> getTurnPointsRightNorth(double carSpeed) const;
    // Same but cul-de-sac
    std::vector<Point2D> getTurnPointsCircleNorth(double carSpeed) const;

    // Now the other directions
    std::vector<Point2D> getTurnPointsLeftEast(double carSpeed) const;
    std::vector<Point2D> getTurnPointsRightEast(double carSpeed) const;
    std::vector<Point2D> getTurnPointsCircleEast(double carSpeed) const;
    std::vector<Point2D> getTurnPointsLeftSouth(double carSpeed) const;
    std::vector<Point2D> getTurnPointsRightSouth(double carSpeed) const;
    std::vector<Point2D> getTurnPointsCircleSouth(double carSpeed) const;
    std::vector<Point2D> getTurnPointsLeftWest(double carSpeed) const;
    std::vector<Point2D> getTurnPointsRightWest(double carSpeed) const;
    std::vector<Point2D> getTurnPointsCircleWest(double carSpeed) const;

    void draw();
};

#endif //COMPLETE_RANDOM_CITY_ROADPLOT_H
