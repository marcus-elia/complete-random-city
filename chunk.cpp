#include "chunk.h"

Chunk::Chunk()
{
    bottomLeft = {0,0};
    sideLength = 1024;
    perlinSeed = 0.5;
    initializeCenter();
    propertySize = sideLength / 8;
    airport = std::experimental::nullopt;
}
Chunk::Chunk(Point2D inputBottomLeft, int inputSideLength, double inputPerlinSeed,
             std::experimental::optional<std::vector<int>> inputLeftRoadIndices,
             std::experimental::optional<std::vector<int>> inputRightRoadIndices,
             std::experimental::optional<std::vector<int>> inputTopRoadIndices,
             std::experimental::optional<std::vector<int>> inputBottomRoadIndices)
{
    bottomLeft = inputBottomLeft;
    sideLength = inputSideLength;
    perlinSeed = inputPerlinSeed;
    initializeCenter();
    propertySize = sideLength / 8;
    airport = std::experimental::nullopt;

    // If the roads entering a side are specified, use them. If not, make them randomly
    srand(time(NULL));
    leftRoadIndices = inputLeftRoadIndices.value_or(makeRandomRoadIndices(perlinSeed, 8));
    rightRoadIndices = inputRightRoadIndices.value_or(makeRandomRoadIndices(perlinSeed, 8));
    topRoadIndices = inputTopRoadIndices.value_or(makeRandomRoadIndices(perlinSeed, 8));
    bottomRoadIndices = inputBottomRoadIndices.value_or(makeRandomRoadIndices(perlinSeed, 8));

    initializeRoadLocations();
    initializePlots();
}

void Chunk::initializeCenter()
{
    center = {sideLength*bottomLeft.x + sideLength/2, sideLength*bottomLeft.z - sideLength/2};
}
std::vector<int> Chunk::makeRandomRoadIndices(double seed, int size)
{
    std::vector<int> indices;
    bool roadAtPrev = (rand() % 100) > 50;
    for(int i = 0; i < size; i++)
    {
        // If there isn't a road adjacent to this one and a random value is less than
        // the perlin seed, put a road entering there 4/5 of the time. This means that
        // higher perlin seed means more roads.
        if(!roadAtPrev && (rand() % 100) > 20 && (rand() % 100)/100.0 < seed)
        {
            indices.push_back(i);
            roadAtPrev = true;
        }
        else
        {
            roadAtPrev = false;
        }
    }
    return indices;
}
void Chunk::initializeRoadLocations()
{
    // Initialize the whole chunk to not having roads
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            roadLocations[i][j] = false;
        }
    }

    // Add in roads on the edges
    for(int i : topRoadIndices)
    {
        roadLocations[i][0] = true;
    }
    for(int i : bottomRoadIndices)
    {
        roadLocations[i][7] = true;
    }
    for(int j : leftRoadIndices)
    {
        roadLocations[0][j] = true;
    }
    for(int j : rightRoadIndices)
    {
        roadLocations[7][j] = true;
    }

    // Now extend the roads as far as they go
    for(int i : topRoadIndices)
    {
        int j = 1;
        while(j < 8 && !makesSquare(i, j, roadLocations))
        {
            roadLocations[i][j] = true;
            j++;
        }
    }
    for(int j : rightRoadIndices)
    {
        int i = 6;
        while(i >= 0 && !makesSquare(i, j, roadLocations))
        {
            roadLocations[i][j] = true;
            i--;
        }
    }
    for(int i : bottomRoadIndices)
    {
        int j = 6;
        while(j >= 0 && !makesSquare(i, j, roadLocations))
        {
            roadLocations[i][j] = true;
            j--;
        }
    }
    for(int j : leftRoadIndices)
    {
        int i = 0;
        while(i < 8 && !makesSquare(i, j, roadLocations))
        {
            roadLocations[i][j] = true;
            i++;
        }
    }
}

