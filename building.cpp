#include "building.h"

Building::Building()
{
    topLeft = {0,0};
    sideLength = 32;

    height = 200;
    color = {0,0.4,1,1};
    edgeColor = {.9,.9,.9, 1};

    buildingType = Plain;

    initializeSolids();
}
Building::Building(Point2D inputTopLeft, int inputSideLength, int inputHeight,
                   RGBAcolor inputColor, RGBAcolor inputEdgeColor, typeOfBuilding inputBuildingType)
{
    topLeft = inputTopLeft;
    sideLength = inputSideLength;
    height = inputHeight;
    color = inputColor;
    edgeColor = inputEdgeColor;
    buildingType = inputBuildingType;

    initializeSolids();

}

void Building::initializeSolids()
{
    RGBAcolor gray = {0.5, 0.5, 0.5, 1};
    if(buildingType == Plain)
    {
        Point center = {(double)topLeft.x + sideLength/2, (double)height/2, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                             sideLength, height, sideLength, edgeColor)));
    }
    else if(buildingType == House)
    {
        Point center = {(double)topLeft.x + sideLength/2, (double)height/2, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, {.8, .8, .1, 1},
                                                             sideLength - 5, height, sideLength - 5,
                                                             edgeColor)));
        Point roofCenter = {center.x, center.y + 3*height/4, center.z};
        solids.push_back(std::make_shared<TriPrism>(TriPrism(roofCenter, {.3, .1, .1,1},
                                                             sideLength, height/2, sideLength, edgeColor)));
    }
    else if(buildingType == Skyscraper)
    {
        Point center = {(double)topLeft.x + sideLength/2, (double)3*height/4, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                             sideLength, 3*height/2, sideLength, edgeColor)));
    }
    else if(buildingType == Empire)
    {
        Point center = {(double)topLeft.x + sideLength/2, (double)height/2, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                             sideLength, height, sideLength, edgeColor)));
        center = {center.x, center.y + height/2.0 + height/12.0, center.z};
        solids.push_back(std::make_shared<Frustum>(Frustum(center, color,
                                                           sideLength, height/6.0, sideLength, edgeColor,
                                                           2*sideLength/3, 2*sideLength/3)));
        center = {center.x, center.y + height/12.0 + height/8.0, center.z};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                             2*sideLength/3, height/4.0,
                                                             2*sideLength/3, edgeColor)));
        center = {center.x, center.y + height/8.0 + height/12.0, center.z};
        solids.push_back(std::make_shared<Frustum>(Frustum(center, color,
                                                           2*sideLength/3, height/6.0,
                                                           2*sideLength/3, edgeColor,
                                                           sideLength/3, sideLength/3)));
        center = {center.x, center.y + height/12.0 + height/4.0, center.z};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                             5, height/2.0, 5, edgeColor)));
    }
    else if(buildingType == UFO)
    {
        Point center = {(double)topLeft.x + sideLength/2, (double)height/2, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                             sideLength, height, sideLength, edgeColor)));
        center = {center.x, center.y + height/2.0 + height/48.0, center.z};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                             sideLength/2, height/24.0,
                                                             sideLength/2, edgeColor)));
        center = {center.x, center.y + height/48.0 + height/24.0, center.z};
        solids.push_back(std::make_shared<Cylinder>(Cylinder(center, color,
                                                             sideLength, height/12.0, sideLength, edgeColor)));
        center = {center.x, center.y + height/24.0 + height/48.0, center.z};
        solids.push_back(std::make_shared<Frustum>(Frustum(center, color,
                                                           sideLength/3, height/24.0,
                                                           sideLength/3, edgeColor,
                                                           sideLength/4, sideLength/4)));
    }
    else if(buildingType == CN)
    {
        Point center = {(double)topLeft.x + sideLength/2, (double)3*height/4, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<Cylinder>(Cylinder(center, color,
                                                             sideLength/2, 3*height/2,
                                                             sideLength/2, edgeColor)));
        center = {center.x, center.y + 3*height/4 + height/24, center.z};
        solids.push_back(std::make_shared<Cylinder>(Cylinder(center, color,
                                                             sideLength, height/12, sideLength, edgeColor)));
        center = {center.x, center.y + height/24 + height/2, center.z};
        solids.push_back(std::make_shared<Cylinder>(Cylinder(center, color,
                                                             sideLength/4, height, sideLength/4, edgeColor)));
    }
    else if(buildingType == Hourglass)
    {
        Point center = {(double)topLeft.x + sideLength/2, (double)height/3, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<Frustum>(Frustum(center, color,
                                                           sideLength, 2*height/3, sideLength, edgeColor,
                                                           3*sideLength/4, 3*sideLength/4)));
        center = {center.x, center.y + 2*height/3, center.z};
        solids.push_back(std::make_shared<Frustum>(Frustum(center, color,
                                                           3*sideLength/4, 2*height/3,
                                                           3*sideLength/4, edgeColor,
                                                           sideLength, sideLength)));
    }
    else if(buildingType == Pyramid)
    {
        Point center = {(double)topLeft.x + sideLength/2.0, (double)height/2, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                             sideLength, height, sideLength, edgeColor)));
        center = {center.x, center.y + height/2.0 + height/20.0, center.z};
        solids.push_back(std::make_shared<Frustum>(Frustum(center, color,
                                                           sideLength, height/10.0, sideLength, edgeColor,
                                                           sideLength/20.0, sideLength/20.0)));
    }
    else if(buildingType == Church)
    {
        RGBAcolor brick = {176.0/256, 74.0/256, 58.0/256, 1};
        RGBAcolor roof = {114.0/256, 130.0/256, 124.0/256, 1};
        // Main rectangle
        Point center = {(double)topLeft.x + sideLength, 30.0, (double)topLeft.z + sideLength};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, brick,
                                                             2*sideLength, 60, sideLength, edgeColor)));
        // Wings
        center = {(double)topLeft.x + 4*sideLength/3.0, 30.0, (double)topLeft.z + sideLength/4.0};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, brick,
                                                             sideLength/2, 60, sideLength/2, edgeColor)));
        center = {center.x, center.y, (double)topLeft.z + 7*sideLength/4.0};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, brick,
                                                             sideLength/2, 60, sideLength/2, edgeColor)));
        // Main roof
        center = {(double)topLeft.x + sideLength, 75.0, (double)topLeft.z + sideLength};
        solids.push_back(std::make_shared<Frustum>(Frustum(center, roof,
                                                             sideLength, 30.0, sideLength/2.0, edgeColor,
                                                             2*sideLength, 2)));
        // Wing roof
        center = {(double)topLeft.x + 4*sideLength/3.0, center.y, center.x};
        solids.push_back(std::make_shared<Frustum>(Frustum(center, roof,
                                                           sideLength/4.0, 30.0, 2*sideLength, edgeColor,
                                                           2, 2*sideLength)));
    }
}

std::vector<std::shared_ptr<Solid>> Building::getSolids() const
{
    return solids;
}
typeOfBuilding Building::getBuildingType() const
{
    return buildingType;
}

void Building::draw() const
{
    for(std::shared_ptr<Solid> s : solids)
    {
        s->draw();
    }
}

std::experimental::optional<Point> Building::correctCollision(Point p, int buffer)
{
    for(std::shared_ptr<Solid> s : solids)
    {
        std::experimental::optional<Point> newPoint = s->correctCollision(p, buffer);
        if(newPoint)
        {
            return newPoint;
        }
    }
    return std::experimental::nullopt;
}
