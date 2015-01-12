#include <DHT.h>
#include <avr/pgmspace.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <Ethernet.h>
#include <Password.h>
#include <SPI.h>
#include <stdint.h>
#include <SeeedTouchScreenMenu.h>
#include <TouchScreen.h>
#include <SeeedTFT.h>
#include <cstddef.h>
#include <email.h>
#include <Smtp_Service.h>
String val;
boolean unlocked = false;
boolean alarm = false;
boolean armed = true;
Password password = Password( "9584" );
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";
bool buttons = false;
bool Keys = false;
boolean handled = false;
boolean activeScreen;
int inactivity= 0;
const unsigned int SMTP_PORT = 587;
byte smtp_server[] = {
  204, 93, 163, 124};
SmtpService smtp_service(smtp_server, SMTP_PORT);
char incString[2500];
String domain =   "reitmeier.org";
String login =    "YXJkdWlub3NlY3VyaXR5K3JlaXRtZWllci5vcmc=";
String passwordMail = "UmVpdG1laWVyMDE=";
Email passcodeReset;
String inputString;
bool screen;
// screen
SeeedTouchScreenMenuItem mainMenuItems[] = {
  SeeedTouchScreenMenuItem("Buttons"),
  SeeedTouchScreenMenuItem("KeyPad"),
  SeeedTouchScreenMenuItem("ENDOFMENU")
  };
  bool Setting;
int indexp = 0;

TouchScreenArea lbl4 = TouchScreenLabel("Seage Mode!", TSC.createColor(255, 0, 0), TSC.createColor(255, 255, 0), 0, 90, 2, 2, false);
TouchScreenArea locklbl = TouchScreenLabel("System Locked.", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 0), 50, 300, 1, 5, true);
TouchScreenArea unlocklbl = TouchScreenLabel("System UnLocked.", TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 0), 50, 300, 1, 5, true);
TouchScreenArea s1 = TouchScreenLabel("*", TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 0), 10, 10, 1, 2, true);
TouchScreenArea s2 = TouchScreenLabel("*", TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 0), 45, 10, 1, 2, true);
TouchScreenArea s3 = TouchScreenLabel("*", TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 0), 90, 10, 1, 2, true);
TouchScreenArea s4 = TouchScreenLabel("*", TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 0), 135, 10, 1, 2, true);
TouchScreenArea s5 = TouchScreenLabel("*", TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 0), 180, 10, 1, 2, true);
TouchScreenArea emailSending = TouchScreenLabel("SENDING MAIL...", TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 0), TSC.getScreenHeight()-200, 200, 1, 2, true);
TouchScreenArea emailSent = TouchScreenLabel("SENT", TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 0), TSC.getScreenHeight()-200, 200, 1, 2, true);

// create the various menus setting the items, font size, spacing, padding, justification and
SeeedTouchScreenMenu mainMenu = SeeedTouchScreenMenu(mainMenuItems, 2, 10, 10, CENTERJ, "Main Menu");
// keep track of which menu is the currently active one
SeeedTouchScreenMenu *curMenu = &mainMenu;
TouchScreenArea header = TouchScreenLabel("Conner Security", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 0), 50, 300, 1, 2, true);
TouchScreenArea backFromKey = TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10);
TouchScreenArea backFromBtns = TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10);
TouchScreenArea backFromSet = TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10);
TouchScreenArea armbut = TouchScreenArrowButton("Togle Arm System", TSC.createColor(255, 255, 255), TSC.createColor(0, 255, 0), 10, TSC.getScreenHeight() - 280, 200, 40, RIGHT);
TouchScreenArea alarmbut = TouchScreenArrowButton("Togle Alarm System", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 10, TSC.getScreenHeight() - 200, 350, 40, RIGHT);
TouchScreenArea codereset = TouchScreenArrowButton("reset passcode", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 255), 10, TSC.getScreenHeight() - 160, 320, 40, RIGHT);

