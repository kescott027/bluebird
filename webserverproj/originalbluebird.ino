/*
 * Project bluebird
 * Description:
 * Author:
 * Date:
 */

 #include "application.h"
 #include "neopixel.h"
 // #include "inttypes.h"
 #include "WebServer.h"

 // IMPORTANT: Set pixel COUNT, PIN and TYPE
 #define PIXEL_COUNT 1
 #define PIXEL_PIN D0
 #define PIXEL_TYPE WS2812B
// setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w)

#define RED 0
#define GREEN 1
#define BLUE 2
#define YELLOW 3
#define MAGENTA 4
#define CYAN 5
#define WHITE 6
#define OFF 9
#define SMARTPIXEL 0

 Adafruit_NeoPixel pixel = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
 TCPClient webClient;
 /* TCPServer webServer = TCPServer(80);
 char myIpAddress[24];
 */
 volatile uint64_t tickCount = 0;
 volatile int colorValue = 0;
 int mom_pin = D4;
 int last_state = 0;

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  pinMode(mom_pin, INPUT_PULLDOWN);
  pixel.begin();
  pixel.show();
  /*
  Spark.variable("ipAddress", myIpAddress, STRING);
  // IPAddress myIp = WiFi.localIP();
  IPAddress myIp = WiFi.localIP();
  // IPAddress myIp = Network.localIP();
  sprintf(myIpAddress, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
  webServer.begin();
  */

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  int mom_state = 0;
  mom_state = digitalRead(mom_pin);
  if (mom_state != last_state) {
    setColor(SMARTPIXEL, colorValue);
    pixel.show();
    if (colorValue >= 6 ) {
      colorValue = 0;
    } else {
      colorValue ++;
    }
  }
  delay(100);
  /*
  if (tickCount >= 1000) {
    setColor(SMARTPIXEL, colorValue);
    pixel.show();
    if (colorValue >= 6 ) {
      colorValue = 0;
    } else {
      colorValue ++;
    }
    tickCount = 0;
  }
  if (webClient.connected() && webClient.available()) {
      serveWebpage();
  }
  else {
      webClient = webServer.available();
  }
  tickCount ++; */

}

void setColor(int target_pixel, int color) {
switch (color) {
  case RED :
    pixel.setPixelColor(target_pixel, 255, 0, 0);
    break;
  case GREEN :
    pixel.setPixelColor(target_pixel, 0, 255, 0);
    break;
  case BLUE :
    pixel.setPixelColor(target_pixel, 0, 0, 255);
    break;
  case CYAN :
  pixel.setPixelColor(target_pixel, 0, 255, 255);
  break;
  case YELLOW :
  pixel.setPixelColor(target_pixel, 255, 255, 0);
  break;
  case MAGENTA :
  pixel.setPixelColor(target_pixel, 255, 0, 255);
  break;
  case WHITE :
  pixel.setPixelColor(target_pixel, 255, 255, 255);
  break;
  pixel.setPixelColor(target_pixel, 0, 0, 0);
  break;
  default : // white
    pixel.setPixelColor(target_pixel, 255, 255, 255);
  }
}

void serveWebpage() {
    //TODO: read in the request to see what page they want:
    //TODO: retrieve larger content from flash?

    webClient.println("<html>I'm serving a webpage!</html>\n\n");
    webClient.flush();
    webClient.stop();
    delay(100);
}
