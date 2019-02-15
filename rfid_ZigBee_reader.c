#include <AddicoreRFID.h>
#include <SPI.h>
#include <Printers.h>
#include <XBee.h>
#include <HX711.h>

// xbee define
#define uchar unsigned char
#define uint unsigned int
#define MAX_LEN 16

// Xbee setting
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();
bool work_on = false;

// RFID setting
bool standby = true;
uchar serNumA[5];
uchar fifobytes;
uchar fifoValue;
AddicoreRFID myRFID;
const int chipSelectPin = 10; // RFID : SDA
const int NRSTPD = 9; // RFID : RESET

void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial); // xbee setting

  // RFID Init
  SPI.begin();
  pinMode(chipSelectPin,OUTPUT);
  digitalWrite(chipSelectPin, LOW);
  pinMode(NRSTPD, OUTPUT);
  digitalWrite(NRSTPD, HIGH);
  myRFID.AddicoreRFID_Init();
}

void loop()
{
  // define variable
  uchar i, tmp, checksum1, status, blockAddr, RC_size;
  uchar str[MAX_LEN];
  String mynum = "";
  str[1] = 0x4400;
  status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str);

  // RIFD detected
  if (status == MI_OK) {
    status = myRFID.AddicoreRFID_Anticoll(str);
    // check RIFD
    if (str[0] == 30 && str[1] == 176 && str[2] == 100 && str[3] == 163){
      Serial.println("running");
    // xbee send (coordinator <- router)
    // code reference : Series2_Tx(XBee-Arduino library)
      uint8_t payload[] = {'K'}; // plate id
      XBeeAddress64 addr64 = XBeeAddress64(0x0013a200,0x418da13f);
      ZBTxRequest tx = ZBTxRequest(addr64, payload, sizeof(payload));
      xbee.send(tx);
      Serial.println(":  done");
      delay(5000);
      }
   }
}
