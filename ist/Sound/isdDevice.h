#ifndef __ist_isd_Device__
#define __ist_isd_Device__

#include "isdTypes.h"

namespace ist {
namespace isd {

class Device
{
private:
    ALCdevice *m_device;
    ALCcontext *m_context;
    std::vector<DeviceResource*> m_resources;
    std::vector<ResourceHandle> m_vacant;

    void addResource(DeviceResource *v);

public:
    Device();
    ~Device();

    Buffer* createBuffer();
    Source* createSource();
    Listener* createListener();

    void deleteResource(ResourceHandle v);
};

} // namespace isd
} // namespace ist

#endif // __ist_isd_Device__