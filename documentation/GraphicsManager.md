# Graphics Manager 
This class contains everything used to manipulate
graphics.

## void OpenWindow(int width, int height)
    Opens a graphics window with a width and height specified in the argument (measured in pixels).

## void CloseWindow()
    Closes the graphics window.

## void StayOpenBlocking()
    Keeps the window open by blocking the program until the user closes the window manually. Used for testing and keeping the window open.

## void PutPixel(int x, int y)
    Places a pixel at (x, y) on the canvas. Measured from (0, 0) in the top left corner.

## void ChangeBrushColor(int r, int g, int b, int a = 255)
    Changes the color that pixels are drawn with. Specify the RGB value of the color with integers between 0 and 255. The fourth parameter is alpha value, which defaults to 255 (fully opaque).

## void RefreshScreen()
    Updates the graphics window with any new pixels drawn (by default, drawing pixels does not update the screen).

## void DrawLine(int x1, int y1, int x2, int y2)
    Draws a line of pixels from (x1, y1) to (x2, y2), inclusive of both endpoints.