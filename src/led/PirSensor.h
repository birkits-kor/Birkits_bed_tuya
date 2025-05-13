#ifndef PIR_SENSOR_H
#define PIR_SENSOR_H

#define PIR_SENSOR_PIN1 34
#define PIR_SENSOR_PIN2 35

class PirSensor
{
public:
    static PirSensor& getInstance();

    bool get(); // 하나라도 감지되면 true

private:
    PirSensor(); // 생성자: 핀 설정
};

#endif // PIR_SENSOR_H
