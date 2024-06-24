#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <set>
#include <map>

// Input handler
class InputHandler {
private:
	std::set<sf::Keyboard::Scan::Scancode> keysPressed;
	std::map<sf::Keyboard::Scan::Scancode, int> keysOnCooldown; // <key, ms>

public:
	// Add a key
	void press(sf::Keyboard::Scan::Scancode key) {
		keysPressed.insert(key);
	}

	// Check if a key is in the cooldown map (not necessarily whether it is active)
	bool inCooldownData(sf::Keyboard::Scan::Scancode key) {
		return keysOnCooldown.find(key) != keysOnCooldown.end();
	}

	// Add key to cooldown
	void addToCooldown(sf::Keyboard::Scan::Scancode key, int ms = -1) {
		//keysOnCooldown.insert({ key, ms });
		keysOnCooldown[key] = ms;
	}

	// Update all keys' cooldowns (except for the high ones)
	void updateCooldowns(int msElapsed) {
		for (auto it = keysOnCooldown.begin(); it != keysOnCooldown.end(); it++) {
			if (it->second != -1) {
				it->second -= msElapsed;
			}
		}
	}

	// Remove a key from cooldown
	void removeFromCooldown(sf::Keyboard::Scan::Scancode key) {
		keysOnCooldown.erase(key);
	}

	// Remove a key
	void release(sf::Keyboard::Scan::Scancode key) {
		keysPressed.erase(key);
		keysOnCooldown.erase(key);
	}

	// Check if a key is pressed AND not in cooldown (thus active)
	bool isActive(sf::Keyboard::Scan::Scancode key) {
		if (keysOnCooldown.find(key) != keysOnCooldown.end() && (keysOnCooldown[key] > 0 || keysOnCooldown[key] == -1)) return false;
		// Not on cooldown: check regularly
		return keysPressed.find(key) != keysPressed.end();
	}
};
