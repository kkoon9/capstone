#include <Printers.h>
#include <XBee.h>

// define xbee variable type, varialbe
#define uchar unsigned char
#define uint unsigned int
#define MAX_LEN 16

// Xbee commuication setting
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

void setup() {
  Serial.begin(9600);
  xbee.begin(Serial); // xbee setting
  }

void loop() {
  // init
  String detected;
  // read packet
  xbee.readPacket();
  if(xbee.getResponse().isAvailable())
  {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
    {
      xbee.getResponse().getZBRxResponse(rx);
      for (int i = 0;i < rx.getDataLength(); i++) {
        detected += (char)rx.getData(i);
      }
      
      // check plate
      if (detected == "K")
        Serial.println("K");
    
    }
    delay(5000);
  }
 }
