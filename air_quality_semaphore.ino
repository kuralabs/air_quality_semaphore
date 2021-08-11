#include <Adafruit_SCD30.h>
#include <Adafruit_SSD1306.h>

const char HEADER[] PROGMEM = 
  "    _    _         ___              _ _ _\n"
  "   / \\  (_)_ __   / _ \\ _   _  __ _| (_) |_ _   _\n"
  "  / _ \\ | | '__| | | | | | | |/ _` | | | __| | | |\n"
  " / ___ \\| | |    | |_| | |_| | (_| | | | |_| |_| |\n"
  "/_/   \\_\\_|_|     \\__\\_\\\\__,_|\\__,_|_|_|\\__|\\__, |\n"
  "                                            |___/\n"
  " ____                             _\n"
  "/ ___|  ___ _ __ ___   __ _ _ __ | |__   ___  _ __ ___\n"
  "\\___ \\ / _ \\ '_ ` _ \\ / _` | '_ \\| '_ \\ / _ \\| '__/ _ \\\n"
  " ___) |  __/ | | | | | (_| | |_) | | | | (_) | | |  __/\n"
  "|____/ \\___|_| |_| |_|\\__,_| .__/|_| |_|\\___/|_|  \\___|\n"
  "                           |_|\n"
;

#define MEASUREMENT_INTERVAL 2

Adafruit_SCD30  air;

#define SCREEN_WIDTH         128
#define SCREEN_HEIGHT        64
#define SCREEN_RESET         33
#define SCREEN_ADDRESS       0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);




void abort(void) {
  while (true) { delay(10); }
}


void setup(void) {

  // Initialize serial interface
  Serial.begin(115200);
  while (!Serial) { delay(10); }
  delay(500); // Pause for half second so that serial console can reconnect

  Serial.println(HEADER);

  // Initialize air quality sensor
  Serial.println(F("STATUS: Initializing air quality sensor ..."));
  
  if (!air.begin()) {
    Serial.println(F("ERROR: Failed to initialize air quality sensor!"));
    abort();
  }

  if (!air.setMeasurementInterval(MEASUREMENT_INTERVAL)){
    Serial.println(F("ERROR: Failed to set measurement interval!"));
    abort();
  }

  Serial.print(F("STATUS: Measurement Interval: "));
  Serial.print(air.getMeasurementInterval()); 
  Serial.println(F(" seconds"));

  Serial.println(F("STATUS: Air quality sensor initialized"));

  // Initialize screen
  Serial.println(F("STATUS: Initializing display ..."));
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("ERROR: Failed to initialize display!"));
    abort();
  }
  display.clearDisplay();

  Serial.println(F("STATUS: Display initialized"));
}


void loop() {
  if (air.dataReady()) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);

    Serial.println("Data available!");

    if (!air.read()){
      Serial.println("Error reading sensor data");
      display.println("READ ERR");
      display.display();
      return;
    }

    Serial.print("CO2: ");
    Serial.print(air.CO2, 3);
    Serial.println(" ppm");
    Serial.println("");

    display.println("CO2:");
    display.print(air.CO2, 2);

    display.setTextSize(1);

    display.setCursor(100, 20);
    display.println(" ppm");
    display.display();
  }

  delay(100);
}
