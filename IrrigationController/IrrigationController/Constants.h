#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifdef __SAM3X8E__
  const uint8_t MAX_STATIONS = 16;
#else
  const uint8_t MAX_STATIONS = 8;
#endif
const uint8_t MAX_STARTS = 12;

#endif
