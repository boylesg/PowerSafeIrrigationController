#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifdef __SAM3X8E__
  const uint8_t MAX_STATIONS = 16;
#else
  const uint8_t MAX_STATIONS = 8;
#endif
const uint8_t MAX_STARTS = 12;

#endif

#define fstr_HTTP_CLOCK       F("clock.htm")
#define fstr_HTTP_WALARMS     F("walarms.htm")
#define fstr_HTTP_PROGRAM     F("program.htm")
#define fstr_HTTP_EMAIL       F("email.htm")
#define fstr_HTTP_MANUAL      F("manual.htm")
#define fstr_FAV_ICON         F("Icon.png")
#define fstr_TXT_WIFI         F("wifi.txt")
#define fstr_TXT_PROGRAM      F("program.txt")
#define fstr_TXT_WALARMS      F("walarms.txt")
#define fstr_TXT_EMAIL        F("email.txt")
#define fstr_TXT_TIMEZONE     F("timezone.txt")
#define fstr_TXT_PROG_PAGE    F("progdata.txt")
#define fstr_TXT_LOG_FILE     F("log.txt")

#define LOG

