#ifndef COMPLETE_RANDOM_CITY_BUTTON_H
#define COMPLETE_RANDOM_CITY_BUTTON_H

// A button is a rounded rectangle with text

#include <string>
#include <cmath>
#include "structs.h"
#include "graphics.h"
#include "mathHelper.h"

class Button
{
private:
    double centerX, centerY;
    double xWidth, yWidth;
    double cornerRadius;
    std::string text;

    double xMin, xMax, yMin, yMax;

    RGBAcolor backgroundColor;
    RGBAcolor textColor;
    RGBAcolor highlightedColor;

    bool isHighlighted;


    double CURVE_SMOOTHNESS = 8;
public:
    Button();
    Button(double inputCenterX, double inputCenterY, double inputXWidth, double inputYWidth, double inputCornerRadius,
           std::string inputText, RGBAcolor inputBackgroundColor, RGBAcolor inputTextColor, RGBAcolor inputHighlightedColor);

    // Initialize  xMin, xMax, yMin, yMax based on dimensions
    void setLimits();

    bool containsPoint(double mx, double my) const;
    bool getIsHighlighted() const;

    void setIsHighlighted(bool input);

    void draw() const;
};

#endif //COMPLETE_RANDOM_CITY_BUTTON_H
