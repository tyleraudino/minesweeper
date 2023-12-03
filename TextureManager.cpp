#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

unordered_map<string, sf::Texture> TextureManager::textures;

sf::Texture& TextureManager::getTexture(string textureName) {
    auto result = textures.find(textureName);
    if (result == textures.end()) {
        // Texture does not already exist in the map, go get it!
        sf::Texture newTexture;
        newTexture.loadFromFile("files/images/" + textureName + ".png");
        textures[textureName] = newTexture;
        return textures[textureName];
    } else {
        // Texture already exists, return it!
        return result->second;
    }
}
