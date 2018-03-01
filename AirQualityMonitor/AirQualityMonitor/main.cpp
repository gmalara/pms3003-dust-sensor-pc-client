#include "AirQualityMonitor.h" 
#include "MeasurementConsumer.h" 
#include <iostream>

namespace{
  using namespace std;
  const auto kEndpointAddress = 1;
  const int kBuff_length = 24;
  const uint16_t kExpected_sensor_id = 60000;
  const uint16_t kFirstByteOfSequence = 0x42;
}


int main() {
  AirQualityMonitor aqm;

  ChartsDrawer charts_drawer(aqm);
  ConsolePrinter console_printer(aqm);
  return 0;
}
