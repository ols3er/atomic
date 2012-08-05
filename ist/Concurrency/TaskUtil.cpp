#include "stdafx.h"
#include "ist/Concurrency/TaskUtil.h"

namespace ist {

TreeTask::TreeTask()
    : m_parent(NULL)
{
}

void TreeTask::setState( State v )
{
    switch(getState()) {
    case State_Ready:
        if(TreeTask *parent=getParent()) { parent->incrementActiveChildren(); }
        break;
    case State_Completed:
        if(TreeTask *parent=getParent()) { parent->decrementActiveChildren(); }
        break;
    }

    super::setState(v);
}

void TreeTask::waitChildren()
{
    while(getActiveChildrenCount()>0) {
        TaskScheduler::getInstance()->processOneTask();
    }
}

} // namespace ist