#include "SFXManager.h"

void SFXManager::loadAll() 
{

    this->create(SFXType::BACKGROUND, "Media/BGM.wav");
    this->create(SFXType::COLLECT, "Media/collect.wav");
    this->create(SFXType::VIDEO, "Media/video.wav");

    this->getSound(SFXType::BACKGROUND)->setVolume(80);
    this->getSound(SFXType::COLLECT)->setVolume(70);
    this->getSound(SFXType::VIDEO)->setVolume(90);
}

void SFXManager::unloadAll() {
    for (sf::Sound* sound : this->vecSound) 
    {
        if (sound->getStatus() == sf::Sound::Stopped) {
            delete sound;
        }
    }
}

sf::Sound* SFXManager::getSound(SFXType EType) {
    return this->mapSound[EType];
}

void SFXManager::create(SFXType EType, std::string strPath) {
    sf::Sound* pSound = new sf::Sound();
    sf::SoundBuffer* pBuffer = new sf::SoundBuffer();

    pBuffer->loadFromFile(strPath);
    pSound->setBuffer(*pBuffer);
    this->add(EType, pSound);
}

void SFXManager::add(SFXType EType, sf::Sound* pSound) {
    this->mapSound[EType] = pSound;
    this->vecSound.push_back(pSound);
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
SFXManager* SFXManager::P_SHARED_INSTANCE = NULL;
SFXManager::SFXManager() {}
SFXManager::SFXManager(const SFXManager&) {}

SFXManager* SFXManager::getInstance() {
    if (P_SHARED_INSTANCE == NULL)
        P_SHARED_INSTANCE = new SFXManager();

    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */