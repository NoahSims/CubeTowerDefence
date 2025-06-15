#pragma once

#include "Gfx.h"
#include "Splashscreen.h"
#include <cstddef>
#include <unordered_map>

using namespace std;

enum GfxObjects
{
    myBackground=0,
    mySprite,
    goblin,
    grass=10,
    ground,
    bridge=30,
    castle=50,
    goblinHouse,
    bannerTL=60,
    bannerTM,
    bannerTR,
    bannerCL,
    bannerCM,
    bannerCR,
    bannerBL,
    bannerBM,
    bannerBR,
    uiBannerL,
    uiBannerR,
    archer=80,
    archerShoot,
    arrow,
    selectionBorder=90,
    groundScreen=200
};

enum GameState 
{
    splashscreenShown = 0,
    gameShown = 1,
    gameOver = 2
};

enum Timers
{
    startupTimer = 1,
    antSpawnTimer
};

// the structure that ecapsulates mapping element's information
struct mapping_element_t
{
    Cubios::TOPOLOGY_place_t place;
    int resourceID;
    unsigned char angle;
};

// the structure to hold virtual mapping information
struct mapping_t
{
    mapping_element_t value[MODULES_MAX][SCREENS_MAX];
};

struct PathInfo
{
    int pathNum, connectedPathNum;
    bool isActive, isExit;
    bool isOnPath = true;

    PathInfo(int argPathNum, int argConnectedPathNum, bool argIsActive, bool argIsExit) : 
    pathNum(argPathNum), connectedPathNum(argConnectedPathNum), isActive(argIsActive), isExit(argIsExit) {}
};

const int ARCHER_COST = 100;
struct ArcherData
{
    bool isActive = false;
    int spriteID = GfxObjects::archer;
    int frameCount = 0;
    int antTarget = -1;
    float rotation = 0;
    float posX = 200;
    float posY = 50;
    int damage = 1;
    int fireRate = 20;
    int level = 1;

    //ArcherData() : isActive(false) {}
};

enum Commands
{
    cmd_generateNextPath = 1,
    cmd_advertiseScreenPaths = 2,
    cmd_spwanAnt = 3,
    cmd_startAntWave = 4,
    cmd_toggleBuildMenu = 5,
    cmd_enemyKilledNotice = 6,
    cmd_waveStatus = 7,
    cmd_gameState = 8,
    cmd_tracePath = 9
};

enum BuildState
{
    bsNone = 0,
    bsBuildMenu,
    bsPlaceBowman
};

const int DIRECTIONS_MAX = 4;
const Cubios::TOPOLOGY_neighbor_t UsableDirections[DIRECTIONS_MAX] = 
    {Cubios::TOPOLOGY_neighbor_t::NEIGHBOR_LEFT, 
     Cubios::TOPOLOGY_neighbor_t::NEIGHBOR_TOP, 
     Cubios::TOPOLOGY_neighbor_t::NEIGHBOR_RIGHT, 
     Cubios::TOPOLOGY_neighbor_t::NEIGHBOR_BOTTOM};

class Mapper
{
    public:
        // Deleting the copy constructor to prevent copies
        Mapper(const Mapper& obj) = delete;
        static Mapper* getInstance(int module);
        int32_t getHomeModule();
        int32_t getHomeScreen();
        int32_t getHillModule();
        int32_t getHillScreen();
        int getPathNum(int screen, int direction);
        int getDirectionRotation(int direction);
        int getScreenNum(int module, int screen);
    private:
        Mapper(int module);
        ~Mapper();
        //int get(int curModule, int curScreen);
        
    public:
        int moduleNum;
        PathInfo* modulePathMap[12];
        int32_t homeModule;
        int32_t homeScreen;
        bool isHomeModule = false;
        int32_t hillModule;
        int32_t hillScreen;
        bool isHillModule = false;
        int screenPathCount;
        unordered_map<int, int> screenPathCountMap;
        vector<int> pathTraceMap;
    
    private:
        static Mapper* instancePtr;
        mapping_t mapping;
};

/*
class BowmanSpriteAtlasElement: public Cubios::Gfx::SpriteAtlasElement
{
    public: 
        BowmanSpriteAtlasElement(Cubios::Gfx::SpriteAtlasBase* host, const Cubios::Math::Rect2 rc): Cubios::Gfx::SpriteAtlasElement(host,rc) 
        {
            //this->currentFrame = Cubios::random(0,3);
        }
        virtual ~BowmanSpriteAtlasElement() {}
        
        void Render() 
        {
            if(this->currentFrame>5) this->currentFrame = 0;

            Cubios::Math::Vec2 pos = this->ScreenPosition();
            Cubios::GFX_drawSubImage(this->host->resourceId, pos.X,pos.Y, 
                                     this->rc.v0.X+this->currentFrame*192,this->rc.v0.Y,this->rc.W,this->rc.H,
                                     0xFF,
                                     Cubios::Gfx::Colors::magenta,
                                     Transform.ScaleX, Transform.ScaleY, 
                                     Transform.SafeRotation()+ScreenAngle, Transform.Mirroring);
                                     
            //this->currentFrame++;
        }

        BowmanSpriteAtlasElement* Copy() 
        {             
            BowmanSpriteAtlasElement* obj = new BowmanSpriteAtlasElement(this->host, this->rc);
            obj->Transform = this->Transform;
            obj->Color = this->Color;
            return obj;
        }

    private:
        uint8_t currentFrame = 0;
};
*/