void Chunk::tryToMakeAirport()
{
    bool airportMade = false;
    int a, b;
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            // The plots that this airport would cover
            int coords[20][2] = {{i+0,j+0},{i+1,j+0},
                                 {i+0,j+1},{i+1,j+1},{i+2,j+1},{i+3,j+1},
                                 {i+0,j+2},{i+1,j+2},{i+2,j+2},{i+3,j+2},
                                           {i+1,j+3},{i+2,j+3},{i+3,j+3},{i+4,j+3},
                                                     {i+2,j+4},{i+3,j+4},{i+4,j+4},{i+5,j+4},
                                                               {i+3,j+5},{i+4,j+5}};
            bool couldWork = true;
            for(auto coord : coords)
            {
                a = coord[0];
                b = coord[1];
                if(plots[a][b]->getPlotType() != Empty)
                {
                    couldWork = false;
                    break;
                }
            }
            if(!airportMade && couldWork)
            {
                Point airportTopLeft = {plots[i][j]->getCenter().x - propertySize/2.0, 0,
                                        plots[i][j]->getCenter().z - propertySize/2.0};
                airport = Airport(airportTopLeft, sideLength/8);
                airportMade = true;

                // Now turn all plots covered by the airport into MultiPlots
                for(auto coord : coords)
                {
                    a = coord[0];
                    b = coord[1];
                    plots[a][b] = std::make_shared<MultiPlot>(MultiPlot({a, b},
                                                                        chunkCoordinatesToCenter(a, b, sideLength, bottomLeft, propertySize), propertySize));
                }

            }
        }
    }
}

void Chunk::tryToMakeMultiplotBuildings()
{
    // Look for spots to put a MultiPlot
    bool churchMade = false;
    bool mansionMade = false;
    for(int i = 0; i < 7; i++)
    {
        for(int j = 0; j < 7; j++)
        {
            if(plots[i][j]->getPlotType() == Empty && plots[i+1][j]->getPlotType() == Empty &&
               plots[i+1][j+1]->getPlotType() == Empty && plots[i][j+1]->getPlotType() == Empty)
            {
                if(perlinSeed > 0.8 && !churchMade && rand() % 100 > 50)
                {
                    Point2D topLeftOfBuilding = {bottomLeft.x*sideLength + i*propertySize,
                                                 bottomLeft.z*sideLength + j*propertySize};
                    plots[i][j] = std::make_shared<BuildingPlot>(BuildingPlot({i,j},
                                                                              chunkCoordinatesToCenter(i, j, sideLength, bottomLeft, propertySize), propertySize,
                                                                              Building(topLeftOfBuilding, propertySize, 60,
                                                                                       {1, 1, 1, 1},
                                                                                       {1,1,1,1}, Church)));
                    churchMade = true;
                    plots[i+1][j] = std::make_shared<MultiPlot>(MultiPlot({i + 1, j},
                                                                          chunkCoordinatesToCenter(i+1, j, sideLength, bottomLeft, propertySize), propertySize));
                    plots[i+1][j+1] = std::make_shared<MultiPlot>(MultiPlot({i + 1, j + 1},
                                                                            chunkCoordinatesToCenter(i+1, j+1, sideLength, bottomLeft, propertySize), propertySize));
                    plots[i][j+1] = std::make_shared<MultiPlot>(MultiPlot({i, j + 1},
                                                                          chunkCoordinatesToCenter(i, j+1, sideLength, bottomLeft, propertySize), propertySize));
                }
                if(perlinSeed < 0.25 && !mansionMade && rand() % 100 > 50 && touchesRoad(i, j))
                {
                    Point2D topLeftOfBuilding = {bottomLeft.x*sideLength + i*propertySize,
                                                 bottomLeft.z*sideLength + j*propertySize};
                    plots[i][j] = std::make_shared<BuildingPlot>(BuildingPlot({i,j},
                                                                              chunkCoordinatesToCenter(i, j, sideLength, bottomLeft, propertySize), propertySize,
                                                                              Building(topLeftOfBuilding, propertySize, 60,
                                                                                       {1, 1, 1, 1}, {1,1,1,1}, Mansion)));
                    mansionMade = true;
                    plots[i+1][j] = std::make_shared<MultiPlot>(MultiPlot({i + 1, j},
                                                                          chunkCoordinatesToCenter(i+1, j, sideLength, bottomLeft, propertySize), propertySize));
                    plots[i+1][j+1] = std::make_shared<MultiPlot>(MultiPlot({i + 1, j + 1},
                                                                            chunkCoordinatesToCenter(i+1, j+1, sideLength, bottomLeft, propertySize), propertySize));
                    plots[i][j+1] = std::make_shared<MultiPlot>(MultiPlot({i, j + 1},
                                                                          chunkCoordinatesToCenter(i, j+1, sideLength, bottomLeft, propertySize), propertySize));
                }
            }
        }
    }

}





