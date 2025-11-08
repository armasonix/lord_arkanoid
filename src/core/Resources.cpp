#include "core/Resources.h"
#include <stdexcept>

sf::Font& ResourceManager::font(const std::string& key, const std::string& path) 
{
    auto it = m_fonts.find(key);
    if (it != m_fonts.end()) return *it->second;
    auto f = std::make_unique<sf::Font>();
    if (!f->loadFromFile(path)) throw std::runtime_error("Failed to load font: " + path);
    auto& ref = *f;
    m_fonts.emplace(key, std::move(f));
    return ref;
}

sf::Texture& ResourceManager::texture(const std::string& key, const std::string& path) 
{
    auto it = m_textures.find(key);
    if (it != m_textures.end()) return *it->second;
    auto t = std::make_unique<sf::Texture>();
    if (!t->loadFromFile(path)) throw std::runtime_error("Failed to load tex: " + path);
    auto& ref = *t;
    m_textures.emplace(key, std::move(t));
    return ref;
}