#include "MeasurementConsumer.h"
#include <iostream>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include "nana/gui/widgets/picture.hpp"


MeasurementConsumerBase::MeasurementConsumerBase(AirQualityMonitor& aqm): aqm_(aqm) {}

ConsolePrinter::ConsolePrinter(AirQualityMonitor& aqm): MeasurementConsumerBase(aqm) { Subscribe(); }
ConsolePrinter::~ConsolePrinter() {}

void ConsolePrinter::Process(Measurements& m) { std::cout << "PM10 " << m.getPm10() << " \tPM25: " << m.getPm25() << std::endl; }
void MeasurementConsumerBase::Subscribe() { aqm_.SubscribeObserver([this](Measurements& m) { Process(m); }); }

///////////////////////////

void ChartsDrawer::StartViewThread() {
  using namespace nana;

  draw_chart = [](nana::paint::graphics& graph)
  {
    graph.rectangle(rectangle{5, 5, 50, 50}, true, colors::red);
    graph.line(point(5, 5), point(55, 55), colors::white);
  };


  viewLoop_ = std::async(std::launch::async, [&]()
  {
    using namespace nana;
    std::shared_ptr<form> fm(new form);
    std::shared_ptr<picture> chart(new picture(*fm));
    (new drawing(*chart));
    chart->size(size(500, 300));
    chart->bgcolor(color(100, 100, 100));
    dw.reset(new drawing(*chart));
    dw->draw(draw_chart);

    fm->caption("Pollution Chart");
    fm->size(size(900, 400));
    //Define a label and display a text.
    label lab{*fm, "X asis"};
    lab.format(true);

    //Define a button and answer the click event.
    button btn{*fm, "Quit"};
    //btn.events().click([&fm] { fm.close(); });

    //Layout management
    // fm.div("vert <><<><weight=80% text><>><><weight=24<><button><>><>");
    fm->div("vert<chart weight=80><text weight=20>");
    (*fm)["chart"] << *chart;
    (*fm)["text"] << lab;
    //  fm.collocate();

    //Show the form
    fm->show();

    //Start to event loop process, it blocks until the form is closed.
    nana::exec();
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  if (dw)
    dw->update();

  viewLoop_.wait();
}

ChartsDrawer::ChartsDrawer(AirQualityMonitor& aqm): MeasurementConsumerBase(aqm) {
  Subscribe();


  StartViewThread();
}

ChartsDrawer::~ChartsDrawer() {}

void ChartsDrawer::Process(Measurements&) {
    if (dw)
    dw->update();
}
