#include "TuyaMQTTClient.h"
#include <ArduinoJson.h>
#include <SHA256.h>
#include <Base64.h>
#include <ArduinoJson.h>
#include "../data/RxMessageQueue.h"
#include "../data/TxMessageQueue.h"


String payload_alarmData = "{"
                           "\"data\":{"
                           "\"topic\":\"init\","
                           "\"alarm_data\":["
                           "{"
                           "\"data\":{"
                           "\"alarm\":{\"active\":true,\"time\":{\"hour\":8,\"minute\":0},\"weekday\":[2]},"
                           "\"bed\":{\"lower\":0,\"table\":325,\"upper\":0}"
                           "},"
                           "\"id\":1,\"index\":1,\"title\":\"알람 테스트\""
                           "},"
                           "{"
                           "\"data\":{"
                           "\"alarm\":{\"active\":true,\"time\":{\"hour\":6,\"minute\":0},\"weekday\":[4]},"
                           "\"bed\":{\"lower\":0,\"table\":435,\"upper\":0}"
                           "},"
                           "\"id\":2,\"index\":2,\"title\":\"알람테스트2\""
                           "}"
                           "]"
                           "}"
                           "}";


void TuyaMQTTClient::begin(WiFiClientSecure &secureClient, const char *broker, int port)
{
  client = &secureClient;
  mqttClient.setClient(secureClient);
  mqttClient.setServer(broker, port);
  mqttClient.setBufferSize(1024);
  mqttClient.setCallback(mqttCallback);
  device_id = NVSStorage::getInstance().getCredential("device_id");
  device_secret = NVSStorage::getInstance().getCredential("device_secret");
}

bool TuyaMQTTClient::connect()
{
  if (mqttClient.connected())
    return true;

  if (device_id.isEmpty() || device_secret.isEmpty())
    return false;

  Serial.printf("TuyaMQTTClient id:%s pw:%s\n", device_id.c_str(), device_secret.c_str());
  client->setCACert(ca_cert);
  const int maxAttempts = 10;
  for (int attempt = 1; attempt <= maxAttempts; ++attempt)
  {
    if (!mqttClient.connected())
    {
      calcSignature(device_id.c_str(), device_secret.c_str());
      Serial.printf("Attempt %d: Connecting to MQTT Broker as %s...\n", attempt, clientID);

      if (mqttClient.connect(clientID, username, password))
      {
        Serial.println("Connected to MQTT broker");

        char auto_subscribe_topic[64];
        sprintf(auto_subscribe_topic, "tylink/%s/channel/downlink/auto_subscribe", device_id.c_str());
        mqttClient.subscribe(auto_subscribe_topic);
        sprintf(publish_topic, "tylink/%s/thing/property/report", device_id.c_str());
        delay(100);
        return true; // 연결 성공
      }
      else
      {
        Serial.print("Failed to connect to MQTT broker, rc=");
        Serial.println(mqttClient.state());
        delay(100); // 실패 시 대기 후 재시도
      }
    }
    else
    {
      return true; // 이미 연결됨
    }
  }
}

void TuyaMQTTClient::loop()
{
  mqttClient.loop();
  String data = TxMessageQueue::getInstance().pop();
  // 데이터가 비어 있으면 무시
  if (data == "")
    return;
  mqttClient.publish(publish_topic, data.c_str());
}

String TuyaMQTTClient::hmac256(const String &signcontent, const String &ds)
{
  byte hashCode[SHA256HMAC_SIZE];
  SHA256 sha256;

  const char *key = ds.c_str();
  size_t keySize = ds.length();

  sha256.resetHMAC(key, keySize);
  sha256.update((const byte *)signcontent.c_str(), signcontent.length());
  sha256.finalizeHMAC(key, keySize, hashCode, sizeof(hashCode));

  String sign = "";
  for (byte i = 0; i < SHA256HMAC_SIZE; ++i)
  {
    sprintf(password + 2 * i, "%02x", hashCode[i]);
    sign += "0123456789ABCDEF"[hashCode[i] >> 4];
    sign += "0123456789ABCDEF"[hashCode[i] & 0xf];
  }

  return sign;
}

int TuyaMQTTClient::calcSignature(const char *deviceId, const char *deviceSecret)
{
  if (NULL == deviceId || NULL == deviceSecret)
  {
    return -1;
  }

  uint32_t timestamp = time(nullptr);

  /* client ID */
  sprintf(username, "%s|signMethod=hmacSha256,timestamp=%d,secureMode=1,accessType=1", deviceId, timestamp);
  Serial.print("username:");
  Serial.println(username);

  /* username */
  sprintf(clientID, "tuyalink_%s", deviceId);
  Serial.print("clientID:");
  Serial.println(clientID);

  /* password */
  int i = 0;
  char passward_stuff[255];
  size_t slen = sprintf(passward_stuff, "deviceId=%s,timestamp=%d,secureMode=1,accessType=1", deviceId, timestamp);
  hmac256(passward_stuff, deviceSecret);

  Serial.print("password:");
  Serial.println(password);

  return 0;
}

void TuyaMQTTClient::mqttCallback(char *topic, byte *payload, unsigned int length)
{
  String message;
  for (unsigned int i = 0; i < length; ++i)
  {
    message += (char)payload[i];
  }
  StaticJsonDocument<4096> doc;
  DeserializationError error = deserializeJson(doc, payload, length);
  if (!error && doc["data"].containsKey("topic"))
  {
    auto a = doc["data"]["topic"].as<String>();
    auto b = doc["data"]["data"].as<String>();
    RxMessageQueue::getInstance().push(a, b);
  }
}



