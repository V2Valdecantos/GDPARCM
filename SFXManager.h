#pragma once
#include "vector"
#include "unordered_map"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "StringUtils.h"

enum class SFXType : int {
    BACKGROUND,
    COLLECT,
    LOADED,
    VIDEO,
};

class SFXManager 
{
    private:
        std::unordered_map<SFXType, sf::Sound*> mapSound;
        std::vector<sf::Sound*> vecSound;

    public:
        void loadAll();
        void unloadAll();
        sf::Sound* getSound(SFXType EType);

    private:
        void create(SFXType EType, std::string strPath);
        void add(SFXType EType, sf::Sound* pSound);

        /* * * * * * * * * * * * * * * * * * * * *
            *       SINGLETON-RELATED CONTENT       *
            * * * * * * * * * * * * * * * * * * * * */
    private:
        static SFXManager* P_SHARED_INSTANCE;

    private:
        SFXManager();
        SFXManager(const SFXManager&);
        SFXManager& operator = (const SFXManager&);

    public:
        static SFXManager* getInstance();
        /* * * * * * * * * * * * * * * * * * * * */


};

