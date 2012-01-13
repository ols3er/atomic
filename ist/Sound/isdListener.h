#ifndef __ist_isd_Listener__
#define __ist_isd_Listener__

#include "isdDeviceResource.h"

namespace ist {
namespace isd {


class Listener : public DeviceResource
{
ISD_DECLARE_DEVICE_RESOURCE(Listener);
private:

protected:
    int     getI(ALenum param) const;
    float   getF(ALenum param) const;
    float4  get3F(ALenum param) const;
    void setI(ALenum param, int v);
    void setF(ALenum param, float v);
    void set3F(ALenum param, const vec3& v);

    Listener();
    virtual ~Listener();

public:
    float getGain() const;
    float4 getPosition() const;
    float4 getVelocity() const;

    void setGain(float v);
    void setPosition(const vec3& v);
    void setVelocity(const vec3& v);
};

} // namespace isd
} // namespace ist

#endif // __ist_isd_Listener__