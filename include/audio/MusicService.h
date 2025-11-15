#pragma once
#include <SFML/Audio.hpp>
#include <string>

namespace ark 
{

    enum class MusicTrack { None, Theme, Level };

    class MusicService 
    {
    public:
        MusicService(const std::string& assetsDir)
            : m_assetsDir(assetsDir) {}

        void playTheme(bool loop = true) { play(MusicTrack::Theme, loop); }
        void playLevel(bool loop = true) { play(MusicTrack::Level, loop); }

        void pause() { if (m_current != MusicTrack::None) m_music.pause(); }
        void resume() 
        {
            if (m_current != MusicTrack::None && m_music.getStatus() == sf::Music::Paused)
                m_music.play();
        }
        void stop() { m_music.stop(); m_current = MusicTrack::None; }

        MusicTrack current() const { return m_current; }
        bool isPlaying() const { return m_music.getStatus() == sf::Music::Playing; }
        bool isPaused()  const { return m_music.getStatus() == sf::Music::Paused; }

    private:
        void play(MusicTrack t, bool loop) {
            if (m_current == t && m_music.getStatus() == sf::Music::Playing) return;

            const std::string path = (t == MusicTrack::Theme)
                ? (m_assetsDir + "/music/theme.wav")
                : (m_assetsDir + "/music/levelms.wav");

            if (m_current != t) 
            {
                m_music.stop();
                if (!m_music.openFromFile(path)) 
                {
                    m_current = MusicTrack::None;
                    return;
                }
                m_current = t;
                m_music.setLoop(loop);
                m_music.setVolume(100.f);
            }
            m_music.play();
        }

    private:
        sf::Music   m_music;
        MusicTrack  m_current{ MusicTrack::None };
        std::string m_assetsDir;
    };

} // namespace ark