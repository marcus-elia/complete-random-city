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
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, gray,
                                                             sideLength, height, sideLength, gray)));
    }
    else if(buildingType == House)
    {
        Point center = {(double)topLeft.x + sideLength/2, (double)height/2, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, gray,
                                                             sideLength - 5, height, sideLength - 5, gray)));
        Point roofCenter = {center.x, center.y + 3*height/4, center.z};
        solids.push_back(std::make_shared<TriPrism>(TriPrism(roofCenter, gray,
                                                             sideLength, height/2, sideLength, gray)));
    }
    else if(buildingType == Skyscraper)
    {
        Point center = {(double)topLeft.x + sideLength/2, (double)3*height/4, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, gray,
                                                             sideLength, 3*height/2, sideLength, gray)));
    }
    else if(buildingType == Empire)
    {
        Point center = {(double)topLeft.x + sideLength/2, (double)height/2, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, gray,
                                                             sideLength, height, sideLength, gray)));
        center = {center.x, center.y + height/2.0 + height/12.0, center.z};
        solids.push_back(std::make_shared<Frustum>(Frustum(center, gray,
                                                           sideLength, height/6.0, sideLength, gray,
                                                           2*sideLength/3, 2*sideLength/3)));
        center = {center.x, center.y + height/12.0 + height/8.0, center.z};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, gray,
                                                             2*sideLength/3, height/4.0,
                                                             2*sideLength/3, gray)));
        center = {center.x, center.y + height/8.0 + height/12.0, center.z};
        solids.push_back(std::make_shared<Frustum>(Frustum(center, gray,
                                                           2*sideLength/3, height/6.0, 2*sideLength/3, gray,
                                                           sideLength/3, sideLength/3)));
        center = {center.x, center.y + height/12.0 + height/4.0, center.z};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, gray,
                                                             5, height/2.0, 5, gray)));
    }
    else if(buildingType == UFO)
    {
        Point center = {(double)topLeft.x + sideLength/2, (double)height/2, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, gray,
                                                             sideLength, height, sideLength, gray)));
        center = {center.x, center.y + height/2.0 + height/48.0, center.z};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, gray,
                                                             sideLength/2, height/24.0, sideLength/2, gray)));
        center = {center.x, center.y + height/48.0 + height/24.0, center.z};
        solids.push_back(std::make_shared<Cylinder>(Cylinder(center, gray,
                                                             sideLength, height/12.0, sideLength, gray)));
        center = {center.x, center.y + height/24.0 + height/48.0, center.z};
        solids.push_back(std::make_shared<Frustum>(Frustum(center, gray,
                                                           sideLength/3, height/24.0, sideLength/3, gray,
                                                           sideLength/4, sideLength/4)));
    }
    else if(buildingType == CN)
    {
        Point center = {(double)topLeft.x + sideLength/2, (double)3*height/4, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<Cylinder>(Cylinder(center, gray,
                                                             sideLength/2, 3*height/2, sideLength/2, gray)));
        center = {center.x, center.y + 3*height/4 + height/24, center.z};
        solids.push_back(std::make_shared<Cylinder>(Cylinder(center, gray,
                                                             sideLength, height/12, sideLength, gray)));
        center = {center.x, center.y + height/24 + height/2, center.z};
        solids.push_back(std::make_shared<Cylinder>(Cylinder(center, gray,
                                                             sideLength/4, height, sideLength/4, gray)));
    }
    else if(buildingType == Hourglass)
    {
        Point center = {(double)topLeft.x + sideLength/2, (double)height/3, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<Frustum>(Frustum(center, gray,
                                                           sideLength, 2*height/3, sideLength, gray,
                                                           3*sideLength/4, 3*sideLength/4)));
        center = {center.x, center.y + 2*height/3, center.z};
        solids.push_back(std::make_shared<Frustum>(Frustum(center, gray,
                                                           3*sideLength/4, 2*height/3, 3*sideLength/4, gray,
                                                           sideLength, sideLength)));
    }
    else if(buildingType == Pyramid)
    {
        Point center = {(double)topLeft.x + sideLength/2.0, (double)height/2, (double)topLeft.z + sideLength/2};
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, gray,
                                                             sideLength, height, sideLength, gray)));
        center = {center.x, center.y + height/2.0 + height/20.0, center.z};
        solids.push_back(std::make_shared<Frustum>(Frustum(center, gray,
                                                           sideLength, height/10.0, sideLength, gray,
                                                           sideLength/20.0, sideLength/20.0)));
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
