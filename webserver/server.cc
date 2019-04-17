TCPClient webClient;
TCPServer webServer = TCPServer(80);
char myIpAddress[24];


int boost = D7;
int analogpowerin = A0;
int analogvalue;
int booststate;
int manualmodesw = D0;
int manualmode;
double dcvalue;
int acvalue;
int analogampsin = A1;
double analogampsvalue;
int acamps = 26;

void setup() {
    Particle.variable("ipAddress", myIpAddress, STRING);
    IPAddress myIp = WiFi.localIP();
    sprintf(myIpAddress, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);

    webServer.begin();

    pinMode(manualmodesw, INPUT_PULLUP);
    pinMode(boost,OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(analogpowerin,INPUT);
    Particle.variable("analogvalue", &analogvalue, INT);
    Particle.function("boost",boostToggle);
    Particle.variable("dcvalue", &dcvalue, DOUBLE);
    Particle.variable("acvalue", &acvalue, INT);
    Particle.variable("analogampsin", &analogampsin, INT);
    Particle.variable("analogampsvalue", &analogampsvalue, DOUBLE);
    Particle.variable("acamps", &acamps, INT);

}



void loop() {
    if (manualmode == HIGH)
    digitalWrite(D6, LOW);
        else
        digitalWrite(D6, HIGH);
    if (booststate == HIGH)
    digitalWrite(D5, HIGH);
        else
        digitalWrite(D5, LOW);
   if (webClient.connected() && webClient.available()) {
        serveWebpage();
    }
    else {
        webClient = webServer.available();
    }
    analogvalue = analogRead(analogpowerin);
    booststate = digitalRead(boost);
    manualmode = digitalRead(manualmodesw);
    dcvalue = (analogvalue * .00081);
    acvalue = (dcvalue * 100 + 9);
    if (manualmode == HIGH)
        {

            if (analogvalue > 1380)
            digitalWrite(boost, LOW);

            else if(analogvalue < 1280)
            digitalWrite(boost, HIGH);
        }
}

void serveWebpage() {

    webClient.print("<html><body><center>This will soon be the place where you can view shore power information and control boost!!!!<br><br><br><br>AC Voltage = ");
    webClient.print(acvalue);
    delay(10);
    webClient.print("<br>AC Amps =");
    delay(10);
    webClient.print(acamps);
    delay(10);
    webClient.print("<br> Manual Mode:");
    delay(10);
    if (manualmode == HIGH)
        webClient.print("OFF<br>");
        else
            webClient.print("ON<br>");
    delay(10);
    webClient.print("<br>(Boost Control only available if Manual Mode is OFF!!!)<br>");
    delay(10);
    webClient.print("<br>Boost State:");
    delay(10);
    if (booststate == HIGH)
    webClient.print("BOOSTING<br>");
        else
            webClient.print("NOT BOOSTING<br>");
    webClient.print("<form action='https://api.particle.io/v1/devices/2e0047000747343232363230/boost?access_token=8ab7c29fd37fe48c089acd1184c2896649779d7e' method='POST'>");
    webClient.print("<br><input type='radio' name='args' value='on'>Turn the BOOST ON.<br>");
    delay(10);
    webClient.print("<br><input type='radio' name='args' value='off'>Turn the BOOST OFF.<br>");
    webClient.print("<br><input type='submit' value='Do it!'>");
    delay(10);
    webClient.print("</html>\n\n");
    webClient.flush();
    webClient.stop();
    delay(100);
}



int boostToggle(String command) {

    if (command=="on") {
        digitalWrite(boost,HIGH);
        return 1;
    }
    else if (command=="off") {
        digitalWrite(boost,LOW);
        return 0;
    }
    else {
        return -1;
    }

}
