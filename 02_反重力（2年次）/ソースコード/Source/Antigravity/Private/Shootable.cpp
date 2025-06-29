// Fill out your copyright notice in the Description page of Project Settings.


#include "Shootable.h"
#include "Bullet.h"
// Add default functionality here for any IShootable functions that are not pure virtual.

 void IShootable::Init()
{

}
 bool IShootable::Hit(AActor* OtherActor)
{
	 return true;
}

 void IShootable::Update(float DeltaTime)
 {

 }