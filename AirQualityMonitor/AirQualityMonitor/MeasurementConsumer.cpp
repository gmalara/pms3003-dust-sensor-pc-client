#include "MeasurementConsumer.h"
#include <iostream>


ConsolePrinter::ConsolePrinter(AirQualityMonitor& aqm):aqm_(aqm){}
ConsolePrinter::~ConsolePrinter(){}
void ConsolePrinter::Process(Measurements& m) {
  std::cout << "PM10 " << m.getPm10() << " \tPM25: " << m.getPm25() << std::endl;
  
}
void ConsolePrinter::Subscribe(IMeasurementConsumer::UpdateHandler::slot_type update) {
  aqm_.SubscribeObserver([this](Measurements& m)
  {
    Process(m);
  });
}
