#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../../../src/visual_session.hpp"

using namespace geode::prelude;
namespace { dynamic::VisualSession session; }

class $modify(DynamicGlowLevel, PlayLayer) {
    bool init(GJGameLevel* level, bool replay, bool skipObjects) {
        session.left();
        bool success = PlayLayer::init(level, replay, skipObjects);
        session.entered(success, success && Mod::get()->getSettingValue<bool>("enabled"));
        if (session.suppress()) {
            dynamic::hideVisualLayers<CCNode>(this, std::array<std::uintptr_t, 9>{{
                dynamic::visualAddress(&this->m_glowLayerT4),
                dynamic::visualAddress(&this->m_glowLayerT3),
                dynamic::visualAddress(&this->m_glowLayerT2),
                dynamic::visualAddress(&this->m_glowLayerT1),
                dynamic::visualAddress(&this->m_glowLayerB1),
                dynamic::visualAddress(&this->m_glowLayerB2),
                dynamic::visualAddress(&this->m_glowLayerB3),
                dynamic::visualAddress(&this->m_glowLayerB4),
                dynamic::visualAddress(&this->m_glowLayerB5)}}, [](CCNode* node) { return typeinfo_cast<CCSpriteBatchNode*>(node); });
        }
        return success;
    }
    void onQuit() {
        session.left();
        PlayLayer::onQuit();
    }
};