void Chunk::initializePlots()
{
    // First, iterate through the whole chunk, adding in roads where they are specified by
    // the roadLocations[][] array. Any non-roads are set as EmptyPlots to start.

    // ----------------------- Corners ------------------------------
    // Top left corner
    //plots.push_back(std::vector<std::shared_ptr<Plot>>()); // initialize column 0
    if(roadLocations[0][0])
    {
        //plots[0].push_back(std::make_shared<RoadPlot>(RoadPlot({0,0}, chunkCoordinatesToCenter(0, 0, bottomLeft, propertySize), propertySize,
        //        hasElement(leftRoadIndices, 0), roadLocations[1][0], hasElement(topRoadIndices,0), roadLocations[0][1])));
        plots[0][0] = std::make_shared<RoadPlot>(RoadPlot({0, 0}, chunkCoordinatesToCenter(0, 0, sideLength, bottomLeft, propertySize), propertySize,
                         hasElement(leftRoadIndices, 0), roadLocations[1][0], hasElement(topRoadIndices, 0),
                         roadLocations[0][1]));
    }
    else
    {
        plots[0][0] = std::make_shared<EmptyPlot>(EmptyPlot({0, 0},
                chunkCoordinatesToCenter(0, 0, sideLength, bottomLeft, propertySize), propertySize));
    }
    // Top right
    if(roadLocations[7][0])
    {
        plots[7][0] = std::make_shared<RoadPlot>(RoadPlot({7, 0},
                chunkCoordinatesToCenter(7, 0, sideLength, bottomLeft, propertySize), propertySize,
                roadLocations[6][0], hasElement(rightRoadIndices, 0),
                hasElement(topRoadIndices, 7), roadLocations[7][1]));
    }
    else
    {
        plots[7][0] = std::make_shared<EmptyPlot>(EmptyPlot({7, 0},
                       chunkCoordinatesToCenter(7, 0, sideLength, bottomLeft, propertySize), propertySize));
    }
    // Bottom right
    if(roadLocations[7][7])
    {
        plots[7][7] = std::make_shared<RoadPlot>(RoadPlot({7, 7},
                           chunkCoordinatesToCenter(7, 7, sideLength, bottomLeft, propertySize), propertySize,
                           roadLocations[6][7], hasElement(rightRoadIndices, 7),
                           roadLocations[7][6], hasElement(bottomRoadIndices, 7)));
    }
    else
    {
        plots[7][7] = std::make_shared<EmptyPlot>(EmptyPlot({7, 7},
                chunkCoordinatesToCenter(7, 7, sideLength, bottomLeft, propertySize), propertySize));
    }
    // Bottom left
    if(roadLocations[0][7])
    {
        plots[0][7] = std::make_shared<RoadPlot>(RoadPlot({0, 7},
                         chunkCoordinatesToCenter(0, 7, sideLength, bottomLeft, propertySize), propertySize,
                          hasElement(leftRoadIndices, 7),roadLocations[1][7],
                          roadLocations[0][6], hasElement(bottomRoadIndices, 0)));
    }
    else
    {
        plots[0][7] = std::make_shared<EmptyPlot>(EmptyPlot({0, 7},
                chunkCoordinatesToCenter(0, 7, sideLength, bottomLeft, propertySize), propertySize));
    }

    // ----------------------- Edges ---------------------------------
    // Top row
    for(int i = 1; i < 7; i++)
    {
        if(roadLocations[i][0])
        {
            plots[i][0] = std::make_shared<RoadPlot>(RoadPlot({i, 0},
                    chunkCoordinatesToCenter(i, 0, sideLength, bottomLeft, propertySize), propertySize,
                          roadLocations[i-1][0], roadLocations[i+1][0],
                          hasElement(topRoadIndices, i),roadLocations[i][1]));
        }
        else
        {
            plots[i][0] = std::make_shared<EmptyPlot>(EmptyPlot({i, 0},
                    chunkCoordinatesToCenter(i, 0, sideLength, bottomLeft, propertySize), propertySize));
        }
    }
    // Bottom row
    for(int i = 1; i < 7; i++)
    {
        if(roadLocations[i][7])
        {
            plots[i][7] = std::make_shared<RoadPlot>(RoadPlot({i, 7},
                               chunkCoordinatesToCenter(i, 7, sideLength, bottomLeft, propertySize), propertySize,
                               roadLocations[i-1][7], roadLocations[i+1][7],
                               roadLocations[i][6], hasElement(bottomRoadIndices, i)));
        }
        else
        {
            plots[i][7] = std::make_shared<EmptyPlot>(EmptyPlot({i, 7},
                    chunkCoordinatesToCenter(i, 7, sideLength, bottomLeft, propertySize), propertySize));
        }
    }
    // Left row
    for(int j = 1; j < 7; j++)
    {
        if(roadLocations[0][j])
        {
            plots[0][j] = std::make_shared<RoadPlot>(RoadPlot({0, j},
                                                          chunkCoordinatesToCenter(0, j, sideLength, bottomLeft, propertySize), propertySize,
                                                          hasElement(leftRoadIndices,j), roadLocations[1][j],
                                                          roadLocations[0][j-1], roadLocations[0][j+1]));
        }
        else
        {
            plots[0][j] = std::make_shared<EmptyPlot>(EmptyPlot({0, j},
                    chunkCoordinatesToCenter(0, j, sideLength, bottomLeft, propertySize), propertySize));
        }
    }
    // Right row
    for(int j = 1; j < 7; j++)
    {
        if(roadLocations[7][j])
        {
            plots[7][j] = std::make_shared<RoadPlot>(RoadPlot({7, j},
                                                          chunkCoordinatesToCenter(7, j, sideLength, bottomLeft, propertySize), propertySize,
                                                          roadLocations[6][j], hasElement(rightRoadIndices, j),
                                                          roadLocations[7][j-1], roadLocations[7][j+1]));
        }
        else
        {
            plots[7][j] = std::make_shared<EmptyPlot>(EmptyPlot({7, j},
                    chunkCoordinatesToCenter(7, j, sideLength, bottomLeft, propertySize), propertySize));
        }
    }

    // ----------------------------- Inside Elements -------------------------------------------
    for(int i = 1; i < 7; i++)
    {
        for(int j = 1; j < 7; j++)
        {
            if(roadLocations[i][j])
            {
                plots[i][j] = std::make_shared<RoadPlot>(RoadPlot({i, j},
                                     chunkCoordinatesToCenter(i, j, sideLength, bottomLeft, propertySize), propertySize,
                                     roadLocations[i-1][j], roadLocations[i+1][j],
                                      roadLocations[i][j-1], roadLocations[i][j+1]));
            }
            else
            {
                plots[i][j] = std::make_shared<EmptyPlot>(EmptyPlot({i, j},
                        chunkCoordinatesToCenter(i, j, sideLength, bottomLeft, propertySize), propertySize));
            }
        }
    }

    if(rand() % 100 < 25)
    {
        tryToMakeAirport();
    }
    if(hasAirport())
    {
        return;
    }

    tryToMakeMultiplotBuildings();


    // Now turn some empty plots into Buildings
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(plots[i][j]->getPlotType() == Empty)
            {
                double r1 = (double)(rand() % 100) / 100;
                double r2 = (double)(rand() % 100) / 100;

                // Make a building if r says yes, more likely when perlin noise is high
                if(r1 < perlinSeed && r2 > 0.2)
                {
                    Point2D topLeftOfBuilding = {bottomLeft.x*sideLength + i*propertySize,
                                                 bottomLeft.z*sideLength + j*propertySize};
                    // What type of building is it? How high is it?
                    typeOfBuilding buildingType;
                    int height;
                    if((perlinSeed < 0.25 || (perlinSeed < 0.5 && r1 < 0.6)) && touchesRoad(i, j))
                    {
                        if(r2 > 0.98)
                        {
                            buildingType = WaterTower;
                            height = 150;
                        }
                        else
                        {
                            if(rand() % 100 < 67)
                            {
                                buildingType = House;
                                height = propertySize/2;
                            }
                            else
                            {
                                buildingType = House2;
                                height = 2*propertySize/3;
                            }
                        }
                    }
                    else if(r1 > 0.85 && perlinSeed > 0.75)
                    {
                        buildingType = Skyscraper;
                        height =  (int)(perlinSeed*180 + r1*80 + r2*80);
                    }
                    else if(0.55 < r1 && r1 < 0.6 && perlinSeed > 0.4)
                    {
                        buildingType = Pyramid;
                        height =  (int)(perlinSeed*180 + r1*80 + r2*80);
                    }
                    else if(0.7 < r1 && r1 < 0.75 && perlinSeed > 0.65)
                    {
                        buildingType = Hourglass;
                        height =  (int)(perlinSeed*180 + r1*80 + r2*80);
                    }
                    else if(0.1 < r1  && r1 < 0.15 && perlinSeed > 0.65)
                    {
                        buildingType = Empire;
                        height =  (int)(perlinSeed*180 + r1*80 + r2*80);
                    }
                    else if(0.45 < r1 && r1 < 0.5 && perlinSeed > 0.65)
                    {
                        buildingType = UFO;
                        height =  (int)(perlinSeed*180 + r1*80 + r2*80);
                    }
                    else if(perlinSeed > 0.95 && r1 < 0.05 && r2 > 0.9)
                    {
                        buildingType = CN;
                        height =  (int)(perlinSeed*180 + r1*80 + r2*80);
                    }
                    else
                    {
                        buildingType = Plain;
                        height =  (int)(perlinSeed*180 + r1*80 + r2*80);
                    }
                    plots[i][j] = std::make_shared<BuildingPlot>(BuildingPlot({i,j},
                            chunkCoordinatesToCenter(i, j, sideLength, bottomLeft, propertySize), propertySize,
                            Building(topLeftOfBuilding, propertySize, height,
                                                 {r1, 0, perlinSeed, 1},
                                                 {1,1,1,1}, buildingType)));
                    //plots[i][j] = std::make_shared<BuildingPlot>(BuildingPlot({i,j},
                    //        chunkCoordinatesToCenter(i, j, sideLength, bottomLeft, propertySize), propertySize, perlinSeed));
                }
            }
        }
    }
}


