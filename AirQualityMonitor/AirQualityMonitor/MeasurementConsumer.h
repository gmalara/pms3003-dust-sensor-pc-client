#pragma once
#include "AirQualityMonitor.h"


class IMeasurementConsumer {
public:
  using UpdateHandler = boost::signals2::signal<void(Measurements&)>;

  virtual void Process(Measurements&) = 0;
  virtual void Subscribe(UpdateHandler::slot_type update) = 0;
};


class ConsolePrinter : public IMeasurementConsumer {
  public:
  ConsolePrinter(AirQualityMonitor& aqm);
  ~ConsolePrinter();
  void Process(Measurements&) override;
  void Subscribe(UpdateHandler::slot_type update) override;
private:
  AirQualityMonitor& aqm_;
};