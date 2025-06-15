
#include "TowerDefence.h"

Mapper* Mapper::getInstance(int module) {
    if (instancePtr == nullptr) {
        instancePtr = new Mapper(module);
    }
    return instancePtr;
}

// Initialize static members
Mapper* Mapper::instancePtr = nullptr;

Mapper::Mapper(int module)
{
    moduleNum = module;
    homeModule = 0;
    homeScreen = 0;

    hillModule = 0;
    hillScreen = 0;

    screenPathCount = 0;

    // get home and hill points
    for (uint8_t module = 0; module < MODULES_MAX; ++module)
    {
        for (uint8_t screen = 0; screen < SCREENS_MAX; ++screen)
        {
            //Cubios::TOPOLOGY_place_t place;
            TOPOLOGY_getPlace(module,screen,Cubios::TOPOLOGY_orientation_mode_t::ORIENTATION_MODE_MENU, &this->mapping.value[module][screen].place);
            this->mapping.value[module][screen].place.face = Cubios::TOPOLOGY_getPlaceOrientation(this->mapping.value[module][screen].place.face, this->mapping.value[module][screen].place.position);

            switch(this->mapping.value[module][screen].place.face)
            {
                case Cubios::TOPOLOGY_orientation_t::ORIENTATION_UP: //0 
                {
                    switch(this->mapping.value[module][screen].place.position)
                    {
                        case 1: 
                        homeModule = module;
                        homeScreen = screen;
                        isHomeModule = (module == this->moduleNum);
                        break;
                    }
                }
                break;
                case Cubios::TOPOLOGY_orientation_t::ORIENTATION_DOWN: //1
                {
                    switch(this->mapping.value[module][screen].place.position)
                    {
                        case 0: 
                        hillModule = module;
                        hillScreen = screen;
                        isHillModule = (module == this->moduleNum);
                        break;
                    }
                }
                break;
            }
        }
    }

    LOG_i("Home = Mod %d; Scr = %d", homeModule, homeScreen);
    LOG_i("Hill = Mod %d; Scr = %d", hillModule, hillScreen);

    // initialize modulePathMap
    for (uint8_t screen = 0; screen < SCREENS_MAX; ++screen) {
        for (uint8_t direction = 0; direction < DIRECTIONS_MAX; ++direction) {
            int pathNum = getPathNum(screen, direction);
            modulePathMap[pathNum] = new PathInfo(pathNum + (moduleNum * 100), -1, false, false);
        }

        screenPathCountMap[getScreenNum(module, screen)] = 0;
    }

    
}

Mapper::~Mapper() 
{
}

int Mapper::getPathNum(int screen, int direction) {
    return (direction >= 0) ? (screen * DIRECTIONS_MAX) + direction : direction;
}

int Mapper::getScreenNum(int module, int screen) {
    return (module * 10) + screen;
}

int Mapper::getDirectionRotation(int direction) {
    switch (direction) {
    case 0:
        return 180;
        break;
    case 1:
        return 270;
        break;
    case 2:
        return 0;
        break;
    case 3:
        return 90;
        break;
    default:
        return -1;
        break;
    }
}

int32_t Mapper::getHomeModule() {
    return homeModule;
}

int32_t Mapper::getHomeScreen() {
    return homeScreen;
}

int32_t Mapper::getHillModule() {
    return hillModule;
}

int32_t Mapper::getHillScreen() {
    return hillScreen;
}