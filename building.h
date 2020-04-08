#ifndef COMPLETE_RANDOM_CITY_BUILDING_H
#define COMPLETE_RANDOM_CITY_BUILDING_H

#include "solid.h"
#include "recPrism.h"
#include "triPrism.h"
#include "frustum.h"
#include "cylinder.h"
#include <vector>
#include <memory>

enum typeOfBuilding {Plain, House, Skyscraper, Empire, UFO, CN, Hourglass, Pyramid};

class Building {
protected:
    std::vector<std::shared_ptr<Solid>> solids;

    // The rectangular base of the building's property
    Point2D topLeft;
    int sideLength;

    int height;
    RGBAcolor color;
    RGBAcolor edgeColor;

    typeOfBuilding buildingType;

public:
    Building();

    Building(Point2D inputTopLeft, int inputSideLength, int inputHeight,
             RGBAcolor inputColor, RGBAcolor inputEdgeColor, typeOfBuilding inputBuildingType);

    void initializeSolids();

    // Getters
    std::vector<std::shared_ptr<Solid>> getSolids() const;

    typeOfBuilding getBuildingType() const;

    void draw() const;

    // Check each solid for a collision with this point (or if the point is within
    // buffer of the solid). If it finds one, it returns that solid's corrected point,
    // and doesn't check any more. If none of the solids have a problem, it returns nullopt.
    std::experimental::optional<Point> correctCollision(Point p, int buffer);
};
#endif //COMPLETE_RANDOM_CITY_BUILDING_H
