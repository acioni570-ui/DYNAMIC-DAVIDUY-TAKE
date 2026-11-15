#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>
#include <Geode/modify/CCParticleBatchNode.hpp>
#include "../../../src/visual_session.hpp"

using namespace geode::prelude;
namespace { dynamic::VisualSession session; }

class $modify(DynamicParticlesLevel, PlayLayer) {
    bool init(GJGameLevel* level, bool replay, bool skipObjects) {
        session.left();
        bool success = PlayLayer::init(level, replay, skipObjects);
        session.entered(success, success && Mod::get()->getSettingValue<bool>("enabled"));
        if (session.suppress()) {
            dynamic::hideVisualLayers(std::array<CCSpriteBatchNode*, 20>{{
                this->m_particleBlendingLayerT5,
                this->m_particleLayerT4,
                this->m_particleBlendingLayerT4,
                this->m_particleLayerT3,
                this->m_particleBlendingLayerT3,
                this->m_particleLayerT2,
                this->m_particleBlendingLayerT2,
                this->m_particleLayerT1,
                this->m_particleBlendingLayerT1,
                this->m_particleBlendingLayerB0,
                this->m_particleLayerB1,
                this->m_particleBlendingLayerB1,
                this->m_particleLayerB2,
                this->m_particleBlendingLayerB2,
                this->m_particleLayerB3,
                this->m_particleBlendingLayerB3,
                this->m_particleLayerB4,
                this->m_particleBlendingLayerB4,
                this->m_particleLayerB5,
                this->m_particleBlendingLayerB5}});
        }
        return success;
    }
    void onQuit() {
        session.left();
        PlayLayer::onQuit();
    }
};

// Keep native simulation, expiry and auto-removal of particle systems. Only
// drawing is omitted; pausing update would freeze emitters and their cleanup.
class $modify(DynamicParticleDraw, CCParticleSystemQuad) {
    void draw() {
        if (!session.suppress()) CCParticleSystemQuad::draw();
    }
};
class $modify(DynamicParticleBatchDraw, CCParticleBatchNode) {
    void draw() {
        if (!session.suppress()) CCParticleBatchNode::draw();
    }
};
