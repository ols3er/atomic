#ifndef __atomic_Game_Character_Attributes__
#define __atomic_Game_Character_Attributes__

#include "Util.h"
#include "Game/Collision.h"

struct sphFluidMessage;

namespace atomic {

class Attr_RefCount
{
private:
    uint32 m_ref_count;

protected:
    void setRefCount(uint32 v) { m_ref_count=v; }

public:
    Attr_RefCount() : m_ref_count(1) {}
    uint32 getRefCount() const  { return m_ref_count; }
    uint32 addRefCount()        { return ++m_ref_count; }
    uint32 release()            { return --m_ref_count; }

    bool call(uint32 call_id, const variant &v)
    {
        switch(call_id) {
            DEFINE_ECALL1(setRefCount, uint32);
            DEFINE_ECALL0(addRefCount);
            DEFINE_ECALL0(release);
        }
        return false;
    }

    bool query(uint32 query_id, variant &v) const
    {
        switch(query_id) {
            DEFINE_EQUERY(getRefCount);
        }
        return false;
    }
};


class Attr_Translate
{
protected:
    vec4 m_pos;

public:
    Attr_Translate() {}
    const vec4& getPosition() const { return m_pos; }
    void setPosition(const vec4& v) { m_pos=v; }

    mat4 computeMatrix() const
    {
        mat4 mat;
        mat = glm::translate(mat, reinterpret_cast<const vec3&>(m_pos));
        return mat;
    }

    bool call(uint32 call_id, const variant &v)
    {
        switch(call_id) {
            DEFINE_ECALL1(setPosition, vec4);
        }
        return false;
    }

    bool query(uint32 query_id, variant &v) const
    {
        switch(query_id) {
            DEFINE_EQUERY(getPosition);
        }
        return false;
    }
};

class Attr_Transform
{
typedef Attr_Translate super;
private:
    vec4 m_pos;
    vec4 m_scale;
    vec4 m_axis;
    float32 m_rot;

public:
    Attr_Transform()
        : m_scale(1.0f, 1.0f, 1.0f, 0.0f)
        , m_axis(0.0f, 0.0f, 1.0f, 0.0f)
        , m_rot(0.0f)
    {}

    const vec4& getPosition() const { return m_pos; }
    const vec4& getScale() const    { return m_scale; }
    const vec4& getAxis() const     { return m_axis; }
    float32 getRotate() const       { return m_rot; }

    void setPosition(const vec4& v) { m_pos=v; }
    void setScale(const vec4& v)    { m_scale=v; }
    void setAxis(const vec4& v)     { m_axis=v; }
    void setRotate(float32 v)       { m_rot=v; }

    mat4 computeMatrix() const
    {
        mat4 mat;
        mat = glm::translate(mat, reinterpret_cast<const vec3&>(m_pos));
        mat = glm::rotate(mat, m_rot, reinterpret_cast<const vec3&>(m_axis));
        mat = glm::scale(mat, reinterpret_cast<const vec3&>(m_scale));
        return mat;
    }

    bool call(uint32 call_id, const variant &v)
    {
        switch(call_id) {
            DEFINE_ECALL1(setPosition, vec4);
            DEFINE_ECALL1(setScale, vec4);
            DEFINE_ECALL1(setAxis, vec4);
            DEFINE_ECALL1(setRotate, float32);
        default: return false;
        }
    }

    bool query(uint32 query_id, variant &v) const
    {
        switch(query_id) {
            DEFINE_EQUERY(getPosition);
            DEFINE_EQUERY(getScale);
            DEFINE_EQUERY(getAxis);
            DEFINE_EQUERY(getRotate);
        default: return false;
        }
    }

    void update(float32 dt) {}
    void asyncupdate(float32 dt) {}
};

class Attr_DoubleAxisRotation
{
private:
    struct Attr_DoubleAxisRotationData
    {
        vec4 m_pos;
        vec4 m_scale;
        vec4 m_axis1;
        vec4 m_axis2;
        float32 m_rot1;
        float32 m_rot2;

        Attr_DoubleAxisRotationData()
            : m_scale(1.0f, 1.0f, 1.0f, 0.0f)
            , m_axis1(0.0f, 1.0f, 0.0f, 0.0f)
            , m_axis2(0.0f, 0.0f, 1.0f, 0.0f)
            , m_rot1(0.0f), m_rot2(0.0f)
        {}
    };
    Attr_DoubleAxisRotationData m_td_sync, m_td_async;

public:
    Attr_DoubleAxisRotation()
    {
    }

    const vec4& getPosition() const { return m_td_async.m_pos; }
    const vec4& getScale() const    { return m_td_async.m_scale; }
    const vec4& getAxis1() const    { return m_td_async.m_axis1; }
    const vec4& getAxis2() const    { return m_td_async.m_axis2; }
    float32 getRotate1() const      { return m_td_async.m_rot1; }
    float32 getRotate2() const      { return m_td_async.m_rot2; }