TouchScreenArea one = TouchScreenArrowButton("1", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 10, TSC.getScreenHeight() - 235, 60, 40, RIGHT);
TouchScreenArea two = TouchScreenArrowButton("2", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 80, TSC.getScreenHeight() - 235, 60, 40, RIGHT);
TouchScreenArea three = TouchScreenArrowButton("3", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 150, TSC.getScreenHeight() - 235, 60, 40, RIGHT);
TouchScreenArea four = TouchScreenArrowButton("4", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 10, TSC.getScreenHeight() - 190, 60, 40, RIGHT);
TouchScreenArea five = TouchScreenArrowButton("5", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 80, TSC.getScreenHeight() - 190, 60, 40, RIGHT);
TouchScreenArea six = TouchScreenArrowButton("6", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 150, TSC.getScreenHeight() - 190, 60, 40, RIGHT);
TouchScreenArea seven = TouchScreenArrowButton("7", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 10, TSC.getScreenHeight() - 145, 60, 40, RIGHT);
TouchScreenArea eight = TouchScreenArrowButton("8", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 80, TSC.getScreenHeight() - 145, 60, 40, RIGHT);
TouchScreenArea nine = TouchScreenArrowButton("9", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 150, TSC.getScreenHeight() - 145, 60, 40, RIGHT);
TouchScreenArea zero = TouchScreenArrowButton("0", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 10, TSC.getScreenHeight() - 100, 105, 40, RIGHT);
TouchScreenArea enter = TouchScreenArrowButton("Enter", TSC.createColor(255, 255, 255), TSC.createColor(0, 255, 0), 110, TSC.getScreenHeight() - 100, 105, 40, RIGHT);
TouchScreenSlider vol = TouchScreenSlider("vol", TSC.createColor(255, 0, 0), TSC.createColor(0, 255, 0), TSC.getScreenWidth() - 55, TSC.getScreenHeight() - 200, 40, 150, VERTICAL);
//ethernet
int buzzer1 = 21;
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x00, 0x4E, 0x10 };
byte ip[]  = { 
  192, 168, 0, 29 };                     // ip-address, please change to fit your network
