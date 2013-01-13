﻿#ifndef __ist_i3dgl_Buffer__
#define __ist_i3dgl_Buffer__

#include "i3dglTypes.h"
#include "i3dglDeviceResource.h"

namespace ist {
namespace i3dgl {

class istInterModule Buffer : public DeviceResource
{
I3DGL_DECLARE_DEVICE_RESOURCE(Buffer);
typedef DeviceResource super;
friend class VertexArray;
public:
    void* map(I3D_MAP_MODE mode);
    void unmap();

    const BufferDesc& getDesc() const { return m_desc; }

protected:
    Buffer(Device *dev, const BufferDesc &desc);
    ~Buffer();
    void bind() const;
    void unbind() const;

    BufferDesc m_desc;
};


class istInterModule VertexArray : public DeviceResource
{
I3DGL_DECLARE_DEVICE_RESOURCE(VertexArray);
typedef DeviceResource super;
private:
    VertexArray(Device *dev);
    ~VertexArray();

public:
    void bind() const;
    void unbind() const;

    void setAttributes(Buffer& vbo, size_t stride, const VertexDesc *descs, size_t num_descs);
};

} // namespace i3d
} // namespace ist
#endif // __ist_i3dgl_Buffer__

