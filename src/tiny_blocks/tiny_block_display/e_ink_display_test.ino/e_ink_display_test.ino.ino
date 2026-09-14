#include <GxEPD2_BW.h>
#include <Fonts/Picopixel.h>

//GxEPD2_BW<GxEPD2_154_D67, 48> display(GxEPD2_154_D67(/*CS=*/ 10, /*DC=*/ 9, /*RST=*/ 12, /*BUSY=*/ 8));
GxEPD2_BW<GxEPD2_154_D67, 30> display(GxEPD2_154_D67(/*CS=*/ -1, /*DC=*/ PIN_PA4, /*RST=*/ PIN_PA2, /*BUSY=*/ PIN_PB2));

unsigned long startTime;
char timeBuffer[10]; 
int lastSecond = -1;

void setup() {
  display.init(115200);
  display.setRotation(3);
  display.setTextColor(GxEPD_BLACK);
  
  // 1. FULL REFRESH: Cleans the whole screen once
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
  } while (display.nextPage());

  startTime = millis();
}

void loop() {
  unsigned long currentMillis = millis() - startTime;
  int seconds = (currentMillis / 1000) % 60;

  if (seconds != lastSecond) {
    lastSecond = seconds;
    
    int minutes = (currentMillis / 60000) % 60;
    int hours   = (currentMillis / 3600000);
    sprintf(timeBuffer, "%02d:%02d", minutes, seconds);

    // 2. ALIGNED WINDOW: x=16 and h=48 are multiples of 8. 
    // This prevents the "shifting" jump after the first update.
    display.setPartialWindow(16, 80, 168, 48); 
      
    display.firstPage();
    do {
      // 3. CLEARING: This wipes the partial window area before drawing new digits
      display.fillScreen(GxEPD_WHITE); 
      display.setFont(&Picopixel);
      display.setTextSize(4);
      display.setCursor(18, 115); // Adjusted to fit inside our 48px height box
      display.print(timeBuffer);
    } while (display.nextPage());
  }
}
