#ifndef CONFIG_H
#define CONFIG_H

// MQTT Broker settings
const int mqtt_port = 8883;                           // MQTT port (TLS)
const char *mqtt_broker = "m1.tuyaus.com";            // EMQX broker endpoint
const char *mqtt_topic = "tylink/%s/thing/model/get"; // MQTT topic

#endif // CONFIG_H