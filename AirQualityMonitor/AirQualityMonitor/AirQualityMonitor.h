#pragma once
#include "Measurements.h"
#include <boost/signals2.hpp>
#include <future>


class IAirQualityMonitor {
public:
  using UpdateHandler = boost::signals2::signal<void(Measurements&)>;

  virtual void Start() = 0;
  virtual void PublishMeasurements(Measurements&) = 0;
  virtual void SubscribeObserver(UpdateHandler::slot_type update) = 0;
};


class AirQualityMonitor : public IAirQualityMonitor {
  public:
  AirQualityMonitor();
  ~AirQualityMonitor();
  void Start()override;
  void PublishMeasurements(Measurements&)override;
  void SubscribeObserver(IAirQualityMonitor::UpdateHandler::slot_type update)override;

private:

  std::atomic<bool> shuttingDown_{false};
  std::future<void> deviceCommunication_;
  UpdateHandler onUpdate;
  void RunLoop();

};