/**
06/16/2026 - RH - Moved code to be independent in main
06/30/2026 - RH - Started integration of CodePen HTML setup
06/30/2026 - RH - Fixed JSON shape, string quoting, CSS dedup, function scope
07/06/2026 - RH - Edits for incoorperating global variables as well as more HTML edits
*/
#pragma once
#include <WiFi.h>
#include <WiFiAP.h>
#include <LittleFS.h>
#include "FS.h"
#include "globals.h"
#include "fileLogging.h"

#include <SimpleBatteryMonitor.h>
#include <Adafruit_LSM6DSO32.h>

const char *ssid     = "TOBI";
const char *password = "password";

WiFiServer server(80);

//Full HTML code
const char homePage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, viewport-fit=cover">
<link href="https://fonts.googleapis.com/css2?family=Ubuntu+Sans+Mono:ital,wght@0,400..700;1,400..700&display=swap" rel="stylesheet">
<title>TOBI</title>
<style>
  * { margin:0; padding:0; box-sizing:border-box; }

  html, body {
    background:#191553;
    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif;
    height:100%;
    overflow:hidden;
  }

  #app {
    display:flex;
    flex-direction:column;
    height:100vh;
    height:100dvh;
  }

  .topbar {
    display:flex;
    align-items:center;
    justify-content:space-between;
    padding:12px 16px;
    padding-top:max(12px, env(safe-area-inset-top));
    background:#191553;
    border-bottom:1px solid rgba(255,255,255,0.06);
    flex-shrink:0;
    z-index:10;
  }

  .logo {
    height:28px;
    display:flex;
    align-items:center;
  }

  .logo img {
    height:100%;
    width:auto;
    display:block;
  }

  /* Fallback text if logo hasn't loaded yet */
  .logo-fallback {
    color:#F5ECD3;
    font-size:14px;
    font-weight:700;
    letter-spacing:2px;
  }

  .topbar-right {
    display:flex;
    align-items:center;
    gap:8px;
  }

  .time {
    font-size:11px;
    color:#F5ECD3;
    letter-spacing:0.5px;
    font-variant-numeric: tabular-nums;
  }

  .status-light {
    width:14px;
    height:14px;
    border-radius:50%;
    flex-shrink:0;
  }

  .light-off  { background:#C83408; }
  .light-on   { background:#7FCB8F; }
  .light-busy { background:#FB961B; }

  .pulse-dot { animation: pulse 1.6s ease-in-out infinite; }
  @keyframes pulse { 0%,100%{opacity:1} 50%{opacity:0.4} }

  .scroll-area {
    flex:1;
    overflow-y:auto;
    -webkit-overflow-scrolling:touch;
    scrollbar-width:none;
    padding:14px 14px;
    padding-bottom:max(24px, env(safe-area-inset-bottom));
  }
  .scroll-area::-webkit-scrollbar { display:none; }

  .card {
    background:#676C73;
    border-radius:5px;
    padding:14px 16px;
    margin-bottom:12px;
  }

  .card-label {
    font-size:11px;
    color:#D0F3D3;
    letter-spacing:0.5px;
    margin-bottom:8px;
    text-transform:uppercase;
  }

  .banner {
    background:#F5ECD3;
    border-radius:14px;
    padding:14px 16px;
    margin-bottom:12px;
    display:flex;
    align-items:center;
    justify-content:space-between;
  }

  .banner-text {
    color:#191553;
    font-size:14px;
    font-weight:600;
    line-height:1.3;
  }

  .toggle-btn {
    width:48px;
    height:48px;
    border-radius:50%;
    border:none;
    display:flex;
    align-items:center;
    justify-content:center;
    font-size:10px;
    font-weight:700;
    letter-spacing:0.5px;
    color:#F5ECD3;
    cursor:pointer;
    flex-shrink:0;
  }

  .toggle-off { background:#C83408; }
  .toggle-on  { background:#7FCB8F; color:#191553; }

  .sensor-value {
    font-size:20px;
    font-weight:700;
    color:#312F29;
    text-align:center;
    font-variant-numeric: tabular-nums;
    background:#D0F3D3;
    border-radius:5px;
    border:2px solid #88C493;
    padding:6px 0;
  }

  .triple-row {
    display:grid;
    grid-template-columns: 1fr 1fr 1fr;
    gap:8px;
  }

  .triple-item {
    background:rgba(245,236,211,0.08);
    border-radius:10px;
    padding:10px 6px;
    text-align:center;
  }

  .triple-axis {
    font-size:10px;
    color:#FFFFFF;
    margin-bottom:6px;
    text-transform:uppercase;
    letter-spacing:0.5px;
  }

  .triple-val {
    font-size:15px;
    font-weight:700;
    color:#F5ECD3;
  }

  .triple-unit {
    font-size:9px;
    color:#F5ECD3;
    opacity:0.5;
    margin-top:2px;
  }

  .dl-row {
    display:flex;
    align-items:center;
    gap:10px;
  }

  .dl-btn {
    flex:1;
    background:#F5ECD3;
    color:#191553;
    border:none;
    border-radius:10px;
    padding:12px 0;
    font-size:13px;
    font-weight:600;
    cursor:pointer;
    text-align:center;
    text-decoration:none;
    display:block;
  }

  .dl-status {
    width:48px;
    height:48px;
    border-radius:50%;
    background:#FB961B;
    color:#191553;
    display:flex;
    align-items:center;
    justify-content:center;
    font-size:9px;
    font-weight:700;
    line-height:1.1;
    text-align:center;
    flex-shrink:0;
  }

  .summary-row {
    display:flex;
    justify-content:space-between;
    padding:6px 0;
    border-bottom:1px solid rgba(245,236,211,0.1);
    font-size:12px;
  }
  .summary-row:last-child { border-bottom:none; }
  .summary-key { color:#F5ECD3; opacity:0.65; }
  .summary-val { color:#F5ECD3; font-weight:600; }

  .chart-wrap { margin-bottom:4px; }

  .scroll-hint {
    text-align:center;
    color:#F5ECD3;
    opacity:0.4;
    font-size:14px;
    padding:4px 0;
    animation: bob 1.6s ease-in-out infinite;
    display:none;
  }
  @keyframes bob {
    0%,100% { transform:translateY(0); opacity:0.4; }
    50%      { transform:translateY(4px); opacity:0.8; }
  }

  .conn-status {
    text-align:center;
    font-size:10px;
    color:#F5ECD3;
    opacity:0.3;
    padding:8px 0 4px;
    letter-spacing:0.5px;
  }

</style>
</head>
<body>
<div id="app">
  <div id="topbar-fixed"></div>
  
<div class="scroll-area" id="scroll-area">
    <div id="phone"></div>
    <div class="scroll-hint" id="scroll-hint">&#9660;</div>
    <div class="conn-status" id="conn-status">Connecting to flight computer&hellip;</div>
  </div>
</div>

<script>
  let deviceState = 'startup';
  let armed = false;
  let live = {
    batteryVoltage: 0,
    barometer: 0,
    accel: {x:0, y:0, z:0},
    gyro:  {x:0, y:0, z:0},
    temp: 0,
    flight: {},
    csvReady: false
  };

  function toggleArmed(){
    armed = !armed;
    render();
    fetch('/' + (armed ? 'arm' : 'disarm')).catch(()=>{});
  }

  async function poll(){
    try {
      const r = await fetch('/data');
      if(!r.ok) throw new Error();
      const d = await r.json();
      live.batteryVoltage = d.batteryVoltage ?? live.batteryVoltage;
      live.barometer      = d.barometer      ?? live.barometer;
      live.temp           = d.temp           ?? live.temp;
      live.accel = { x: d.accelX ?? live.accel.x, y: d.accelY ?? live.accel.y, z: d.accelZ ?? live.accel.z };
      live.gyro  = { x: d.gyroX  ?? live.gyro.x,  y: d.gyroY  ?? live.gyro.y,  z: d.gyroZ  ?? live.gyro.z  };
      live.flight   = d.flight   ?? live.flight;
      live.csvReady = !!d.csvReady;
      if(d.state){ deviceState = d.state; armed = (d.state === 'armed'); }
      document.getElementById('conn-status').textContent = 'Connected to TOBI';
    } catch(e) {
      document.getElementById('conn-status').textContent = 'Connection lost \u2014 retrying\u2026';
    }
    render();
  }

  function nowTime(){
    const d = new Date();
    return String(d.getHours()).padStart(2,'0')+':'+String(d.getMinutes()).padStart(2,'0');
  }

  function topbar(lightClass){
    return '<div class="topbar">'
      + '<div class="logo"><img src="/assets/TOBI_Logo.png" alt="TOBI"></div>'
      + '<div class="topbar-right">'
      + '<div class="time">'+nowTime()+'</div>'
      + '<div class="status-light '+lightClass+'"></div>'
      + '</div></div>';
  }

  function sv(v){ return '<div class="sensor-value">'+v+'</div>'; }

  function tripleRow(data, unit){
    return '<div class="triple-row">'
      + ['x','y','z'].map(ax=>
          '<div class="triple-item">'
          + '<div class="triple-axis">'+ax+'</div>'
          + '<div class="triple-val">'+Number(data[ax]).toFixed(2)+'</div>'
          + '<div class="triple-unit">'+unit+'</div>'
          + '</div>'
        ).join('')
      + '</div>';
  }

  function sp(){ return '<div style="height:10px"></div>'; }

  function renderStartup(){
    return {
      top: topbar('light-busy pulse-dot'),
      body: ''
        + '<div style="height:6px"></div>'
        + '<div class="banner"><div class="banner-text">Connected.<br>Device online.</div><div class="status-light light-on"></div></div>'
        + '<div class="banner"><div class="banner-text">Device connected.<br>Starting&hellip;</div><div class="status-light light-busy pulse-dot"></div></div>'
        + '<div class="card">'
          + '<div class="card-label">Active systems</div>'
          + '<div class="card-label" style="margin-bottom:4px">Battery</div>' + sv(Number(live.batteryVoltage).toFixed(2)+' V') + sp()
          + '<div class="card-label" style="margin-bottom:4px">Barometer</div>' + sv(Number(live.barometer).toFixed(1)+'&deg;') + sp()
          + '<div class="card-label">Accelerometer</div>' + tripleRow(live.accel,'m/s&sup2;')
        + '</div>'
        + '<div class="card">'
          + '<div class="card-label">System settings</div>'
          + '<div class="card-label" style="margin-bottom:4px">Accelerometer range</div>' + sv('&plusmn;32 G') + sp()
          + '<div class="card-label" style="margin-bottom:4px">Accelerometer data rate</div>' + sv('6.66 kHz') + sp()
          + '<div class="card-label" style="margin-bottom:4px">Gyro range</div>' + sv('2000 &deg;/s') + sp()
          + '<div class="card-label" style="margin-bottom:4px">Gyro data rate</div>' + sv('6.66 kHz')
        + '</div>'
    };
  }

  function renderReady(){
    const a = armed, col = a ? '#7FCB8F' : '#C83408';
    return {
      top: topbar(a ? 'light-on' : 'light-busy'),
      body: ''
        + '<div style="height:6px"></div>'
        + '<div class="banner"><div class="banner-text">Device ready<br>for flight</div>'
        + '<button class="toggle-btn '+(a?'toggle-on':'toggle-off')+'" onclick="toggleArmed()">'+(a?'ON':'OFF')+'</button></div>'
        + '<div class="card"><div class="card-label" style="color:'+col+'">Barometric pressure</div>'+sv(Number(live.barometer).toFixed(1)+'&deg;')+'</div>'
        + '<div class="card"><div class="card-label" style="color:'+col+'">Acceleration</div>'+tripleRow(live.accel,'m/s&sup2;')+'</div>'
        + '<div class="card"><div class="card-label" style="color:'+col+'">Gyroscope</div>'+tripleRow(live.gyro,'&deg;/s')+'</div>'
        + '<div class="card"><div class="card-label" style="color:'+col+'">Battery</div>'+sv(Number(live.batteryVoltage).toFixed(2)+' V')+'</div>'
    };
  }

  function buildChart(){
    const W=360, H=240, padL=44, padR=36, padT=14, padB=28;
    const plotW=W-padL-padR, plotH=H-padT-padB;
    const altSeries=[0,3800,4650,5300,5300,4900,4150,3050,1950,850,600];
    const timeLabels=['s','','3','','6','','9','','20','',''];
    const maxAlt=5500;
    const yTicks=[0,500,1000,2000,3000,4000,5000];
    function xAt(i){ return padL+(i/(altSeries.length-1))*plotW; }
    function yAt(v){ return padT+plotH-(v/maxAlt)*plotH; }
    let g='', vg='';
    yTicks.forEach(v=>{
      const y=yAt(v);
      g+='<line x1="'+padL+'" y1="'+y+'" x2="'+(W-padR)+'" y2="'+y+'" stroke="#5a6478" stroke-width="1" stroke-dasharray="4 4" opacity="0.5"/>';
      g+='<text x="'+(padL-5)+'" y="'+(y+4)+'" fill="#cfd6e0" font-size="10" font-family="monospace" text-anchor="end">'+v.toLocaleString()+'</text>';
    });
    timeLabels.forEach((l,i)=>{
      if(!l) return;
      const x=xAt(i);
      vg+='<line x1="'+x+'" y1="'+padT+'" x2="'+x+'" y2="'+(H-padB)+'" stroke="#5a6478" stroke-width="1" opacity="0.4"/>';
      vg+='<text x="'+x+'" y="'+(H-padB+16)+'" fill="#cfd6e0" font-size="10" font-family="monospace" text-anchor="middle">'+l+'</text>';
    });
    const ax='<line x1="'+padL+'" y1="'+padT+'" x2="'+padL+'" y2="'+(H-padB)+'" stroke="#1a1a2a" stroke-width="2"/>'
      +'<line x1="'+padL+'" y1="'+(H-padB)+'" x2="'+(W-padR)+'" y2="'+(H-padB)+'" stroke="#1a1a2a" stroke-width="2"/>';
    const pts=altSeries.map((v,i)=>xAt(i).toFixed(1)+','+yAt(v).toFixed(1)).join(' ');
    let dots='';
    altSeries.forEach((v,i)=>{ dots+='<circle cx="'+xAt(i).toFixed(1)+'" cy="'+yAt(v).toFixed(1)+'" r="3" fill="#cdeecb"/>'; });
    return '<svg viewBox="0 0 '+W+' '+H+'" width="100%" preserveAspectRatio="xMidYMid meet">'
      +'<text x="'+padL+'" y="11" fill="#cfd6e0" font-size="10" font-family="monospace">ft</text>'
      +'<text x="'+(W-padR)+'" y="11" fill="#cfd6e0" font-size="10" font-family="monospace" text-anchor="end">ft/s</text>'
      +g+vg+ax
      +'<polyline points="'+pts+'" fill="none" stroke="#9fe09a" stroke-width="2.5" stroke-linejoin="round" stroke-linecap="round"/>'
      +dots+'</svg>';
  }

  function renderLanded(){
    const f=live.flight||{};
    const dlBadge=live.csvReady?'<div class="dl-status" style="background:#7FCB8F;color:#191553">Ready</div>':'<div class="dl-status">In<br>progress</div>';
    const dlBtn=live.csvReady?'<a class="dl-btn" href="/imu.csv">Download</a>':'<button class="dl-btn" disabled style="opacity:0.5">Download</button>';
    return {
      top: topbar('light-on'),
      body: ''
        + '<div style="height:6px"></div>'
        + '<div class="card"><div class="chart-wrap">'+buildChart()+'</div></div>'
        + '<div class="card"><div class="card-label">Download CSV</div><div class="dl-row">'+dlBtn+dlBadge+'</div></div>'
        + '<div class="card"><div class="card-label">Flight summary</div>'
          + '<div class="summary-row"><div class="summary-key">Apogee</div><div class="summary-val">'+(f.apogee??'--')+' ft</div></div>'
          + '<div class="summary-row"><div class="summary-key">Drogue deployment</div><div class="summary-val">'+(f.drogue??'--')+' ft</div></div>'
          + '<div class="summary-row"><div class="summary-key">Main deployment</div><div class="summary-val">'+(f.main??'--')+' ft</div></div>'
          + '<div class="summary-row"><div class="summary-key">Max velocity</div><div class="summary-val">'+(f.maxVel??'--')+' ft/s</div></div>'
          + '<div class="summary-row"><div class="summary-key">Average velocity</div><div class="summary-val">'+(f.avgVel??'--')+' ft/s</div></div>'
          + '<div class="summary-row"><div class="summary-key">Time to apogee</div><div class="summary-val">'+(f.timeToApogee??'--')+' s</div></div>'
          + '<div class="summary-row"><div class="summary-key">Time to main</div><div class="summary-val">'+(f.timeToMain??'--')+' s</div></div>'
        + '</div>'
        + '<div class="card"><div class="card-label">Barometric pressure</div>'+sv(Number(live.barometer).toFixed(1)+'&deg;')+'</div>'
        + '<div class="card"><div class="card-label">Acceleration</div>'+tripleRow(live.accel,'m/s&sup2;')+'</div>'
        + '<div class="card"><div class="card-label">Gyroscope</div>'+tripleRow(live.gyro,'&deg;/s')+'</div>'
        + '<div class="card"><div class="card-label">Battery</div>'+sv(Number(live.batteryVoltage).toFixed(2)+' V')+'</div>'
    };
  }


  function render(){
    const out = deviceState==='startup' ? renderStartup()
              : deviceState==='landed'  ? renderLanded()
              :                           renderReady();
    document.getElementById('topbar-fixed').innerHTML = out.top;
    document.getElementById('phone').innerHTML = out.body;
    document.getElementById('scroll-hint').style.display = deviceState==='landed' ? 'block' : 'none';
  }

  render();
  poll();
  setInterval(poll, 1000);
</script>
</body>
</html>
)=====";

static void serveCSV(WiFiClient &client, const char *path, const char *filename){
    File file = LittleFS.open(path, "r");
    if(file){
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/csv");
        client.print("Content-Disposition: attachment; filename=\"");
        client.print(filename);
        client.println("\"");
        client.print("Content-Length: ");
        client.println(file.size());
        client.println("Connection: close");
        client.println();
        client.flush();
        uint8_t buf[1024];
        while(file.available()){
            size_t len = file.read(buf, sizeof(buf));
            client.write(buf, len);
            yield();
        }
        file.close();
    } else {
        client.println("HTTP/1.1 404 Not Found");
        client.println("Content-Type: text/plain");
        client.println("Connection: close");
        client.println();
        client.println("File not found.");
    }
}

void startWifi(){
    Serial.println("\nStarting WiFi server...");

    if(!LittleFS.begin(true)){
        Serial.println("ERROR: LittleFS mount failed! Did you upload the filesystem image?");
        Serial.println("Run: pio run --target uploadfs  (PlatformIO)");
        Serial.println("  or use the ESP32 Sketch Data Upload tool (Arduino IDE)");
    } else {
        Serial.println("LittleFS mounted OK.");
    }

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    IPAddress apIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(apIP);
    server.begin();
    Serial.println("WiFi server started!");
}

void WiFiInterface(){
    WiFiClient client = server.available();
    if(!client) return;

    Serial.println("New client connected.");

    String requestLine = "";
    String currentLine = "";
    bool gotRequest = false;
    unsigned long timeout = millis();

    while(client.connected() && (millis()-timeout) < 3000){
        if(!client.available()) continue;
        char c = client.read();
        timeout = millis();

        if(c == '\n'){
            if(!gotRequest){ requestLine = currentLine; gotRequest = true; }

            if(currentLine.length() == 0){

                //Live data from JSON
                if(requestLine.startsWith("GET /data")){
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: application/json");
                    client.println("Connection: close");
                    client.println();

                    client.print("{");
                    client.print("\"state\":\"");        client.print(flightStatus);             client.print("\",");
                    client.print("\"accelX\":");         client.print(g_accelX,3);              client.print(",");
                    client.print("\"accelY\":");         client.print(g_accelY,3);              client.print(",");
                    client.print("\"accelZ\":");         client.print(g_accelZ,3);              client.print(",");
                    client.print("\"gyroX\":");          client.print(g_gyroX,3);               client.print(",");
                    client.print("\"gyroY\":");          client.print(g_gyroY,3);               client.print(",");
                    client.print("\"gyroZ\":");          client.print(g_gyroZ,3);               client.print(",");
                    client.print("\"barometer\":");      client.print(g_barometer,2);           client.print(",");
                    client.print("\"csvReady\":");       client.print(csvReady?"true":"false"); client.print(",");
                    client.print("\"flight\":{");
                    client.print("\"apogee\":");         client.print(g_apogee,0);              client.print(",");
                    client.print("\"drogue\":");         client.print(g_drogue,0);              client.print(",");
                    client.print("\"main\":");           client.print(g_main,0);                client.print(",");
                    client.print("\"maxVel\":");         client.print(g_maxVel,0);              client.print(",");
                    client.print("\"avgVel\":");         client.print(g_avgVel,0);              client.print(",");
                    client.print("\"timeToApogee\":"); client.print(g_timeToApogee,1);          client.print(",");
                    client.print("\"timeToMain\":");   client.print(g_timeToMain,1);
                    client.print("}}");
                    break;
                }

                //Disarm switch
                if(requestLine.startsWith("GET /arm")){
                    flightStatus = "armed";
                    loggerArmed();
                    client.println("HTTP/1.1 200 OK"); client.println("Connection: close"); client.println();
                    client.println("armed"); break;
                }
                if(requestLine.startsWith("GET /disarm")){
                    flightStatus = "ready";
                    client.println("HTTP/1.1 200 OK"); client.println("Connection: close"); client.println();
                    client.println("disarmed"); break;
                }

                //CSV download
                if(requestLine.startsWith("GET /imu.csv")){
                    serveCSV(client, "/IMU.csv", "imu.csv");
                    break;
                }

                //Wiping
                if(requestLine.startsWith("GET /wipe")){
                    csvReady = false;
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close"); client.println();
                    client.println("<html><body style='font-family:sans-serif;text-align:center;background:#191553;color:#F5ECD3'>");
                    client.println("<h2 style='color:#FB961B;margin-top:40px'>Files wiped.</h2>");
                    client.println("<p>Reboot the flight computer to start fresh.</p>");
                    client.println("<a href='/' style='color:#FB961B'>Back</a>");
                    client.println("</body></html>");
                    break;
                }

                //Embedded image
                if(requestLine.startsWith("GET /assets/")){
                    String path = requestLine.substring(4, requestLine.indexOf(' ', 4));
                    File f = LittleFS.open(path, "r");
                    if(f){
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: image/png");
                        client.print("Content-Length: "); client.println(f.size());
                        client.println("Connection: close"); client.println();
                        uint8_t buf[512];
                        while(f.available()){ size_t n=f.read(buf,sizeof(buf)); client.write(buf,n); }
                        f.close();
                    } else {
                        client.println("HTTP/1.1 404 Not Found"); client.println("Connection: close"); client.println();
                    }
                    break;
                }

                //Including the HTML page
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/html");
                client.println("Connection: close");
                client.println();
                client.print(homePage);
                break;
            }

            currentLine = "";
        } else if(c != '\r'){
            currentLine += c;
        }
    }

    client.stop();
    Serial.println("Client disconnected.");
}