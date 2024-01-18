#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DEG2RAD 0.0174532925
#define centerX 64
#define centerY 32
#define HOR_WIDTH 75
#define SCL_WIDTH 20

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

// '10', 7x5px
const unsigned char epd_bitmap_10 [] PROGMEM = {
	0x4e, 0x4a, 0x4a, 0x4a, 0x4e
};
// '20', 7x5px
const unsigned char epd_bitmap_20 [] PROGMEM = {
	0xee, 0x2a, 0xea, 0x8a, 0xee
};
// '30', 7x5px
const unsigned char epd_bitmap_30 [] PROGMEM = {
	0xee, 0x2a, 0xea, 0x2a, 0xee
};
// '40', 7x5px
const unsigned char epd_bitmap_40 [] PROGMEM = {
	0xae, 0xaa, 0xea, 0x2a, 0x2e
};
// '50', 7x5px
const unsigned char epd_bitmap_50 [] PROGMEM = {
	0xee, 0x8a, 0xea, 0x2a, 0xee
};
// '70', 7x5px
const unsigned char epd_bitmap_70 [] PROGMEM = {
	0xee, 0x2a, 0x2a, 0x2a, 0x2e
};
// '60', 7x5px
const unsigned char epd_bitmap_60 [] PROGMEM = {
	0xee, 0x8a, 0xea, 0xaa, 0xee
};
// '80', 7x5px
const unsigned char epd_bitmap_80 [] PROGMEM = {
	0xee, 0xaa, 0xea, 0xaa, 0xee
};
// '90', 7x5px
const unsigned char epd_bitmap_90 [] PROGMEM = {
	0xee, 0xaa, 0xea, 0x2a, 0xee
};

const unsigned char* number_bitmaps[9] = {
  epd_bitmap_10,
  epd_bitmap_20,
  epd_bitmap_30,
  epd_bitmap_40,
  epd_bitmap_50,
  epd_bitmap_60,
  epd_bitmap_70,
  epd_bitmap_80,
  epd_bitmap_90
};

void setup() 
{
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();
}