void Chunk::setRoadPlotPointers(std::experimental::optional<std::shared_ptr<Chunk>> leftChunk,
                         std::experimental::optional<std::shared_ptr<Chunk>> rightChunk,
                         std::experimental::optional<std::shared_ptr<Chunk>> topChunk,
                         std::experimental::optional<std::shared_ptr<Chunk>> bottomChunk)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(getPlotAt(i,j)->getPlotType() == Road)
            {
                auto rpl = getRoadPlotAt(i, j);
                std::experimental::optional<RoadPlot*> up, down, left, right;
                // Check for a road above this one
                if(rpl->getUp())
                {
                    if(j == 0) // If we are in the top row, look at the chunk above
                    {
                        if(topChunk && topChunk.value()->getPlotAt(i,7)->getPlotType() == Road)
                        {
                            up = topChunk.value()->getRoadPlotAt(i,7);
                        }
                        else
                        {
                            up = std::experimental::nullopt;
                        }
                    }
                    else // If not in the top row, just look up one entry
                    {
                        if(plots[i][j-1]->getPlotType() == Road)
                        {
                            up = getRoadPlotAt(i, j-1);
                        }
                        else
                        {
                            up = std::experimental::nullopt;
                        }
                    }
                    // Now have the roads point to each other
                    if(up)
                    {
                        rpl->setUpRoad(up.value());
                        up.value()->setDownRoad(rpl);
                    }
                }
                // Check for a road below this one
                if(rpl->getDown())
                {
                    if(j == 7) // If we are in the bottom row, look at the chunk below
                    {
                        if(bottomChunk && bottomChunk.value()->getPlotAt(i,0)->getPlotType() == Road)
                        {
                            down = bottomChunk.value()->getRoadPlotAt(i,0);
                        }
                        else
                        {
                            down = std::experimental::nullopt;
                        }
                    }
                    else // If not in the bottom row, just look down one entry
                    {
                        if(plots[i][j+1]->getPlotType() == Road)
                        {
                            down = getRoadPlotAt(i, j+1);
                        }
                        else
                        {
                            down = std::experimental::nullopt;
                        }
                    }
                    // Now have the roads point to each other
                    if(down)
                    {
                        rpl->setDownRoad(down.value());
                        down.value()->setUpRoad(rpl);
                    }
                }
                // Check for a road left of this one
                if(rpl->getLeft())
                {
                    if(i == 0) // If we are in the left column, look at the chunk to the left
                    {
                        if(leftChunk && leftChunk.value()->getPlotAt(7,j)->getPlotType() == Road)
                        {
                            left = leftChunk.value()->getRoadPlotAt(7,j);
                        }
                        else
                        {
                            left = std::experimental::nullopt;
                        }
                    }
                    else // If not in the left column, just look left one entry
                    {
                        if(plots[i-1][j]->getPlotType() == Road)
                        {
                            left = getRoadPlotAt(i-1, j);
                        }
                        else
                        {
                            left = std::experimental::nullopt;
                        }
                    }
                    // Now have the roads point to each other
                    if(left)
                    {
                        rpl->setLeftRoad(left.value());
                        left.value()->setRightRoad(rpl);
                    }
                }
                // Check for a road right of this one
                if(rpl->getRight())
                {
                    if(i == 7) // If we are in the right column, look at the chunk to the righ
                    {
                        if(rightChunk && rightChunk.value()->getPlotAt(0,j)->getPlotType() == Road)
                        {
                            right = rightChunk.value()->getRoadPlotAt(0,j);
                        }
                        else
                        {
                            right = std::experimental::nullopt;
                        }
                    }
                    else // If not in the right column, just look right one entry
                    {
                        if(plots[i+1][j]->getPlotType() == Road)
                        {
                            right = getRoadPlotAt(i+1, j);
                        }
                        else
                        {
                            right = std::experimental::nullopt;
                        }
                    }
                    // Now have the roads point to each other
                    if(right)
                    {
                        rpl->setRightRoad(right.value());
                        right.value()->setLeftRoad(rpl);
                    }
                }
            }

        }
    }
}



