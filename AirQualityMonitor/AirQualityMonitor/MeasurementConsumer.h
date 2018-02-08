#pragma once
#include "AirQualityMonitor.h"

class IMeasurementConsumer {
public:
  virtual ~IMeasurementConsumer() = default;
  using UpdateHandler = boost::signals2::signal<void(Measurements&)>;

  virtual void Process(const Measurements&) = 0;
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
  void Process(const Measurements&) override;

};

namespace nana {
namespace plot {class trace; }
}//forward decl

class ChartsDrawer : public MeasurementConsumerBase {

public:
  void StartViewThread();
  ChartsDrawer(AirQualityMonitor& aqm);
  ~ChartsDrawer();
  void Process(const Measurements&) override;
private:
  std::future<void> viewLoop_;
  std::shared_ptr<nana::plot::trace> chart_trace_;
};
