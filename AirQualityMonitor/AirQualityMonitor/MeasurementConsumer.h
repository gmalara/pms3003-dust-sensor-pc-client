#pragma once
#include "AirQualityMonitor.h"
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>

class IMeasurementConsumer {
public:
  virtual ~IMeasurementConsumer() = default;
  using UpdateHandler = boost::signals2::signal<void(Measurements&)>;

  virtual void Process(Measurements&) = 0;
  //virtual void Subscribe(/*UpdateHandler::slot_type update*/) = 0;
};

class MeasurementConsumerBase : public IMeasurementConsumer {
protected:
  MeasurementConsumerBase(AirQualityMonitor& aqm);
  void Subscribe(/*UpdateHandler::slot_type update*/);

private:
  AirQualityMonitor& aqm_;
};

class ConsolePrinter : public MeasurementConsumerBase {
public:
  ConsolePrinter(AirQualityMonitor& aqm);
  ~ConsolePrinter();
  void Process(Measurements&) override;

};


class ChartsDrawer : public MeasurementConsumerBase {

public:
  void StartViewThread();
  ChartsDrawer(AirQualityMonitor& aqm);
  void Process(Measurements&) override;
private:
  std::future<void> viewLoop_;

};
