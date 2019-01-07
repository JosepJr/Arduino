/*
#define PIN_RESET 9  // Connect RST to pin 9 (req. for SPI and I2C)
#define PIN_DC    8  // Connect DC to pin 8 (required for SPI)
#define PIN_CS    10 // Connect CS to pin 10 (required for SPI)
//#define DC_JUMPER 0 // (I2C only) Set to either 0 or 1, matching the value of the DC Jumper

// Declare a MicroOLED object. The parameters include:
// 1 - Reset pin: Any digital pin
// 2 - D/C pin: Any digital pin (SPI mode only)
// 3 - CS pin: Any digital pin (SPI mode only, 10 recommended)
MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS); // Example SPI Declaration
//MicroOLED oled(PIN_RESET, DC_JUMPER); // Example I2C Declaration


// Draw a pixel in the middle of the screen
Let’s begin by drawing the simplest shape out there – a pixel. Drawing anything requires at least two steps. First you have to tell the screen what you want to draw, then you have to tell it to draw it.

To draw a pixel, start by calling the pixel(int x, int y) function.
oled.pixel(LCDWIDTH/2, LCDHEIGHT/2); // Add a pixel to the display buffer.

Lines, Rectangles, Circles, Oh My!
Now that we know how to draw pixels, it’ll be easy to draw all sorts of shapes.
To draw a line, you need two sets of x/y coordinates, the line will be drawn between them. Here’s an example:
int x0 = 7; int y0 = 7;   // (x0,y0) = (7, 7)
int x1 = 42; int y1 = 24; // (x1,y1) = (42, 24)
oled.line(x0, y0, x1, y1);  // Draw a line from (x0,y0) to (x1,y1);
oled.display(); // Draw to the screen


Things are a little different if you want to draw a rectangle. In this case, you give it a x/y coordinate to start at, then a width and a height.
int x0 = 7; int y0 = 5;
int width = 24;
int height = 13;
oled.rect(x0, y0, width, height);  // Draw a rectange from (7,5) to (31,18)
oled.display(); // Draw to the screen


The rectangle will be drawn from (x0, y0) to (x0+width, y0+height).
Want to fill that rectangle? Use the rectFill function instead!
oled.rectFill(7, 5, 35, 5); // Fill a rectangle from (7, 5) to (42, 10)
oled.display(); // Draw to the screen

Circles require a set of coordinates for the middle, and then a radius.
int radius = 13;
// Draw a 13-pixel radius (26-pixel diameter) 
// circle centered in the middle of the display:
oled.circle(LCDWIDTH/2, LCDHEIGHT/2, radius);   

As with the rectangle function, you can also fill the circle with circleFill:
oled.circleFill(42, 20, 7); // Fill a circle, 7 radius, centered at (42, 20)
oled.display(); // Draw to the screen

Drawing Text

In addition to basic shapes, you can also draw text with the SFE_MicroOLED library. There are a few settings to adjust before you get to texting, though. 
First, set the font type with setFontType(type). 
The parameter in this function can be either 0, 1, 2, or 3, each size gets progressively larger.
oled.setFontType(0);  // Set the text to small (10 columns, 6 rows worth of characters).
oled.setFontType(1);  // Set the text to medium (6 columns, 3 rows worth of characters).
oled.setFontType(2);  // Set the text to medium/7-segment (5 columns, 3 rows worth of characters).
oled.setFontType(3);  // Set the text to large (5 columns, 1 row worth of characters).

Next, after setting the font type, define your text cursor with setCursor(x, y). This will define the top-left corner of the first character you print.
oled.setCursor(0, 0);  // Set the text cursor to the upper-left of the screen.

oled.print("Hello, world"); // Print a const string
oled.print(analogRead(0));  // Print an integer
oled.print(42.07);  // Print a float
oled.display(); // Draw to the screen

*/

#include <SPI.h>
#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D

MicroOLED oled(PIN_RESET, DC_JUMPER); // Example I2C declaration

// I2C is great, but will result in a much slower update rate. The
// slower framerate may be a worthwhile tradeoff, if you need more
// pins, though.
void setup()
{  
  Serial.begin(115200);

  // These three lines of code are all you need to initialize the
  // OLED and print the splash screen.
  
  // Before you can start using the OLED, call begin() to init
  // all of the pins and configure the OLED.
  oled.begin();
  // clear(ALL) will clear out the OLED's graphic memory.
  oled.clear(PAGE); //clear will clear the Arduino's display buffer.
  oled.clear(ALL); // Clear the display's memory (gets rid of artifacts)
  oled.setFontType(0);  // Set the text to small (10 columns, 6 rows worth of characters).

  oled.print("aaaaaaaaaaaaaaaaaaaaaaaa");
  
  // To actually draw anything on the display, you must call the
  // display() function. 
  oled.display();   
}

void loop()
{
}
