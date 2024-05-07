#ifndef N_DEVICE_H
#define N_DEVICE_H

#if IDF_TARGET = "esp32"
  #define N_BOARD  "Base board"
#elif IDF_TARGET = "esp32s3"
  #define N_BOARD  "Heltec"
#else
  #error "Unknown target"
#endif

#endif /* end of include guard: N_DEVICE_H */
