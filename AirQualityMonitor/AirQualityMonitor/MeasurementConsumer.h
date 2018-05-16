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
  MeasurementConsumerBase(IAirQualityMonitor& aqm);

  void Subscribe(/*UpdateHandler::slot_type update*/);

private:
  IAirQualityMonitor& aqm_;
};

class ConsolePrinter : public MeasurementConsumerBase {
public:
  ConsolePrinter(IAirQualityMonitor& aqm);
  void Process(const Measurements&) override;

};

namespace nana {
namespace plot {
class trace;
}
} //forward decl

class ChartsDrawer : public MeasurementConsumerBase {

public:
  ChartsDrawer(IAirQualityMonitor& aqm);
  ChartsDrawer(const ChartsDrawer&) = delete;
  ~ChartsDrawer();
  void Process(const Measurements&) override;
private:
  void StartViewThread();
  std::shared_ptr<nana::plot::trace> chart_trace_25_;
  std::shared_ptr<nana::plot::trace> chart_trace_10_;
  std::future<void> viewLoop_;
};
