TCPClient webClient;
TCPServer webServer = TCPServer(80);
char myIpAddress[24];

void setup() {
    Spark.variable("ipAddress", myIpAddress, STRING);
    IPAddress myIp = Network.localIP();
    sprintf(myIpAddress, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);

    webServer.begin();
}

void loop() {
    if (webClient.connected() && webClient.available()) {
        serveWebpage();
    }
    else {
        webClient = webServer.available();
    }
}

void serveWebpage() {
    //TODO: read in the request to see what page they want:
    //TODO: retrieve larger content from flash?

    webClient.println("<html>I'm serving a webpage!</html>\n\n");
    webClient.flush();
    webClient.stop();
    delay(100);
}
