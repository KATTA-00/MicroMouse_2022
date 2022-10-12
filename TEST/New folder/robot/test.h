#ifndef MOUSE
#define MOUSE
//#include <Arduino.h>
#include <Wire.h>

#define rightMotorForward 10
#define rightMotorBack 11
#define leftMotorForward 9
#define leftMotorBack 6

#define disSensorEchoLeft 2
#define disSensorEchoFront 3
#define disSensorEchoRight 4
#define disSensorTrigLeft 5
#define disSensorTrigFront 7
#define disSensorTrigRight 8

#define speedForwardMove 100
#define speedRotation 50

#define kpGoForward 15
#define kpGoLeftRight 0.5
#define kdGoForward 1
#define kpRotation 0.2
#define kdRotaion 1

#define kpMotors 1
#define kdMotors 1

#define frontSensorUpLim 20
#define frontSensorDownLim 10
#define rightleftSesorUpLim 5
#define rightleftSesorDownLim 10
#define wallSenseLimit 10

#define angle90 80
#define angle180 160

float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float elapsedTime, currentTime, previousTime;

class Mouse
{
public:
    short rightMotorSpeed;
    short leftMotorSpeed;
    byte measuredRightSpeed;
    byte measuredLeftSpeed;
    byte pid;
    byte distances[3];
    long durationSensors[3];
    short currentAngle;
    short startAngle;
    byte turnAngle;
    byte distanceErros[3];
    byte disatacneErrorsPre[3];
    byte motorError;

    Mouse()
    {
        pinMode(rightMotorForward, OUTPUT);
        pinMode(rightMotorBack, OUTPUT);
        pinMode(leftMotorForward, OUTPUT);
        pinMode(leftMotorBack, OUTPUT);

        pinMode(disSensorTrigFront, OUTPUT);
        pinMode(disSensorTrigLeft, OUTPUT);
        pinMode(disSensorTrigRight, OUTPUT);
        pinMode(disSensorEchoFront, INPUT);
        pinMode(disSensorEchoLeft, INPUT);
        pinMode(disSensorEchoRight, INPUT);

        analogWrite(rightMotorForward, 0);
        analogWrite(rightMotorBack, 0);
        analogWrite(leftMotorForward, 0);
        analogWrite(leftMotorBack, 0);
    }

    void move(byte nowDirection, byte nextDirection)
    {
        rotation(nowDirection, nextDirection);
    }

    void goForward()
    {
        limiteMotorSpeed();
        analogWrite(leftMotorForward, leftMotorSpeed);
        analogWrite(rightMotorForward, rightMotorSpeed);
    }

    void rightRotate()
    {
        limiteMotorSpeed();
        analogWrite(leftMotorForward, leftMotorSpeed);
        analogWrite(rightMotorBack, rightMotorSpeed);
    }

    void leftRotate()
    {
        limiteMotorSpeed();
        analogWrite(leftMotorBack, leftMotorSpeed);
        analogWrite(rightMotorForward, rightMotorSpeed);
    }

    void rotation(byte nowDirection, byte nextDirection)
    {
        startAngle = getAngle();
        updateCurrentAngle();
        switch (nowDirection)
        {
        case 1:
            if (nextDirection == 2)
            {
                turnAngle = angle90;
                while (currentAngle <= turnAngle)
                {
                    updateRotationMotorSpeed();
                    rightRotate();
                }
            }
            else if (nextDirection == 4)
            {
                turnAngle = angle90;
                while (currentAngle <= turnAngle)
                {
                    updateRotationMotorSpeed();
                    leftRotate();
                }
            }
            else if (nextDirection == 3)
            {
                turnAngle = angle180;
                while (currentAngle <= turnAngle)
                {
                    updateRotationMotorSpeed();
                    rightRotate();
                }
            }
            else
                turnAngle = 0;
            break;

        case 2:
            if (nextDirection == 3)
            {
                turnAngle = angle90;
                while (currentAngle <= turnAngle)
                {
                    updateRotationMotorSpeed();
                    rightRotate();
                }
            }
            else if (nextDirection == 1)
            {
                turnAngle = angle90;
                while (currentAngle <= turnAngle)
                {
                    updateRotationMotorSpeed();
                    leftRotate();
                }
            }
            else if (nextDirection == 4)
            {
                turnAngle = angle180;
                while (currentAngle <= turnAngle)
                {
                    updateRotationMotorSpeed();
                    rightRotate();
                }
            }
            else
                turnAngle = 0;
            break;

        case 3:
            if (nextDirection == 4)
            {
                turnAngle = angle90;
                while (currentAngle <= turnAngle)
                {
                    updateRotationMotorSpeed();
                    rightRotate();
                }
            }
            else if (nextDirection == 2)
            {
                turnAngle = angle90;
                while (currentAngle <= turnAngle)
                {
                    updateRotationMotorSpeed();
                    leftRotate();
                }
            }
            else if (nextDirection == 1)
            {
                turnAngle = angle180;
                while (currentAngle <= turnAngle)
                {
                    updateRotationMotorSpeed();
                    rightRotate();
                }
            }
            else
                turnAngle = 0;
            break;

        case 4:
            if (nextDirection == 1)
            {
                turnAngle = angle90;
                while (currentAngle <= turnAngle)
                {
                    updateRotationMotorSpeed();
                    rightRotate();
                }
            }
            else if (nextDirection == 3)
            {
                turnAngle = angle90;
                while (currentAngle <= turnAngle)
                {
                    updateRotationMotorSpeed();
                    leftRotate();
                }
            }
            else if (nextDirection == 2)
            {
                turnAngle = angle180;
                while (currentAngle <= turnAngle)
                {
                    updateRotationMotorSpeed();
                    rightRotate();
                }
            }
            else
                turnAngle = 0;
            break;
        default:
            break;
        }
        stopMotors();
    }