class AntSpriteAtlasElement: public Cubios::Gfx::SpriteAtlasElement
{
    public: 
        AntSpriteAtlasElement(Cubios::Gfx::SpriteAtlasBase* host, const Cubios::Math::Rect2 rc): Cubios::Gfx::SpriteAtlasElement(host,rc) 
        {
            //this->currentFrame = Cubios::random(0,3);
        }
        virtual ~AntSpriteAtlasElement() {}
        void Render() 
        {
        if(this->currentFrame>=8) this->currentFrame = 0;
         Cubios::Math::Vec2 pos = this->ScreenPosition();
         Cubios::GFX_drawSubImage(this->host->resourceId, pos.X,pos.Y, 
                                 this->rc.v0.X+(this->currentFrame/4)*128,this->rc.v0.Y+(this->damageFrames/5)*74,this->rc.W,this->rc.H,
                                 0xFF,
                                 Cubios::Gfx::Colors::transparent,
                                 Transform.ScaleX, Transform.ScaleY, 
                                 Transform.SafeRotation()+ScreenAngle, Transform.Mirroring);
        this->currentFrame++;
        if (damageFrames > 0) this->damageFrames--;
        }
        /*
        
        void Tick(int speed)
        {
            //Transform.Position.X+=speed;
            //if(Transform.Position.X>250) { 
            //    Transform.Position.X = -20;
            //    Transform.Position.Y = Cubios::random(30,180);
            //    }
        }
            */
        AntSpriteAtlasElement* Copy() 
        {             
            AntSpriteAtlasElement* obj = new AntSpriteAtlasElement(this->host, this->rc);
            obj->Transform = this->Transform;
            obj->Color = this->Color;
            return obj;
        }

        void onDamaged() {
            damageFrames = 9;
        }
    public:
        int8_t screen;
        int8_t speed = 3;
        int pathNum;
        bool isMovingIn;
        int health = 2;
    private:
        uint8_t currentFrame = 0;
        uint8_t damageFrames = 0;
};

class TowerDefence: public Cubios::Application 
{
public:
    TowerDefence();
    virtual ~TowerDefence();

    virtual void on_PhysicsTick(const std::array<Cubios::Screen, 3>& screens) override;
    virtual void on_Twist(const Cubios::TOPOLOGY_twistInfo_t& twist) override;
    virtual void on_Message(uint32_t type, uint8_t* pkt, u32_t size) override;
    virtual void on_ExternalMessage(uint8_t* pkt, u32_t size) override;
    virtual void on_Pat(uint32_t count) override;
    virtual void on_Render(std::array<Cubios::Screen, 3>& screens) override;
    virtual void on_Tick(uint32_t currentTime, uint32_t deltaTime) override;
    virtual void on_Timer(uint8_t timerID) override;
    virtual void on_Close() override;

    void InitializeResources();
    //void sendNetworkMessage(Cubios::NetworkMessage msg);
private:
    void initializeSplashscreen();
    void initializeSprites();
    //void drawRectangle(int direction);
    void sendDataToNeighbours();
    void searchNextPath(int curModule, int curScreen, int fromDirection, bool isAntGen);
    void nextPathReceived(int fromModule, int fromScreen, int curScreen);
    void antForgePath(int curScreen, int direction);
    void tracePath(int startModule, int startScreen, int startDirection);
    void sendAntToNeighbor(int curModule, int curScreen, int direction, int antId);
    void spawnAnt(int fromModule, int fromScreen, int curScreen, int argHealth);
    void deleteAnt(int antId);
    void antDeath(int antId);
    void startAntWave();
    void sendBuildStateMsg(int newBuildState, int argMoney);
    bool isScreenHill(int screen);
    bool isScreenHome(int screen);
    void updateTargetForArcher(int screen);
    void renderBridgeLayer(int screen);
    void shootAnt(int antId, int damage);
    int directionOrdinalToDegrees(int direction);
    void damageBase(int damage);
    
private:
    Cubios::Gfx::SpriteAtlas<Cubios::Gfx::SpriteAtlasElement>* terrainAtlas;
    Cubios::Gfx::SpriteAtlas<Cubios::Gfx::SpriteAtlasElement>* bridgeAtlas;
    Cubios::Gfx::SpriteAtlas<Cubios::Gfx::SpriteAtlasElement>* bannerAtlas;
    Cubios::Gfx::SpriteAtlas<Cubios::Gfx::SpriteAtlasElement>* archerAtlas;
    int bridgeList[12];
    Cubios::Splashscreen* splashScreen;
    int gameState;
    Mapper* mapInstance;
    Cubios::Gfx::SpriteAtlas<AntSpriteAtlasElement>* antAtlas;
    int antCounter = 1000;
    vector<int> antList;
    vector<int> antsForDeletionList;
    int waveEnemyCounter = 0;
    int waveEnemyDeadCounter = 0;
    int waveLength = 20;
    int currentWave = 1;
    bool isWaveActive = false;
    int screenAngles[SCREENS_MAX];
    int buildState = BuildState::bsNone;
    ArcherData archerList[SCREENS_MAX];
    int money = 100;
    int baseHealth = 50;
    int enemySpeed = 3;
    int enemyHealth = 2;
    int enemySpawnDelay = 2000;
};