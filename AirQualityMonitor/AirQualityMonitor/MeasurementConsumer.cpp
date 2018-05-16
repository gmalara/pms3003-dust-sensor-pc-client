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

    chart_trace_25_.reset(&chart.AddRealTimeTrace(100));
    chart_trace_25_->color(colors::blue);
    chart_trace_25_->setLabel("PM25");

    chart_trace_10_.reset(&chart.AddRealTimeTrace(100));
    chart_trace_10_->color(colors::red);
    chart_trace_10_->setLabel("PM10");


    // show and run
    fm.show();
    exec();
  });

  viewLoop_.wait();
}

ChartsDrawer::ChartsDrawer(IAirQualityMonitor& aqm): MeasurementConsumerBase(aqm) {
  StartViewThread();
  Subscribe();
}

ChartsDrawer::~ChartsDrawer() {}

void ChartsDrawer::Process(const Measurements& m) {
  if (chart_trace_25_)
    chart_trace_25_->add(m.getPm25());

  if (chart_trace_10_)
    chart_trace_10_->add(m.getPm10());
}
