#include "Measurements.h"
#include <iostream>

namespace {
using namespace std;
const auto kEndpointAddress = 1;
const int kBuff_length = 24;
const uint16_t kExpected_sensor_id = 60000;
const uint16_t kFirstByteOfSequence = 0x42;
}


int Measurements::setPm25TSI(const unsigned char* buf) {
  PM25v_ = ((buf[6] << 8) + buf[7]);
  return PM25v_;
}

int Measurements::setPM10TSI(const unsigned char* buf) {
  PM10v_ = ((buf[8] << 8) + buf[9]);
  return PM10v_;
}

int Measurements::setPM25real(const unsigned char* buf) {
  PM25v_ = ((buf[12] << 8) + buf[13]);
  return PM25v_;
}

int Measurements::setPM10real(const unsigned char* buf) {
  PM10v_ = ((buf[14] << 8) + buf[15]);
  return PM10v_;
}

bool Measurements::checkDataCRC(unsigned char* buf, int length) {
  bool flag = false;
  int sum = 0;

  for (int i = 0; i < (length - 3); i++)
    sum += buf[i];

  if (sum == ((buf[length - 2] << 8) + buf[length - 1])) {
    sum = 0;
    flag = true;
  }
  return flag;
}

bool Measurements::HandleIncomingData(unsigned char* buf, int length) {
  int startFound = -1; //not found flag
  for (int i = 0; i < length; ++i)
    if (int(buf[i]) == kFirstByteOfSequence && startFound < 0)
      startFound = i;

  auto rightBuffer = &buf[startFound];
  auto dataok = checkDataCRC(rightBuffer, kBuff_length);
  if (dataok) {
    setPM10real(rightBuffer);
    setPM25real(rightBuffer);
  }
  return dataok;
}
