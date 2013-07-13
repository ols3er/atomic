﻿#ifndef atm_Graphics_Renderer_GBuffer_h
#define atm_Graphics_Renderer_GBuffer_h
namespace atm {



class PassGBuffer_Particle : public IRenderer
{
public:
    PassGBuffer_Particle();
    ~PassGBuffer_Particle();
    void beforeDraw();
    void draw();

    void addParticle(const SingleParticle *particles, uint32 num);

private:
    ist::vector<SingleParticle> m_particles;
};

class PassGBuffer_Fluid : public IRenderer
{
public:
    PassGBuffer_Fluid();
    ~PassGBuffer_Fluid();
    void beforeDraw();
    void draw();

    void addParticles(PSET_RID psid, const PSetInstance &inst);
    void addParticlesSolid(PSET_RID psid, const PSetInstance &inst);

private:
    void drawFluid();
    void drawParticleSets(PSetDrawData &pdd);
    bool culling(PSET_RID psid, const PSetInstance &inst);

    PSetDrawData m_rigid_sp;
    PSetDrawData m_rigid_so;
};

} // namespace atm
#endif // atm_Graphics_Renderer_GBuffer_h
