#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>

class ResourceManager 
{
public:
    ResourceManager() = default;

    sf::Font& font(const std::string& key, const std::string& path);
    sf::Texture& texture(const std::string& key, const std::string& path);

private:
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> m_fonts;
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textures;
};