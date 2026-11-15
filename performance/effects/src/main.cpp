#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../../../src/visual_session.hpp"

using namespace geode::prelude;
namespace { dynamic::VisualSession session; }

class $modify(DynamicEffectsLevel, PlayLayer) {
    bool init(GJGameLevel* level, bool replay, bool skipObjects) {
        session.left();
        bool success = PlayLayer::init(level, replay, skipObjects);
        session.entered(success, success && Mod::get()->getSettingValue<bool>("enabled"));
        return success;
    }
    void onQuit() {
        session.left();
        PlayLayer::onQuit();
    }
};

class $modify(DynamicShineLite, GameObject) {
    void playShineEffect() {
        if (!session.suppress()) GameObject::playShineEffect();
    }
};
class $modify(DynamicBumpLite, PlayerObject) {
    void playBumpEffect(int type, GameObject* object) {
        if (!session.suppress()) PlayerObject::playBumpEffect(type, object);
    }
    void playSpiderDashEffect(CCPoint from, CCPoint to) {
        if (!session.suppress()) PlayerObject::playSpiderDashEffect(from, to);
    }
};