    void setPosition(const vec4& v) { m_td_async.m_pos=v; }
    void setScale(const vec4& v)    { m_td_async.m_scale=v; }
    void setAxis1(const vec4& v)    { m_td_async.m_axis1=v; }
    void setAxis2(const vec4& v)    { m_td_async.m_axis2=v; }
    void setRotate1(float32 v)      { m_td_async.m_rot1=v; }
    void setRotate2(float32 v)      { m_td_async.m_rot2=v; }

    mat4 computeMatrix() const
    {
        mat4 mat;
        mat = glm::translate(mat, reinterpret_cast<const vec3&>(m_td_async.m_pos));
        mat = glm::rotate(mat, m_td_async.m_rot2, reinterpret_cast<const vec3&>(m_td_async.m_axis2));
        mat = glm::rotate(mat, m_td_async.m_rot1, reinterpret_cast<const vec3&>(m_td_async.m_axis1));
        mat = glm::scale(mat, reinterpret_cast<const vec3&>(m_td_async.m_scale));
        return mat;
    }

    bool call(uint32 call_id, const variant &v)
    {
        switch(call_id) {
            DEFINE_ECALL1(setPosition, vec4);
            DEFINE_ECALL1(setScale, vec4);
            DEFINE_ECALL1(setAxis1, vec4);
            DEFINE_ECALL1(setAxis2, vec4);
            DEFINE_ECALL1(setRotate1, float32);
            DEFINE_ECALL1(setRotate2, float32);
            default: return false;
        }
    }

    bool query(uint32 query_id, variant &v) const
    {
        switch(query_id) {
            DEFINE_EQUERY(getPosition);
            DEFINE_EQUERY(getScale);
            DEFINE_EQUERY(getAxis1);
            DEFINE_EQUERY(getAxis2);
            DEFINE_EQUERY(getRotate1);
            DEFINE_EQUERY(getRotate2);
            default: return false;
        }
    }

    void update(float32 dt)
    {
        m_td_sync = m_td_async;
    }

    void asyncupdate(float32 dt) {}
};

template<class T>
class TAttr_RotateSpeed : public T
{
typedef T super;
private:
    struct TAttr_RotateSpeedData
    {
        float32 m_rspeed1;
        float32 m_rspeed2;

        TAttr_RotateSpeedData() : m_rspeed1(0.0f), m_rspeed2(0.0f) {}
    };

    TAttr_RotateSpeedData m_rs_sync, m_rs_async;

public:
    TAttr_RotateSpeed()
    {}

    float32 getRotateSpeed1() const { return m_rs_async.m_rspeed1; }
    float32 getRotateSpeed2() const { return m_rs_async.m_rspeed2; }
    void setRotateSpeed1(float32 v) { m_rs_async.m_rspeed1=v; }
    void setRotateSpeed2(float32 v) { m_rs_async.m_rspeed2=v; }

    bool call(uint32 call_id, const variant &v)
    {
        switch(call_id) {
        DEFINE_ECALL1(setRotateSpeed1, float32);
        DEFINE_ECALL1(setRotateSpeed2, float32);
        default: return super::call(call_id, v);
        }
    }

    bool query(uint32 query_id, variant &v) const
    {
        switch(query_id) {
            DEFINE_EQUERY(getRotateSpeed1);
            DEFINE_EQUERY(getRotateSpeed2);
            default: return super::query(query_id, v);
        }
    }

    void update(float32 dt)
    {
        super::update(dt);
        m_rs_sync = m_rs_async;
    }

    void asyncupdate(float32 dt)
    {
        super::asyncupdate(dt);
        this->setRotate1(this->getRotate1()+getRotateSpeed1());
        this->setRotate2(this->getRotate2()+getRotateSpeed2());
    }
};




class Attr_ParticleSet
{
private:
    vec4 m_diffuse_color;
    vec4 m_glow_color;
    PSET_RID m_psetid;

public:
    Attr_ParticleSet() : m_psetid(PSET_CUBE_SMALL)
    {}

    void setDiffuseColor(const vec4 &v) { m_diffuse_color=v; }
    void setGlowColor(const vec4 &v)    { m_glow_color=v; }
    void setModel(PSET_RID v)           { m_psetid=v; }
    const vec4& getDiffuseColor() const { return m_diffuse_color; }
    const vec4& getGlowColor() const    { return m_glow_color; }
    PSET_RID getModel() const           { return m_psetid; }

    bool call(uint32 call_id, const variant &v)
    {
        switch(call_id) {
            DEFINE_ECALL1(setDiffuseColor, vec4);
            DEFINE_ECALL1(setGlowColor, vec4);
            DEFINE_ECALL1(setModel, PSET_RID);
        }
        return false;
    }

    bool query(uint32 query_id, variant &v) const
    {
        switch(query_id) {
            DEFINE_EQUERY(getDiffuseColor);
            DEFINE_EQUERY(getGlowColor);
            DEFINE_EQUERY(getModel);
        }
        return false;
    }
};


class Attr_Collision
{
private:
    CollisionHandle m_collision;
    EntityHandle m_owner_handle;

public:
    Attr_Collision() : m_collision(0), m_owner_handle(0)
    {
    }

