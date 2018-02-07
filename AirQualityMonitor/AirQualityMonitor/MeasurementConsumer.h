#pragma once
#include "AirQualityMonitor.h"

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

namespace nana {
class drawing;
namespace paint {class graphics; }
}//forward decl

class ChartsDrawer : public MeasurementConsumerBase {

public:
  void StartViewThread();
  ChartsDrawer(AirQualityMonitor& aqm);
  ~ChartsDrawer();
  void Process(Measurements&) override;
private:
  std::future<void> viewLoop_;
  std::shared_ptr<nana::drawing> dw;
  std::function<void(nana::paint::graphics& graph)> draw_chart;
};
