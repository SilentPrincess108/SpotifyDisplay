#include <Arduino.h>
#include <Arduino_JSON.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <SpotifyEsp32.h>
#include <SPI.h>

//pin assignments - change later according to my own
#define TFT_CS 1
#define TFT_RST 2
#define TFT_DC 3
#define TFT_SCLK 4
#define TFT_MOSI 5

//key pin assignments
int b1 = 1;
int b2 = 2;
int b3 = 3;

//wifi info
char* SSID = "PRIVATE";
char* PASSWORD = "ALSOPRIVATE"

//spotify info
const char* CLIENT_ID = "36be5523849942e888539922cd2508b2";
const char* CLIENT_SECRET = "8fdf583f21f74da7abe41b89999b133b";
Spotify spot(CLIENT_ID, CLIENT_SECRET);

//screen obj
Adafruit_ST7735 screen = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//vars
String current_artist = "";
String current_track = "";
String pause_play = "";
String skip = "";
String previous = "";
String is_playing = "";
bool playing = false;
int lastPressed = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115300);

  //spotify api init
  sp.begin();
  while(!sp.is_auth())
  {
      sp.handle_client();
      delay(2000);
  }
  Serial.println("Connected to Spotify!");

  //screen init
  tft.initR(INITR_BLACKTAB); // the type of screen
  tft.setRotation(1); // this makes the screen landscape! remove this line for portrait
  Serial.println("TFT Initialized!");
  tft.fillScreen(ST77XX_BLACK); // make sure there is nothing in the buffer

  //wifi init
    WiFi.begin(SSID, PASSWORD); // Attempt to connect to wifi
    Serial.print("Connecting to WiFi..."); // This will print into the console!
    while(WiFi.status() != WL_CONNECTED) // Checking if the wifi has connected
    {
        delay(1000);
        Serial.print("."); // show a loading dot every second
    }
    Serial.printf("\nConnected!\n"); // Wifi connected!

  tft.setCursor(0,0); // make the cursor at the top left
  tft.write(WiFi.localIP().toString().c_str()); // print out IP on the screen

  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);

}

void loop() {
  displayInfo();
  detectButtons();
}

void displayInfo(){
  current_artist = spot.current_artist_names();
  current_track = spot.current_track_name();

  screen.write("Currently playing...");
  screen.setCursor(3,0);
  screen.write(current_track.toString().c_str());
  screen.setCursor(6,0);
  screen.write("By " + current_artist.toString().c_str());

}

void detectButtons(){
    if (millis() - lastPressed > 1000){
      if (digitalRead(b1) == LOW){
        //sdkdsdksd
        lastPressed = millis();
      }

      if (digitalRead(b2) == LOW){ //double check when built
        if (playing){
          spot.start_resume_playback();
        }
        else{
          spot.start_resume_playback();
        }

        lastPressed = millis();
      }

      if (digitalRead(b3) == LOW){
        spot.skip();
        lastPressed = millis();
      }
  }
}
