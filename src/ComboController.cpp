#include "ComboController.h"

#define ARM_L 0
#define ARM_R 1
#define NECK_UD 2
#define NECK_LR 4
#define TIME_TURN_360 3200
#define TIME_DRIVE_4 1500

ComboController::ComboController(MotorController *r, AudioPlayer *a, ServoController *s, WebSocketsServer *w)
{
    robot = r;
    audio = a;
    servos = s;
    ws = w;
    activeCombo = 0;
    servoStep = 0;
    servoTimer = 0;
    motorStep = 0;
    motorTimer = 0;
}

void ComboController::centerAllServos()
{
    for (int i = 0; i < 7; i++)
    {
        servos->setTarget(i, 307);
    }
}

void ComboController::finishCombo()
{
    robot->stop();
    centerAllServos();
    ws->broadcastTXT("LOG:COMBO " + String(activeCombo) + " FINISHED");
    activeCombo = 0;
}

void ComboController::startCombo(int comboId)
{
    robot->stop();
    centerAllServos();

    activeCombo = comboId;
    servoStep = 0;
    motorStep = 0;
    servoTimer = millis();
    motorTimer = millis();

    ws->broadcastTXT("LOG:START COMBO " + String(comboId));

    if (comboId == 1)
        audio->play("/1_WALL-E.mp3");
    else if (comboId == 2)
        audio->play("/2_EEEEE-VA.mp3");
    else if (comboId == 3)
        audio->play("/3_WALL-EEEEE.mp3");
    else if (comboId == 4)
        audio->play("/4_Hello Dolly.mp3");
}

void ComboController::update()
{
    if (activeCombo == 0)
        return;

    unsigned long now = millis();

    if (activeCombo == 1)
    {
        switch (servoStep)
        {
        case 0:
            if (now - servoTimer >= 600)
            {
                servos->setTarget(ARM_L, 200);
                servoTimer = now;
                servoStep = 1;
            }
            break;
        case 1:
            if (now - servoTimer >= 1000)
            {
                servos->setTarget(ARM_L, 410);
                servoTimer = now;
                servoStep = 2;
            }
            break;
        case 2:
            if (now - servoTimer >= 1000)
            {
                servos->setTarget(ARM_L, 307);
                servoTimer = now;
                servoStep = 3;
            }
            break;
        case 3:
            if (now - servoTimer >= 700)
            {
                finishCombo();
            }
            break;
        }
    }

    else if (activeCombo == 2)
    {
        switch (servoStep)
        {
        case 0:
            if (now - servoTimer >= 600)
            {
                servos->setTarget(NECK_UD, 265);
                servoTimer = now;
                servoStep = 1;
            }
            break;
        case 1:
            if (now - servoTimer >= 1000)
            {
                servos->setTarget(NECK_UD, 350);
                servoTimer = now;
                servoStep = 2;
            }
            break;
        case 2:
            if (now - servoTimer >= 1000)
            {
                servos->setTarget(NECK_UD, 307);
                servoTimer = now;
                servoStep = 3;
            }
            break;
        case 3:
            if (now - servoTimer >= 800)
            {
                servos->setTarget(NECK_LR, 250);
                servoTimer = now;
                servoStep = 4;
            }
            break;
        case 4:
            if (now - servoTimer >= 1000)
            {
                servos->setTarget(NECK_LR, 365);
                servoTimer = now;
                servoStep = 5;
            }
            break;
        case 5:
            if (now - servoTimer >= 1000)
            {
                servos->setTarget(NECK_LR, 307);
                servoTimer = now;
                servoStep = 6;
            }
            break;
        case 6:
            if (now - servoTimer >= 700)
            {
                finishCombo();
            }
            break;
        }
    }

    else if (activeCombo == 3)
    {
        switch (servoStep)
        {
        case 0:
            if (now - servoTimer >= 600)
            {
                servos->setTarget(ARM_L, 210);
                servos->setTarget(NECK_UD, 265);
                servos->setTarget(NECK_LR, 255);
                servoTimer = now;
                servoStep = 1;
            }
            break;
        case 1:
            if (now - servoTimer >= 1200)
            {
                servos->setTarget(ARM_L, 400);
                servos->setTarget(NECK_UD, 350);
                servos->setTarget(NECK_LR, 360);
                servoTimer = now;
                servoStep = 2;
            }
            break;
        case 2:
            if (now - servoTimer >= 1200)
            {
                centerAllServos();
                servoTimer = now;
                servoStep = 3;
            }
            break;
        case 3:
            if (now - servoTimer >= 800)
            {
                finishCombo();
            }
            break;
        }
    }

    else if (activeCombo == 4)
    {
        switch (motorStep)
        {
        case 0:
            if (now - motorTimer >= 800)
            {
                robot->goBack(200);
                motorTimer = now;
                motorStep = 1;
            }
            break;
        case 1:
            if (now - motorTimer >= TIME_DRIVE_4)
            {
                robot->stop();
                motorTimer = now;
                motorStep = 2;
            }
            break;
        case 2:
            if (now - motorTimer >= 800)
            {
                robot->goForward(182);
                motorTimer = now;
                motorStep = 3;
            }
            break;
        case 3:
            if (now - motorTimer >= TIME_DRIVE_4)
            {
                robot->stop();
                motorTimer = now;
                motorStep = 4;
            }
            break;
        case 4:
            if (now - motorTimer >= 800)
            {
                robot->turnRight(128);
                motorTimer = now;
                motorStep = 5;
            }
            break;
        case 5:
            if (now - motorTimer >= TIME_TURN_360)
            {
                robot->stop();
                motorTimer = now;
                motorStep = 6;
            }
            break;
        case 6:
            if (now - motorTimer >= 800)
            {
                robot->turnLeft(128);
                motorTimer = now;
                motorStep = 7;
            }
            break;
        case 7:
            if (now - motorTimer >= TIME_TURN_360)
            {
                robot->stop();
                motorTimer = now;
                motorStep = 8;
            }
            break;
        case 8:
            if (!audio->isPlaying())
            {
                finishCombo();
                return;
            }
            break;
        }

        switch (servoStep)
        {
        case 0:
            if (now - servoTimer >= 1200)
            {
                servos->setTarget(NECK_UD, 270);
                servos->setTarget(ARM_L, 250);
                servos->setTarget(ARM_R, 360);
                servoTimer = now;
                servoStep = 1;
            }
            break;
        case 1:
            if (now - servoTimer >= 1300)
            {
                servos->setTarget(NECK_LR, 255);
                servos->setTarget(ARM_L, 360);
                servos->setTarget(ARM_R, 250);
                servoTimer = now;
                servoStep = 2;
            }
            break;
        case 2:
            if (now - servoTimer >= 1300)
            {
                servos->setTarget(NECK_UD, 345);
                servos->setTarget(NECK_LR, 360);
                servoTimer = now;
                servoStep = 3;
            }
            break;
        case 3:
            if (now - servoTimer >= 1300)
            {
                centerAllServos();
                servoTimer = now;
                servoStep = 0;
            }
            break;
        }
    }
}