#include "button.h"

Button::Button()
{
    centerX = 128;
    centerY = 128;
    xWidth = 96;
    yWidth = 64;
    cornerRadius = 16;
    text = "Button";
    backgroundColor = {1,0,0,1};
    textColor = {1,1,1,1};
    highlightedColor = {1, 0, 0, 0.5};
    setLimits();
}
Button::Button(double inputCenterX, double inputCenterY, double inputXWidth, double inputYWidth, double inputCornerRadius,
               std::string inputText, RGBAcolor inputBackgroundColor, RGBAcolor inputTextColor, RGBAcolor inputHighlightedColor)
{
    centerX = inputCenterX, centerY = inputCenterY;
    xWidth = inputXWidth, yWidth = inputYWidth;
    cornerRadius = inputCornerRadius;
    text = inputText;
    backgroundColor = inputBackgroundColor;
    textColor = inputTextColor;
    highlightedColor = inputHighlightedColor;
    isHighlighted = false;
    setLimits();
}

void Button::setLimits()
{
    xMin = centerX - xWidth/2;
    xMax = centerX + xWidth/2;
    yMin = centerY - yWidth/2;
    yMax = centerY + yWidth/2;
}

bool Button::containsPoint(double mx, double my) const
{
    if(mx < xMin || mx > xMax || my < yMin || my > yMax)
    {
        return false;
    }
    // Left
    if(mx < xMin + cornerRadius)
    {
        // Upper Left
        if(my > yMax - cornerRadius)
        {
            return distanceFormula(mx, my, xMin + cornerRadius, yMax - cornerRadius) < cornerRadius;
        }
            // Lower left
        else if(my < yMin + cornerRadius)
        {
            return distanceFormula(mx, my, xMin + cornerRadius,  yMin + cornerRadius) < cornerRadius;
        }
        // Middle left
        return true;
    }
    // Right
    if(mx > xMax - cornerRadius)
    {
        // Upper Right
        if(my > yMax - cornerRadius)
        {
            return distanceFormula(mx, my, xMax - cornerRadius, yMax - cornerRadius) < cornerRadius;
        }
            // Lower Right
        else if(my < yMin + cornerRadius)
        {
            return distanceFormula(mx, my, xMax - cornerRadius,  yMin + cornerRadius) < cornerRadius;
        }
        // Middle Right
        return true;
    }
    return true;
}
bool Button::getIsHighlighted() const
{
    return isHighlighted;
}

void Button::setIsHighlighted(bool input)
{
    isHighlighted = input;
}

void Button::draw() const
{
    // Draw the text first
    glColor4f(textColor.r, textColor.g, textColor.b, textColor.a);
    glRasterPos2i(centerX - (4*text.length()), centerY - 4);
    for(const char &letter : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
    }

    glDisable(GL_CULL_FACE);

    // Set the color for the Button background
    if(isHighlighted)
    {
        glColor4f(highlightedColor.r, highlightedColor.g, highlightedColor.b, highlightedColor.a);
    }
    else
    {
        glColor4f(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    }

    double curTheta, curX, curY;
    double cx, cy; // the center of the quarter circle
    // Top Left Quarter Circle
    glBegin(GL_TRIANGLE_FAN);
    curTheta = PI/2;
    cx = xMin + cornerRadius;
    cy = yMax - cornerRadius;
    glVertex2f(cx, cy); // Circle center
    for(int i = 0; i < CURVE_SMOOTHNESS + 2; i++)
    {
        curX = cx + cornerRadius*cos(curTheta);
        curY = cy + cornerRadius*sin(curTheta);
        curTheta += PI/2/CURVE_SMOOTHNESS;
        glVertex2f(curX, curY);
    }
    glEnd();

    // Bottom Left Quarter Circle
    glBegin(GL_TRIANGLE_FAN);
    curTheta = PI;
    cx = xMin + cornerRadius;
    cy = yMin + cornerRadius;
    glVertex2f(cx, cy); // Circle center
    for(int i = 0; i < CURVE_SMOOTHNESS + 2; i++)
    {
        curX = cx + cornerRadius*cos(curTheta);
        curY = cy + cornerRadius*sin(curTheta);
        curTheta += PI/2/CURVE_SMOOTHNESS;
        glVertex2f(curX, curY);
    }
    glEnd();

    // Top Right Quarter Circle
    glBegin(GL_TRIANGLE_FAN);
    curTheta = 0;
    cx = xMax - cornerRadius;
    cy = yMax - cornerRadius;
    glVertex2f(cx, cy); // Circle center
    for(int i = 0; i < CURVE_SMOOTHNESS + 2; i++)
    {
        curX = cx + cornerRadius*cos(curTheta);
        curY = cy + cornerRadius*sin(curTheta);
        curTheta += PI/2/CURVE_SMOOTHNESS;
        glVertex2f(curX, curY);
    }
    glEnd();

    // Bottom Right Quarter Circle
    glBegin(GL_TRIANGLE_FAN);
    curTheta = 3*PI/2;
    cx = xMax - cornerRadius;
    cy = yMin + cornerRadius;
    glVertex2f(cx, cy); // Circle center
    for(int i = 0; i < CURVE_SMOOTHNESS + 2; i++)
    {
        curX = cx + cornerRadius*cos(curTheta);
        curY = cy + cornerRadius*sin(curTheta);
        curTheta += PI/2/CURVE_SMOOTHNESS;
        glVertex2f(curX, curY);
    }
    glEnd();

    // Fill in the middle with 3 rectangles
    glBegin(GL_QUADS);
    glVertex2f(xMin, yMin + cornerRadius);
    glVertex2f(xMin + cornerRadius, yMin + cornerRadius);
    glVertex2f(xMin + cornerRadius, yMax - cornerRadius);
    glVertex2f(xMin, yMax - cornerRadius);

    glVertex2f(xMin + cornerRadius, yMin);
    glVertex2f(xMax - cornerRadius, yMin);
    glVertex2f(xMax - cornerRadius, yMax);
    glVertex2f(xMin + cornerRadius, yMax);

    glVertex2f(xMax - cornerRadius, yMin + cornerRadius);
    glVertex2f(xMax, yMin + cornerRadius);
    glVertex2f(xMax, yMax - cornerRadius);
    glVertex2f(xMax - cornerRadius, yMax - cornerRadius);
    glEnd();

    glEnable(GL_CULL_FACE);
}
