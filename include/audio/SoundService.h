#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include <iostream>
#include <filesystem>

namespace ark 
{

    enum class Sfx { Win, Lose };

    class SoundService 
    {
    public:
        explicit SoundService(std::string assetsDir, unsigned channels = 8)
            : m_assetsDir(std::move(assetsDir)), m_channels(channels)
        {
            m_sounds.resize(m_channels);
            preload();
        }

        void setVolume(float v) { m_volume = std::clamp(v, 0.f, 100.f); }
        float volume() const { return m_volume; }

        void play(Sfx id) { (void)playEnsure(id); }

        bool playEnsure(Sfx id) {
            auto it = m_buffers.find(id);
            if (it == m_buffers.end()) {
                if (!load(id, pathFor(id))) {
                    // fallback: ./assets/sfx/...
                    const std::string alt = runtimePathFor(id);
                    if (!alt.empty() && !load(id, alt)) {
                        std::cerr << "[SFX] failed to load: " << pathFor(id)
                            << " and " << alt << std::endl;
                        return false;
                    }
                }
                it = m_buffers.find(id);
                if (it == m_buffers.end()) return false;
            }
            playFromBuffer(it->second);
            return true;
        }

    private:
        void preload() 
        {
            load(Sfx::Win, pathFor(Sfx::Win));
            load(Sfx::Lose, pathFor(Sfx::Lose));
        }

        std::string pathFor(Sfx id) const 
        {
            switch (id) 
            {
                case Sfx::Win:  return m_assetsDir + "/sfx/win.wav";
                case Sfx::Lose: return m_assetsDir + "/sfx/lose.wav";
                default:        return {};
            }
        }

        std::string runtimePathFor(Sfx id) const 
        {
            switch (id) 
            {
                case Sfx::Win:  return "assets/sfx/win.wav";
                case Sfx::Lose: return "assets/sfx/lose.wav";
                default:        return {};
            }
        }

        bool load(Sfx id, const std::string& path) 
        {
            if (path.empty()) return false;
            sf::SoundBuffer buf;
            if (!buf.loadFromFile(path)) return false;
            m_buffers.emplace(id, std::move(buf));
            return true;
        }

        void playFromBuffer(const sf::SoundBuffer& buf) 
        {
            for (auto& s : m_sounds) 
            {
                if (s.getStatus() != sf::Sound::Playing) 
                {
                    s.setBuffer(buf);
                    s.setVolume(m_volume);
                    s.play();
                    return;
                }
            }
            m_rotate = (m_rotate + 1) % m_sounds.size();
            m_sounds[m_rotate].stop();
            m_sounds[m_rotate].setBuffer(buf);
            m_sounds[m_rotate].setVolume(m_volume);
            m_sounds[m_rotate].play();
        }

    private:
        std::string m_assetsDir;
        std::unordered_map<Sfx, sf::SoundBuffer> m_buffers;
        std::vector<sf::Sound> m_sounds;
        std::size_t m_rotate{ 0 };
        float m_volume{ 100.f };
        unsigned m_channels;
    };

} // namespace ark