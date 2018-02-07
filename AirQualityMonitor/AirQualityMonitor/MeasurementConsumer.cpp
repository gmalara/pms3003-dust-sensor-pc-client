#include "MeasurementConsumer.h"
#include <iostream>


MeasurementConsumerBase::MeasurementConsumerBase(AirQualityMonitor& aqm): aqm_(aqm) {}

ConsolePrinter::ConsolePrinter(AirQualityMonitor& aqm): MeasurementConsumerBase(aqm) { Subscribe(); }
ConsolePrinter::~ConsolePrinter() {}

void ConsolePrinter::Process(Measurements& m) { std::cout << "PM10 " << m.getPm10() << " \tPM25: " << m.getPm25() << std::endl; }
void MeasurementConsumerBase::Subscribe() { aqm_.SubscribeObserver([this](Measurements& m) { Process(m); }); }

///////////////////////////

void ChartsDrawer::StartViewThread() {
  viewLoop_ = std::async(std::launch::async, [this]()
  {
    using namespace nana;
    form fm;
    drawing dw(fm);
    dw.draw([](paint::graphics& graph)
    {
        graph.rectangle(rectangle{5, 5, 50, 50}, true, colors::red );
        graph.line(point(5, 5), point(55, 55), colors::white);
    });

    fm.caption("Pollution Chart");
    fm.size(size(900,400));
    //Define a label and display a text.
   // label lab{fm, "X asis"};
    //lab.format(true);

    //Define a button and answer the click event.
   // button btn{fm, "Quit"};
   // btn.events().click([&fm] { fm.close(); });

    //Layout management
   // fm.div("vert <><<><weight=80% text><>><><weight=24<><button><>><>");
 //   fm.div("vert<chart weight=80><text weight=20>");
  //  fm["chart"] << btn;
  //  fm["text"] << lab;
  //  fm.collocate();

    dw.update();

    //Show the form
    fm.show();

    //Start to event loop process, it blocks until the form is closed.
    nana::exec();
  });
  viewLoop_.wait();
}

ChartsDrawer::ChartsDrawer(AirQualityMonitor& aqm): MeasurementConsumerBase(aqm) {
  Subscribe();


  StartViewThread();
}

void ChartsDrawer::Process(Measurements&) { }
