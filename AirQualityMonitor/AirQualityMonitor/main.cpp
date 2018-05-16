#include "AirQualityMonitor.h" 
#include "MeasurementConsumer.h" 
#include <iostream>

namespace{
  const auto kEndpointAddress = 1;
  const int kBuff_length = 24;
  const uint16_t kExpected_sensor_id = 60000;
  const uint16_t kFirstByteOfSequence = 0x42;
}


int main() {
  AirQualityMonitor aqm;
  aqm.Start();

  ConsolePrinter console_printer(aqm);
  ChartsDrawer charts_drawer(aqm);

  std::string nothing;
  getline(std::cin, nothing);

  return 0;
}
