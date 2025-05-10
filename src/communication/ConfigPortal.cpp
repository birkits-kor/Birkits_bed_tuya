#include "ConfigPortal.h"
#include "index_html.h"
#include "../storage/NVSStorage.h"

ConfigPortal::ConfigPortal() : server(80) {}

void ConfigPortal::begin()
{
    delay(1000);
    wifi_ssid = NVSStorage::getInstance().getCredential("wifi_ssid");
    wifi_password = NVSStorage::getInstance().getCredential("wifi_password");

    if (wifi_ssid.isEmpty() || wifi_password.isEmpty())
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
            wifi_ssid = request->getParam("ssid")->value();
            wifi_password = request->getParam("password")->value();
            String device_id = request->getParam("deviceid")->value();
            String device_secret = request->getParam("devicesecret")->value();

            // Check device_id length (22 characters) and device_secret length (16 characters)
            if (device_id.length() == 22 && device_secret.length() == 16)
            {
                request->send(200, "text/html", "<h1>Save done. Restarting the device.</h1>");
                printf("Credentials saved: wifi_ssid=%s, wifi_password=%s, device_id=%s, device_secret=%s\n", wifi_ssid.c_str(), wifi_password.c_str(), device_id.c_str(), device_secret.c_str());
                NVSStorage::getInstance().saveCredential("wifi_ssid", wifi_ssid);
                NVSStorage::getInstance().saveCredential("wifi_password", wifi_password);
                NVSStorage::getInstance().saveCredential("device_id", device_id);
                NVSStorage::getInstance().saveCredential("device_secret", device_secret);
            }
            else
                request->send(400, "text/html", "<h1>Save failed. Device ID must be 22 characters and Device Secret must be 16 characters. Please go back and try again.</h1><script>setTimeout(function(){window.history.back();}, 5000);</script>");
        }
        else 
            request->send(400, "text/html", "<h1>Save failed. Missing required fields. Please go back and try again.</h1><script>setTimeout(function(){window.history.back();}, 5000);</script>"); });

    server.begin();
}