    void getDisSensors()
    {
        digitalWrite(disSensorTrigFront, LOW);
        //digitalWrite(disSensorTrigLeft, LOW);
        //digitalWrite(disSensorTrigRight, LOW);
        delayMicroseconds(2);
        digitalWrite(disSensorTrigFront, HIGH);
        //digitalWrite(disSensorTrigLeft, HIGH);
        //digitalWrite(disSensorTrigRight, HIGH);
        delayMicroseconds(10);
        digitalWrite(disSensorTrigFront, LOW);
        //digitalWrite(disSensorTrigLeft, LOW);
        //digitalWrite(disSensorTrigRight, LOW);

        //durationSensors[0] = pulseIn(disSensorEchoLeft, HIGH);
        durationSensors[1] = pulseIn(disSensorEchoFront, HIGH);
        //durationSensors[2] = pulseIn(disSensorEchoRight, HIGH);

        //distances[0] = durationSensors[0] * 0.034 / 2;
        distances[1] = durationSensors[1] * 0.034 / 2;
        //distances[2] = durationSensors[2] * 0.034 / 2;
    }

    void updateDisEorrs()
    {
        for (byte i = 0; i < 3; i++)
        {
            disatacneErrorsPre[i] = distanceErros[i];
        }

        if (distances[1] >= frontSensorUpLim)
        {
            distanceErros[1] = frontSensorUpLim - frontSensorDownLim;
        }
        else if (distances[1] < frontSensorUpLim && distances[1] >= frontSensorDownLim)
        {
            distanceErros[1] = distances[1] - frontSensorDownLim;
        }
        else
        {
            distanceErros[1] = 0;
        }

        for (byte i = 0; i < 3; i += 2)
        {
            if (distances[i] >= rightleftSesorUpLim)
            {
                distanceErros[i] = rightleftSesorUpLim - rightleftSesorDownLim;
            }
            else if (distances[i] < rightleftSesorUpLim && distances[i] >= rightleftSesorDownLim)
            {
                distanceErros[i] = distances[i] - rightleftSesorDownLim;
            }
            else
            {
                distanceErros[i] = 0;
            }
        }

        for (byte i = 0; i < 3; i++)
        {
            disatacneErrorsPre[i] = distanceErros[i] - disatacneErrorsPre[i];
        }
    }

    byte getAngelError()
    {
        updateCurrentAngle();
        if (turnAngle - currentAngle >= 180)
        {
            return 180;
        }
        else if (turnAngle - currentAngle <= 0)
        {
            return 0;
        }
        else
        {
            return turnAngle - currentAngle;
        }
    }

    byte getDisPDval()
    {
        return distanceErros[1] * kpGoForward + disatacneErrorsPre[1] * kdGoForward;
    }

    void updateForwardMotorSpeed()
    {
        updateDisEorrs();
        leftMotorSpeed = speedForwardMove + getDisPDval() + distanceErros[0] * kpGoLeftRight;
        rightMotorSpeed = speedForwardMove + getDisPDval() + distanceErros[2] * kpGoLeftRight;
    }

