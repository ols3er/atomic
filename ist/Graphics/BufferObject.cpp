#include "stdafx.h"
#include "GraphicsAssert.h"
#include "BufferObject.h"

namespace ist {
namespace graphics {

template<GLuint BufferType>
BufferObject<BufferType>::BufferObject()
: m_handle(0)
, m_size(0)
, m_capacity(0)
{
}

template<GLuint BufferType>
BufferObject<BufferType>::~BufferObject()
{
    finalize();
}


template<GLuint BufferType>
bool BufferObject<BufferType>::initialize()
{
    glGenBuffers(1, &m_handle);
    CheckGLError();
    return true;
}

template<GLuint BufferType>
void BufferObject<BufferType>::finalize()
{
    if(m_handle!=0) {
        glDeleteBuffers(1, &m_handle);
        CheckGLError();
    }
    m_handle = 0;
}

template<GLuint BufferType>
void BufferObject<BufferType>::allocate(GLuint size, USAGE usage, void *data)
{
    m_size = size;
    if(size==0) {
        return;
    }
    else if(size > m_capacity) {
        m_capacity = size;
        glBindBuffer(BufferType, m_handle);
        glBufferData(BufferType, size, data, usage);
        glBindBuffer(BufferType, 0);
    }
    else if(data!=NULL) {
        glBindBuffer(BufferType, m_handle);
        void *p = glMapBuffer(BufferType, GL_WRITE_ONLY);
        memcpy(p, data, size);
        glUnmapBuffer(BufferType);
        glBindBuffer(BufferType, 0);
    }
}

template<GLuint BufferType>
GLuint BufferObject<BufferType>::size() const
{
    return m_size;
}

template<GLuint BufferType>
void BufferObject<BufferType>::bind() const
{
    glBindBuffer(BufferType, m_handle);
}

template<GLuint BufferType>
void BufferObject<BufferType>::unbind() const
{
    glBindBuffer(BufferType, 0);
}

template<GLuint BufferType>
void* BufferObject<BufferType>::map(MAP_MODE mode)
{
    glBindBuffer(BufferType, m_handle);
    void *r = glMapBuffer(BufferType, mode);
    glBindBuffer(BufferType, 0);
    return r;
}

template<GLuint BufferType>
void BufferObject<BufferType>::unmap()
{
    glBindBuffer(BufferType, m_handle);
    glUnmapBuffer(BufferType);
    glBindBuffer(BufferType, 0);
}

template BufferObject<GL_ARRAY_BUFFER>;
template BufferObject<GL_ELEMENT_ARRAY_BUFFER>;
template BufferObject<GL_PIXEL_PACK_BUFFER>;
template BufferObject<GL_PIXEL_UNPACK_BUFFER>;
template BufferObject<GL_UNIFORM_BUFFER>;


void UniformBufferObject::bindBase(GLuint index) const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, getHandle());
}

void UniformBufferObject::bindRange(GLuint index, GLintptr offset, GLsizeiptr size) const
{
    glBindBufferRange(GL_UNIFORM_BUFFER, index, getHandle(), offset, size);
}


VertexArray::VertexArray()
: m_handle(0)
{
}

VertexArray::~VertexArray()
{
    finalize();
}

bool VertexArray::initialize()
{
    glGenVertexArrays(1, &m_handle);
    return true;
}

void VertexArray::finalize()
{
    if(m_handle!=0) {
        glDeleteVertexArrays(1, &m_handle);
    }
    m_handle = 0;
}

void VertexArray::bind() const
{
    glBindVertexArray(m_handle);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::setAttribute(GLuint i, GLint num_elements, VertexBufferObject &vbo)
{
    glBindVertexArray(m_handle);
    glEnableVertexAttribArray(i);
    vbo.bind();
    glVertexAttribPointer(i, num_elements, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindVertexArray(0);
}

void VertexArray::setInstanceAttribute(GLuint i, GLint num_elements, VertexBufferObject &vbo)
{
    glBindVertexArray(m_handle);
    glEnableVertexAttribArray(i);
    vbo.bind();
    glVertexAttribPointer(i, num_elements, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(i, 1);
    glBindVertexArray(0);
}

void VertexArray::setAttributes( VertexBufferObject& vbo, size_t stride, const Descriptor *descs, size_t num_descs )
{
    glBindVertexArray(m_handle);
    vbo.bind();
    for(size_t i=0; i<num_descs; ++i) {
        const Descriptor& desc = descs[i];
        glEnableVertexAttribArray(desc.location);
        glVertexAttribPointer(desc.location, desc.num_elements, desc.type, desc.normalize, stride, (GLvoid*)desc.offset);
        glVertexAttribDivisor(desc.location, desc.divisor);
    }
    vbo.unbind();
    glBindVertexArray(0);
}

void VertexArray::setAttributes( VertexBufferObject *vbos, size_t *strides, size_t num_vbos, const Descriptor *descs, size_t num_descs )
{
    glBindVertexArray(m_handle);
    for(size_t i=0; i<num_descs; ++i) {
        const Descriptor& desc = descs[i];
        vbos[desc.vbo_index].bind();
        glEnableVertexAttribArray(desc.location);
        glVertexAttribPointer(desc.location, desc.num_elements, desc.type, desc.normalize, strides[desc.vbo_index], (GLvoid*)desc.offset);
        glVertexAttribDivisor(desc.location, desc.divisor);
        vbos[desc.vbo_index].unbind();
    }
    glBindVertexArray(0);
}


} // namespace graphics
} // namespace ist