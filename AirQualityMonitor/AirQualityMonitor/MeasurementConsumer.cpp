#include "MeasurementConsumer.h"
#include <iostream>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include "nana/gui/widgets/picture.hpp"
#include "nana/plot/plot.h"
#include "nana/gui/timer.hpp"

MeasurementConsumerBase::MeasurementConsumerBase(IAirQualityMonitor& aqm): aqm_(aqm) {}

ConsolePrinter::ConsolePrinter(IAirQualityMonitor& aqm): MeasurementConsumerBase(aqm) { Subscribe(); }

void ConsolePrinter::Process(const Measurements& m) { std::cout << "PM10 " << m.getPm10() << " \tPM25: " << m.getPm25() << std::endl; }
void MeasurementConsumerBase::Subscribe() { aqm_.SubscribeObserver([this](Measurements& m) { Process(m); }); }

///////////////////////////

void ChartsDrawer::StartViewThread() {
  using namespace nana;

  viewLoop_ = std::async(std::launch::async, [&]()
  {
    form fm;

    plot::plot chart(fm);
    chart_trace_.reset(&chart.AddRealTimeTrace(100));
    chart_trace_->color(colors::blue);
    // create timer to provide new data regularly
    timer theTimer;
    theTimer.interval(10);
    theTimer.elapse([ this ]()
    {
      static int p = 0;
      chart_trace_->add(10 * sin(p++ / 10.0));
    });
    theTimer.start();

    // show and run
    fm.show();
    exec();
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  //viewLoop_.wait();
}

ChartsDrawer::ChartsDrawer(IAirQualityMonitor& aqm): MeasurementConsumerBase(aqm) {
  StartViewThread();
  Subscribe();
}

ChartsDrawer::~ChartsDrawer() {}

void ChartsDrawer::Process(const Measurements& m) {
  if (chart_trace_)
    chart_trace_->add(1100/* m.getPm25()*/);
}