void loop() 
{
    int roll = map(analogRead(A0), 0, 1023, -180, 180);
    int pitch = map(analogRead(A1), 0, 1023, -90, 90);

    display.clearDisplay();

    drawHorizon(pitch, roll);
    drawWings(pitch);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print(pitch / 2);
    display.setCursor(110, 0);
    display.print(roll);
    
    display.display();
}
// Roll from -180 to 180
// Pitch from -90 to 90
void drawHorizon(int pitch, int roll) {
    pitch *= 2;
    float crollMain = cos((-roll - 270) * DEG2RAD);
    float srollMain = sin((-roll - 270) * DEG2RAD);

    float croll = cos((-roll) * DEG2RAD);
    float sroll = sin((-roll) * DEG2RAD);

    int16_t x = crollMain * pitch;
    int16_t y = srollMain * pitch;

    int16_t x0 = croll * HOR_WIDTH; int16_t y0 = sroll * HOR_WIDTH;

    // HORIZON
    // commented cuz up side anyway is white
    // display.drawLine(centerX + (x - x0), centerY + (y - y0),
    //                  centerX + (x + x0), centerY + (y + y0), WHITE);

    // White up side
    int16_t triX = crollMain * HOR_WIDTH; int16_t triY = srollMain * HOR_WIDTH;
    display.fillTriangle(centerX + (x - x0), centerY + (y - y0),
                         centerX + (x + x0), centerY + (y + y0),
                         centerX - (triX - x0), centerY - (triY - y0), WHITE);

    display.fillTriangle(centerX + (x - x0), centerY + (y - y0),
                         centerX - (triX - x0), centerY - (triY - y0),
                         centerX - (triX + x0), centerY - (triY + y0), WHITE);

    ////// SCALES AND NUMBERS
    x0 = croll * 20; y0 = sroll * 20;  // LARGE
    int16_t x1 = croll * 10; int16_t y1 = sroll * 10;  // MEDIUM
    int16_t x2 = croll * 6; int16_t y2 = sroll * 6;  // SMALL

    int16_t x3 = croll * 30; int16_t y3 = sroll * 30;  // NUMBERS

    for(int i = 20; i < 200; i += 20) {
        x = crollMain * (pitch - i);
        y = srollMain * (pitch - i);

        // NUMBERS
        display.drawBitmap(centerX + (x - x3) - 3, centerY + (y - y3) - 2, number_bitmaps[(i / 20) - 1], 7, 5, BLACK);
        display.drawBitmap(centerX + (x + x3) - 3, centerY + (y + y3) - 2, number_bitmaps[(i / 20) - 1], 7, 5, BLACK);

        // SCALES
        display.drawLine(centerX + (x - x0), centerY + (y - y0),
                         centerX + (x + x0), centerY + (y + y0), BLACK);

        x = crollMain * (pitch - i + 5);
        y = srollMain * (pitch - i + 5);
        display.drawLine(centerX + (x - x2), centerY + (y - y2),
                         centerX + (x + x2), centerY + (y + y2), BLACK);
        x = crollMain * (pitch - i + 10);
        y = srollMain * (pitch - i + 10);
        display.drawLine(centerX + (x - x1), centerY + (y - y1),
                         centerX + (x + x1), centerY + (y + y1), BLACK);
        x = crollMain * (pitch - i + 15);
        y = srollMain * (pitch - i + 15);
        display.drawLine(centerX + (x - x2), centerY + (y - y2),
                         centerX + (x + x2), centerY + (y + y2), BLACK);
    }

    for(int i = 20; i < 200; i += 20) {
        x = crollMain * (pitch + i);
        y = srollMain * (pitch + i);

        // NUMBERS
        display.drawBitmap(centerX + (x - x3) - 3, centerY + (y - y3) - 2, number_bitmaps[(i / 20) - 1], 7, 5, WHITE);
        display.drawBitmap(centerX + (x + x3) - 3, centerY + (y + y3) - 2, number_bitmaps[(i / 20) - 1], 7, 5, WHITE);

        // SCALES
        display.drawLine(centerX + (x - x0), centerY + (y - y0),
                         centerX + (x + x0), centerY + (y + y0), WHITE);

        x = crollMain * (pitch + i - 5);
        y = srollMain * (pitch + i - 5);
        display.drawLine(centerX + (x - x2), centerY + (y - y2),
                         centerX + (x + x2), centerY + (y + y2), WHITE);
        x = crollMain * (pitch + i - 10);
        y = srollMain * (pitch + i - 10);
        display.drawLine(centerX + (x - x1), centerY + (y - y1),
                         centerX + (x + x1), centerY + (y + y1), WHITE);
        x = crollMain * (pitch + i - 15);
        y = srollMain * (pitch + i - 15);
        display.drawLine(centerX + (x - x2), centerY + (y - y2),
                         centerX + (x + x2), centerY + (y + y2), WHITE);
    }
    //////
}

void drawWings(int pitch)
{
    if(pitch < 1) {
        display.drawFastHLine(44, 32, 14, WHITE);
        display.drawFastVLine(58, 32, 4, WHITE);
        display.drawFastHLine(63, 32, 3, WHITE);
        display.drawPixel(63, 33, WHITE);
        display.drawPixel(65, 33, WHITE);
        display.drawFastHLine(63, 34, 3, WHITE);
        display.drawFastHLine(71, 32, 14, WHITE);
        display.drawFastVLine(70, 32, 4, WHITE);
    } else {
        display.drawFastHLine(44, 32, 14, BLACK);
        display.drawFastVLine(58, 32, 4, BLACK);
        display.drawFastHLine(63, 32, 3, BLACK);
        display.drawPixel(63, 33, BLACK);
        display.drawPixel(65, 33, BLACK);
        display.drawFastHLine(63, 34, 3, BLACK);
        display.drawFastHLine(71, 32, 14, BLACK);
        display.drawFastVLine(70, 32, 4, BLACK);
    }
}