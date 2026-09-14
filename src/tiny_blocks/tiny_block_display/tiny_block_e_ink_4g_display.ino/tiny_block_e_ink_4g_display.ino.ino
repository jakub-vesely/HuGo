
#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_4G.h>
//#include <GxEPD2_BW.h>
#include <Fonts/FreeSansBold9pt7b.h>

// Mapping for Arduino Uno
// BUSY -> 7, RST -> 8, DC -> 9, CS -> 10, CLK -> 13, DIN -> 11

GxEPD2_4G<GxEPD2_154_GDEY0154D67, 32> display(GxEPD2_154_GDEY0154D67(/*CS=*/ -1, /*DC=*/ HUGO_PIN_D4, /*RST=*/ HUGO_PIN_D4, /*BUSY=*/ HUGO_PIN_D6));
void setup() {
  display.init(115200); // Initialize with serial baud rate for debugging
  
  // The library uses 4-bit grayscale (0-3 or 0-15 depending on mapping)
  // For GxEPD2_4G, we use GxEPD_WHITE, GxEPD_LIGHTGREY, GxEPD_DARKGREY, GxEPD_BLACK
  
  drawFourGrayscales();
  
  display.hibernate(); 
}

void loop() {
  // Empty
}

void drawFourGrayscales() {
  display.setRotation(1);
  display.setFullWindow();
  display.firstPage();
  
  do {
    display.fillScreen(GxEPD_WHITE);

    uint16_t w = display.width() / 4;
    uint16_t h = display.height();

    // 1. White
    display.fillRect(0, 0, w, h, GxEPD_WHITE);
    
    // 2. Light Gray
    display.fillRect(w, 0, w, h, GxEPD_LIGHTGREY);
    
    // 3. Dark Gray
    display.fillRect(w * 2, 0, w, h, GxEPD_DARKGREY);
    
    // 4. Black
    display.fillRect(w * 3, 0, w, h, GxEPD_BLACK);

    // Labeling
    display.setFont(&FreeSansBold9pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(5, h / 2);
    display.print("4-GRAY");

  } while (display.nextPage());
}