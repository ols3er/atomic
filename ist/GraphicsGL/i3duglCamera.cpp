﻿#include "istPCH.h"
#ifdef ist_with_OpenGL
#include "ist/Base.h"
#include "ist/GraphicsGL/i3duglCamera.h"
#include "ist/GraphicsGL/i3dglTypes.h"

namespace ist {
namespace i3dgl {


bool Viewport::bind() const
{
    glViewport(m_pos.x, m_pos.y, m_size.x, m_size.y);
    return true;
}



void Camera::updateMatrix()
{
    m_v_matrix = glm::lookAt(vec3(m_position), vec3(m_target), vec3(m_up));
}


void OrthographicCamera::updateMatrix()
{
    super::updateMatrix();
    m_p_matrix = glm::ortho( m_left, m_right, m_bottom, m_top, m_znear, m_zfar);
    m_vp_matrix = getViewMatrix()*getProjectionMatrix();
}



void PerspectiveCamera::updateMatrix()
{
    super::updateMatrix();
    m_p_matrix = glm::perspective(m_fovy, m_aspect, m_znear, m_zfar);
    m_vp_matrix = getProjectionMatrix()*getViewMatrix();
}


} // namespace i3d
} // namespace ist
#endif // ist_with_OpenGL
