#pragma once

using MeasuremetUnit = int;

class Measurements {
public:
  MeasuremetUnit getPm25() const { return PM25v_; }

  MeasuremetUnit getPm10() const { return PM10v_; }

  int setPm25TSI(const unsigned char* buf);
  int setPM10TSI(const unsigned char* buf);
  int setPM25real(const unsigned char* buf);
  int setPM10real(const unsigned char* buf);
  static bool checkDataCRC(unsigned char* buf, int length);
  bool HandleIncomingData(unsigned char* buf, int length);
private:
  MeasuremetUnit PM25v_{0};
  MeasuremetUnit PM10v_{0};

};