EthernetServer serv(8899);
String readString = String(100);      // string for fetching data from address
EthernetUDP Udp;
int alsl = 25;
int arsl = 31;
void genResetCode();
char data;
String resetcode;
void setup() {

  Ethernet.begin(mac, ip);
  //serv.begin();
  Udp.begin(8899);
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("startup confermed");
  if (!SD.begin()) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  TSC.setBackColor(TSC.createColor(255, 255, 255)); // change the default background color
  TSC.init(); // make sure everything get initializede sliders
  curMenu->draw(); // put up the main menu
  locklbl.draw();
  genResetCode();
  passcodeReset.setDomain(domain);
  passcodeReset.setLogin(login);
  passcodeReset.setPassword(passwordMail);
  passcodeReset.setFrom("arduinosecurity@reitmeier.org");
  passcodeReset.setTo("conner@reitmeier.org");
  passcodeReset.setCc("");
  passcodeReset.setSubject("resetcode");
  passcodeReset.setBody(resetcode);
  locklbl.draw();
}
bool seage =false;
bool seagedrawed = false;
void genResetCode() {
}
void loop(){
  if (!seagedrawed){
    if (seage) {
      lbl4.draw();
      seagedrawed = true;
    } 
    else {
      curMenu = &mainMenu;
      TSC.clearScreen();
      curMenu->draw();
      if (unlocked) {
        unlocklbl.draw();
      } 
      else {
        locklbl.draw();
      }
      seagedrawed = true;
    }
  }

  if (armed) {
    Serial.print("armed");
    int door = analogRead(14);
    //Serial.print(String(door));
    if (door > 270) {
      alarm = true;
      // DateTime now = rtc.now();
      // Serial.print("door sensor has been activated at:" + String(now.year(), DEC) + '/' + String(now.month(), DEC) + '/' + String(now.day(), DEC) + ' ' + String(now.hour(), DEC) + ':' + String(now.minute(), DEC) + ':' + String(now.second(), DEC) );
    }
    /*if (digitalRead(40)) {
     alarm = true;
     DateTime now = rtc.now();
     Serial.print("motion sensor has been activated at:" + String(now.year(), DEC) + '/' + String(now.month(), DEC) + '/' + String(now.day(), DEC) + ' ' + String(now.hour(), DEC) + ':' + String(now.minute(), DEC) + ':' + String(now.second(), DEC) );
     }*/
    digitalWrite(arsl, HIGH);
    if (alarm) {
      digitalWrite(alsl, HIGH);
      //analogWrite(A10,  255*volv);
      digitalWrite(buzzer1, HIGH);


    } 
    else {
      digitalWrite(alsl, LOW);
      digitalWrite(buzzer1, LOW);
    }
  } 
  else if (!armed) {
    digitalWrite(arsl, LOW);
  }
  if (Serial.available()){
    Serial.print(val);
    // read it and store it in val
    if (val == "1") { // If H was received
      armed = true;
    } 
    else if (val == "2") {
      armed = false;

    }
    else if (val == "3") {
      alarm = true;
    }
    else if (val == "4") {
      alarm = false;

    }
    else if (val == "5") {
      Serial.print("Seage Mode");
      if (seage) {
        seage = true;
      }
      else{
        seage = false;
      }  
      seagedrawed = false;
    }

    Serial.print(String(val));
    Serial.print(val);
    val="";
  }
  val= " ";

  if (digitalRead(40) == HIGH) {

    curMenu = &mainMenu;
    TSC.clearScreen();
    curMenu->draw();
    if (unlocked) {
      unlocklbl.draw();
    } 
    else {
      locklbl.draw();
    }
    Serial.println("lock:" + String(unlocked));
    Serial.println  ("unlock");
  } 
  else
  {
  }
  // handle the current menu
  if (curMenu != NULL) {
    // process the current menu
    SeeedTouchScreenMenuItem *item = curMenu->process(false);
    // check to see which, if any, menu item was pressed
    checkMenuSelection(item);
  } 
  else {
    // if there isn't a current menu being displayed check all of the buttons
    // to see if any of them was pressed
    checkButtons();
  } 
}
bool stringComplete;
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    val += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      Serial.print(val); 
 
                                 
    } 
  }
}
void udp() {
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.print(String(packetSize));
    Serial.print("From ");
    String rip;
    IPAddress remote = Udp.remoteIP();
    for (int i = 0; i < 4; i++)
    {
      rip = rip + String(remote[i], DEC);
      if (i < 3)
      {
        rip = rip + ".";
      }
    }
    Serial.print(rip + ",port" + String(Udp.remotePort()));

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.print("Contents:");
    Serial.print(packetBuffer);
    if (packetBuffer == "1") {
      armed = true;
    }
    if (packetBuffer == "2") {
      armed = false;
    }
    if (packetBuffer == "3") {
      alarm = true;
    }
    if (packetBuffer == "4") {
      alarm = false;
    }
    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
    packetBuffer[UDP_TX_PACKET_MAX_SIZE] = NULL; //buffer to hold incoming packet,

  }
  delay(10);
}// check various buttons and perform actions if any was pressed
void checkButtons() {
  activeScreen = false;

  if (buttons) {
    if (backFromBtns.process()) { // return from the buttons screen
      curMenu = &mainMenu;
      TSC.clearScreen();
      curMenu->draw();
      if (unlocked) {
        unlocklbl.draw();
      } 
      else {
        locklbl.draw();

      }
      Serial.println("lock:" + String(unlocked));
      activeScreen = true;
    }
    if (armbut.process()) {
      if (unlocked) {
        if (armed) {
          armed =  false;

        }
        else
        {
          armed = true;
        }
      }
      activeScreen = true;

    }
    else if (alarmbut.process()) {
      activeScreen = true;

      if (unlocked) {
        if (alarm) {
          alarm =  false;

        }
        else
        {
          alarm = true;
        }
      }
    }
    else if (codereset.process()) {
      Serial.print("rest passcode email sent confirm form app");
    }
  } 
  else if (Keys) {

    if (backFromKey.process()) {
      activeScreen = true;
      // return from the graphics function screen
      curMenu = &mainMenu;
      TSC.clearScreen();
      curMenu->draw();
      indexp = 0;
      if (unlocked) {
        unlocklbl.draw();
      } 
      else {
        locklbl.draw();
      }
    }
    if (one.process()) {
      activeScreen = true;
      password.append('1');
      indexp++;
      startype();
    }
    else if (two.process()) {
      activeScreen = true;
      password.append('2');
      indexp++;
      startype();
    }
    else if (three.process()) {
      activeScreen = true;

      password.append('3');
      indexp++;
      startype();
    }
    else if (four.process()) {
      activeScreen = true;

      password.append('4');
      indexp++;
      startype();
    }
    else if (five.process()) {
      activeScreen = true;

      password.append('5');
      indexp++;
      startype();
    }
    else if (six.process()) {
      activeScreen = true;

      password.append('6');
      indexp++;
      startype();
    }
    else if (seven.process()) {
      activeScreen = true;

      password.append('7');
      indexp++;
      startype();
    }
    else if (eight.process()) {
      activeScreen = true;

      password.append('8');
      indexp++;
    }
    else if (nine.process()) {
      activeScreen = true;

      password.append('9');
      indexp++;
      startype();
    }
    else if (zero.process()) {
      activeScreen = true;

      password.append('0');
      indexp++;
      startype();
    }
    else if (enter.process()) {
      activeScreen = true;

      unlocked = password.evaluate();
      password.reset();
      indexp = 0;
      curMenu = &mainMenu;
      TSC.clearScreen();
      curMenu->draw();
      if (unlocked) {
        unlocklbl.draw();
      } 
      else {
        locklbl.draw();
      }
    }
    delay(250);

  } 
  else if (Setting) {
    if (codereset.process()) {
      activeScreen = true;

      emailSending.draw();
      delay(900);
      smtp_service.send_email(passcodeReset); 
      curMenu = NULL;
      TSC.clearScreen();
      Setting = true;
      backFromSet.draw();
      handled = true;
      codereset.draw();
      delay(100);
      emailSent.draw();
      delay(1000);
      curMenu = NULL;
      TSC.clearScreen();
      Setting = true;
      backFromSet.draw();
      handled = true;
      codereset.draw(); 
    }
    else if (backFromSet.process()) {
      activeScreen = true;

      curMenu = &mainMenu;
      TSC.clearScreen();
      curMenu->draw();
      indexp = 0;
      if (unlocked) {
        unlocklbl.draw();
      } 
      else {
        locklbl.draw();
      }
    } 
  }
  //if (!activeScreen) {
  //  inactivity++;
  //  if (inactivity>200)
  //  screen=false;
  //  Serial.print("inactivity"+String(inactivity));
  //}else{
  //inactivity = 0;
  //screen = true;
  //}
  //if (screen) {
  //  digitalWrite(7,HIGH);
  //} else{
  // digitalWrite(7,LOW);
  //} 
}
// check to see if any menu item was pressed and do something
void checkMenuSelection(SeeedTouchScreenMenuItem *item) {
  if (item != NULL) {
    buttons = false;
    Keys = false;
    Setting = false;
    // main menu items
    if (curMenu == &mainMenu) {
      if (!strcmp(item->getText(), "Buttons")) {
        curMenu = NULL;
        TSC.clearScreen();
        backFromBtns.draw();
        buttons = true;
        armbut.draw();
        alarmbut.draw();
        handled = true;
        codereset.draw();
      } 
      else if (!strcmp(item->getText(), "KeyPad")) {
        curMenu = NULL;
        TSC.clearScreen();
        backFromKey.draw();
        one.draw();
        Keys = true;
        two.draw();
        three.draw();
        four.draw();
        five.draw();
        six.draw();
        seven.draw();
        eight.draw();
        nine.draw();
        zero.draw();
        enter.draw();
        handled = true;

      } 
      else if (!strcmp(item->getText(), "Settings")) {
        curMenu = NULL;
        TSC.clearScreen();
        Setting = true;
        backFromSet.draw();
        handled = true;
        codereset.draw();
      }
    }
    // if the menu item didn't get handled redraw it unpressed
    if (handled == false)
      curMenu->drawItem(item, false);
  }
}
void startype() {
  if (indexp == 1) {
    s1.draw();
  } 
  else if (indexp == 2) {
    s2.draw();
  } 
  else if (indexp == 3) {
    s3.draw();
  } 
  else if (indexp == 4) {
    s4.draw();
  } 
  else if (indexp == 5) {
    s5.draw();
  } 
  else if (indexp > 5) {
    curMenu = &mainMenu;
    TSC.clearScreen();
    curMenu->draw();
    indexp = 0;
  }
}



