#include <ArduinoHttpClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Bridge.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 12

const char* SSID = "Untrusted Network";
const char* PSK = "Fanta113#";

char serverAddress[] = "census.daybreakgames.com";  // server address
int port = 80;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(115200);
    setup_wifi();
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  strip.begin();
  strip.setBrightness(100);
  strip.show(); // Initialize all pixels to 'off'
}

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(SSID);
 
    WiFi.begin(SSID, PSK);
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    HttpClient httpclient = HttpClient(espClient, serverAddress, port);
    httpclient.get("/s:elmecensusdata/get/ps2/character?name.first=P3RF3CTION&c:resolve=online_status&c:join=type:profile%5Eon:profile_id%5Eto:profile_id%5Elist:0%5Eshow:name.en%27image_path%5Einject_at:main_class&c:join=type:faction%5Eon:faction_id%5Eto:faction_id%5Elist:0%5Eshow:name.en%27image_path%5Einject_at:faction&c:join=type:world%5Eon:world_id%5Eto:world_id%5Elist:0%5Einject_at:world&c:join=type:characters_stat_history%5Eon:character_id%5Eto:character_id%5Elist:0%5Eshow:stat_name%27all_time%5Elist:1%5Einject_at:stats_history%5Eterms:stat_name=deaths%27stat_name=kills&c:join=type:characters_stat%5Eon:character_id%5Eto:character_id%5Elist:0%5Eshow:stat_name%27value_forever%27profile_id%5Elist:1%5Einject_at:stats%5Eterms:stat_name=score%27stat_name=hit_count%27stat_name=fire_count(profile%5Eon:profile_id%5Eto:profile_type_id%5Eshow:name.en%5Einject_at:class)&c:join=type:outfit_member%5Eon:character_id%5Eto:character_id%5Elist:0%5Einject_at:outfit_member(outfit%5Eon:outfit_id%5Eto:outfit_id%5Elist:0%5Einject_at:outfit)&c:join=type:title%5Eon:title_id%5Eto:title_id%5Elist:0%5Eshow:name.en%5Einject_at:title");

    
    // Parse response
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + JSON_ARRAY_SIZE(18) + 40*JSON_OBJECT_SIZE(1) + 7*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 18*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(7) + JSON_OBJECT_SIZE(8) + JSON_OBJECT_SIZE(9) + JSON_OBJECT_SIZE(18) + 2980;
    DynamicJsonDocument doc(capacity);
    String response = httpclient.responseBody();
    DeserializationError error = deserializeJson(doc, response);
     if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    JsonObject character_list_0 = doc["character_list"][0];
    const char* character_list_0_online_status = character_list_0["online_status"]; // "0"
    String status = String(character_list_0_online_status);
    
    // Read values
    if (status != "0") {
      colorWipe(strip.Color(0,   255,   0), 50); // Green
      Serial.println("Player Online");
    }
    else   {
      colorWipe(strip.Color(255,   0,   0), 50); // Red
      Serial.println("Player Offline");
    }
    client.loop();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
