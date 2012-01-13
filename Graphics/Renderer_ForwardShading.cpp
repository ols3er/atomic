#include "stdafx.h"
#include "types.h"
#include "Game/AtomicApplication.h"
#include "Game/AtomicGame.h"
#include "Game/World.h"
#include "Game/Collision.h"
#include "GPGPU/SPH.cuh"
#include "AtomicRenderingSystem.h"
#include "Renderer.h"
#include "Util.h"

namespace atomic {




PassForwardShading_DistanceField::PassForwardShading_DistanceField()
{
    m_sh_grid       = atomicGetShader(SH_FILL);
    m_va_grid       = atomicGetVertexArray(VA_FIELD_GRID);

    m_sh_cell       = atomicGetShader(SH_DISTANCE_FIELD);
    m_vbo_cell_pos  = atomicGetVertexBuffer(VBO_DISTANCE_FIELD_POS);
    m_vbo_cell_dist = atomicGetVertexBuffer(VBO_DISTANCE_FIELD_DIST);
    m_va_cell       = atomicGetVertexArray(VA_DISTANCE_FIELD);
}

void PassForwardShading_DistanceField::beforeDraw()
{
}

void PassForwardShading_DistanceField::draw()
{
#ifdef __atomic_enable_distance_field__
    if(atomicGetConfig()->show_distance) {
        MapAndWrite(*m_vbo_cell_dist, atomicGetCollisionSet()->getDistanceField()->getDistances(),
            sizeof(vec4) * SPH_DISTANCE_FIELD_DIV_X * SPH_DISTANCE_FIELD_DIV_Y);
        m_sh_cell->bind();
        m_va_cell->bind();
        glDrawArraysInstanced(GL_QUADS, 0, 4, SPH_DISTANCE_FIELD_DIV_X * SPH_DISTANCE_FIELD_DIV_Y);
        m_va_cell->unbind();
        m_sh_cell->unbind();
    }
#endif // __atomic_enable_distance_field__

    if(atomicGetConfig()->show_grid) {
        m_sh_grid->bind();
        m_va_grid->bind();
        glDrawArrays(GL_LINES, 0, (SPH_DISTANCE_FIELD_DIV_X+1) * (SPH_DISTANCE_FIELD_DIV_Y+1) * 2);
        m_va_grid->unbind();
        m_sh_grid->unbind();
    }
}


} // namespace atomic