// ==============================================
//
//                  Getters
//
// ==============================================
Point2D Chunk::getBottomLeft() const
{
    return bottomLeft;
}
int Chunk::getSideLength() const
{
    return sideLength;
}
Point2D Chunk::getCenter() const
{
    return center;
}
std::vector<int> Chunk::getLeftRoadIndices() const
{
    return leftRoadIndices;
}
std::vector<int> Chunk::getRightRoadIndices() const
{
    return rightRoadIndices;
}
std::vector<int> Chunk::getTopRoadIndices() const
{
    return topRoadIndices;
}
std::vector<int> Chunk::getBottomRoadIndices() const
{
    return bottomRoadIndices;
}
bool Chunk::hasAirport() const
{
    if(airport)
    {
        return true;
    }
    return false;
}
Plot* Chunk::getPlotAt(int i, int j)
{
    return plots[i][j].get();
}
RoadPlot* Chunk::getRoadPlotAt(int i, int j)
{
    return dynamic_cast<RoadPlot *>(plots[i][j].get());
}
std::experimental::optional<RoadPlot*> Chunk::getRandomRoadPlot()
{
    int numTrials = 30;
    for(int x = 0; x < numTrials; x++)
    {
        int i = rand() % 8;
        int j = rand() % 8;
        if(plots[i][j]->getPlotType() == Road)
        {
            return getRoadPlotAt(i, j);
        }
    }
    return std::experimental::nullopt;
}

