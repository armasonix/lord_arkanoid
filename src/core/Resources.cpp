#include "core/Resources.h"
#include <stdexcept>

namespace ark 
{

sf::Font& ResourceManager::font(const std::string& key, const std::string& path) 
{
    if (auto it = m_fonts.find(key); it != m_fonts.end()) return *it->second;
    auto f = std::make_unique<sf::Font>();
    if (!f->loadFromFile(path)) throw std::runtime_error("Failed to load font: " + path);
    auto& ref = *f;
    m_fonts.emplace(key, std::move(f));
    return ref;
}

sf::Texture& ResourceManager::texture(const std::string& key, const std::string& path) 
{
    if (auto it = m_textures.find(key); it != m_textures.end()) return *it->second;
    auto t = std::make_unique<sf::Texture>();
    if (!t->loadFromFile(path)) throw std::runtime_error("Failed to load texture: " + path);
    auto& ref = *t;
    m_textures.emplace(key, std::move(t));
    return ref;
}

sf::Font& ResourceManager::font(const std::string& key) 
{
    if (auto it = m_fonts.find(key); it != m_fonts.end()) return *it->second;
    throw std::runtime_error("Font not loaded: " + key);
}

sf::Texture& ResourceManager::texture(const std::string& key) 
{
    if (auto it = m_textures.find(key); it != m_textures.end()) return *it->second;
    throw std::runtime_error("Texture not loaded: " + key);
}

} // namespace ark