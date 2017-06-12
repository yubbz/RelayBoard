// This is a demo of the RBBB running as webserver with the Ether Card
// 2010-05-28 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <EtherCard.h>

#define relayOne 8
#define relayTwo 7
#define relayThree 6
#define relayFour 5

#define numberOfRelays 4
int RELAY[numberOfRelays] = {8, 7, 6, 5};

//All leds begin off, so the LEDs in the array are off except for the one which a given hyperlinks would turn on.
char RelayURL[numberOfRelays][numberOfRelays + 1] = { "1000", "0100", "0010", "0001"};

byte stateOne = 0;
byte stateTwo = 0;
byte stateThree = 0;
byte stateFour = 0;

const char http_OK[] PROGMEM =
  "HTTP/1.0 200 OK\r\n"
  "Content-Type: text/html\r\n"
  "Pragma: no-cache\r\n\r\n";

const char http_Found[] PROGMEM =
  "HTTP/1.0 302 Found\r\n"
  "Location: /\r\n\r\n";

const char http_Unauthorized[] PROGMEM =
  "HTTP/1.0 401 Unauthorized\r\n"
  "Content-Type: text/html\r\n\r\n"
  "<h1>401 Unauthorized</h1>";

const char htmlHeader[] PROGMEM =
  "<meta http-equiv='refresh' content='1'/>"
  "<title>Web Relay</title>";

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 };
static byte myip[] = { 10, 0, 0, 51 };
static int port = 8060;

byte Ethernet::buffer[1100];
BufferFiller bfill;

void setup () {
  //CS is connected to pin 10
  if (ether.begin(sizeof Ethernet::buffer, mymac, 10) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
  ether.staticSetup(myip);
  ether.hisport = port;

  for (byte i = 0; i < numberOfRelays; i++) {
    pinMode(RELAY[i], OUTPUT); //RELAY's are outputs
    digitalWrite(RELAY[i], LOW); //and are off to begin with
  }
}

static word homePage() {
  long t = millis() / 1000;
  word h = t / 3600;
  byte m = (t / 60) % 60;
  byte s = t % 60;
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
                 "$F" //HTTP_OK
                 "$F" //Header
    
                 "<h1 style=\"text-align:center; font-size:50px\">Web Relay Control</h1>"
                 "<br />"
                 "<br />"
                 
                 "<table  style=\"width:100%\">"
                 "<tr>"
                  "<td style=\"text-align:right; font-size:30px; width:50%\">"
                 "Relay $D: </td><td style=\"font-size:30px \"><a href=\"?relay=$S\">$S</a>"
                 "</td></tr><tr><td>&nbsp;</td></tr>"
                 "<td style=\"text-align:right; font-size:30px \">"                 
                 "Relay $D: </td><td style=\"font-size:30px \"><a href=\"?relay=$S\">$S</a>"
                 "</td></tr><tr><td>&nbsp;</td></tr>"
                 "<td style=\"text-align:right; font-size:30px \">"
                 "Relay $D: </td><td style=\"font-size:30px \"><a href=\"?relay=$S\">$S</a>"
                 "</td></tr><tr><td>&nbsp;</td></tr>"
                 "<td style=\"text-align:right; font-size:30px \">"
                 "Relay $D: </td><td style=\"font-size:30px \"><a href=\"?relay=$S\">$S</a>"
                 "</td></tr></table>"
                 
                 "<br />"
                 "<br />"
                 "<br />"
                 "<br />"
                 "<div style=\"text-align:center\">Uptime: $D$D:$D$D:$D$D</div>"
                 "<br />"
                 "<div style=\"text-align:center\">&#169;2016 - Kyle MacLachlan</div>"
                 "<div style=\"text-align:center\">kylemac06@gmail.com</div>"

               ),
               http_OK,
               htmlHeader,

               (word)1, RelayURL[0], (RelayURL[0][0] == '1') ? "OFF" : "ON",
               (word)2, RelayURL[1], (RelayURL[1][1] == '1') ? "OFF" : "ON",
               (word)3, RelayURL[2], (RelayURL[2][2] == '1') ? "OFF" : "ON",
               (word)4, RelayURL[3], (RelayURL[3][3] == '1') ? "OFF" : "ON",

               h / 10, h % 10, m / 10, m % 10, s / 10, s % 10);
  return bfill.position();
}

void loop () {
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

  // check if valid tcp data is received
  if (pos)
  {
    // write to LED digital output
    delay(10);   // necessary for my system
    bfill = ether.tcpOffset();
    char *data = (char *) Ethernet::buffer + pos;
    if (strncmp("GET /", data, 5) != 0) { //I also accidentally copied this bit in twice (though that shouldn't be an issue)
      //if (1 == 0) { //I also accidentally copied this bit in twice (though that shouldn't be an issue)
      // Unsupported HTTP request
      // 304 or 501 response would be more appropriate
      bfill.emit_p(http_Unauthorized);
    } else {
      data += 5;
      if (data[0] == ' ') { //Check if the home page, i.e. no URL
        //if (1==1) {
        homePage();
      } else if (!strncmp("?relay=", data, 5)) { //Check if a url which changes the leds has been recieved
        data += strcspn (data, "01"); //Move to the start of the URL
        char tempURL[numberOfRelays + 1] = {0};
        strncpy(tempURL, data, numberOfRelays); //Extract the recieved URL to the temporary string
        Serial.print("temp = ");
        Serial.println(tempURL); //Just some quick logging
        for (byte i = 0; i < numberOfRelays ; i++) { //Check through each of the LEDs URLs
          if (!strcmp(tempURL, RelayURL[i])) {
            //The recieved URL matches the one required to turn this LED on, so we will turn it on.
            digitalWrite(RELAY[i], (tempURL[i] == '1') ? HIGH : LOW); //Set the led state to match the URL.
            //Now we need to toggle the state of this LED in all of the URLs, so that all the hyperlinks can be corrected to match this state.
            for (byte j = 0; j < numberOfRelays ; j++) {
              if (j == i) {
                RelayURL[j][i] = (tempURL[i] == '1') ? '0' : '1'; //Notice how we toggle the 'i'th led in each url.
                Serial.print("led = ");
                Serial.println(RelayURL[j]);
                continue;
              }
              RelayURL[j][i] = tempURL[i]; //Notice how we toggle the 'i'th led in each url.
              Serial.print("led = ");
              Serial.println(RelayURL[j]);
            }
            //The URL was found, so we don't need to check any others.
            break; //Exit the for loop.
          }
        }
        bfill.emit_p(http_Found);
      } else { //Otherwise, page isn't found
        // Page not found
        bfill.emit_p(http_Unauthorized);
      }
    }
    ether.httpServerReply(bfill.position());    // send http response
  }
}

