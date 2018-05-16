#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "trompeloeil.hpp"
#include "AirQualityMonitor/AirQualityMonitor/MeasurementConsumer.h"

constexpr unsigned char kProperPMI[] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf,16};

class MeasurementConsumerMock : public MeasurementConsumerBase {

public:
  MeasurementConsumerMock(IAirQualityMonitor& aqm):MeasurementConsumerBase(aqm){} 
  MAKE_MOCK1(Process, void (const Measurements&));
};


TEST_CASE("Measurement consumer processes data")
{
    AirQualityMonitor aqm;
    Measurements m;
    MeasurementConsumerMock consumer_mock(aqm);
    int call_counter = 0;
   // REQUIRE_CALL(consumer_mock, Process(trompeloeil::_));
    aqm.PublishMeasurements(m);
}



TEST_CASE("Air quality subscriptions")
{
    AirQualityMonitor aqm;
    Measurements m;

    int call_counter = 0;
    aqm.SubscribeObserver([&call_counter](Measurements& m) { ++call_counter; });
    aqm.PublishMeasurements(m);
    REQUIRE(call_counter == 1);


    SECTION("new subscription added")
    {
        aqm.SubscribeObserver([&call_counter](Measurements& m) { ++call_counter; });
        aqm.PublishMeasurements(m);

        REQUIRE(call_counter == 3);
    }

    SECTION("received measurement has correct value")
    {
        bool was_executed = false;
        m.setPM10real(kProperPMI);
        aqm.SubscribeObserver([&was_executed](Measurements& m) { 
           REQUIRE(m.getPm10() == 0xe0f);
           was_executed = true;
        });
        aqm.PublishMeasurements(m);

        REQUIRE(was_executed == true);

    }

}
