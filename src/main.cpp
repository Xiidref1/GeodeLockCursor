/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <Windows.h>


/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
using namespace geode::prelude;


void clipCursorOnScreen(bool sideIsLeft) {
		RECT rect;

		// Try to look for the curent window and take it's real position 
		HWND window = GetForegroundWindow();
		char windowTitle[18];
		GetWindowTextA(window, windowTitle, 18);
		std::string windowTitleStr(windowTitle);
		if (windowTitleStr == "Geometry Dash") {
            GetWindowRect(window, &rect);
		}else {
			// Otherwise just say it's on the top left of the screen 
			CCDirector *director = CCDirector::get();
			rect = {0, 0, (long)director->getOpenGLView()->getWindowedSize().width, (long)director->getOpenGLView()->getWindowedSize().height};
		}


		if (sideIsLeft) {
			rect.right = ((rect.right - rect.left) / 2) + rect.left;
		} else {
			rect.left = ((rect.right - rect.left) / 2) + rect.left;
		}

		int centerX, centerY, width10, height10;
		centerX = rect.left + ((rect.right - rect.left) / 2);
		centerY = rect.top + ((rect.bottom - rect.top) / 2);
		width10 = (rect.right - rect.left) * 0.1; 
		height10 = (rect.bottom - rect.top) * 0.1;

		RECT finalRect;
		finalRect.left = centerX - (width10 / 2);
		finalRect.top = centerY - (height10 / 2);
		finalRect.right = centerX + (width10 / 2);
		finalRect.bottom = centerY + (height10 / 2);

		SetCursorPos(centerX, centerY);
    	ClipCursor(&finalRect);
}

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
class $modify(PlayLayer) {

	struct Fields {
		bool doLock;
		bool lockOnLeft;
	};

	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
			return false;
		}

		m_fields->doLock = Mod::get()->getSettingValue<bool>("mod_enabled");
		m_fields->lockOnLeft = Mod::get()->getSettingValue<bool>("lock_on_left");

		if (m_fields->doLock) {
			clipCursorOnScreen(m_fields->lockOnLeft);
		}

		return true;
	}

	void pauseGame(bool value) {
		PlayLayer::pauseGame(value);
		// Release the lock
		ClipCursor(nullptr);
	}

	void levelComplete() {
		PlayLayer::levelComplete();
		// Release the lock
		ClipCursor(nullptr);
	}

	void resume() {
		PlayLayer::resume();

		if (m_fields->doLock) {
			clipCursorOnScreen(m_fields->lockOnLeft);
		}
	}
};


class $modify(EndLevelLayer) {
	struct Fields {
		bool doLock;
		bool lockOnLeft;
	};

	void customSetup() {
		EndLevelLayer::customSetup();
		
		m_fields->doLock = Mod::get()->getSettingValue<bool>("mod_enabled");
		m_fields->lockOnLeft = Mod::get()->getSettingValue<bool>("lock_on_left");
	}

    void onReplay(cocos2d::CCObject* sender) {
		EndLevelLayer::onReplay(sender);
		
		if (m_fields->doLock) {
			clipCursorOnScreen(m_fields->lockOnLeft);
		}
	}

    void onRestartCheckpoint(cocos2d::CCObject* sender) {
		EndLevelLayer::onRestartCheckpoint(sender);
		
		if (m_fields->doLock) {
			clipCursorOnScreen(m_fields->lockOnLeft);
		}
	}
};