// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleWheelsFront.h"

UVehicleWheelsFront::UVehicleWheelsFront() 
{
	AxleType = EAxleType::Front;
	bAffectedByEngine = true;
	bAffectedBySteering = true;
	MaxSteerAngle = 45;
}