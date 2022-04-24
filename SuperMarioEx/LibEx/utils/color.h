//
//  color.h
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/4.
//

#ifndef color_h
#define color_h
/*
#define LIGHTGRAY  { 200/255.0f, 200/255.0f, 200/255.0f, 255 /255.0f }   // Light Gray
#define GRAY       { 130/255.0f, 130/255.0f, 130/255.0f, 255 /255.0f }   // Gray
#define DARKGRAY   { 80/255.0f, 80/255.0f, 80/255.0f, 255 /255.0f }      // Dark Gray
#define YELLOW     { 253/255.0f, 249/255.0f, 0/255.0f, 255 /255.0f }     // Yellow
#define GOLD       { 255/255.0f, 203/255.0f, 0/255.0f, 255 /255.0f }     // Gold
#define ORANGE     { 255/255.0f, 161/255.0f, 0/255.0f, 255 /255.0f }     // Orange
#define PINK       { 255/255.0f, 109/255.0f, 194/255.0f, 255 /255.0f }   // Pink
#define RED        { 230/255.0f, 41/255.0f, 55/255.0f, 255 /255.0f }     // Red
#define MAROON     { 190/255.0f, 33/255.0f, 55/255.0f, 255 /255.0f }     // Maroon
#define GREEN      { 0/255.0f, 228/255.0f, 48/255.0f, 255 /255.0f }      // Green
#define LIME       { 0/255.0f, 158/255.0f, 47/255.0f, 255 /255.0f }      // Lime
#define DARKGREEN  { 0/255.0f, 117/255.0f, 44/255.0f, 255 /255.0f }      // Dark Green
#define SKYBLUE    { 102/255.0f, 191/255.0f, 255/255.0f, 255 /255.0f }   // Sky Blue
#define BLUE       { 0/255.0f, 121/255.0f, 241/255.0f, 255 /255.0f }     // Blue
#define DARKBLUE   { 0/255.0f, 82/255.0f, 172/255.0f, 255 /255.0f }      // Dark Blue
#define PURPLE     { 200/255.0f, 122/255.0f, 255/255.0f, 255 /255.0f }   // Purple
#define VIOLET     { 135/255.0f, 60/255.0f, 190/255.0f, 255 /255.0f }    // Violet
#define DARKPURPLE { 112/255.0f, 31/255.0f, 126/255.0f, 255 /255.0f }    // Dark Purple
#define BEIGE      { 211/255.0f, 176/255.0f, 131/255.0f, 255 /255.0f }   // Beige
#define BROWN      { 127/255.0f, 106/255.0f, 79/255.0f, 255 /255.0f }    // Brown
#define DARKBROWN  { 76/255.0f, 63/255.0f, 47/255.0f, 255 /255.0f }      // Dark Brown

#define WHITE      { 255/255.0f, 255/255.0f, 255/255.0f, 255 /255.0f }   // White
#define BLACK      { 0/255.0f, 0/255.0f, 0/255.0f, 255 /255.0f }         // Black
#define BLANK      { 0/255.0f, 0/255.0f, 0/255.0f, 0 }           // Blank (Transparent)
#define MAGENTA    { 255/255.0f, 0/255.0f, 255/255.0f, 255 /255.0f }     // Magenta
*/

// Some Basic Colors
#define LIGHTGRAY  { 200, 200, 200, 255 }   // Light Gray
#define GRAY       { 130, 130, 130, 255 }   // Gray
#define DARKGRAY   { 80, 80, 80, 255 }      // Dark Gray
#define YELLOW     { 253, 249, 0, 255 }     // Yellow
#define GOLD       { 255, 203, 0, 255 }     // Gold
#define ORANGE     { 255, 161, 0, 255 }     // Orange
#define PINK       { 255, 109, 194, 255 }   // Pink
#define RED        { 230, 41, 55, 255 }     // Red
#define MAROON     { 190, 33, 55, 255 }     // Maroon
#define GREEN      { 0, 228, 48, 255 }      // Green
#define LIME       { 0, 158, 47, 255 }      // Lime
#define DARKGREEN  { 0, 117, 44, 255 }      // Dark Green
#define SKYBLUE    { 102, 191, 255, 255 }   // Sky Blue
#define BLUE       { 0, 121, 241, 255 }     // Blue
#define DARKBLUE   { 0, 82, 172, 255 }      // Dark Blue
#define PURPLE     { 200, 122, 255, 255 }   // Purple
#define VIOLET     { 135, 60, 190, 255 }    // Violet
#define DARKPURPLE { 112, 31, 126, 255 }    // Dark Purple
#define BEIGE      { 211, 176, 131, 255 }   // Beige
#define BROWN      { 127, 106, 79, 255 }    // Brown
#define DARKBROWN  { 76, 63, 47, 255 }      // Dark Brown

#define WHITE      { 255, 255, 255, 255 }   // White
#define BLACK      { 0, 0, 0, 255 }         // Black
#define BLANK      { 0, 0, 0, 0 }           // Blank (Transparent)
#define MAGENTA    { 255, 0, 255, 255 }     // Magenta

// Color, 4 components, R8G8B8A8 (32bit)
typedef struct Color {
    unsigned char r;        // Color red value
    unsigned char g;        // Color green value
    unsigned char b;        // Color blue value
    unsigned char a;        // Color alpha value
} Color;

#endif /* color_h */
