#include <SPI.h>
#include <Ethernet.h>
#include "EmonLib.h"                    // Include Emon Library
EnergyMonitor emon1;                    // Create an instance
EnergyMonitor emon2;
double value2;
double value1;

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);      // numeric IP for Google (no DNS)
char server[] = "165.227.105.231";       // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  emon1.current(1, 63);                 // Current: input pin, calibration.
  emon2.current(2, 63);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:

  Serial.println("connecting...");

  // if you get a connection, report back via serial:

}

void current()
{
  double Irms1 = emon1.calcIrms(1480);  // Calculate Irms only
  Irms1= Irms1-0.03;
  double power1 = Irms1*230;
  Serial.print("Device 1: ");
  Serial.print(power1);                 // Apparent power
  Serial.print("  ");
  Serial.print(Irms1);                  // Irms
  value1 = (power1/360000);
  Serial.print("  ");
  Serial.println(value1,8);
  value1 = value1*100000;

  double Irms2 = emon2.calcIrms(1480);  // Calculate Irms only
  Irms2= Irms2-0.03;
  double power2 = Irms2*230;
  Serial.print("Device 2: ");
  Serial.print(power2);                 // Apparent power
  Serial.print(" ");
  Serial.print(Irms2);                  // Irms
  value2 = (power2/360000);
  Serial.print("  ");
  Serial.println(value2,8);
  value2 = value2*100000;
}

void loop() {
      current();
   // if there are incoming bytes available
  // from the server, read them and print them:
  String temp1="GET /write_data1.php?application_id=12&Service_id=23&units=";
  temp1.concat(value1);
  String temp2="GET /write_data1.php?application_id=13&Service_id=23&units=";
  temp2.concat(value2);
  String temp3="GET /sangam/arduinoRelay.php";


    if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println(temp1);
      client.println("HTTP/1.1");
      client.println("Host: 165.227.105.231");
      client.println();
      client.println("Connection: close");
      client.stop();

  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }


  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println(temp2);
      client.println("HTTP/1.1");
      client.println("Host: 165.227.105.231");
      
      client.println();
     
      client.println("Connection: close");
      client.stop();

  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  

  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println(temp3);
      client.println("HTTP/1.1");
      client.println("Host: 165.227.105.231");
      
      client.println();
    delay(500);
    while(client.available())
    {
          char c = client.read();
          Serial.println(c);
        if (c == '0')
        {
          digitalWrite(13,LOW);
        }
       else if (c=='1')
        {
         digitalWrite(13,HIGH);
        }
        else if (c=='2')
        {
          
         digitalWrite(7,LOW);
        }
        else if (c=='3')
        {
         digitalWrite(7,HIGH);
        }
    }

      client.println("Connection: close");
      client.stop();

  } 
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  
  delay(9000);
}
