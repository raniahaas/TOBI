/**
06/16/2026 - RH - Moved code to be independent in main
06/30/2026 - RH - Started integration of CodePen HTML setup
*/
#pragma once
#include <WiFi.h>
#include <WiFiAP.h>
#include <LittleFS.h>
#include "FS.h"

const char *ssid = "TOBI";
const char *password = "password";

WiFiServer server(80);

String flightStatus = "startup";

void startWifi(){
    Serial.println("\nStarting WiFi server...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid,password);
    IPAddress apIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(apIP);
    
    server.begin();
    Serial.println("WiFi server started!");
}

static void serveCSV(WiFiClient &client, const char *path, const char *filename) {
    File file = LittleFS.open(path, "r");
    if (file) {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/csv");
        client.print("Content-Disposition: attachment; filename=\"");
        client.print(filename);
        client.println("\"");
        client.println("Connection: close");
        client.println();
        client.flush();

        uint8_t buf[1024];
        while (file.available()) {
            size_t len = file.read(buf, sizeof(buf));
            client.write(buf, len);
            yield();
        }
        file.close();
    } else {
        client.println("HTTP/1.1 404 Not Found");
        client.println("Content-Type: text/plain");
        client.println();
        client.println("File not found.");
    }
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
        
        char c = client.read();
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

            //Startup 
            if (currentLine.startsWith("GET /data")) {

            }

            //Main Page content 
            if (currentLine.length() == 0) {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/html");
                client.println("Connection: close");
                client.println();

                client.println("<!DOCTYPE html><html><head>");
                client.println("<html lang='en'>");
                client.println("<head>");
                client.println("<meta charset='UTF-8'>");
                client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
                client.println("<link href='https://fonts.googleapis.com/css2?family=Ubuntu+Sans+Mono:ital,wght@0,400..700;1,400..700&display=swap' rel='stylesheet'>");
                client.println("");
                client.println("");
                client.println("");
                client.println("");
                client.println("");
                client.println("");
                client.println("");
                client.println("");
                client.println("");
                client.println("");
                client.println("");
                client.println("");
                client.println("");
                client.println("");
                client.println("");
                client.println("");
            }

            currentLine = "";
        } else if (c != '\r') {
        currentLine += c;
    } // End char loop
        
    } //End while loop

    client.stop();
    Serial.println("Client disconnected.");

}