    byte getAnglePDVal()
    {
        return getAngelError() * kpRotation;
    }

    void updateRotationMotorSpeed()
    {
        leftMotorSpeed = speedRotation + getAngelError() * kpRotation;
        rightMotorSpeed = speedRotation + getAngelError() * kpRotation;
        limiteMotorSpeed();
    }

    void updateMeasuredSpeed()
    {
    }

    void updateCurrentAngle()
    {
        if (getAngle() - startAngle > 0)
        {
            currentAngle = getAngle() - startAngle;
        }
        else
        {
            currentAngle = startAngle - getAngle();
        }
    }

    void calibrateForwardMotorSpeed()
    {
        updateMeasuredSpeed();
        if (measuredLeftSpeed != leftMotorSpeed)
        {
            if (measuredLeftSpeed > leftMotorSpeed)
            {
                motorError = measuredLeftSpeed - leftMotorSpeed;
                leftMotorSpeed = leftMotorSpeed - motorError * kpMotors;
            }
            else
            {
                motorError = leftMotorSpeed - measuredLeftSpeed;
                leftMotorSpeed = leftMotorSpeed + motorError * kpMotors;
            }
        }

        if (measuredRightSpeed != rightMotorSpeed)
        {
            if (measuredRightSpeed > rightMotorSpeed)
            {
                motorError = measuredRightSpeed - rightMotorSpeed;
                rightMotorSpeed = rightMotorSpeed - motorError * kpMotors;
            }
            else
            {
                motorError = rightMotorSpeed - measuredRightSpeed;
                rightMotorSpeed = rightMotorSpeed + motorError * kpMotors;
            }
        }
    }

    void calibrateRotationSpeed()
    {
        updateMeasuredSpeed();
        if (measuredLeftSpeed != measuredRightSpeed)
        {
            if (measuredLeftSpeed > measuredRightSpeed)
            {
                motorError = measuredLeftSpeed - measuredRightSpeed;
                leftMotorSpeed = leftMotorSpeed - motorError * kpMotors;
                rightMotorSpeed = rightMotorSpeed + motorError * kpMotors;
            }
            else
            {
                motorError = measuredRightSpeed - measuredLeftSpeed;
                rightMotorSpeed = rightMotorSpeed - motorError * kpMotors;
                leftMotorSpeed = leftMotorSpeed + motorError * kpMotors;
            }
        }
    }

    short getAngle()
    {

        Wire.beginTransmission(0x68);
        Wire.write(0x3B);
        Wire.endTransmission(false);
        Wire.requestFrom(0x68, 6, true);
        AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
        AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
        AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
        // Calculating Roll and Pitch from the accelerometer data
        accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - 0.58;
        accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 1.58;
        // === Read gyroscope data === //
        previousTime = currentTime;
        currentTime = millis();
        elapsedTime = (currentTime - previousTime) / 1000;
        Wire.beginTransmission(0x68);
        Wire.write(0x43);
        Wire.endTransmission(false);
        Wire.requestFrom(0x68, 6, true);
        GyroX = (Wire.read() << 8 | Wire.read()) / 131.0;
        GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
        GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
        // Correct the outputs with the calculated error values
        GyroX = GyroX + 0.56;
        GyroY = GyroY - 2;
        GyroZ = GyroZ + 0.79;
        // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
        gyroAngleX = gyroAngleX + GyroX * elapsedTime;
        gyroAngleY = gyroAngleY + GyroY * elapsedTime;
        yaw = yaw + GyroZ * elapsedTime;
        // Complementary filter - combine acceleromter and gyro angle values
        roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
        pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;

        return roll;
    }

    void startConnectionMPU()
    {
        Wire.begin();
        Wire.beginTransmission(0x68);
        Wire.write(0x6B);
        Wire.write(0x00);
        Wire.endTransmission(true);
    }

    void stopMotors()
    {
        analogWrite(rightMotorForward, 0);
        analogWrite(rightMotorBack, 0);
        analogWrite(leftMotorForward, 0);
        analogWrite(leftMotorBack, 0);
    }

    void limiteMotorSpeed()
    {
        if (leftMotorSpeed <= 0)
        {
            leftMotorSpeed = 0;
        }
        else if (leftMotorSpeed >= 255)
        {
            leftMotorSpeed = 255;
        }
        if (rightMotorSpeed <= 0)
        {
            rightMotorSpeed = 0;
        }
        else if (rightMotorSpeed >= 255)
        {
            rightMotorSpeed = 255;
        }
    }
};

#endif
