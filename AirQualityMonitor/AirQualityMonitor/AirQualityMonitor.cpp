#include "AirQualityMonitor.h"
#include "libusb.h"
#include <iostream>

namespace {
using namespace std;
const auto kEndpointAddress = 1;
const int kBuff_length = 24;
const uint16_t kExpected_sensor_id = 60000;
const uint16_t kFirstByteOfSequence = 0x42;


void DescribeDevice(libusb_device_handle* dev_handle, libusb_device_descriptor desc) {
  unsigned char data[200] = "\0";
  if (dev_handle) {
    libusb_get_string_descriptor_ascii(dev_handle, desc.iProduct, data, 200);
    cout << "Product: " << data << "\n";
    memcpy(data, "\0", 100);
    libusb_get_string_descriptor_ascii(dev_handle, desc.iManufacturer, data, 200);
    cout << "Manufacturer: " << data << "\n";
    memcpy(data, "\0", 100);
    libusb_get_string_descriptor_ascii(dev_handle, desc.iSerialNumber, data, 200);
    cout << "SerialNo: " << data << "\n\n\n\n";
    memcpy(data, "\0", 100);
  }
}

libusb_device_handle* OpenDevice(libusb_context* ctx, libusb_device_descriptor desc) {
  cout << "Opening device, product: " << desc.idProduct << ", vendor: " << desc.idVendor << endl;
  auto dev_handle = libusb_open_device_with_vid_pid(ctx, desc.idVendor, desc.idProduct);
  if (dev_handle == NULL)
    cout << "Cannot open device" << endl;

  DescribeDevice(dev_handle, desc);
  return dev_handle;
}

bool DeviceMatch(const libusb_device_descriptor& desc) {
  auto deviceMatch = desc.idProduct == kExpected_sensor_id;
  cout << boolalpha << "DeviceMatch: " << deviceMatch << endl;
  return deviceMatch;
}

pair<bool, libusb_device_descriptor> FindDeviceAndPrintInfo(libusb_context* ctx, libusb_device* dev) {
  libusb_device_descriptor desc;
  int r = libusb_get_device_descriptor(dev, &desc);

  if (r < 0) {
    cout << "failed to get device descriptor" << endl;
    return make_pair(false, desc);
  }
  if (!DeviceMatch(desc))
    return make_pair(false, desc);

  cout << "Number of possible configurations: " << (int)desc.bNumConfigurations << " \n";
  cout << "Device Class: " << (int)desc.bDeviceClass << " \n";
  cout << "VendorID: " << desc.idVendor << "  \n";
  cout << "ProductID: " << desc.idProduct << endl;
  cout << "SerialNo: " << desc.iSerialNumber << endl;

  libusb_config_descriptor* config = nullptr;
  r = libusb_get_config_descriptor(dev, 0, &config);
  if (r < 0)
    cout << "Failed to get device config" << endl;
  else {
    cout << "Interfaces: " << (int)config->bNumInterfaces << " |||\n";

    const libusb_interface* inter;
    const libusb_interface_descriptor* interdesc;
    const libusb_endpoint_descriptor* epdesc;

    for (int i = 0; i < (int)config->bNumInterfaces; i++) {
      inter = &config->interface[i];
      cout << "Number of alternate settings: " << inter->num_altsetting << " \n";

      for (int j = 0; j < inter->num_altsetting; j++) {
        interdesc = &inter->altsetting[j];
        cout << "Interface Number: " << (int)interdesc->bInterfaceNumber << " \n";
        cout << "Number of endpoints: " << (int)interdesc->bNumEndpoints << " \n";

        for (int k = 0; k < (int)interdesc->bNumEndpoints; k++) {
          epdesc = &interdesc->endpoint[k];
          cout << "Descriptor Type: " << (int)epdesc->bDescriptorType << " \n";
          cout << "End Point Address: " << (int)epdesc->bEndpointAddress << " \n";
        }
      }
      cout << "---------" << endl;
    }
  }

  libusb_free_config_descriptor(config);

  return make_pair(true, desc);
}
}

void AirQualityMonitor::PublishMeasurements(Measurements& m) { onUpdate(m); }

void AirQualityMonitor::SubscribeObserver(IAirQualityMonitor::UpdateHandler::slot_type update) { onUpdate.connect(update); }

AirQualityMonitor::AirQualityMonitor() { }

AirQualityMonitor::~AirQualityMonitor() {
  onUpdate.disconnect_all_slots();
  shuttingDown_.store(true);
}

void AirQualityMonitor::Start(){
  RunLoop();
}

void AirQualityMonitor::RunLoop() {
  deviceCommunication_ = std::async(std::launch::async, [this]()
  {
    libusb_device** devs; //pointer to pointer of device, used to retrieve a list of devices
    libusb_context* ctx = nullptr; //a libusb session

    int r; //for return values
    ssize_t cnt; //holding number of devices in list
    r = libusb_init(&ctx); //initialize a library session
    if (r < 0) {
      std::cout << "Init Error " << r << std::endl; //there was an error
      return;
    }
    libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_WARNING); //set verbosity level to 3
    cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
    if (cnt < 0)
      cout << "Get Device Error" << std::endl; //there was an error

    cout << cnt << " Devices in list." << endl; //print total number of usb devices
    ssize_t i; //for iterating through the list

    for (i = 0; i < cnt; i++) {
      auto successDescriptorPair = FindDeviceAndPrintInfo(ctx, devs[i]);
      auto deviceFound = successDescriptorPair.first;
      if (deviceFound) {
        auto dev_handle = OpenDevice(ctx, successDescriptorPair.second);
        libusb_claim_interface(dev_handle, 0);
        struct libusb_transfer* transfer = libusb_alloc_transfer(0);
        unsigned char buf[kBuff_length * 2];
        int actualLength = kBuff_length * 2;

        DescribeDevice(dev_handle, successDescriptorPair.second);

        while (!shuttingDown_) {
          std::this_thread::sleep_for(std::chrono::milliseconds(500));

          auto ret = libusb_bulk_transfer(dev_handle, kEndpointAddress | LIBUSB_ENDPOINT_IN, buf, kBuff_length * 2, &actualLength, 5000);
          Measurements m;
          if (m.HandleIncomingData(buf, actualLength))
            PublishMeasurements(m);
        }

        cout << boolalpha << "Closing Device. \n ";
        libusb_close(dev_handle);

        break;
      }

      libusb_free_device_list(devs, 1); //free the list, unref the devices in it
      libusb_exit(ctx); //close the session

      // Exit
      cout << endl << "Finnished.";
      return;
    }
  });
}