// ==============================================
//
//                  Setters
//
// ==============================================
void Chunk::setBottomRoad(int i, int j, RoadPlot *road)
{
    Plot* pl = plots[i][j].get();
    dynamic_cast<RoadPlot*>(pl)->setDownRoad(road);
}
void Chunk::setTopRoad(int i, int j, RoadPlot *road)
{
    Plot* pl = plots[i][j].get();
    dynamic_cast<RoadPlot*>(pl)->setUpRoad(road);
}
void Chunk::setRightRoad(int i, int j, RoadPlot *road)
{
    Plot* pl = plots[i][j].get();
    dynamic_cast<RoadPlot*>(pl)->setRightRoad(road);
}
void Chunk::setLeftRoad(int i, int j, RoadPlot *road)
{
    Plot* pl = plots[i][j].get();
    dynamic_cast<RoadPlot*>(pl)->setLeftRoad(road);
}


void Chunk::makeAirportCreatePlane()
{
    if(hasAirport())
    {
        airport.value().createAirplane();
    }
}


void Chunk::tick()
{
    if(airport)
    {
        airport.value().tick();
    }
}




void Chunk::draw() const
{
    glBegin(GL_QUADS);
    /*if((bottomLeft.x + bottomLeft.z) % 2 == 0)
    {
        glColor4f(1, 1, 0.3, 1);
    }
    else
    {
        glColor4f(0, 1, 0.8, 1);
    }*/
    //glColor4f(perlinSeed, 0, 1, 1);
    if(perlinSeed < 0.25)
    {
        glColor4f(0, 0.7, 0.1, 1);
    }
    else
    {
        glColor4f(0.7, 0.7, 0.7, 1);
    }
    glVertex3f(sideLength*bottomLeft.x,-1, sideLength*bottomLeft.z);
    glVertex3f(sideLength*bottomLeft.x,-1, sideLength*bottomLeft.z + sideLength);
    glVertex3f(sideLength*bottomLeft.x + sideLength,-1, sideLength*bottomLeft.z + sideLength);
    glVertex3f(sideLength*bottomLeft.x + sideLength,-1, sideLength*bottomLeft.z);

    glEnd();

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            plots[i][j]->draw();
        }
    }

    if(airport)
    {
        airport.value().draw();
    }
}

