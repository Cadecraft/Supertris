#include <SFML/Graphics.hpp>
#include <iostream>
#include <set>
#include "constants.h"
#include "assethandler.h"
#include "renderer.h"
#include "renderdata.h"
#include "gamestate.h"
#include "worldspawner.h"
#include "worldobject.h"
#include "worldobjectincludes.h"

// Entry point
int main() {
    auto window = sf::RenderWindow{ { 1920u, 1080u }, "PlatScifi" };
    // TODO: better framerate limit ?
    window.setFramerateLimit(144);

    // TODO: build not working on Linux?

    // Main data and objects
    AssetHandler assetHandler;
    Renderer renderer(window, assetHandler);
    GameState gameState;
    WorldSpawner worldSpawner;
    std::set<sf::Keyboard::Key> keysPressed;

    // Main world objects
    Player* player = new Player(5, 10);
    gameState.spawnObject(player);

    // Load the first level (depends on the mode)
    worldSpawner.spawnWorld(gameState, LEVEL_DESIGN_MODE ? "level_design" : FIRST_LEVEL);
    /*// Example level (level_0.csv)
    // Bottom colliders
    gameState.spawnObject(new StaticCollider(3, 25, 6, 2));
    gameState.spawnObject(new StaticCollider(9, 27, 10, 2));
    gameState.spawnObject(new StaticCollider(19, 25, 6, 2));
    // Vertical colliders
    gameState.spawnObject(new StaticCollider(0, 0, 2, 25));
    gameState.spawnObject(new StaticCollider(25, 0, 2, 19));
    // Extra blocks
    gameState.spawnObject(new StaticCollider(23, 29, 44, 19));
    // Spike
    gameState.spawnObject(new Spike(28, 28));*/

    // Main game loop
    while (window.isOpen()) {
        // TODO: Frame acceleration thing (for consistent speed/timing)
        // Poll events
        for (auto event = sf::Event{}; window.pollEvent(event);) {
            if (event.type == sf::Event::Closed) {
                // Close window
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                keysPressed.insert(event.key.code);
            } else if (event.type == sf::Event::KeyReleased) {
                keysPressed.erase(event.key.code);
            }
        }

        // Use input
        bool resetPlayer = false;
        if (keysPressed.find(sf::Keyboard::A) != keysPressed.end()) {
            player->accelerate(-0.05, 0);
        }
        if (keysPressed.find(sf::Keyboard::D) != keysPressed.end()) {
            player->accelerate(0.05, 0);
        }
        if (
            keysPressed.find(sf::Keyboard::W) != keysPressed.end()
            || keysPressed.find(sf::Keyboard::Space) != keysPressed.end()
        ) {
            if (LEVEL_DESIGN_MODE) player->accelerate(0, -0.05);
            else player->jump();
        }
        if (keysPressed.find(sf::Keyboard::S) != keysPressed.end()) {
            player->accelerate(0, 0.05);
        }
        if (keysPressed.find(sf::Keyboard::R) != keysPressed.end()) {
            // Restart to spawn location (counts as a death)
            player->die();
            resetPlayer = true;
            keysPressed.erase(sf::Keyboard::R);
        }
        if (keysPressed.find(sf::Keyboard::P) != keysPressed.end()) {
            // Debug: spawn blue cubes
            gameState.spawnObject(new BlueCube(player->getLocx(), player->getLocy()));
            keysPressed.erase(sf::Keyboard::P);
            gameState.addUIMessage("Player spawned a debug blue cube", { 0, 0, 255 });
            /*// Debug: spawn a procedural world
            worldSpawner.spawnWorldProceduralBlock(gameState);*/
        }
        if (keysPressed.find(sf::Keyboard::Escape) != keysPressed.end()) {
            // Escape: quit
            // TODO: pause instead of quitting
            break;
        }

        // Update the game and world
        if (LEVEL_DESIGN_MODE) {
            // Level Design Mode: keep reloading the world without updating it
            worldSpawner.spawnWorld(gameState, gameState.getLevelName());
        }
        UpdateResult updateResult = gameState.update();
        if (resetPlayer) updateResult = UpdateResult::DieReset;
        if (updateResult == UpdateResult::NextLevel) {
            // Load the next level, if possible
            // TODO: impl better
            worldSpawner.spawnWorld(gameState, gameState.getNextLevelName());
            Particle* particle = new Particle(
                player->getLocx(), player->getLocy(), "pplayerwin"
            );
            gameState.spawnObject(particle);
        } else if (updateResult == UpdateResult::DieReset) {
            // Died: re-load the same level
            worldSpawner.spawnWorld(gameState, gameState.getLevelName());
            gameState.addUIMessage("Death #" + std::to_string(player->getDeathCounter()), { 255, 0, 0 });
        }

        // Render
        window.clear(sf::Color(0, 0, 0));
        // Game and world
        double cameraZoom = 1.0 - 0.1 * sqrt(pow(player->getVelx(), 2) + 0.3 * pow(player->getVely(), 2));
        renderer.setCamera(player->getLocx(), player->getLocy(), 0.8, cameraZoom);
        renderer.renderWorld(gameState);
        // Update
        window.display();
    }
    return 0;
}
