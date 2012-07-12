#include "stdafx.h"
#include "types.h"
#include "Util.h"
#include "Sound/AtomicSound.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/Renderer.h"
#include "Game/AtomicApplication.h"
#include "Game/AtomicGame.h"
#include "Game/World.h"
#include "Game/SPHManager.h"
#include "Game/Collision.h"
#include "Game/Message.h"
#include "Enemy.h"

namespace atomic {


class Bullet_Particle
    : public IEntity
    , public TAttr_TransformMatrix< Attr_Translate >
    , public Attr_Collision
    , public Attr_MessageHandler
{
typedef IEntity super;
typedef TAttr_TransformMatrix< Attr_Translate > transform;
typedef Attr_Collision collision;
typedef Attr_MessageHandler mhandler;
private:
    vec4            m_vel;
    EntityHandle    m_owner;
    float32         m_power;
    int32           m_past_frame;
    int32           m_lifetime;

public:
    Bullet_Particle() : m_owner(0), m_power(50.0f), m_past_frame(0), m_lifetime(600) {}

    EntityHandle getOwner() const   { return m_owner; }
    const vec4& getVelocity() const { return m_vel; }
    int32 getPastFrame() const      { return m_past_frame; }
    int32 getLifeTime() const       { return m_lifetime; }
    float32 getPower() const        { return m_power; }

    void setOwner(EntityHandle v)   { m_owner=v; }
    void setVelocity(const vec4 &v) { m_vel=v; }
    void setPower(float32 v)        { m_power=v; }


    void initialize()
    {
        collision::initializeCollision(getHandle());
        setCollisionShape(CS_SPHERE);
        setCollisionFlags(CF_RECEIVER | CF_AFFECT_SPH);
    }

    virtual void update(float32 dt)
    {
        super::update(dt);

        ++m_past_frame;
        if(m_past_frame==m_lifetime) {
            atomicDeleteEntity(getHandle());
            return;
        }
    }

    virtual void asyncupdate(float32 dt)
    {
        super::asyncupdate(dt);
        move();
        transform::updateTransformMatrix();
        collision::updateCollisionAsSphere(getTransform(), 0.015f);
    }

    void move()
    {
        vec4 pos = getPosition();
        pos += getVelocity();
        setPosition(pos);
    }

    virtual void draw()
    {
        {
            IndivisualParticle particles;
            particles.position = getPosition();
            particles.color = vec4(0.6f, 0.3f, 0.3f, 50.0f);
            particles.glow = vec4(0.45f, 0.15f, 0.15f, 1.0f);
            particles.scale = 1.5f;
            atomicGetParticleRenderer()->addParticle(&particles, 1);
        }
    }

    virtual void eventCollide(const CollideMessage *m)
    {
        if(m->from == getOwner()) { return; }

        if(IEntity *e=atomicGetEntity(m->from)) {
            atomicCall(e, damage, m_power);
        }
        //atomicGetSPHManager()->addFluid(getModel(), getTransform());
        //atomicPlaySE(SE_CHANNEL2, SE_EXPLOSION2, getPosition(), true);
        atomicDeleteEntity(getHandle());
    }

    virtual bool call(uint32 call_id, const variant &v)
    {
        switch(call_id) {
        DEFINE_ECALL1(setOwner, EntityHandle);
        DEFINE_ECALL1(setVelocity, vec4);
        DEFINE_ECALL1(setPower, float32);
        default: return super::call(call_id, v) ||
                     transform::call(call_id, v) ||
                     mhandler::call(call_id, v);
        }
    }

    virtual bool query(uint32 query_id, variant &v) const
    {
        switch(query_id) {
        DEFINE_EQUERY(getOwner);
        DEFINE_EQUERY(getVelocity);
        DEFINE_EQUERY(getPower);
        default: return super::query(query_id, v) ||
                     transform::query(query_id, v);
        }
        return false;
    }
};
atomicImplementEntity(Bullet_Particle, ECID_BULLET, ESID_BULLET_PARTICLE);


class Bullet_Simple
    : public IEntity
    , public TAttr_TransformMatrix< TAttr_RotateSpeed<Attr_DoubleAxisRotation> >
    , public Attr_ParticleSet
    , public Attr_Collision
    , public Attr_MessageHandler
{
typedef IEntity super;
typedef TAttr_TransformMatrix< TAttr_RotateSpeed<Attr_DoubleAxisRotation> > transform;
typedef Attr_ParticleSet model;
typedef Attr_Collision collision;
typedef Attr_MessageHandler mhandler;
private:
    vec4            m_vel;
    EntityHandle    m_owner;
    float32         m_power;
    int32           m_past_frame;
    int32           m_lifetime;

public:
    Bullet_Simple() : m_owner(0), m_power(50.0f), m_past_frame(0), m_lifetime(600) {}

    EntityHandle getOwner() const   { return m_owner; }
    const vec4& getVelocity() const { return m_vel; }
    int32 getPastFrame() const      { return m_past_frame; }
    int32 getLifeTime() const       { return m_lifetime; }
    float32 getPower() const        { return m_power; }

    void setOwner(EntityHandle v)   { m_owner=v; }
    void setVelocity(const vec4 &v) { m_vel=v; }
    void setPower(float32 v)        { m_power=v; }

    void initialize()
    {
        collision::initializeCollision(getHandle());
        setCollisionShape(CS_SPHERE);
        setCollisionFlags(CF_RECEIVER | CF_AFFECT_SPH);

        setModel(PSET_SPHERE_SMALL);
        setDiffuseColor(vec4(0.6f, 0.6f, 0.6f, 80.0f));
        setGlowColor(vec4(1.0f, 0.7f, 0.1f, 0.0f));
        //setAxis1(GenRandomUnitVector3());
        //setAxis2(GenRandomUnitVector3());
        setRotateSpeed1(1.5f);
        setRotateSpeed2(1.5f);
    }

    virtual void update(float32 dt)
    {
        super::update(dt);

        ++m_past_frame;
        if(m_past_frame==m_lifetime) {
            atomicDeleteEntity(getHandle());
            return;
        }
    }

    virtual void asyncupdate(float32 dt)
    {
        super::asyncupdate(dt);
        move();
        transform::updateRotate(dt);
        transform::updateTransformMatrix();
        collision::updateCollisionByParticleSet(getModel(), getTransform(), 0.5f);
    }

    void move()
    {
        vec4 pos = getPosition();
        pos += getVelocity();
        setPosition(pos);
    }

    virtual void draw()
    {
        vec4 diffuse= getDiffuseColor();
        vec4 glow   = getGlowColor();
        vec4 light  = glow;

        {
            PointLight l;
            l.setPosition(getPosition() + vec4(0.0f, 0.0f, 0.15f, 1.0f));
            l.setRadius(0.4f);
            l.setColor(light);
            atomicGetLights()->addLight(l);
        }
        PSetInstance inst;
        inst.diffuse = diffuse;
        inst.glow = glow;
        inst.flash = vec4();
        inst.elapsed = (float32)m_past_frame;
        atomicGetSPHRenderer()->addPSetInstance(getModel(), getTransform(), inst);
    }

    virtual void eventCollide(const CollideMessage *m)
    {
        if(m->from == getOwner()) { return; }

        if(IEntity *e=atomicGetEntity(m->from)) {
            atomicCall(e, damage, m_power);
        }
        atomicGetSPHManager()->addFluid(getModel(), getTransform());
        atomicPlaySE(SE_CHANNEL2, SE_EXPLOSION2, getPosition(), true);
        atomicDeleteEntity(getHandle());
    }

    virtual bool call(uint32 call_id, const variant &v)
    {
        switch(call_id) {
        DEFINE_ECALL1(setOwner, EntityHandle);
        DEFINE_ECALL1(setVelocity, vec4);
        DEFINE_ECALL1(setPower, float32);
        default: return super::call(call_id, v) ||
                        transform::call(call_id, v) ||
                        model::call(call_id, v) ||
                        mhandler::call(call_id, v);
        }
    }

    virtual bool query(uint32 query_id, variant &v) const
    {
        switch(query_id) {
        DEFINE_EQUERY(getOwner);
        DEFINE_EQUERY(getVelocity);
        DEFINE_EQUERY(getPower);
        default: return super::query(query_id, v) ||
                     transform::query(query_id, v) ||
                     model::query(query_id, v);
        }
        return false;
    }
};
atomicImplementEntity(Bullet_Simple, ECID_BULLET, ESID_BULLET_SIMPLE);

} // namespace atomic
