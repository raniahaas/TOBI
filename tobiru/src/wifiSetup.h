#pragma once
#include <WiFi.h>
#include <WiFiAP.h>

const char *ssid = "TOBI";
const char *password = "password";

WiFiServer server(80);

void startWifi(){
    Serial.println("Starting WiFi server...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid,password);
    IPAddress ap IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(apIP);
    
    server.begin();
    Serial.println("WiFi server started!");
}

void WiFiInterface() {
    //Establish connection with client 
    WiFiClient client = server.available();
    if(!client) return;

    Serial.println("New Client connected.");
    String currentLine = "";

    //Loop for actual connection
    while (client.connected()) {
        if(!client.available()) continue;
        
        char c = client.read;
        if (c == '\n'){
            //Add connection data
            if (currentLine.startsWith("GET /wipe")) {
                client.println("HTTP/1.1 200 OK");
                client.println("Connection: close");
                client.println();

                //Modify terminated page, placeholder from DON
                client.println("<html><body style='font-family:Times;text-align:center;background:#1a1a2e;color:#eee'>");
                client.println("<h2 style='color:#e94560'>Files wiped.</h2>");
                client.println("<p>Reboot the flight computer to start fresh.</p>");
                client.println("<a href='/' style='color:#e94560'>Back</a>");
                client.println("</body></html>");
                break;
            }

            //example for CSV downloading
            // if (currentLine.startsWith("GET /imu.csv")) {
            //         serveCSV(client, "/IMU.csv", "imu.csv");
            //         break;
            //     }

            //Main Page content
            if (currentLine.length() == 0) {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/html");
                client.println("Connection: close");
                client.println();

                client.println("<!DOCTYPE html><html><head>");
            }

            currentLine = "";
        } else if (c != '\r') {
        currentLine += c;
    } // End char loop
        
    } //End while loop

    client.stop();
    Serial.println("Client disconnected.");

}