int Chunk::chunkToInt() const
{
    return pointToInt({bottomLeft.x, bottomLeft.z});
}

std::vector<Point2D> Chunk::getChunksAround(int radius)
{
    return getChunksAroundPointByPoint(bottomLeft, radius);
}

int pointToInt(Point2D p)
{
    int a = p.x;
    int b = p.z;
    if(a > 0 && b <= a-1 && b > -a)
    {
        return 4*a*a - 3*a - b;
    }
    else if(b < 0 && a <= -b && a > b+1)
    {
        return 4*b*b + b - a;
    }
    else if(a < 0 && b > a-1 && b < -a)
    {
        return 4*a*a - a + b;
    }
    else if(b > -1 && a >= -b && a < b+1)
    {
        return 4*b*b + 3*b + a;
    }
    return (2*a-1)*(2*a-3); // idk why, but this works for (0,-1) and (-1,-2)
}

std::vector<int> getChunksAroundPoint(Point2D p, int radius)
{
    std::vector<int> result;

    // Start at the top of the diamond and work down from there
    for(int b = p.z + radius; b >= p.z - radius; b--)
    {
        int distanceFromZ = abs(b - p.z);
        for(int a = p.x - (radius - distanceFromZ); a <= p.x + (radius - distanceFromZ); a++)
        {
            result.push_back(pointToInt({a,b}));
        }
    }
    return result;
}
std::vector<Point2D> getChunksAroundPointByPoint(Point2D p, int radius)
{
    std::vector<Point2D> result;

    // Start at the top of the diamond and work down from there
    for(int b = p.z + radius; b >= p.z - radius; b--)
    {
        int distanceFromZ = abs(b - p.z);
        for(int a = p.x - (radius - distanceFromZ); a <= p.x + (radius - distanceFromZ); a++)
        {
            result.push_back({a,b});
        }
    }
    return result;
}


