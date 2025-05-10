#include "ConfigPortal.h"
#include "index_html.h"
#include "../storage/EEPROMStorage.h"

ConfigPortal::ConfigPortal() : server(80) {}

void ConfigPortal::begin()
{
    delay(1000);
    if (!EEPROMStorage::getInstance().isDataPresent())
    {
        startAP();
        setupServer();
    }
}

void ConfigPortal::handle()
{
    // 필요 시 loop에서 호출
}

void ConfigPortal::startAP()
{
    IPAddress local_ip(10, 10, 10, 1);
    IPAddress gateway(10, 10, 10, 1);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP(ap_ssid, "");

    Serial.println("AP started at: " + WiFi.softAPIP().toString());
}

void ConfigPortal::setupServer()
{
    Serial.println("Set config Server!!");
    server.on("/", HTTP_GET, [=](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html); });

    server.on("/save", HTTP_GET, [=](AsyncWebServerRequest *request)
              {
        if (request->hasParam("ssid") && request->hasParam("password") && 
            request->hasParam("deviceid") && request->hasParam("devicesecret")) 
        {
            String ssid = request->getParam("ssid")->value();
            String password = request->getParam("password")->value();
            String device_id = request->getParam("deviceid")->value();
            String device_secret = request->getParam("devicesecret")->value();

            // Check device_id length (22 characters) and device_secret length (16 characters)
            if (device_id.length() == 22 && device_secret.length() == 16)
            {
                request->send(200, "text/html", "<h1>Save done. Restarting the device.</h1>");
                EEPROMStorage::getInstance().saveCredentials(ssid, password, device_id, device_secret);
            }
            else
                request->send(400, "text/html", "<h1>Save failed. Device ID must be 22 characters and Device Secret must be 16 characters. Restarting the device.</h1>");
        } 
        else 
            request->send(400, "text/html", "<h1>Save failed. Missing required fields. Restarting the device and try again.</h1>"); });

    server.begin();
}
