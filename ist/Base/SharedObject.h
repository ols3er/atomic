﻿#ifndef __ist_Base_SharedObject_h_
#define __ist_Base_SharedObject_h_

#include "ist/Base/Types.h"
#include "ist/Base/New.h"
#include "ist/Concurrency/Atomic.h"

namespace ist {

    class SharedObject
    {
    public:
        SharedObject() : m_ref_counter(1) {}
        virtual ~SharedObject() {}
        void addRef()           { ++m_ref_counter; }
        void release()          { if(--m_ref_counter==0) { onZeroRef(); } }
        void setRef(int32 v)    { m_ref_counter=v; }
        int32 getRef() const    { return m_ref_counter; }

    protected:
        virtual void onZeroRef() { istDelete(this); }

    private:
        atomic_int32 m_ref_counter;
    };

    inline void intrusive_ptr_add_ref( SharedObject *obj ) { obj->addRef(); }
    inline void intrusive_ptr_release( SharedObject *obj ) { obj->release(); }

} // namespace ist

#endif // __ist_Base_SharedObject_h_
