#ifndef COMPLETE_RANDOM_CITY_PLOT_H
#define COMPLETE_RANDOM_CITY_PLOT_H

#include "graphics.h"
#include "structs.h"
#include "recPrism.h"


enum PlotType {Road, Buildings, Empty, Multi, Forest};


// A plot of land. Each chunk is divided into square plots.
class Plot
{
protected:
    // Where it is within the chunk
    Point2D topLeftChunkCoords;
    // The center in world coordinates
    Point2D center;
    int sideLength;
    PlotType plotType;
public:
    Plot();
    Plot(Point2D inputTopLeftChunkCoords, Point2D inputCenter, int inputSideLength);

    // Getters
    Point2D getTopLeftChunkCoords() const;
    Point2D getCenter() const;
    int getSideLength() const;
    PlotType getPlotType() const;

    // Setters
    void setTopLeftChunkCoords(Point2D inputTopLeftChunkCoords);
    void setCenter(Point2D inputCenter);
    void setSideLength(int inputSideLength);

    virtual void draw();
};


#endif //COMPLETE_RANDOM_CITY_PLOT_H
