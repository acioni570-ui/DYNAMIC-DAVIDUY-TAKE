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
            dynamic::hideVisualLayers(std::array<CCSpriteBatchNode*, 9>{{
                this->m_glowLayerT4,
                this->m_glowLayerT3,
                this->m_glowLayerT2,
                this->m_glowLayerT1,
                this->m_glowLayerB1,
                this->m_glowLayerB2,
                this->m_glowLayerB3,
                this->m_glowLayerB4,
                this->m_glowLayerB5}});
        }
        return success;
    }
    void onQuit() {
        session.left();
        PlayLayer::onQuit();
    }
};
