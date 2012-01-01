#include "stdafx.h"
#include "../Base.h"
#include "../Math.h"
#include "i3dDevice.h"
#include "i3dShader.h"
#include "i3dUtil.h"
#include <string>
#include <sstream>
#include <fstream>

namespace ist {
namespace i3d {

bool CreateTexture2DFromFile(Texture2D& tex, const char *filename)
{
    std::ifstream  st(filename, std::ios::binary);
    if(st.fail()) {
        istAssert("file not found %s", filename);
        return false;
    }
    return CreateTexture2DFromStream(tex, st);
}

bool CreateTexture2DFromStream(Texture2D& tex, std::istream& st)
{
    istAssert("not implemented");
    return false;
}

bool GenerateRandomTexture(Texture2D &tex, GLsizei width, GLsizei height, I3D_COLOR_FORMAT format)
{
    static SFMT random;
    if(!random.isInitialized()) { random.initialize((uint32_t)::time(0)); }
    return GenerateRandomTexture(tex, width, height, format, random);
}

bool GenerateRandomTexture(Texture2D &tex, GLsizei width, GLsizei height, I3D_COLOR_FORMAT format, SFMT& random)
{
    std::string buffer;
    if(format==I3D_RGB8U) {
        int data_size = width*height*3;
        buffer.resize(data_size);
        for(int i=0; i<data_size; ++i) {
            buffer[i] = random.genInt32();
        }
    }
    else if(format==I3D_RGBA8U) {
        int data_size = width*height*4;
        buffer.resize(data_size);
        for(int i=0; i<data_size; ++i) {
            buffer[i] = random.genInt32();
        }
    }
    else if(format==I3D_RGB32F) {
        int data_size = width*height*sizeof(float)*3;
        buffer.resize(data_size);
        float *w = (float*)&buffer[0];
        for(int i=0; i<width*height*3; ++i) {
            w[i] = random.genFloat32();
        }
    }
    else if(format==I3D_RGBA32F) {
        int data_size = width*height*sizeof(float)*4;
        buffer.resize(data_size);
        float *w = (float*)&buffer[0];
        for(int i=0; i<width*height*4; ++i) {
            w[i] = random.genFloat32();
        }
    }
    else {
        istAssert("������");
    }

    bool ret =  tex.allocate(width, height, format, &buffer[0]);
    return ret;
}


template<class ShaderType>
inline bool CreateShaderFromFile(ShaderType& sh, const char *filename)
{
    std::ifstream  st(filename, std::ios::binary);
    if(st.fail()) {
        istAssert("file not found %s", filename);
        return false;
    }
    return CreateShaderFromStream<ShaderType>(sh, st);
}

template<class ShaderType>
inline bool CreateShaderFromStream(ShaderType& sh, std::istream& st)
{
    std::string source;
    std::ostringstream str_out;
    str_out << st.rdbuf();
    source = str_out.str();

    return sh.compile(source.c_str(), source.size());
}

template<class ShaderType>
inline bool CreateShaderFromString(ShaderType& sh, const char* source)
{
    return sh.compile(source, strlen(source));
}

bool CreateVertexShaderFromFile(VertexShader& sh, const char *filename)     { return CreateShaderFromFile<VertexShader>(sh, filename); }
bool CreateGeometryShaderFromFile(GeometryShader& sh, const char *filename) { return CreateShaderFromFile<GeometryShader>(sh, filename); }
bool CreateFragmentShaderFromFile(PixelShader& sh, const char *filename)    { return CreateShaderFromFile<PixelShader>(sh, filename); }
bool CreateVertexShaderFromStream(VertexShader& sh, std::istream& st)       { return CreateShaderFromStream<VertexShader>(sh, st); }
bool CreateGeometryShaderFromStream(GeometryShader& sh, std::istream& st)   { return CreateShaderFromStream<GeometryShader>(sh, st); }
bool CreateFragmentShaderFromStream(PixelShader& sh, std::istream& st)      { return CreateShaderFromStream<PixelShader>(sh, st); }
bool CreateVertexShaderFromString(VertexShader& sh, const char* source)     { return CreateShaderFromString<VertexShader>(sh, source); }
bool CreateGeometryShaderFromString(GeometryShader& sh, const char* source) { return CreateShaderFromString<GeometryShader>(sh, source); }
bool CreateFragmentShaderFromString(PixelShader& sh, const char* source)    { return CreateShaderFromString<PixelShader>(sh, source); }



Texture2D* CreateRenderBufferTexture(Device *dev, uint32 width, uint32 height, I3D_COLOR_FORMAT color_format)
{
    Texture2D *r = dev->createTexture2D();
    r->allocate(width, height, color_format);
    r->bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    r->unbind();
    return r;
}

RenderTarget* CreateRenderTarget(Device *dev, uint32 num_color_buffers, uint32 width, uint32 height,
    I3D_COLOR_FORMAT color_format)
{
    I3D_COLOR_FORMAT color_formats[RenderTarget::MAX_RENDER_BUFFERS];
    std::fill_n(color_formats, num_color_buffers, color_format);
    return CreateRenderTarget(dev, num_color_buffers, width, height, color_formats);
}

RenderTarget* CreateRenderTarget(Device *dev, uint32 num_color_buffers, uint32 width, uint32 height,
    I3D_COLOR_FORMAT *color_formats)
{
    Texture2D *rb[RenderTarget::MAX_RENDER_BUFFERS];
    RenderTarget *rt = dev->createRenderTarget();
    for(uint32 i=0; i<num_color_buffers; ++i) {
        rb[i] = CreateRenderBufferTexture(dev, width, height, color_formats[i]);
    }
    rt->setRenderBuffers(rb, num_color_buffers, NULL);
    for(uint32 i=0; i<num_color_buffers; ++i) {
        rb[i]->release();
    }
    return rt;
}

RenderTarget* CreateRenderTarget(Device *dev, uint32 num_color_buffers, uint32 width, uint32 height,
    I3D_COLOR_FORMAT color_format, I3D_COLOR_FORMAT depthstencil_format)
{
    I3D_COLOR_FORMAT color_formats[RenderTarget::MAX_RENDER_BUFFERS];
    std::fill_n(color_formats, num_color_buffers, color_format);
    return CreateRenderTarget(dev, num_color_buffers, width, height, color_formats, depthstencil_format);
}

RenderTarget* CreateRenderTarget(Device *dev, uint32 num_color_buffers, uint32 width, uint32 height,
    I3D_COLOR_FORMAT *color_formats, I3D_COLOR_FORMAT depthstencil_format)
{
    Texture2D *rb[RenderTarget::MAX_RENDER_BUFFERS];
    Texture2D *ds;
    RenderTarget *rt = dev->createRenderTarget();
    for(uint32 i=0; i<num_color_buffers; ++i) {
        rb[i] = CreateRenderBufferTexture(dev, width, height, color_formats[i]);
    }
    {
        ds = CreateRenderBufferTexture(dev, width, height, depthstencil_format);
    }
    rt->setRenderBuffers(rb, num_color_buffers, ds);
    for(uint32 i=0; i<num_color_buffers; ++i) {
        rb[i]->release();
    }
    {
        ds->release();
    }
    return rt;
}


} // namespace i3d
} // namespace ist
