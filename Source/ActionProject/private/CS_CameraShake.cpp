// Fill out your copyright notice in the Description page of Project Settings.


#include "CS_CameraShake.h"

UCS_CameraShake::UCS_CameraShake()
{
	OscillationDuration = 0.25f;			//진동지속시간
	OscillationBlendInTime = 0.05f;			//진동이 0에서 1까지 조정되는 블렌드인 기간
    OscillationBlendOutTime = 0.05f;        //진동이 0에서 1까지 조정되는 블렌드아웃 기간

    RotOscillation.Pitch.Amplitude = FMath::RandRange(5.0f, 10.0f);         //pitch 진폭
    RotOscillation.Pitch.Frequency = FMath::RandRange(25.0f, 35.0f);        //pitch 주파수

    RotOscillation.Yaw.Amplitude = FMath::RandRange(5.0f, 10.0f);           //yaw 진폭
    RotOscillation.Yaw.Frequency = FMath::RandRange(25.0f, 35.0f);          //yaw 주파수
}
