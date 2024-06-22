#include "renderer.h"

Renderer::Renderer(sf::RenderWindow& window, AssetHandler& assetHandler)
    : window(window), assetHandler(assetHandler) {
        // Update the camera defaults if needed
        if (LEVEL_DESIGN_MODE) {
            screenPixPerWorld = 10; // Zoom out
        }
    }

void Renderer::renderFromData(RenderData data) {
    // TODO: implement other types (like images)
    double worldScale = zoomFactor * screenPixPerWorld;
    if (data.type == RenderType::Rectangle) {
        // Rectangle
        sf::RectangleShape toRender;
        if (data.coordType == CoordType::Screen) {
            // Literal coordinates
            toRender.setSize(sf::Vector2f(data.width, data.height));
            toRender.setPosition(sf::Vector2f(data.locx, data.locy));
        } else {
            // Convert from world to screen coordinates
            // TODO: render from the center position?
            // TODO: do not render if out of screen range
            toRender.setSize(sf::Vector2f(data.width * worldScale, data.height * worldScale));
            toRender.setPosition(sf::Vector2f((data.locx - worldCameraCenterX) * worldScale + window.getSize().x / 2.0, (data.locy - worldCameraCenterY) * worldScale + window.getSize().y / 2.0));
        }
        toRender.setFillColor(sf::Color(data.color.r, data.color.g, data.color.b, data.alpha));
        window.draw(toRender);
    } else if (data.type == RenderType::Circle) {
        // Circle
        sf::CircleShape toRender;
        if (data.coordType == CoordType::Screen) {
            // Literal coordinates
            toRender.setRadius((data.width + data.height) / 4);
            toRender.setPosition(sf::Vector2f(data.locx, data.locy));
        } else {
            // Convert from world to screen coordinates
            // TODO: render from the center position?
            // TODO: do not render if off screen range
            toRender.setRadius((data.width + data.height) / 4 * worldScale);
            toRender.setPosition(sf::Vector2f((data.locx - worldCameraCenterX) * worldScale + window.getSize().x / 2.0, (data.locy - worldCameraCenterY) * worldScale + window.getSize().y / 2.0));
        }
        toRender.setFillColor(sf::Color(data.color.r, data.color.g, data.color.b, data.alpha));
        window.draw(toRender);
    } else {
        // Image
        // Load the image source file
        // TODO: refactor loading the texture as well?
        sf::Image img = assetHandler.getImage(data.imageSource);
        sf::Texture tex;
        sf::Sprite toRender;
        if (data.coordType == CoordType::Screen) {
            // Literal coordinates
            tex.create(img.getSize().x, img.getSize().y);
            tex.update(img);
            toRender.setTexture(tex);
            toRender.setColor(sf::Color(255, 255, 255, data.alpha));
            toRender.setPosition(data.locx, data.locy);
            toRender.setScale(data.width / img.getSize().x, data.height / img.getSize().y);
        } else {
            // Convert from world to screen coordinates
            // TODO: render from the center position?
            // TODO: do not render if off screen range
            tex.create(img.getSize().x, img.getSize().y);
            tex.update(img);
            toRender.setTexture(tex);
            toRender.setColor(sf::Color(255, 255, 255, data.alpha));
            toRender.setPosition(sf::Vector2f((data.locx - worldCameraCenterX) * worldScale + window.getSize().x / 2.0, (data.locy - worldCameraCenterY) * worldScale + window.getSize().y / 2.0));
            toRender.setScale(data.width * worldScale / img.getSize().x, data.height * worldScale / img.getSize().y);
        }
        window.draw(toRender);
    }
}

void Renderer::renderMessage(UIMessage& uiMessage, int locx, int locy) {
    sf::Text text;
    text.setFont(assetHandler.getMainFont());
    text.setString(uiMessage.message);
    text.setCharacterSize(uiMessage.fontSize);
    double alpha = std::min(sqrt(uiMessage.durationFramesRemaining) / 12.0, 1.0);
    text.setFillColor(sf::Color(uiMessage.color.r, uiMessage.color.g, uiMessage.color.b, alpha * 255));
    text.setPosition(locx, locy);
    window.draw(text);
}

void Renderer::setCamera(double newWorldCameraCenterX, double newWorldCameraCenterY, double percentEasing, double zoomFactor) {
    worldCameraCenterX = worldCameraCenterX * percentEasing + newWorldCameraCenterX * (1 - percentEasing);
    worldCameraCenterY = worldCameraCenterY * percentEasing + newWorldCameraCenterY * (1 - percentEasing);
    this->zoomFactor = this->zoomFactor * percentEasing + zoomFactor * (1 - percentEasing);
}

void Renderer::renderWorld(GameState& gameState) {
	// Clear/render sky
	window.clear(sf::Color(111, 201, 252));
	// Render world objects
    for (WorldObject* object : gameState.objects) {
        renderFromData(object->getRenderData());
    }

    // Render debug info, if needed
    /*for (int y = 0; y < 40; y++) {
        for (int x = 0; x < 40; x++) {
            int cellSat = gameState.getCollisionMatrix().getCellSaturation(x, y);
            renderFromData({
                RenderType::Rectangle,
                CoordType::World,
                (double) x * gameState.getCollisionMatrix().getCellSize(),
                (double) y * gameState.getCollisionMatrix().getCellSize(),
                (double) gameState.getCollisionMatrix().getCellSize(),
                (double) gameState.getCollisionMatrix().getCellSize(),
                { 255, 255, 0 },
                "",
                std::min(255, cellSat * 20)
            });
        }
    }*/

    // Render UI messages
    int level = 0;
    for (int i = gameState.uiMessages.size() - 1; i >= 0; i--) {
        renderMessage(gameState.uiMessages[i], 24, 24 + level * 20);
        level++;
    }
    // Render other UI
    UIMessage levelName = { gameState.getLevelName(), { 255, 255, 255 }, 24 };
    renderMessage(levelName, window.getSize().x - 140, 18);
}
