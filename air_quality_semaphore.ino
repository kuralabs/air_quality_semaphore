#include <Adafruit_SCD30.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>
#include <Adafruit_NeoPixel.h>


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


#define AIR_MEASUREMENT_INTERVAL 2

Adafruit_SCD30  air;


#define SCREEN_WIDTH                128
#define SCREEN_HEIGHT               64
#define SCREEN_RESET                33
#define SCREEN_ADDRESS              0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);


#define SENSOR_ADDRESS              0x77
#define SENSOR_SEALEVELPRESSURE_HPA 1013.25

Adafruit_BME280 sensor;


#define SEMAPHORE_IO                32
#define SEMAPHORE_LEDS              3
Adafruit_NeoPixel semaphore = Adafruit_NeoPixel(SEMAPHORE_LEDS, SEMAPHORE_IO, NEO_GRB + NEO_KHZ800);


void abort(void) {
  while (true) { delay(10); }
}


void setup(void) {
  uint16_t time = millis();

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

  if (!air.setMeasurementInterval(AIR_MEASUREMENT_INTERVAL)){
    Serial.println(F("ERROR: Failed to set measurement interval!"));
    abort();
  }

  Serial.print(F("STATUS: Measurement Interval: "));
  Serial.print(air.getMeasurementInterval()); 
  Serial.println(F(" seconds"));

  Serial.println(F("STATUS: Air quality sensor initialized"));

  // Initialize display
  Serial.println(F("STATUS: Initializing display ..."));

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("ERROR: Failed to initialize display!"));
    abort();
  }

  display.clearDisplay();
  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setRotation(0);

  Serial.println(F("STATUS: Display initialized"));

  // Initialize sensor
  Serial.println(F("STATUS: Initializing barometric, humidity and temperature sensor ..."));
  
  if (!sensor.begin(SENSOR_ADDRESS, &Wire)) {
    Serial.println(F("ERROR: Failed to initialize barometric, humidity and temperature sensor!"));
    abort();
  }

  Serial.println(F("STATUS: Barometric, humidity and temperature sensor initialized"));

  // Initialize semaphore
  Serial.println(F("STATUS: Initializing semaphore lights ..."));

  semaphore.begin();
  semaphore.show(); // Initialize all pixels to 'off'

  Serial.println(F("STATUS: Semaphore lights initialized"));


  Serial.print(F("STATUS: Board successfully initialized in "));
  Serial.print(millis() - time, DEC);
  Serial.println(" ms");
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

  Serial.print("Temperature = ");
  Serial.print(sensor.readTemperature());
  Serial.println(" °C");
  Serial.print("Pressure = ");
  Serial.print(sensor.readPressure() / 100.0F);
  Serial.println(" hPa");
  Serial.print("Approx. Altitude = ");
  Serial.print(sensor.readAltitude(SENSOR_SEALEVELPRESSURE_HPA));
  Serial.println(" m");
  Serial.print("Humidity = ");
  Serial.print(sensor.readHumidity());
  Serial.println(" %");
  Serial.println();

  delay(1000);
}
