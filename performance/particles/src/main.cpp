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
            dynamic::hideVisualLayers<CCNode>(this, std::array<std::uintptr_t, 20>{{
                dynamic::visualAddress(&this->m_particleBlendingLayerT5),
                dynamic::visualAddress(&this->m_particleLayerT4),
                dynamic::visualAddress(&this->m_particleBlendingLayerT4),
                dynamic::visualAddress(&this->m_particleLayerT3),
                dynamic::visualAddress(&this->m_particleBlendingLayerT3),
                dynamic::visualAddress(&this->m_particleLayerT2),
                dynamic::visualAddress(&this->m_particleBlendingLayerT2),
                dynamic::visualAddress(&this->m_particleLayerT1),
                dynamic::visualAddress(&this->m_particleBlendingLayerT1),
                dynamic::visualAddress(&this->m_particleBlendingLayerB0),
                dynamic::visualAddress(&this->m_particleLayerB1),
                dynamic::visualAddress(&this->m_particleBlendingLayerB1),
                dynamic::visualAddress(&this->m_particleLayerB2),
                dynamic::visualAddress(&this->m_particleBlendingLayerB2),
                dynamic::visualAddress(&this->m_particleLayerB3),
                dynamic::visualAddress(&this->m_particleBlendingLayerB3),
                dynamic::visualAddress(&this->m_particleLayerB4),
                dynamic::visualAddress(&this->m_particleBlendingLayerB4),
                dynamic::visualAddress(&this->m_particleLayerB5),
                dynamic::visualAddress(&this->m_particleBlendingLayerB5)}}, [](CCNode* node) { return typeinfo_cast<CCSpriteBatchNode*>(node); });
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
    // These methods prepare vertices and upload their buffer for draw().
    // Keep CCParticleSystem::update running: age, emission, expiry, pooling,
    // and auto-removal must continue even when rendering is suppressed.
    void updateQuadWithParticle(sCCParticle* particle, CCPoint const& position) {
        if (!session.suppress()) CCParticleSystemQuad::updateQuadWithParticle(particle, position);
    }
    void postStep() {
        if (!session.suppress()) CCParticleSystemQuad::postStep();
    }
    void draw() {
        if (!session.suppress()) CCParticleSystemQuad::draw();
    }
};
class $modify(DynamicParticleBatchDraw, CCParticleBatchNode) {
    void draw() {
        if (!session.suppress()) CCParticleBatchNode::draw();
    }
};
