#ifndef __atomic_Game_Collision__
#define __atomic_Game_Collision__

#include "GPGPU/SPH.cuh"

namespace atomic {

enum COLLISION_SHAPE {
    CS_UNKNOWN,
    CS_PLANE,
    CS_SPHERE,
    CS_BOX,
    CS_BEAM,
    CS_END,
};
enum COLLISION_FLAG {
    CF_RECEIVER     = 1 << 0,
    CF_SENDER       = 1 << 1,
    CF_AFFECT_SPH   = 1 << 2,
};

struct BoundingBox
{
    vec4 ur;
    vec4 bl;

    void adjust() {
        if(ur.x < bl.x) { stl::swap<float32>(ur.x, bl.x); }
        if(ur.y < bl.y) { stl::swap<float32>(ur.y, bl.y); }
        if(ur.z < bl.z) { stl::swap<float32>(ur.z, bl.z); }
    }
    float32 getXLength() const { return ur.x-bl.x; }
    float32 getYLength() const { return ur.y-bl.y; }
    float32 getZLength() const { return ur.z-bl.z; }
    vec4 getUUU() const { return vec4(ur.x, ur.y, ur.z, 0.0f); }
    vec4 getBUU() const { return vec4(bl.x, ur.y, ur.z, 0.0f); }
    vec4 getUBU() const { return vec4(ur.x, bl.y, ur.z, 0.0f); }
    vec4 getUUB() const { return vec4(ur.x, ur.y, bl.z, 0.0f); }
    vec4 getBBU() const { return vec4(bl.x, bl.y, ur.z, 0.0f); }
    vec4 getBUB() const { return vec4(bl.x, ur.y, bl.z, 0.0f); }
    vec4 getUBB() const { return vec4(ur.x, bl.y, bl.z, 0.0f); }
    vec4 getBBB() const { return vec4(bl.x, bl.y, bl.z, 0.0f); }
};

class CollisionSet;
typedef uint32 CollisionHandle;


// virtual �ȃf�X�g���N�^���Ȃ��͈̂Ӑ}�I�B
// ������p������I�u�W�F�N�g�̓f�[�^�ێ��ȊO�̂��Ƃ͂���Ă͂����Ȃ��B
struct CollisionEntity
{
friend class CollisionSet;
private:
    union {
        struct {
            COLLISION_SHAPE m_shape;
            CollisionHandle m_col_handle;
            EntityHandle    m_gobj_handle;
            int32           m_flags; // FLAG
        };
        float4 padding;
    };

    void SetCollisionHandle(CollisionHandle v) { m_col_handle=v; }

protected:
    void setShape(COLLISION_SHAPE v) { m_shape=v; }

public:
    CollisionEntity() : m_shape(CS_UNKNOWN), m_col_handle(0), m_gobj_handle(0), m_flags(CF_RECEIVER|CF_SENDER|CF_AFFECT_SPH) {}
    COLLISION_SHAPE getShape() const            { return m_shape; }
    CollisionHandle getCollisionHandle() const  { return m_col_handle; }
    EntityHandle    getGObjHandle() const       { return m_gobj_handle; }
    int32           getFlags() const            { return m_flags; }

    void setGObjHandle(EntityHandle v)  { m_gobj_handle=v; }
    void setFlags(int32 v)              { m_flags=v; }
};

struct CollisionPlane : public CollisionEntity
{
public:
    vec4 plane;

    CollisionPlane() { setShape(CS_PLANE); }

};

struct CollisionSphere : public CollisionEntity
{
public:
    BoundingBox bb;
    vec4 pos_r; // w=radius

    CollisionSphere() { setShape(CS_SPHERE); }
};

struct CollisionBox : public CollisionEntity
{
public:
    BoundingBox bb;
    vec4 position;
    vec4 planes[6];

    CollisionBox() { setShape(CS_BOX); }
};


struct CollideMessage
{
    union {
        struct {
            EntityHandle from;
            EntityHandle to;
        };
        float4 padding;
    };
    vec4 direction; // w=�߂荞�ݗ�
};


class CollideTask;

// ���󑍓�������c
class CollisionSet : boost::noncopyable
{
friend class CollideTask;
public:
    typedef stl::vector<CollideTask*>       TaskCont;
    typedef stl::vector<CollisionHandle>    HandleCont;
    typedef stl::vector<CollisionEntity*>   EntityCont;
    typedef stl::vector<CollideMessage>     MessageCont;

private:
    TaskCont    m_tasks;
    EntityCont  m_entities;
    HandleCont  m_vacant;
    uint32      m_active_tasks;

    void addEntity(CollisionEntity *e);
    void resizeTasks(uint32 n);

public:
    CollisionSet();
    ~CollisionSet();

    void updateBegin(float32 dt);
    void update(float32 dt);
    void updateEnd();
    void asyncupdate(float32 dt);

    CollisionEntity* getEntity(CollisionHandle h);
    template<class T> T* createEntity();
    void deleteEntity(CollisionHandle e);
    void deleteEntity(CollisionEntity *e);

    uint32 collide(CollisionEntity *e, MessageCont &m);
};



} // namespace atomic
#endif //__atomic_Game_Collision__