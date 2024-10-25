#ifndef CONFIG_H
#define CONFIG_H

// Constants
#define SAMPLES 64                                                              // Must be 2^n
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW                                       // Set display type for MD_MAX72xx library
#define MAX_DEVICES 4                                                           // Total number of display modules
#define CLK_PIN 12                                                              // Clock pin to communicate with display
#define DATA_PIN 13                                                             // Data pin to communicate with display
#define CS_PIN 11                                                               // Control pin to communicate with display
#define xres 32                                                                 // Total number of columns in the display, must be <= SAMPLES/2
#define yres 8                                                                  // Total number of rows in the display
#define DECAY 1

// Array for LED patterns
int ARR[] = {0, 128, 192, 224, 240, 248, 252, 254, 255};                        // Default = standard pattern

#endif // CONFIG_H
