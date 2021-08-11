/*
 * Copyright (C) 2021 KuraLabs S.R.L
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <Adafruit_SCD30.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>
#include <Adafruit_NeoPixel.h>


static const char HEADER[] PROGMEM = 
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


#define ICON_KURALABS_W 120
#define ICON_KURALABS_H 43

static const unsigned char ICON_KURALABS[] PROGMEM = {
  0x38, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 
  0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 
  0x07, 0xfc, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf3, 0xff, 
  0xff, 0xf9, 0xfe, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe7, 0xff, 0xff, 0xff, 
  0xff, 0xfc, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 
  0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x7f, 0xff, 0xff, 0xff, 0xc0, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x7f, 0xff, 0xff, 0xff, 0xc1, 0xc0, 
  0x0e, 0x0e, 0x1c, 0x0e, 0x1f, 0xf0, 0x01, 0xc0, 0x38, 0xff, 0xff, 0xff, 0xff, 0xe3, 0x80, 0x0e, 
  0x1c, 0x1c, 0x0e, 0x1f, 0xfc, 0x01, 0xe0, 0x3c, 0xff, 0xff, 0xff, 0xff, 0xe7, 0x80, 0x0e, 0x38, 
  0x1c, 0x0e, 0x1f, 0xfc, 0x03, 0xe0, 0x3c, 0xff, 0xff, 0xff, 0xff, 0xe3, 0x80, 0x0e, 0x70, 0x1c, 
  0x0e, 0x1c, 0x1e, 0x03, 0xf0, 0x38, 0xff, 0xff, 0xff, 0xff, 0xe3, 0x80, 0x0e, 0xe0, 0x1c, 0x0e, 
  0x1c, 0x0e, 0x07, 0x70, 0x19, 0xff, 0xff, 0xff, 0xff, 0xf3, 0x00, 0x0f, 0xe0, 0x1c, 0x0e, 0x1c, 
  0x1e, 0x07, 0x38, 0x1b, 0xff, 0xff, 0xff, 0xff, 0xf9, 0x00, 0x0f, 0xe0, 0x1c, 0x0e, 0x1e, 0x3c, 
  0x06, 0x38, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xf0, 0x1c, 0x0e, 0x1f, 0xfc, 0x0e, 
  0x38, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0x70, 0x0e, 0x0e, 0x1f, 0xf0, 0x0f, 0xfc, 
  0x0f, 0xfe, 0xff, 0xff, 0xef, 0xfe, 0x00, 0x0e, 0x38, 0x0e, 0x0e, 0x1e, 0x70, 0x1f, 0xfc, 0x0f, 
  0xe3, 0x9f, 0xff, 0xb8, 0xfe, 0x00, 0x0e, 0x3c, 0x0f, 0x1e, 0x1c, 0x38, 0x1c, 0x1e, 0x0f, 0xf3, 
  0xcf, 0xfe, 0x79, 0xfe, 0x00, 0x0e, 0x1e, 0x0f, 0xfc, 0x1c, 0x3c, 0x38, 0x0e, 0x0f, 0xfb, 0xdf, 
  0xff, 0xfb, 0xfe, 0x00, 0x0e, 0x0e, 0x07, 0xf8, 0x1c, 0x1c, 0x38, 0x0f, 0x0f, 0xfe, 0x1f, 0xff, 
  0x0f, 0xfe, 0x00, 0x0c, 0x06, 0x01, 0xe0, 0x0c, 0x0e, 0x30, 0x06, 0x0f, 0xff, 0xff, 0xff, 0xff, 
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xfe, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x80, 0x00, 0xc0, 0x03, 
  0xc0, 0x06, 0x80, 0x00, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x80, 0x00, 0xc0, 0x02, 0x20, 
  0x00, 0x00, 0x00, 0x7f, 0xfe, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x80, 0x01, 0x20, 0x02, 0x20, 0x04, 
  0x00, 0x00, 0x3f, 0xfc, 0x03, 0xff, 0x80, 0x00, 0x00, 0x80, 0x01, 0x20, 0x03, 0xc0, 0x06, 0x00, 
  0x00, 0x1f, 0xff, 0xbf, 0xff, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x02, 0x20, 0x00, 0x80, 0x00, 
  0x0f, 0xfb, 0xfb, 0xfe, 0x00, 0x00, 0x00, 0x80, 0x03, 0xf0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x07, 
  0xfc, 0xf7, 0xfc, 0x00, 0x00, 0x00, 0x80, 0x02, 0x10, 0x02, 0x20, 0x00, 0x00, 0x00, 0x07, 0xff, 
  0xff, 0xfc, 0x00, 0x00, 0x00, 0xf8, 0x04, 0x08, 0x03, 0xc0, 0x07, 0x80, 0x00, 0x03, 0xff, 0xff, 
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xf8, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xf0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00
};


#define ICON_SMILE_NEUTRAL_W 44
#define ICON_SMILE_NEUTRAL_H 44

static const unsigned char ICON_SMILE_NEUTRAL[] PROGMEM = {
  0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 
  0x00, 0x00, 0x00, 0x3f, 0xc0, 0x3f, 0xc0, 0x00, 0x00, 0x7e, 0x00, 0x07, 0xe0, 0x00, 0x01, 0xf8, 
  0x00, 0x01, 0xf8, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x7c, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x3e, 0x00, 
  0x07, 0x80, 0x00, 0x00, 0x1e, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x1e, 0x00, 0x00, 0x00, 
  0x07, 0x80, 0x1c, 0x00, 0x00, 0x00, 0x03, 0x80, 0x3c, 0x07, 0x00, 0x0e, 0x03, 0xc0, 0x38, 0x0f, 
  0x80, 0x1f, 0x01, 0xc0, 0x78, 0x0f, 0x80, 0x1f, 0x01, 0xe0, 0x70, 0x0f, 0x80, 0x1f, 0x00, 0xe0, 
  0x70, 0x07, 0x80, 0x1e, 0x00, 0xe0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xe0, 0x00, 0x00, 0x00, 
  0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 
  0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 
  0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xf0, 0x00, 0x00, 0x00, 
  0x00, 0xf0, 0x70, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x70, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x78, 0x00, 
  0x00, 0x00, 0x01, 0xe0, 0x38, 0x3f, 0xff, 0xff, 0xc1, 0xc0, 0x3c, 0x3f, 0xff, 0xff, 0xc3, 0xc0, 
  0x1c, 0x00, 0x00, 0x00, 0x03, 0x80, 0x1e, 0x00, 0x00, 0x00, 0x07, 0x80, 0x0f, 0x00, 0x00, 0x00, 
  0x0f, 0x00, 0x07, 0x80, 0x00, 0x00, 0x1e, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x3e, 0x00, 0x03, 0xe0, 
  0x00, 0x00, 0x7c, 0x00, 0x01, 0xf8, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x7e, 0x00, 0x07, 0xe0, 0x00, 
  0x00, 0x3f, 0xc0, 0x3f, 0xc0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 
  0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00
};


#define ICON_SMILE_GOOD_W 44
#define ICON_SMILE_GOOD_H 44

static const unsigned char ICON_SMILE_GOOD[] PROGMEM = {
  0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 
  0x00, 0x00, 0x00, 0x3f, 0xc0, 0x3f, 0xc0, 0x00, 0x00, 0x7e, 0x00, 0x07, 0xe0, 0x00, 0x01, 0xf8, 
  0x00, 0x01, 0xf8, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x7c, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x3e, 0x00, 
  0x07, 0x80, 0x00, 0x00, 0x1e, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x1e, 0x00, 0x00, 0x00, 
  0x07, 0x80, 0x1c, 0x00, 0x00, 0x00, 0x03, 0x80, 0x3c, 0x07, 0x00, 0x0e, 0x03, 0xc0, 0x38, 0x0f, 
  0x80, 0x1f, 0x01, 0xc0, 0x78, 0x0f, 0x80, 0x1f, 0x01, 0xe0, 0x70, 0x0f, 0x80, 0x1f, 0x00, 0xe0, 
  0x70, 0x07, 0x80, 0x1e, 0x00, 0xe0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xe0, 0x00, 0x00, 0x00, 
  0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 
  0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 
  0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xf0, 0x00, 0x00, 0x00, 
  0x00, 0xf0, 0x70, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x70, 0x30, 0x00, 0x00, 0xc0, 0xe0, 0x78, 0x1c, 
  0x00, 0x01, 0x81, 0xe0, 0x38, 0x0e, 0x00, 0x07, 0x81, 0xc0, 0x3c, 0x07, 0xc0, 0x3e, 0x03, 0xc0, 
  0x1c, 0x01, 0xff, 0xf8, 0x03, 0x80, 0x1e, 0x00, 0x3f, 0xc0, 0x07, 0x80, 0x0f, 0x00, 0x00, 0x00, 
  0x0f, 0x00, 0x07, 0x80, 0x00, 0x00, 0x1e, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x3e, 0x00, 0x03, 0xe0, 
  0x00, 0x00, 0x7c, 0x00, 0x01, 0xf8, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x7e, 0x00, 0x07, 0xe0, 0x00, 
  0x00, 0x3f, 0xc0, 0x3f, 0xc0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 
  0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00
};


#define ICON_SMILE_BAD_W 44
#define ICON_SMILE_BAD_H 44

static const unsigned char ICON_SMILE_BAD[] PROGMEM = {
  0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 
  0x00, 0x00, 0x00, 0x3f, 0xc0, 0x3f, 0xc0, 0x00, 0x00, 0x7e, 0x00, 0x07, 0xe0, 0x00, 0x01, 0xf8, 
  0x00, 0x01, 0xf8, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x7c, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x3e, 0x00, 
  0x07, 0x80, 0x00, 0x00, 0x1e, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x1e, 0x00, 0x00, 0x00, 
  0x07, 0x80, 0x1c, 0x08, 0x80, 0x11, 0x03, 0x80, 0x3c, 0x1d, 0xc0, 0x3b, 0x83, 0xc0, 0x38, 0x0f, 
  0x80, 0x1f, 0x01, 0xc0, 0x78, 0x07, 0x00, 0x0e, 0x01, 0xe0, 0x70, 0x0f, 0x80, 0x1f, 0x00, 0xe0, 
  0x70, 0x1d, 0xc0, 0x3b, 0x80, 0xe0, 0xf0, 0x18, 0xc0, 0x31, 0x80, 0xf0, 0xe0, 0x00, 0x00, 0x00, 
  0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 
  0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 
  0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x70, 0xf0, 0x00, 0x3f, 0xc0, 
  0x00, 0xf0, 0x70, 0x01, 0xff, 0xf8, 0x00, 0xe0, 0x70, 0x07, 0xe0, 0x3e, 0x00, 0xe0, 0x78, 0x0e, 
  0x00, 0x07, 0x01, 0xe0, 0x38, 0x1c, 0x00, 0x01, 0x81, 0xc0, 0x3c, 0x30, 0x00, 0x00, 0xc3, 0xc0, 
  0x1c, 0x00, 0x00, 0x00, 0x03, 0x80, 0x1e, 0x00, 0x00, 0x00, 0x07, 0x80, 0x0f, 0x00, 0x00, 0x00, 
  0x0f, 0x00, 0x07, 0x80, 0x00, 0x00, 0x1e, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x3e, 0x00, 0x03, 0xe0, 
  0x00, 0x00, 0x7c, 0x00, 0x01, 0xf8, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x7e, 0x00, 0x07, 0xe0, 0x00, 
  0x00, 0x3f, 0xc0, 0x3f, 0xc0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 
  0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00
};


#define ICON_CO2_W 18
#define ICON_CO2_H 12

static const unsigned char ICON_CO2[] PROGMEM = {
  0x00, 0x78, 0x00, 0x00, 0xfc, 0x00, 0x0f, 0xfe, 0x00, 0x1f, 0xfe, 0x00, 0x3f, 0xff, 0x80, 0x7f, 
  0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0x7f, 0xff, 0x80, 0x07, 0xf8, 
  0x00, 0x07, 0x30, 0x00
};


#define ICON_TEMP_W 8
#define ICON_TEMP_H 18

static const unsigned char ICON_TEMP[] PROGMEM = {
  0x18, 0x24, 0x42, 0x42, 0x42, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x99, 0xbd, 0xbd, 0x99, 
  0x42, 0x3c
};


#define ICON_HUM_W 11
#define ICON_HUM_H 14

static const unsigned char ICON_HUM[] PROGMEM = {
  0x04, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x31, 0x80, 0x66, 0xc0, 0x62, 0xc0, 0xc1, 0x60, 0xc0, 0x60, 
  0xc0, 0x60, 0xc0, 0x60, 0xc0, 0x60, 0x60, 0xc0, 0x3f, 0x80, 0x1f, 0x00
};



#define SCREEN_WIDTH                128
#define SCREEN_HEIGHT               64
#define SCREEN_RESET                33
#define SCREEN_ADDRESS              0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);


#define AIR_MEASUREMENT_INTERVAL 2

Adafruit_SCD30  air;


#define SENSOR_ADDRESS              0x77
#define SENSOR_SEALEVELPRESSURE_HPA 1013.25

Adafruit_BME280 sensor;


#define SEMAPHORE_IO                32
#define SEMAPHORE_LEDS              3

Adafruit_NeoPixel semaphore = Adafruit_NeoPixel(SEMAPHORE_LEDS, SEMAPHORE_IO, NEO_GRB + NEO_KHZ800);


#define MARGIN 5
#define LOADTIME 5000


void abort() {
  while (true) { delay(10); }
}


void loadscreen(const __FlashStringHelper* message) {
  int16_t  x1, y1;
  uint16_t w, h;

  display.clearDisplay();

  display.drawBitmap(
    (SCREEN_WIDTH - ICON_KURALABS_W) / 2,
    MARGIN,
    ICON_KURALABS,
    ICON_KURALABS_W,
    ICON_KURALABS_H,
    SSD1306_WHITE
  );

  display.setTextSize(1);
  display.getTextBounds(message, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(
    (SCREEN_WIDTH - w) / 2,
    (SCREEN_HEIGHT - h)
  );
  display.println(message);

  display.display();
}


void updateAirSensor() {
  if (air.dataReady()) {
    if (!air.read()){
      Serial.println("ERROR: Error reading sensor data!");
    }
  }
}


void setup() {
  uint16_t time = millis();

  // Initialize serial interface
  Serial.begin(115200);
  while (!Serial) { delay(10); }
  delay(500); // Pause for half second so that serial console can reconnect

  Serial.println(HEADER);

  // Initialize display
  Serial.println(F("STATUS: Initializing display ..."));

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("ERROR: Failed to initialize display!"));
    abort();
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);
  display.display();

  loadscreen(F("Iniciando ..."));

  Serial.println(F("STATUS: Display initialized"));

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

  updateAirSensor();

  Serial.println(F("STATUS: Air quality sensor initialized"));

  // Initialize barometric, humidity and temperature sensor
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

  delay(LOADTIME);
}


void loop() {

  updateAirSensor();

  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(ICON_CO2_W + 2 * MARGIN, MARGIN);
  display.print(air.CO2, 1);
  display.setTextSize(1);
  display.println(" ppm");

  display.setTextSize(2);
  display.setCursor(ICON_CO2_W + 2 * MARGIN, ICON_CO2_H + 2 * MARGIN);
  display.print(sensor.readTemperature(), 1);
  display.setTextSize(1);
  display.println(" \xf7\x43");

  display.setTextSize(2);
  display.setCursor(ICON_CO2_W + 2 * MARGIN, ICON_CO2_H + ICON_TEMP_H + 3 * MARGIN);
  display.print(sensor.readHumidity(), 1);
  display.setTextSize(1);
  display.println(" %");
  
  display.drawBitmap(MARGIN, MARGIN, ICON_CO2, ICON_CO2_W, ICON_CO2_H, SSD1306_WHITE);
  display.drawBitmap(MARGIN + (ICON_CO2_W - ICON_TEMP_W) / 2, ICON_CO2_H + 2 * MARGIN, ICON_TEMP, ICON_TEMP_W, ICON_TEMP_H, SSD1306_WHITE);
  display.drawBitmap(MARGIN + (ICON_CO2_W - ICON_HUM_W) / 2, ICON_CO2_H + ICON_TEMP_H + 3 * MARGIN, ICON_HUM, ICON_HUM_W, ICON_HUM_H, SSD1306_WHITE);

  display.display();
  delay(1000);

  /*
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
  */
}
