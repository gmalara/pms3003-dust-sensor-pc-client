#pragma once

using MeasuremetUnit = int;
class Measurements {
public:
  MeasuremetUnit getPm25() {
    return PM25v_;
  }

    MeasuremetUnit getPm10() {
    return PM10v_;
  }

  int setPm25TSI(unsigned char *buf) ;
  int setPM10TSI(unsigned char *buf) ;
  int setPM25real(unsigned char *buf) ;
  int setPM10real(unsigned char *buf) ;
  static bool checkDataCRC(unsigned char *buf, int length) ;
  void HandleIncomingData(unsigned char* buf, int length);
private:
  MeasuremetUnit PM25v_;
  MeasuremetUnit PM10v_;

};
