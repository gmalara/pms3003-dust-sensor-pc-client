#include "AirQualityMonitor.h" //TODO remove preheaders
#include "libusb.h"

namespace{
  using namespace std;
  const auto kEndpointAddress = 1;
  const int kBuff_length = 24;
  const uint16_t kExpected_sensor_id = 60000;
  const uint16_t kFirstByteOfSequence = 0x42;
}


class Measurements {
};


void AirQualityMonitor::PublishMeasurements(Measurements& m) {
 // onUpdate(m);
}

void AirQualityMonitor::SubscribeObserver(IAirQualityMonitor::UpdateHandler::slot_type update) {
 // onUpdate.connect(update);
}

AirQualityMonitor::AirQualityMonitor() {
  RunLoop();
}

AirQualityMonitor::~AirQualityMonitor() {
  shuttingDown_.store(true);
}

void AirQualityMonitor::RunLoop() {
  loop_ = std::async(std::launch::async, [this](){ 
  while (!shuttingDown_) {
      Sleep(500);
      //auto ret = libusb_bulk_transfer(dev_handle, kEndpointAddress | LIBUSB_ENDPOINT_IN, buf, kBuff_length * 2, &actualLength, 5000);
      //HandleIncomingData(buf, actualLength);
    }});

  
}