    ~Attr_Collision()
    {
        finalizeCollision();
    }

    void setCollisionFlags(int32 v)
    {
        if(CollisionEntity *ce=atomicGetCollision(m_collision)) {
            ce->setFlags(v);
        }
    }

    uint32 getCollisionFlags() const
    {
        if(CollisionEntity *ce=atomicGetCollision(m_collision)) {
            return ce->getFlags();
        }
        return 0;
    }

    void initializeCollision(EntityHandle h)
    {
        m_owner_handle = h;
    }

    void finalizeCollision()
    {
        if(m_collision!=0) {
            atomicDeleteCollision(m_collision);
            m_collision = 0;
        }
    }

    void setCollisionShape(COLLISION_SHAPE cs)
    {
        finalizeCollision();
        if(cs==CS_NULL) {
            m_collision = 0;
            return;
        }

        CollisionEntity *ce = NULL;
        switch(cs) {
        case CS_BOX:    ce = atomicCreateCollision(CollisionBox);   break;
        case CS_SPHERE: ce = atomicCreateCollision(CollisionSphere);break;
        default: istAssert("unknown collision shape\n"); return;
        }
        ce->setGObjHandle(m_owner_handle);
        m_collision = ce->getCollisionHandle();
    }

    CollisionHandle getCollisionHandle() const
    {
        return m_collision;
    }

    void updateCollision(PSET_RID psid, const mat4 &t, float32 scale)
    {
        if(CollisionEntity *ce = atomicGetCollision(m_collision)) {
            switch(ce->getShape()) {
            case CS_SPHERE:
                {
                    vec4 pos = t * vec4(0.0f, 0.0f, 0.0f, 1.0f);
                    float radius = atomicGetRigidInfo(psid)->sphere_radius * scale;
                    UpdateCollisionSphere(*static_cast<CollisionSphere*>(atomicGetCollision(m_collision)), pos, radius);
                }
                break;
            case CS_BOX:
                {
                    vec4 box_size = (vec4&)atomicGetRigidInfo(psid)->box_size * scale;
                    UpdateCollisionBox(*static_cast<CollisionBox*>(atomicGetCollision(m_collision)), t, box_size);
                }
                break;
            }
        }
    }

    bool call(uint32 call_id, const variant &v)
    {
        switch(call_id) {
            DEFINE_ECALL1(setCollisionFlags, uint32);
            DEFINE_ECALL1(setCollisionShape, COLLISION_SHAPE);
         }
        return false;
    }

    bool query(uint32 query_id, variant &v) const
    {
        switch(query_id) {
            DEFINE_EQUERY(getCollisionFlags);
            DEFINE_EQUERY(getCollisionHandle);
        }
        return false;
    }
};



struct CollideMessage;
struct DamageMessage;
struct DestroyMessage;
struct KillMessage;

class Attr_MessageHandler
{
public:
    virtual void eventCollide(const CollideMessage *m)  {}
    virtual void eventFluid(const sphFluidMessage *m)   {}
    virtual void eventDamage(const DamageMessage *m)    {}
    virtual void eventDestroy(const DestroyMessage *m)  {}
    virtual void eventKill(const KillMessage *m)        {}

    bool call(uint32 call_id, const variant &v)
    {
        switch(call_id) {
            DEFINE_ECALL1(eventCollide, const CollideMessage*);
            DEFINE_ECALL1(eventFluid,   const sphFluidMessage*);
            DEFINE_ECALL1(eventDamage,  const DamageMessage*);
            DEFINE_ECALL1(eventDestroy, const DestroyMessage*);
            DEFINE_ECALL1(eventKill,    const KillMessage*);
        }
        return false;
    }
};

class Attr_Bloodstain
{
private:
    stl::vector<BloodstainParticle> m_bloodstain;
    uint32 m_bloodstain_count;

public:
    Attr_Bloodstain() : m_bloodstain_count(0)
    {
        m_bloodstain.reserve(256);
    }

    void addBloodstain(const vec4 pos)
    {
        if(!atomicGetConfig()->show_bloodstain) { return; }

        if(++m_bloodstain_count % 128 == 0) {
            BloodstainParticle tmp;
            tmp.position = pos;
            tmp.lifetime = 1.0f;
            m_bloodstain.push_back(tmp);
        }
    }

    void updateBloodstain()
    {
        uint32 n = m_bloodstain.size();
        for(uint32 i=0; i<n; ++i) {
            BloodstainParticle &bsp = m_bloodstain[i];
            bsp.lifetime -= 0.002f;
        }
        m_bloodstain.erase(
            stl::remove_if(m_bloodstain.begin(), m_bloodstain.end(), BloodstainParticle_IsDead()),
            m_bloodstain.end());
    }

    uint32 getNumBloodstainParticles() const { return m_bloodstain.size(); }
    const BloodstainParticle* getBloodStainParticles() const { return &m_bloodstain[0]; }
};

} // namespace atomic
#endif // __atomic_Game_Character_Attributes__
