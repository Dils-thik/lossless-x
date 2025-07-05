#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/ShaderLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/SimplePlayer.hpp>
#include <Geode/modify/PlayerControlGameObject.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/CustomizeObjectLayer.hpp>
#include <Geode/modify/SetGroupIDLayer.hpp>

struct BetterX
{
	const int digits_after_point = 10; // I mean .34167347 and stuff like that
	const double overall_coeff = std::pow(10, digits_after_point);
	const __int64 x05 = 0.7 * 5.98 * overall_coeff;
	const __int64 x1 = 0.9 * 5.77 * overall_coeff;
	const __int64 x2 = 1.1 * 5.87 * overall_coeff;
	const __int64 x3 = 1.3 * 6 * overall_coeff;
	const __int64 x4 = 1.6 * 6 * overall_coeff;
	__int64 value = 0;
	float remainder = 0;
	float update(long double delta_time, float playerSpeed, bool is_second_player) {
		if (is_second_player) return value / overall_coeff;
		__int64 delta_value = 0;
		playerSpeed *= 10; // FIXME: temporary
		if (int(playerSpeed) == 7) {
			delta_value = (x05 * delta_time);
		}
		else if (int(playerSpeed) == 9) {
			delta_value = (x1 * delta_time);
		}
		else if (int(playerSpeed) == 11) {
			delta_value = (x2 * delta_time);
		}
		else if (int(playerSpeed) == 13) {
			delta_value = (x3 * delta_time);
		}
		else if (int(playerSpeed) == 16) {
			delta_value = (x4 * delta_time);
		}
		else {
			log::error("Invalid player speed? Don't use 2.2 features loser.");
		}
		value += delta_value;
		/* Remainder? */
		return value / overall_coeff;
	}
	void setValue(float x) {
		value = x * overall_coeff;
	}
	BetterX() { };
};

BetterX X;

class $modify(MyPlayerObject, PlayerObject) {
	using Base = PlayerObject;
	void update(float p0) {
		auto previous_player_speed = this->m_playerSpeed;
		Base::update(p0);
		auto posx = X.update(p0, previous_player_speed, this->m_isSecondPlayer);
		auto pos2 = getPosition();
		auto posy = pos2.y;
		setPosition(CCPoint(posx, posy));
	}
};

class $modify(MyPlayLayer, PlayLayer) {
	using Base = PlayLayer;
	void startGame() {
		Base::startGame();
		X.setValue(this->m_startPosObject ? this->m_startPosObject->m_speedStart.at(0) : 0);
	}
	static void onModify(auto & self) {
		self.setHookPriority("PlayLayer::resetLevel", 10).unwrap();
	}
	void resetLevel() {
		Base::resetLevel();
		if (m_isPracticeMode) {
			auto last_checkpoint = (CheckpointObject*)(m_checkpointArray->lastObject());
			if (last_checkpoint) {
				X.setValue(last_checkpoint->m_player1Checkpoint->m_position.x);
				return;
			}
		}
		X.setValue(this->m_startPosObject ? this->m_startPosObject->m_speedStart.at(0) : 0);
	}
};

class $modify(MyLevelEditorLayer, LevelEditorLayer) {
	using Base = LevelEditorLayer;
	void onPlaytest() {
		Base::onPlaytest();
		X.setValue(this->m_startPosObject ? this->m_startPosObject->m_speedStart.at(0) : 0);
	}
};