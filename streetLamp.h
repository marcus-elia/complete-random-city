#ifndef COMPLETE_RANDOM_CITY_STREETLAMP_H
#define COMPLETE_RANDOM_CITY_STREETLAMP_H

#include "recPrism.h"
#include "cylinder.h"
#include "frustum.h"
#include <vector>
#include <memory>

class StreetLamp
{
private:
    Point location; // location.y is halfway up the pole
    double poleRadius;
    double poleHeight;
    double lightRadius;
    double lightHeight;
    double baseHeight;
    RGBAcolor poleColor;
    RGBAcolor lightColor;
    std::vector<std::shared_ptr<Solid>> solids;
public:
    StreetLamp();
    StreetLamp(Point inputLocation, double inputPoleRadius, double inputPoleHeight, double inputLightRadius,
             double inputLightHeight, RGBAcolor inputPoleColor, RGBAcolor inputLightColor);

    void initializeSolids();

    // Getters
    Point getLocation() const;
    Point getLightLocation() const;

    void draw() const;
};
#endif //COMPLETE_RANDOM_CITY_STREETLAMP_H
