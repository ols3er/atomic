﻿#ifndef __ist_i3dgl_Texture__
#define __ist_i3dgl_Texture__

#include "i3dglTypes.h"
#include "i3dglDeviceResource.h"

namespace ist {
namespace i3dgl {

class Sampler : public DeviceResource
{
I3DGL_DECLARE_DEVICE_RESOURCE(Sampler)
typedef DeviceResource super;
friend class DeviceContext;
private:
    SamplerDesc m_desc;

    Sampler(Device *dev, const SamplerDesc &desc);
    ~Sampler();

public:
    void bind(uint32 slot);
    void unbind(uint32 slot);
};


class Texture : public DeviceResource
{
typedef DeviceResource super;
friend class DeviceContext;
protected:
    Texture(Device *dev);
    virtual void bind(uint32 slot) const=0;
    virtual void unbind(uint32 slot) const=0;
};

class Texture1D : public Texture
{
I3DGL_DECLARE_DEVICE_RESOURCE(Texture1D)
typedef Texture super;
private:
    static const int TEXTURE_TYPE;
    Texture1DDesc m_desc;

private:
    Texture1D(Device *dev, const Texture1DDesc &desc);
    ~Texture1D();

    void bind() const;
    void unbind() const;

public:
    void copy(uint32 mip_level, uint32 pos, uint32 size, I3D_COLOR_FORMAT format, void *data);
    void generateMipmap();

    void bind(uint32 slot) const;
    void unbind(uint32 slot) const;

    const Texture1DDesc& getDesc() const;
};


class Texture2D : public Texture
{
I3DGL_DECLARE_DEVICE_RESOURCE(Texture2D)
typedef Texture super;
private:
    static const int TEXTURE_TYPE;
    Texture2DDesc m_desc;

private:
    Texture2D(Device *dev, const Texture2DDesc &desc);
    ~Texture2D();

    void bind() const;
    void unbind() const;

public:
    void copy(uint32 mip_level, const uvec2 &pos, const uvec2 &size, I3D_COLOR_FORMAT format, void *data);
    void generateMipmap();

    void bind(uint32 slot) const;
    void unbind(uint32 slot) const;

    const Texture2DDesc& getDesc() const;
};


class Texture3D : public Texture
{
I3DGL_DECLARE_DEVICE_RESOURCE(Texture3D)
typedef Texture super;
private:
    static const int TEXTURE_TYPE;
    Texture3DDesc m_desc;

private:
    Texture3D(Device *dev, const Texture3DDesc &desc);
    ~Texture3D();

    void bind() const;
    void unbind() const;

public:
    void copy(uint32 mip_level, const uvec3 &pos, const uvec3 &size, I3D_COLOR_FORMAT format, void *data);
    void generateMipmap();

    void bind(uint32 slot) const;
    void unbind(uint32 slot) const;

    const Texture3DDesc& getDesc() const;
};

} // namespace i3dgl
} // namespace ist

#endif // __ist_i3dgl_Texture__
