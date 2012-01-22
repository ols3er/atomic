#ifndef __atomic_Game_Entity__
#define __atomic_Game_Entity__

#include "EntityClass.h"

#define atomicImplementEntity(class_name, category_id, class_id)\
class class_name;                                               \
template<> struct EntityTraits<class_name>                      \
{                                                               \
    enum {                                                      \
    CATEGORY_ID = category_id,                                  \
    CLASS_ID = class_id,                                        \
    };                                                          \
};                                                              \
template<> IEntity* EntitySet::createEntity<class_name>()    \
{                                                               \
    class_name *t = istNew(class_name)();                      \
    typedef EntityTraits<class_name> traits;                    \
    addEntity(traits::CATEGORY_ID, traits::CLASS_ID, t);        \
    return t;                                                   \
}



namespace atomic {


class IEntity;
template<class T> struct EntityTraits;




class IEntity
{
friend class EntitySet;
private:
    EntityHandle m_ehandle;

    bool isValidHandle(EntityHandle h);
    void setHandle(uint32 h) { m_ehandle=h; }

public:
    // コンストラクタではメンバ変数初期化以外の処理を行なってはいけない。他は initialize() で行う。
    // (ID がコンストラクタの後に決まるため、子オブジェクトの処理順などを適切に行うにはこうする必要がある)
    IEntity() : m_ehandle(0) {}
    virtual ~IEntity() {}
    uint32 getHandle() const    { return m_ehandle; }

    // 初期化処理
    virtual void initialize()   {}

    // 終了処理。
    // EntitySet から開放されたタイミングで呼ばれる。
    // 参照カウンタ方式を取る場合 EntitySet から解放されても delete はされない可能性があるため、
    // そのようなケースでデストラクタと使い分ける必要が出てくる。
    virtual void finalize()     {}

    // 同期更新
    virtual void update(float32 dt)     {}

    // 非同期更新。
    // Entity 間の更新は並列に行われるが、その間、衝突判定や描画などの他のモジュールの更新は行われない。
    // (それらは Entity の更新が全て終わってから行われる)
    // 位置などの更新を 1 フレーム遅らせて他モジュールとも並列に更新したかったが、それだとどうしても衝突の押し返しが不自然になるため、こうなった。
    virtual void asyncupdate(float32 dt){}

    // 描画データを作って Renderer に渡す。
    // (渡すだけ。この中で i3d::Device などの描画 API を直接呼んではならない)
    virtual void draw()                 {}


    // call_id に対応するメソッドを引数 v で呼ぶ。 (主に setHoge() 系)
    // Routine や外部スクリプトとの連動用。
    virtual bool call(uint32 call_id, const variant &v) { return false; }

    // query_id に対応するメソッドを呼んで v に結果を格納する。(主に getHoge() 系)
    // Routine や外部スクリプトとの連動用。
    virtual bool query(uint32 query_id, variant &v) const { return false; }
};



class EntitySet : public IAtomicGameModule
{
public:
    typedef stl::vector<EntityHandle> HandleCont;
    typedef stl::vector<IEntity*> EntityCont;
    typedef stl::vector<Task*> TaskCont;

private:
    HandleCont m_vacant[ECID_END][ESID_MAX];
    EntityCont m_entities[ECID_END][ESID_MAX];
    EntityCont m_new_entities;
    HandleCont m_all;
    TaskCont m_tasks;

    void addEntity(uint32 categoryid, uint32 classid, IEntity *e);
    void resizeTasks(uint32 n);

public:
    EntitySet();
    ~EntitySet();

    void frameBegin();
    void update(float32 dt);

    // なにもしない。
    // 不本意ながら Entity の非同期更新は update() 内で行う。 (IEntity を参照)
    void asyncupdate(float32 dt);

    void draw();
    void frameEnd();

    IEntity* getEntity(EntityHandle h);
    void deleteEntity(EntityHandle h);
    template<class T> IEntity* createEntity();
};



} // namespace atomic
#endif // __atomic_Game_Entity__
