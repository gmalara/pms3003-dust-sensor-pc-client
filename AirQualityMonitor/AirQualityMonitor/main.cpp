#include "AirQualityMonitor.h" //TODO remove preheaders
#include <iostream>
#include "libusb.h"

namespace{
  using namespace std;
  const auto kEndpointAddress = 1;
  const int kBuff_length = 24;
  const uint16_t kExpected_sensor_id = 60000;
  const uint16_t kFirstByteOfSequence = 0x42;
}



int getPm25TSI(unsigned char *buf) 
{
  int PM25v;
  PM25v = ((buf[6] << 8) + buf[7]);
  return PM25v;
}

int getPM10TSI(unsigned char *buf) 
{
  int PM10v;
  PM10v = ((buf[8] << 8) + buf[9]);
  return PM10v;
}

int getPM25real(unsigned char *buf) 
{
  int PM25v;
  PM25v = ((buf[12] << 8) + buf[13]);
  return PM25v;
}

int getPM10real(unsigned char *buf) 
{
  int PM10v;
  PM10v = ((buf[14] << 8) + buf[15]);
  return PM10v;
}


bool checkDataCRC(unsigned char *buf, int length) 
{
  bool flag = false;
  int sum = 0;

  for (int i = 0; i < (length - 3); i++)
    sum += buf[i];

  if (sum == ((buf[length - 2] << 8) + buf[length - 1]))
  {
    sum = 0;
    flag = true;
  }
  return flag;
}

void HandleIncomingData(unsigned char* buf, int length) {
  int startFound = -1;//not found flag
  for (int i = 0; i < length; ++i) 
    if (int(buf[i]) == kFirstByteOfSequence && startFound < 0)
      startFound = i;

  auto rightBuffer = &buf[startFound];
  auto dataok = checkDataCRC(rightBuffer, kBuff_length);
  if (!dataok) return;
  cout << "PM10 " << getPM10real(rightBuffer) << " \tPM25: " << getPM25real(rightBuffer) << endl;
}

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
void DescribeDeviceAndLoopForTheData(libusb_context* ctx, libusb_device_descriptor desc) {
  cout << "Opening device, product: " << desc.idProduct << ", vendor: " << desc.idVendor << endl;
  auto dev_handle = libusb_open_device_with_vid_pid(ctx, desc.idVendor, desc.idProduct);
  if (dev_handle == NULL)
    cout << "Cannot open device" << endl;

  DescribeDevice(dev_handle, desc);

    libusb_claim_interface(dev_handle, 0);
    struct libusb_transfer* transfer = libusb_alloc_transfer(0);
    unsigned char buf[kBuff_length * 2];
    int actualLength = kBuff_length * 2;

    while (1) {
      Sleep(500);
      auto ret = libusb_bulk_transfer(dev_handle, kEndpointAddress | LIBUSB_ENDPOINT_IN, buf, kBuff_length * 2, &actualLength, 5000);
      HandleIncomingData(buf, actualLength);
    }
  
  cout << boolalpha << "Closing Device. \n ";
  libusb_close(dev_handle);
}

bool DeviceMatch(const libusb_device_descriptor& desc) {
  auto deviceMatch = desc.idProduct == kExpected_sensor_id;
  cout << boolalpha << "DeviceMatch: " << deviceMatch << endl;
  return  deviceMatch;
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

int main() {
  libusb_device** devs; //pointer to pointer of device, used to retrieve a list of devices
  libusb_context* ctx = nullptr; //a libusb session

  int r; //for return values
  ssize_t cnt; //holding number of devices in list
  r = libusb_init(&ctx); //initialize a library session

  if (r < 0) {
    cout << "Init Error " << r << endl; //there was an error
    return 1;
  }

  libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_WARNING);  //set verbosity level to 3

  cnt = libusb_get_device_list(ctx, &devs); //get the list of devices

  if (cnt < 0)
    cout << "Get Device Error" << endl; //there was an error

  cout << cnt << " Devices in list." << endl; //print total number of usb devices
  ssize_t i; //for iterating through the list

  for (i = 0; i < cnt; i++) {
    auto successDescriptorPair = FindDeviceAndPrintInfo(ctx, devs[i]);
    auto deviceFound = successDescriptorPair.first;
    if (deviceFound) {
      DescribeDeviceAndLoopForTheData(ctx, successDescriptorPair.second);
      break;
    }

    libusb_free_device_list(devs, 1); //free the list, unref the devices in it
    libusb_exit(ctx); //close the session

    // Exit
    cout << endl << "Finnished.";
    cin.get();
    return 0;
  }
  
}