bool Chunk::touchesRoad(int i, int j) const
{
    // Look up
    if(j > 0)
    {
        if(roadLocations[i][j-1])
        {
            return true;
        }
    }
    // Left
    if(i > 0)
    {
        if(roadLocations[i-1][j])
        {
            return true;
        }
    }
    // Down
    if(j < 7)
    {
        if(roadLocations[i][j+1])
        {
            return true;
        }
    }
    if(i < 7)
    {
        if(roadLocations[i+1][j])
        {
            return true;
        }
    }
    return false;
}


bool makesSquareUpLeft(int i, int j, bool roadLocs[8][8])
{
    return roadLocs[i-1][j] && roadLocs[i-1][j-1] && roadLocs[i][j-1];
}
bool makesSquareUpRight(int i, int j, bool roadLocs[8][8])
{
    return roadLocs[i][j-1] && roadLocs[i+1][j-1] && roadLocs[i+1][j];
}
bool makesSquareDownRight(int i, int j, bool roadLocs[8][8])
{
    return roadLocs[i+1][j] && roadLocs[i+1][j+1] && roadLocs[i][j+1];
}
bool makesSquareDownLeft(int i, int j, bool roadLocs[8][8])
{
    return roadLocs[i][j+1] && roadLocs[i-1][j+1] && roadLocs[i-1][j];
}
bool makesSquare(int i, int j, bool roadLocs[8][8])
{
    if(i == 0 && j == 0)
    {
        return makesSquareDownRight(i, j, roadLocs);
    }
    if(i == 7 && j == 0)
    {
        return makesSquareDownLeft(i, j, roadLocs);
    }
    if(i == 7 && j == 7)
    {
        return makesSquareUpLeft(i, j, roadLocs);
    }
    if(i == 0 && j == 7)
    {
        return makesSquareUpRight(i, j, roadLocs);
    }
    if(j == 0)
    {
        return makesSquareDownLeft(i, j, roadLocs) || makesSquareDownRight(i, j, roadLocs);
    }
    if(i == 7)
    {
        return makesSquareUpLeft(i, j, roadLocs) || makesSquareDownLeft(i, j, roadLocs);
    }
    if(j == 7)
    {
        return makesSquareUpLeft(i, j, roadLocs) || makesSquareUpRight(i, j, roadLocs);
    }
    if(i == 0)
    {
        return makesSquareUpRight(i, j, roadLocs) || makesSquareDownRight(i, j, roadLocs);
    }
    return makesSquareUpRight(i, j, roadLocs) || makesSquareDownRight(i, j, roadLocs) ||
            makesSquareUpLeft(i, j, roadLocs) || makesSquareDownLeft(i, j, roadLocs);
}

// Check if the vector contains the given element
template <class Object>
bool hasElement(std::vector<Object> vec, Object obj)
{
    for(Object o : vec)
    {
        if(o == obj)
        {
            return true;
        }
    }
    return false;
}

Point2D chunkCoordinatesToCenter(int i, int j, int sideLength, Point2D bottomLeft, int propertySize)
{
    return {bottomLeft.x*sideLength + i*propertySize + propertySize/2, bottomLeft.z*sideLength + j*propertySize + propertySize/2};
}