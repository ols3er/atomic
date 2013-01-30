﻿#ifndef atomic_FunctionID_h
#define atomic_FunctionID_h

namespace atomic {

istSEnumBlock(FunctionID,
    istSEnum(FID_unknown),

    istSEnum(FID_kill),
    istSEnum(FID_destroy),
    istSEnum(FID_setRefCount),
    istSEnum(FID_addRefCount),
    istSEnum(FID_release),
    istSEnum(FID_eventCollide),
    istSEnum(FID_eventFluid),
    istSEnum(FID_eventDamage),
    istSEnum(FID_eventDestroy),
    istSEnum(FID_eventKill),
    istSEnum(FID_damage),
    istSEnum(FID_setDiffuseColor),
    istSEnum(FID_setGlowColor),
    istSEnum(FID_setModel),
    istSEnum(FID_setCollisionShape),
    istSEnum(FID_setCollisionFlags),
    istSEnum(FID_setLife),
    istSEnum(FID_setRoutine),
    istSEnum(FID_setOwner),
    istSEnum(FID_setVelocity),
    istSEnum(FID_setAccel),
    istSEnum(FID_setPower),
    istSEnum(FID_setPosition),
    istSEnum(FID_setScale),
    istSEnum(FID_setAxis),
    istSEnumEq(FID_setAxis1, FID_setAxis),
    istSEnum(FID_setAxis2),
    istSEnum(FID_setRotate),
    istSEnumEq(FID_setRotate1, FID_setRotate),
    istSEnum(FID_setRotate2),
    istSEnum(FID_setRotateSpeed),
    istSEnumEq(FID_setRotateSpeed1, FID_setRotateSpeed),
    istSEnum(FID_setRotateSpeed2),
    istSEnum(FID_setDirection),
    istSEnum(FID_setSpeed),
    istSEnum(FID_setLightRadius),
    istSEnum(FID_setExplosionSE),
    istSEnum(FID_setExplosionChannel),

    istSEnum(FID_getRefCount),
    istSEnum(FID_getDiffuseColor),
    istSEnum(FID_getGlowColor),
    istSEnum(FID_getModel),
    istSEnum(FID_getCollisionHandle),
    istSEnum(FID_getCollisionFlags),
    istSEnum(FID_getLife),
    istSEnum(FID_getOwner),
    istSEnum(FID_getVelocity),
    istSEnum(FID_getPower),
    istSEnum(FID_getPosition),
    istSEnum(FID_getScale),
    istSEnum(FID_getAxis),
    istSEnumEq(FID_getAxis1, FID_getAxis),
    istSEnum(FID_getAxis2),
    istSEnum(FID_getRotate),
    istSEnumEq(FID_getRotate1, FID_getRotate),
    istSEnum(FID_getRotate2),
    istSEnum(FID_getDirection),
    istSEnum(FID_getSpeed),
    istSEnum(FID_getRotateSpeed),
    istSEnumEq(FID_getRotateSpeed1, FID_setRotateSpeed),
    istSEnum(FID_getRotateSpeed2),

    istSEnum(FID_End),
);

} // namespace atomic

#endif atomic_FunctionID_h
