#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#define PIN_I2C_SDA			0
#define PIN_I2C_SCL			1
#define PIN_NEOPIXEL		16
#define PIN_SSD1306_RESET 	(-1)
#define SSD1306_ADDRESS 	0x3c
#define SSD1306_WIDTH 		128
#define SSD1306_HEIGHT 		64
#define SSD1306_OFFSET_X 	32
#define SSD1306_OFFSET_Y 	32

#define COLOR_BLUE      0x000040UL    // blue
#define COLOR_RED       0x400000UL    // red
#define COLOR_GREEN     0x004000UL    // green

Adafruit_NeoPixel led(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
Adafruit_SSD1306 ssd1306(SSD1306_WIDTH, SSD1306_HEIGHT, &Wire, PIN_SSD1306_RESET);
uint32_t lastTime = 0;
int state=0;

void drawText(int16_t yp, const char *text) {
	ssd1306.setCursor(SSD1306_OFFSET_X, (int16_t)(yp+SSD1306_OFFSET_Y));
	ssd1306.println(text);
	ssd1306.display();
}


void setup() {
	led.begin();
	led.setPixelColor(0, COLOR_GREEN);
	led.show();

	Wire.setSDA(PIN_I2C_SDA);
	Wire.setSCL(PIN_I2C_SCL);
	Wire.begin();
	delay(100);

	if (!ssd1306.begin(SSD1306_SWITCHCAPVCC,SSD1306_ADDRESS,false)) {
		for (;;) {
			uint32_t now = millis();
			if (now - lastTime > 400UL) {
				if (state == 0) {
					led.setPixelColor(0, COLOR_RED);
					state = 1;
				} else {
					led.setPixelColor(0, 0);
					state = 0;
				}
				led.show();
				lastTime = now;
			}
		}
	}

	ssd1306.clearDisplay();
	ssd1306.setTextColor(SSD1306_WHITE);
	ssd1306.setTextSize(1);
	drawText(0,"ABCDEFGHIJK");
	drawText(12,"LMNOPQRSTUVWXYZ");
	drawText(24,"0123456789#");
}

void loop() {
	uint32_t now = millis();
	if (now - lastTime > 1000UL) {
		switch (state) {
			case 0:
				ssd1306.invertDisplay(true);
				led.setPixelColor(0, COLOR_BLUE);
				state = 1;
				break;
			case 1:
				ssd1306.invertDisplay(false);
				led.setPixelColor(0, COLOR_RED);
				state = 0;
				break;
		}
		led.show();
		lastTime = now;
	}
}
