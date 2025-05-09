#ifndef INDEX_HTML_H
#define INDEX_HTML_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>WiFi Configuration</title><style>body{font-family:Arial,sans-serif;background-color:#f4f4f9;color:#333;text-align:center;padding:20px}h2{color:#4CAF50}.form-container{background-color:white;border-radius:8px;box-shadow:0 4px 8px rgba(0,0,0,0.1);padding:30px;width:300px;margin:auto}input[type="text"],input[type="password"]{width:100%;padding:10px;margin:10px 0;border:2px solid #ccc;border-radius:4px;font-size:16px}input[type="submit"]{width:100%;padding:10px;background-color:#4CAF50;color:white;border:none;border-radius:4px;font-size:16px;cursor:pointer}input[type="submit"]:hover{background-color:#45a049}</style></head><body><h2>WiFi Configuration</h2><div class="form-container"><form action="/save" method="GET"><label for="ssid">SSID:</label><br><input type="text" id="ssid" name="ssid" required><br><label for="password">Password:</label><br><input type="password" id="password" name="password" required><br><label for="deviceid">Device ID:</label><br><input type="text" id="deviceid" name="deviceid" required><br><label for="devicesecret">Device Secret:</label><br><input type="text" id="devicesecret" name="devicesecret" required><br><input type="submit" value="Save"></form></div></body></html>
)rawliteral";

#endif
