#include <stdint.h>
#include <string>
#include <vector>
#include <string>
#include <memory>

#include "AppManager.h"
#include "TowerDefence.h"

using namespace Cubios;
using namespace Cubios::Math;
using namespace Cubios::Gfx;

//Applicaton initialization callback. Called once when CUB application starts
void OnInit(Cubios::AppManager& appMgr) 
{
    TowerDefence* theApp = new TowerDefence();

    appMgr.SetApplication(theApp,"cubeTowerDefence");

    theApp->InitializeResources();
}


TowerDefence::TowerDefence()
{
   this->gameState = GameState::gameShown;

   this->splashScreen = new Cubios::Splashscreen(this, Cubios::SplsParms::e_LeadersDataType::typeNumber);
}

TowerDefence::~TowerDefence() 
{ 
    this->Scene.DisposeAllObjects();

    if(this->splashScreen!=nullptr) delete this->splashScreen; 
}

void TowerDefence::InitializeResources()
{    
    //Initialize splash screen
    //this->initializeSplashscreen();

    // conserve screen angles
    for (int screen = 0; screen < SCREENS_MAX; screen++) {
        screenAngles[screen] = Cubios::TOPOLOGY_getAngle(this->Module, screen, TOPOLOGY_orientation_mode_t::ORIENTATION_MODE_MENU);
        archerList[screen].rotation = screenAngles[screen];
    }

    //Create a Cubios::Gfx::Sprite object
    //this->Scene.CreateObjectWithID(GfxObjects::mySprite,new Sprite("cube.png",Transform(120,120,0)));
    //this->Scene[GfxObjects::mySprite]->Color.SetA(255);

    this->mapInstance = Mapper::getInstance(this->Module);

    // initialize sprite maps
    this->terrainAtlas = new Cubios::Gfx::SpriteAtlas<Cubios::Gfx::SpriteAtlasElement>("grassTile.png", &this->Scene);
    this->terrainAtlas->AddSprite(GfxObjects::grass, Cubios::Math::Rect2(0, 0, 64, 64));

    this->bridgeAtlas = new Cubios::Gfx::SpriteAtlas<Cubios::Gfx::SpriteAtlasElement>("Bridge.png", &this->Scene);
    this->bridgeAtlas->AddSprite(GfxObjects::bridge, Cubios::Math::Rect2(0, 17, 64, 156));

    this->bannerAtlas = new Cubios::Gfx::SpriteAtlas<Cubios::Gfx::SpriteAtlasElement>("Banner_Horizont", &this->Scene);
    this->bannerAtlas->AddSprite(GfxObjects::bannerTL, Cubios::Math::Rect2(0, 0, 64, 64));
    this->bannerAtlas->AddSprite(GfxObjects::bannerTM, Cubios::Math::Rect2(64, 0, 64, 64));
    this->bannerAtlas->AddSprite(GfxObjects::bannerTR, Cubios::Math::Rect2(128, 0, 64, 64));
    this->bannerAtlas->AddSprite(GfxObjects::bannerCL, Cubios::Math::Rect2(0, 64, 64, 64));
    this->bannerAtlas->AddSprite(GfxObjects::bannerCM, Cubios::Math::Rect2(64, 64, 64, 64));
    this->bannerAtlas->AddSprite(GfxObjects::bannerCR, Cubios::Math::Rect2(128, 64, 64, 64));
    this->bannerAtlas->AddSprite(GfxObjects::bannerBL, Cubios::Math::Rect2(0, 128, 64, 64));
    this->bannerAtlas->AddSprite(GfxObjects::bannerBM, Cubios::Math::Rect2(64, 128, 64, 64));
    this->bannerAtlas->AddSprite(GfxObjects::bannerBR, Cubios::Math::Rect2(128, 128, 64, 64));

    this->archerAtlas = new Cubios::Gfx::SpriteAtlas<Cubios::Gfx::SpriteAtlasElement>("Archer_Blue", &this->Scene);
    this->archerAtlas->AddSprite(GfxObjects::archer, Cubios::Math::Rect2(0, 0, 80, 75));
    this->archerAtlas->AddSprite(GfxObjects::archerShoot, Cubios::Math::Rect2(81, 0, 80, 75));

    if (this->Module == mapInstance->getHomeModule()) {
        this->Scene.CreateObjectWithID(GfxObjects::castle, new Sprite("Castle_Blue.png",Transform(120,120,0)));
        this->Scene[GfxObjects::castle]->Transform.ScaleX = 50;
        this->Scene[GfxObjects::castle]->Transform.ScaleY = 50;
    }
    if (this->Module == mapInstance->getHillModule()) {
        this->Scene.CreateObjectWithID(GfxObjects::goblinHouse, new Sprite("Goblin_House",Transform(120,120,0)));
    }

    this->antAtlas = new Cubios::Gfx::SpriteAtlas<AntSpriteAtlasElement>("Torch_Red4.png",&this->Scene);
    this->antAtlas->AddSprite(GfxObjects::goblin, Cubios::Math::Rect2(0,0,128,74));
    this->antAtlas->AddSprite(GfxObjects::goblin+1, Cubios::Math::Rect2(128,0,128,74));

    this->Scene.CreateObjectWithID(GfxObjects::selectionBorder, new Sprite("selection.png",Transform(120,120,0)));

    // initialize sprites
    this->initializeSprites();

    //for (int screen = 0; screen < SCREENS_MAX; screen++) {
    //    renderBridgeLayer(screen);
    //}

    // wait for cubes to turn on before mapping
    this->SetTimer(Timers::startupTimer, 1000);
}

void TowerDefence::initializeSplashscreen()
{
      //Initialize splashscreen
      /*
    if(this->splashScreen!=nullptr)
    {
        bool show_saved_values = false;

        //load splash screen images
        this->splashScreen->InitSplashScreenSprites(Cubios::GFX_getAssetId("ss_back.png"),
                                                    Cubios::GFX_getAssetId("ss_main.png"),
                                                    Cubios::GFX_getAssetId("ss_name.png"),
                                                    Cubios::GFX_getAssetId("ss_QR_code.png"),
                                                    Cubios::GFX_getAssetId("ss_board.png"),
                                                    Cubios::GFX_getAssetId("ss_results.png"),
                                                    Cubios::GFX_getAssetId("ss_twist_1.png"),
                                                    Cubios::GFX_getAssetId("ss_twist_2.png"),
                                                    Cubios::GFX_getAssetId("ss_pat.png"));

        //set text colours
        this->splashScreen->SetColors(Cubios::Gfx::Colors::orangeRed, Cubios::Gfx::Colors::lightSteelBlue);

        //set the type of leaderboard table
        if(!show_saved_values)
        {
            this->splashScreen->SetLeaderBoardTable(Cubios::SplsParms::LeaderBoardWithoutSaves);
        }
        else
        {
            this->splashScreen->SetLeaderBoardTable(Cubios::SplsParms::LeaderBoardWithSaves);

            this->splashScreen->SetNamedValue(0, "time", 1100,  Cubios::SplsParms::e_LeadersDataType::typeTime);
            this->splashScreen->SetNamedValue(1, "level", 1,  Cubios::SplsParms::e_LeadersDataType::typeNumber);
            this->splashScreen->SetNamedValue(2, "score", 1234,  Cubios::SplsParms::e_LeadersDataType::typeNumber); 
        }

        //set personal best value
        this->splashScreen->SetRecord(1234567);
    }
        */
}

void TowerDefence::initializeSprites() {
    OffscreenRenderTarget* rt = new OffscreenRenderTarget(240,240,Cubios::GFX_PixelFormat_t::FORMAT_RGB565);
    rt->SetPosition(120, 120);

    rt->Begin(true);
    rt->Add(terrainAtlas->Get(GfxObjects::grass))->SetPosition(32, 32);
    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(96, 32);
    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(160, 32);
    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(224, 32);

    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(32, 96);
    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(96, 96);
    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(160, 96);
    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(224, 96);

    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(32, 160);
    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(96, 160);
    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(160, 160);
    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(224, 160);

    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(32, 224);
    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(96, 224);
    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(160, 224);
    rt->AddCopy(terrainAtlas->Get(GfxObjects::grass)->Copy())->SetPosition(224, 224);
    
    rt->End();
    this->Scene.CreateObjectWithID(GfxObjects::ground, rt);

    for (int screen = 0; screen < SCREENS_MAX; screen++) {
        for (int direction = 0; direction < DIRECTIONS_MAX; direction++) {
            int bridgeNum = mapInstance->getPathNum(screen, direction);
            bridgeList[bridgeNum] = GfxObjects::bridge + 1 + bridgeNum;
            /*
            this->Scene.CreateObjectWithID(bridgeList[bridgeNum], bridgeAtlas->Get(GfxObjects::bridge)->Copy());
            switch (direction)
            {
            case 0:
                this->Scene[bridgeList[bridgeNum]]->SetTransform(Transform(78, 120, 90));
                break;
            case 1:
                this->Scene[bridgeList[bridgeNum]]->SetTransform(Transform(120, 78, 180));
                break;
            case 2:
                this->Scene[bridgeList[bridgeNum]]->SetTransform(Transform(162, 120, 270));
                break;
            case 3:
                this->Scene[bridgeList[bridgeNum]]->SetTransform(Transform(120, 162, 0));
                break;
            
            default:
                break;
            }
                */
        }
    }

    OffscreenRenderTarget* rtLeftBuildBanner = new OffscreenRenderTarget(240,240,Cubios::GFX_PixelFormat_t::FORMAT_ARGB6666);
    rtLeftBuildBanner->SetPosition(120, 120);
    rtLeftBuildBanner->Begin(true);
    rtLeftBuildBanner->Add(bannerAtlas->Get(GfxObjects::bannerTL))->SetPosition(32, 32);
    rtLeftBuildBanner->Add(bannerAtlas->Get(GfxObjects::bannerTM))->SetPosition(96, 32);
    rtLeftBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerTM)->Copy())->SetPosition(160, 32);
    rtLeftBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerTM)->Copy())->SetPosition(224, 32);

    rtLeftBuildBanner->Add(bannerAtlas->Get(GfxObjects::bannerCL))->SetPosition(32, 96);
    rtLeftBuildBanner->Add(bannerAtlas->Get(GfxObjects::bannerCM))->SetPosition(96, 96);
    rtLeftBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerCM)->Copy())->SetPosition(160, 96);
    rtLeftBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerCM)->Copy())->SetPosition(224, 96);

    rtLeftBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerCL)->Copy())->SetPosition(32, 160);
    rtLeftBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerCM)->Copy())->SetPosition(96, 160);
    rtLeftBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerCM)->Copy())->SetPosition(160, 160);
    rtLeftBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerCM)->Copy())->SetPosition(224, 160);

    rtLeftBuildBanner->Add(bannerAtlas->Get(GfxObjects::bannerBL))->SetPosition(32, 224);
    rtLeftBuildBanner->Add(bannerAtlas->Get(GfxObjects::bannerBM))->SetPosition(96, 224);
    rtLeftBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerBM)->Copy())->SetPosition(160, 224);
    rtLeftBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerBM)->Copy())->SetPosition(224, 224);

    this->archerAtlas->Get(GfxObjects::archer)->SetPosition(140, 130);
    rtLeftBuildBanner->AddCopy(this->archerAtlas->Get(GfxObjects::archer)->Copy());

    Text tLabel = Text("Archer", Transform(140,70), 8, Cubios::Gfx::Colors::steelBlue, Cubios::text_align_t::TEXT_ALIGN_CENTER);
    Text tCost = Text("Cost $" + to_string(ARCHER_COST), Transform(140,190), 8, Cubios::Gfx::Colors::darkGoldenrod, Cubios::text_align_t::TEXT_ALIGN_CENTER);
    rtLeftBuildBanner->Add(&tLabel);
    rtLeftBuildBanner->Add(&tCost);

    rtLeftBuildBanner->End();
    this->Scene.CreateObjectWithID(GfxObjects::uiBannerL, rtLeftBuildBanner);

    OffscreenRenderTarget* rtRightBuildBanner = new OffscreenRenderTarget(240,240,Cubios::GFX_PixelFormat_t::FORMAT_ARGB6666);
    rtRightBuildBanner->SetPosition(120, 120);
    rtRightBuildBanner->Begin(true);
    rtRightBuildBanner->Add(bannerAtlas->Get(GfxObjects::bannerTM))->SetPosition(32, 32);
    rtRightBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerTM)->Copy())->SetPosition(96, 32);
    rtRightBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerTM)->Copy())->SetPosition(160, 32);
    rtRightBuildBanner->Add(bannerAtlas->Get(GfxObjects::bannerTR))->SetPosition(224, 32);

    rtRightBuildBanner->Add(bannerAtlas->Get(GfxObjects::bannerCM))->SetPosition(32, 96);
    rtRightBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerCM)->Copy())->SetPosition(96, 96);
    rtRightBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerCM)->Copy())->SetPosition(160, 96);
    rtRightBuildBanner->Add(bannerAtlas->Get(GfxObjects::bannerCR))->SetPosition(224, 96);

    rtRightBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerCM)->Copy())->SetPosition(32, 160);
    rtRightBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerCM)->Copy())->SetPosition(96, 160);
    rtRightBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerCM)->Copy())->SetPosition(160, 160);
    rtRightBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerCR)->Copy())->SetPosition(224, 160);

    rtRightBuildBanner->Add(bannerAtlas->Get(GfxObjects::bannerBM))->SetPosition(32, 224);
    rtRightBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerBM)->Copy())->SetPosition(96, 224);
    rtRightBuildBanner->AddCopy(bannerAtlas->Get(GfxObjects::bannerBM)->Copy())->SetPosition(160, 224);
    rtRightBuildBanner->Add(bannerAtlas->Get(GfxObjects::bannerBR))->SetPosition(224, 224);
    rtRightBuildBanner->End();
    this->Scene.CreateObjectWithID(GfxObjects::uiBannerR, rtRightBuildBanner);
}

//This callback is called every "tick" of cubeapp application loop
void TowerDefence::on_Tick(uint32_t currentTime, uint32_t deltaTime)
{
    //tick splash screen
    //this->splashScreen->Tick(currentTime, deltaTime);
    //move ants
    for(int ant = 0; ant < antList.size(); ant++) {
        AntSpriteAtlasElement* antPtr = dynamic_cast<AntSpriteAtlasElement*>(this->Scene[antList[ant]]);

        float xSpeed = 0;
        float ySpeed = 0;
        int direction = antPtr->pathNum % DIRECTIONS_MAX;
        if (!mapInstance->modulePathMap[antPtr->pathNum]->isActive) {
            // make new path
            antForgePath(antPtr->screen, direction);
        }
        //LOG_i("%d Following path %d, in direction %d",antList[ant], antPtr->pathNum, direction);
        if ((direction == 0 && antPtr->isMovingIn) || (direction == 2 && !antPtr->isMovingIn)) {
            xSpeed = antPtr->speed;
            Scene[antList[ant]]->SetRotation(0);
        }
        else if ((direction == 0 && !antPtr->isMovingIn) || (direction == 2 && antPtr->isMovingIn)) {
            xSpeed = (-1) * antPtr->speed;
            Scene[antList[ant]]->SetRotation(180);
        }
        else if ((direction == 1 && antPtr->isMovingIn) || (direction == 3 && !antPtr->isMovingIn)) {
            ySpeed = antPtr->speed;
            Scene[antList[ant]]->SetRotation(90);
        }
        else if ((direction == 1 && !antPtr->isMovingIn) || (direction == 3 && antPtr->isMovingIn)) {
            ySpeed = (-1) * antPtr->speed;
            Scene[antList[ant]]->SetRotation(270);
        }
        Transform antTransform = this->Scene[antList[ant]]->Transform;
        //LOG_i("moving ant%d? x%f, y%f", antList[ant], antTransform.Position.X, antTransform.Position.Y);
        //LOG_i("moving ant%d? xspeed%f, yspeed%f", antList[ant], xSpeed, ySpeed);
        Scene[antList[ant]]->Transform.Position.X += xSpeed;
        Scene[antList[ant]]->Transform.Position.Y += ySpeed;

        switch (direction)
        {
        case 0:
            if (antTransform.Position.X < 0) {
                sendAntToNeighbor(this->Module, antPtr->screen, direction, antList[ant]);
            }
            else if (antTransform.Position.X > 120) {
                if (isScreenHome(antPtr->screen)) {
                    // ant does damage
                    antDeath(antList[ant]);
                    damageBase(1);
                }
                // rotate ant
                antPtr->pathNum = mapInstance->modulePathMap[antPtr->pathNum]->connectedPathNum;
                antPtr->isMovingIn = false;
            }
            break;
        case 1:
            if (antTransform.Position.Y < 0) {
                sendAntToNeighbor(this->Module, antPtr->screen, direction, antList[ant]);
            }
            else if (antTransform.Position.Y > 120) {
                if (isScreenHome(antPtr->screen)) {
                    // ant does damage
                    antDeath(antList[ant]);
                }
                // rotate ant
                antPtr->pathNum = mapInstance->modulePathMap[antPtr->pathNum]->connectedPathNum;
                antPtr->isMovingIn = false;
            }
            break;
        case 2:
            if (antTransform.Position.X > 240) {
                sendAntToNeighbor(this->Module, antPtr->screen, direction, antList[ant]);
            }
            else if (antTransform.Position.X < 120) {
                if (isScreenHome(antPtr->screen)) {
                    // ant does damage
                    antDeath(antList[ant]);
                    damageBase(1);
                }
                // rotate ant
                antPtr->pathNum = mapInstance->modulePathMap[antPtr->pathNum]->connectedPathNum;
                antPtr->isMovingIn = false;
            }
            break;
        case 3:
            if (antTransform.Position.Y > 240) {
                sendAntToNeighbor(this->Module, antPtr->screen, direction, antList[ant]);
            }
            else if (antTransform.Position.Y < 120) {
                if (isScreenHome(antPtr->screen)) {
                    // ant does damage
                    antDeath(antList[ant]);
                    damageBase(1);
                }
                // rotate ant
                antPtr->pathNum = mapInstance->modulePathMap[antPtr->pathNum]->connectedPathNum;
                antPtr->isMovingIn = false;
            }
            break;
        
        default:
            break;
        }
        
    }

    // this is gross as hell, I hate c++ and its 2AM
    // TODO idk, fix this?
    for (int ant = 0; ant < antsForDeletionList.size(); ant++) {
        deleteAnt(antsForDeletionList[ant]);
        antsForDeletionList.erase(antsForDeletionList.begin()+ant);
        ant--;
    }
    //antsForDeletionList.clear();

    for (int screen = 0; screen < SCREENS_MAX; screen++) {
        if (archerList[screen].isActive) {
            

            if(archerList[screen].antTarget > 0) {
                archerList[screen].rotation = atanf((this->Scene[archerList[screen].antTarget]->Transform.Position.Y - archerList[screen].posY) / (this->Scene[archerList[screen].antTarget]->Transform.Position.X - archerList[screen].posX));
                //archerList[screen].rotation = atanf((archerList[screen].posY - this->Scene[archerList[screen].antTarget]->Transform.Position.Y) / (archerList[screen].posX - this->Scene[archerList[screen].antTarget]->Transform.Position.X + 1));
                //LOG_i("Target %d = X%f, Y%f", archerList[screen].antTarget, this->Scene[archerList[screen].antTarget]->Transform.Position.X, this->Scene[archerList[screen].antTarget]->Transform.Position.Y);
                archerList[screen].rotation = archerList[screen].rotation * 180 / PI + 180;
                //archerList[screen].rotation = screenAngles[screen];
                //LOG_i("Halp, angle = %f", archerList[screen].rotation);

                if(archerList[screen].frameCount > (archerList[screen].fireRate / 2)) {
                    archerList[screen].spriteID = GfxObjects::archerShoot;
                }
                if(archerList[screen].frameCount > archerList[screen].fireRate) {
                    archerList[screen].spriteID = GfxObjects::archer;
                    archerList[screen].frameCount = 0;
                    shootAnt(archerList[screen].antTarget, archerList[screen].damage);
                }
            }
            else {
                archerList[screen].rotation = 180;
                archerList[screen].frameCount = 0;
                //LOG_i("Halp, angle = %f", archerList[screen].rotation);
            }

            archerList[screen].frameCount++;
        }
    }
}

void TowerDefence::sendAntToNeighbor(int curModule, int curScreen, int direction, int antId) {
    Cubios::TOPOLOGY_faceletInfo_t nextScreen;
    Cubios::TOPOLOGY_getAdjacentFacelet(curModule, curScreen, UsableDirections[direction], &nextScreen);
    // if moving to screen within module
    if (nextScreen.module == curModule) {
        int8_t toDirection;
        for (toDirection = 0; toDirection < DIRECTIONS_MAX; toDirection++) {
            Cubios::TOPOLOGY_faceletInfo_t previousScreen;
            Cubios::TOPOLOGY_getAdjacentFacelet(curModule, nextScreen.screen, UsableDirections[toDirection], &previousScreen);
            if(previousScreen.module == curModule && previousScreen.screen == curScreen) {
                // found right direction
                break;
            }
        }

        Vec2 spawnPoint;
        switch (toDirection) {
            case 0: spawnPoint = Vec2(0, 120); break;
            case 1: spawnPoint = Vec2(120, 0); break;
            case 2: spawnPoint = Vec2(240, 120); break;
            case 3: spawnPoint = Vec2(120, 240); break;
        }

        this->Scene[antId]->SetTransform(Transform(spawnPoint.X, spawnPoint.Y));
        AntSpriteAtlasElement* antPtr = dynamic_cast<AntSpriteAtlasElement*>(this->Scene[antId]);
        antPtr->screen = nextScreen.screen;
        antPtr->isMovingIn = true;
        antPtr->pathNum = mapInstance->getPathNum(nextScreen.screen, toDirection);

        updateTargetForArcher(antPtr->screen);
        updateTargetForArcher(curScreen);
    }
    else {
        // send message
        Cubios::NetworkMessage msg;
        msg.Reset(true);
        msg.WriteInt(nextScreen.module, 16); // intended module
        msg.WriteInt(nextScreen.screen, 16); // intended screen
        msg.WriteInt(curModule, 16); // from module
        msg.WriteInt(curScreen, 16); // from screen
        msg.WriteInt(dynamic_cast<AntSpriteAtlasElement*>(this->Scene[antId])->health, 16); // current health
        this->SendNetworkMessage(Commands::cmd_spwanAnt, &msg);

        antsForDeletionList.push_back(antId);
    }
}

void TowerDefence::deleteAnt(int antId) {
    LOG_i("Deleting ant %d", antId);
    if(find(antList.begin(), antList.end(), antId) == antList.end()) {
        LOG_i("SEVERE: atempting to delete enemy that has already been deleted - %d", antId);
        return;
        //TODO investigate this. Maybe an enemy takes enough damage to die on the same frame it leaves screen??
    }
    int screen = dynamic_cast<AntSpriteAtlasElement*>(this->Scene[antId])->screen;
    dynamic_cast<AntSpriteAtlasElement*>(this->Scene[antId])->~AntSpriteAtlasElement();
    this->Scene.DisposeObjectWithID(antId);
    antList.erase(find(antList.begin(), antList.end(), antId));
    updateTargetForArcher(screen);
}

void TowerDefence::antDeath(int antId) {
    antsForDeletionList.push_back(antId);
    Cubios::NetworkMessage msg;
    msg.Reset(true);
    this->SendNetworkMessage(Commands::cmd_enemyKilledNotice, &msg);
}

//This callback is called every time device is about to render visuals. Use it for calling your rendering code.  
void TowerDefence::on_Render(std::array<Cubios::Screen, 3>& screens)
{
    for(auto i = screens.begin(); i != screens.end(); ++i)
    {
        if(this->gameState == GameState::splashscreenShown)
        {
           //render splash screen
            //this->splashScreen->Render(i);
        }
        else
        {
            i->Begin();
            
            // render ground
            this->Scene[GfxObjects::ground]->SetRotation(screenAngles[i->ID()]);
            i->Add(this->Scene[GfxObjects::ground]);
            /*

            // render paths
            for (int direction = 0; direction < DIRECTIONS_MAX; direction++) {
                if (mapInstance->modulePathMap[mapInstance->getPathNum(i->ID(), direction)]->isActive) {
                    i->Add(this->Scene[bridgeList[mapInstance->getPathNum(i->ID(), direction)]]);
                }
            }
                */

            for (int direction = 0; direction < DIRECTIONS_MAX; direction++) {
                if (mapInstance->modulePathMap[mapInstance->getPathNum(i->ID(), direction)]->isActive) {
                    int bridgeNum = mapInstance->getPathNum(i->ID(), direction);
                    bridgeList[bridgeNum] = GfxObjects::bridge + 1 + bridgeNum;
                    switch (direction)
                    {
                    case 0: this->bridgeAtlas->Get(GfxObjects::bridge)->SetTransform(Transform(78, 120, 90)); break;
                    case 1: this->bridgeAtlas->Get(GfxObjects::bridge)->SetTransform(Transform(120, 78, 180)); break;
                    case 2: this->bridgeAtlas->Get(GfxObjects::bridge)->SetTransform(Transform(162, 120, 270)); break;
                    case 3: this->bridgeAtlas->Get(GfxObjects::bridge)->SetTransform(Transform(120, 162, 0)); break;
                    default: break;
                    }
                    i->AddCopy(bridgeAtlas->Get(GfxObjects::bridge)->Copy());
                }
            }
            
            //i->Add(this->Scene[GfxObjects::groundScreen + mapInstance->getScreenNum(this->Module, i->ID())]);

            // render buildings
            //Text tHome = Text("HOME", Transform(40,40), 8, Cubios::Gfx::Colors::white, Cubios::text_align_t::TEXT_ALIGN_CENTER);
            Text tInstruction = Text("Tap to build towers", Transform(100,120, screenAngles[i->ID()]), 7, Cubios::Gfx::Colors::white, Cubios::text_align_t::TEXT_ALIGN_CENTER);
            Text tHealth = Text("Base: " + to_string(this->baseHealth), Transform(240,240, screenAngles[i->ID()]), 7, Cubios::Gfx::Colors::darkRed, Cubios::text_align_t::TEXT_ALIGN_LEFT_BOTTOM_CORNER);
            Text tMoney = Text("$" + to_string(this->money), Transform(240,0,screenAngles[i->ID()]), 7, Cubios::Gfx::Colors::white, Cubios::text_align_t::TEXT_ALIGN_RIGHT_BOTTOM_CORNER);
            Text tWave = Text("Wave: " + to_string(this->currentWave), Transform(0,240), 7, Cubios::Gfx::Colors::white, Cubios::text_align_t::TEXT_ALIGN_LEFT_BOTTOM_CORNER);
            if (isScreenHome(i->ID())) {
                this->Scene[GfxObjects::castle]->SetRotation(screenAngles[i->ID()]);
                i->Add(this->Scene[GfxObjects::castle]);

                if (!isWaveActive && buildState == BuildState::bsNone) {
                    i->Add(&tInstruction);
                }
                else if (buildState != BuildState::bsNone) {
                    tInstruction.SetContent("Tap to cancel build");
                    i->Add(&tInstruction);
                }
                
                //tHealth.SetTransform(Transform(240, 240, screenAngles[i->ID()]));
                //tMoney.SetTransform(Transform(240, 0, screenAngles[i->ID()]));
                i->Add(&tHealth);
                i->Add(&tMoney);
            }
            else if (isScreenHill(i->ID())) {
                //tHome.SetContent("HILL");
                this->Scene[GfxObjects::goblinHouse]->SetRotation(screenAngles[i->ID()]);
                i->Add(this->Scene[GfxObjects::goblinHouse]);
                i->Add(&tWave);
                if (!isWaveActive) {
                    tInstruction.SetContent("Tap to start wave");
                    tInstruction.SetPosition(120, 100);
                    i->Add(&tInstruction);
                }
            }
            //else {
            //    tHome.SetContent("Mod: " + std::to_string(this->Module) + "; ID: " + std::to_string(i->ID()));
            //    tHome.Transform.Position.X = 80;
            //}
            //i->Add(&tHome);

            // render towers
            Text tLevel = Text("lvl" + to_string(archerList[i->ID()].level), Transform(240, 0, 180), 7, Cubios::Gfx::Colors::white, Cubios::text_align_t::TEXT_ALIGN_LEFT_BOTTOM_CORNER);
            if (archerList[i->ID()].isActive) {
                this->archerAtlas->Get(archerList[i->ID()].spriteID)->SetPosition(archerList[i->ID()].posX, archerList[i->ID()].posY);
                //LOG_i("rotation int = %d", static_cast<int>(archerList[i->ID()].rotation));
                //int temp = static_cast<int>(archerList[i->ID()].rotation);
                this->archerAtlas->Get(archerList[i->ID()].spriteID)->SetRotation(static_cast<int>(archerList[i->ID()].rotation));
                //this->archerAtlas->Get(archerList[i->ID()].spriteID)->SetRotation(screenAngles[i->ID()]);
                i->Add(&tLevel);
                i->Add(this->archerAtlas->Get(archerList[i->ID()].spriteID));
            }

            // render enemies
            for(int ant = 0; ant < antList.size(); ant++) {
                if(dynamic_cast<AntSpriteAtlasElement*>(this->Scene[antList[ant]])->screen == i->ID()) {
                    i->Add(this->Scene[antList[ant]]);
                }
            }

            // render build menu ui
            if(buildState == BuildState::bsBuildMenu) {
                Cubios::TOPOLOGY_place_t homePlace;
                Cubios::TOPOLOGY_getPlace(mapInstance->getHomeModule(), mapInstance->getHomeScreen(), Cubios::TOPOLOGY_orientation_mode_t::ORIENTATION_MODE_MENU, &homePlace);
                if(homePlace.face == i->Face() && i->Position() == (homePlace.position + 1) % 4) {
                    this->Scene[GfxObjects::uiBannerL]->SetRotation(180);
                    i->Add(this->Scene[GfxObjects::uiBannerL]);
                }
                if(homePlace.face == i->Face() && i->Position() == (homePlace.position + 2) % 4) {
                    this->Scene[GfxObjects::uiBannerR]->SetRotation(90);
                    i->Add(this->Scene[GfxObjects::uiBannerR]);
                }
            }

            Text tGameOver1 = Text("Game Over", Transform(0, 0), 12, Cubios::Gfx::Colors::darkRed, Cubios::text_align_t::TEXT_ALIGN_CENTER);
            Text tGameOver2 = Text("Survived " + to_string(this->currentWave - 1) + " Waves", Transform(0, 0), 8, Cubios::Gfx::Colors::white, Cubios::text_align_t::TEXT_ALIGN_CENTER);
            if(this->gameState == GameState::gameOver) {
                switch (i->Position()) {
                case 0:
                    tGameOver1.SetTransform(Transform(120, 100, screenAngles[i->ID()]));
                    tGameOver2.SetTransform(Transform(120, 140, screenAngles[i->ID()]));
                    break;
                case 1:
                    tGameOver1.SetTransform(Transform(100, 120, screenAngles[i->ID()]));
                    tGameOver2.SetTransform(Transform(140, 120, screenAngles[i->ID()]));
                    break;
                case 2:
                    tGameOver1.SetTransform(Transform(120, 140, screenAngles[i->ID()]));
                    tGameOver2.SetTransform(Transform(120, 100, screenAngles[i->ID()]));
                    break;
                case 3:
                    tGameOver1.SetTransform(Transform(140, 120, screenAngles[i->ID()]));
                    tGameOver2.SetTransform(Transform(100, 120, screenAngles[i->ID()]));
                    break;
                default:
                    break;
                }
                i->Add(&tGameOver1);
                i->Add(&tGameOver2);
            }

            // render selected screen
            //Text tUp = Text("UP", Transform(200,200), 8, Cubios::Gfx::Colors::white, Cubios::text_align_t::TEXT_ALIGN_CENTER);
            Text tCost = Text("Cost -$" + to_string(ARCHER_COST), Transform(120,120,screenAngles[i->ID()]), 8, Cubios::Gfx::Colors::white, Cubios::text_align_t::TEXT_ALIGN_CENTER);
            if (i->Direction() == Cubios::TOPOLOGY_orientation_t::ORIENTATION_UP && MS_getFaceAccelX(i->ID()) < 0 && MS_getFaceAccelY(i->ID()) < 0) {
                //i->Add(&tUp);

                if(buildState == BuildState::bsPlaceBowman && !isScreenHome(i->ID()) && !isScreenHill(i->ID())) {
                    if(!archerList[i->ID()].isActive) {
                        // new archer
                        this->archerAtlas->Get(GfxObjects::archer)->SetPosition(200, 50);
                        this->archerAtlas->Get(GfxObjects::archer)->SetRotation(screenAngles[i->ID()]);
                        i->Add(this->archerAtlas->Get(GfxObjects::archer));
                    }
                    else {
                        // upgrade archer
                        tCost.SetContent("Upgrade-$" + to_string(ARCHER_COST + (archerList[i->ID()].level * ARCHER_COST / 2)));
                    }
                    i->Add(&tCost);
                }

                i->Add(this->Scene[GfxObjects::selectionBorder]);
            }

            i->End();
        }
    }
}

void TowerDefence::renderBridgeLayer(int screen) {
    /*
    this->Scene.DisposeObjectWithID(GfxObjects::groundScreen + mapInstance->getScreenNum(this->Module, screen));
    OffscreenRenderTarget* rt = new OffscreenRenderTarget(240,240,Cubios::GFX_PixelFormat_t::FORMAT_ARGB6666);
    rt->SetPosition(120, 120);

    rt->Begin(false);
    // render ground
    //this->Scene[GfxObjects::ground]->SetRotation(screenAngles[screen]);
    //rt->Add(this->Scene[GfxObjects::ground]);

    // render paths
    /*
    for (int direction = 0; direction < DIRECTIONS_MAX; direction++) {
        if (mapInstance->modulePathMap[mapInstance->getPathNum(screen, direction)]->isActive) {
            rt->Add(this->Scene[bridgeList[mapInstance->getPathNum(screen, direction)]]);
        }
    }
        */
       /*

    for (int direction = 0; direction < DIRECTIONS_MAX; direction++) {
        if (mapInstance->modulePathMap[mapInstance->getPathNum(screen, direction)]->isActive) {
            int bridgeNum = mapInstance->getPathNum(screen, direction);
            bridgeList[bridgeNum] = GfxObjects::bridge + 1 + bridgeNum;
            switch (direction)
            {
            case 0:
                this->bridgeAtlas->Get(GfxObjects::bridge)->SetTransform(Transform(78, 120, 90));
                break;
            case 1:
                this->bridgeAtlas->Get(GfxObjects::bridge)->SetTransform(Transform(120, 78, 180));
                break;
            case 2:
                this->bridgeAtlas->Get(GfxObjects::bridge)->SetTransform(Transform(162, 120, 270));
                break;
            case 3:
                this->bridgeAtlas->Get(GfxObjects::bridge)->SetTransform(Transform(120, 162, 0));
                break;
            
            default:
                break;
            }
            rt->AddCopy(bridgeAtlas->Get(GfxObjects::bridge)->Copy());
        }
    }

    // render buildings
    if (isScreenHome(screen)) {
        this->Scene[GfxObjects::castle]->SetRotation(screenAngles[screen]);
        rt->Add(this->Scene[GfxObjects::castle]);
    }
    
    rt->End();
    this->Scene.CreateObjectWithID(GfxObjects::groundScreen + mapInstance->getScreenNum(this->Module, screen), rt);
    */
}

/*
void TowerDefence::drawRectangle(int direction) {
    switch (direction) {
    case 0: // LEFT
        Cubios::GFX_drawRectangle(0, 100, 120, 40, Cubios::Gfx::Colors::white);
        break;
    case 1: // TOP
        Cubios::GFX_drawRectangle(100, 0, 40, 120, Cubios::Gfx::Colors::white);
        break;
    case 2: // RIGHT
        Cubios::GFX_drawRectangle(120, 100, 120, 40, Cubios::Gfx::Colors::white);
        break;
    case 3: // BOTTOM
        Cubios::GFX_drawRectangle(100, 120, 40, 120, Cubios::Gfx::Colors::white);
        break;
    default:
        break;
    }
}
*/

//The "physics" callback. Gets called recurrently with at least 33ms resolution or less depending on the load. 
void TowerDefence::on_PhysicsTick(const std::array<Cubios::Screen, 3>& screens)
{
}

//This callback is called on programmable timer (if any)
void TowerDefence::on_Timer(uint8_t timerID)
{
    switch(timerID)
    {
        case Timers::startupTimer:
        {
            this->StopTimer(Timers::startupTimer);
            // start mapping path
            if(mapInstance->isHillModule) {
                searchNextPath(mapInstance->hillModule, mapInstance->hillScreen, -1, false);
            }

            // log modulePathMap
            for (uint8_t screen = 0; screen < SCREENS_MAX; ++screen) {
                for (uint8_t direction = 0; direction < DIRECTIONS_MAX; ++direction) {
                    int pathNum = mapInstance->getPathNum(screen, direction);
                    LOG_i("PathMap %d = %d", mapInstance->modulePathMap[pathNum]->pathNum, mapInstance->modulePathMap[pathNum]->isActive);
                }
            }
        }
        break;
        case Timers::antSpawnTimer:
        {
            // ant Wave logic
            if (waveEnemyCounter < waveLength) {
                spawnAnt(0, 0, mapInstance->getHillScreen(), enemyHealth);
                waveEnemyCounter++;
            }
            else {
                this->KillTimer(Timers::antSpawnTimer);
                waveEnemyCounter = 0;
            }
        }
        break;
        default:
        break;
    }
}

//The cube topology change callback. Gets called when cube is twisted and its topological desctiption has been changed
void TowerDefence::on_Twist(const Cubios::TOPOLOGY_twistInfo_t& twist) 
{
    this->gameState = GameState::gameShown;
}

//The "inner network" callback. Gets called when WOWCube module receives a data packet from other module
void TowerDefence::on_Message(uint32_t type, uint8_t* pkt, u32_t size) 
{
    LOG_i("Receiving msg: %d", type);
    NetworkMessage msg(pkt,size);
    int intendedModule, intendedScreen, fromModule, fromScreen, health;
    int modScreenNum, pathCount;
    int direction;
    switch (type)
    {
    case Commands::cmd_generateNextPath:
    /*
        msg.WriteInt(nextScreen.module, 32);
        msg.WriteInt(nextScreen.screen, 32);
        msg.WriteInt(curModule, 32);
        msg.WriteInt(curScreen, 32);
    */
        intendedModule = (int)msg.ReadInt(32);
        LOG_i("Received msg for module: %d", intendedModule);
        if (this->Module == intendedModule)
        {
            intendedScreen = (int)msg.ReadInt(32);
            fromModule = (int)msg.ReadInt(32);
            fromScreen = (int)msg.ReadInt(32);
            nextPathReceived(fromModule, fromScreen, intendedScreen);
        }
        break;
    case Commands::cmd_advertiseScreenPaths:
        modScreenNum = (int)msg.ReadInt(32);
        pathCount = (int)msg.ReadInt(32);
        mapInstance->screenPathCountMap[modScreenNum] = pathCount;
        break;
    case Commands::cmd_spwanAnt:
        intendedModule = (int)msg.ReadInt(16);
        if (this->Module == intendedModule) {
            intendedScreen = (int)msg.ReadInt(16);
            fromModule = (int)msg.ReadInt(16);
            fromScreen = (int)msg.ReadInt(16);
            health = (int)msg.ReadInt(16);
            spawnAnt(fromModule, fromScreen, intendedScreen, health);
        }
        break;
    //case Commands::cmd_startAntWave:
    //    if(this->Module == mapInstance->getHillModule()) {
    //        this->SetTimer(Timers::antSpawnTimer, 2000);
    //    }
    //    break;
    case Commands::cmd_toggleBuildMenu:
        buildState = msg.ReadInt(16);
        money = msg.ReadInt(16);
        break;
    case Commands::cmd_enemyKilledNotice:
        waveEnemyDeadCounter++;
        money++;
        if(this->Module == mapInstance->getHillModule() && waveEnemyDeadCounter == waveLength) {
            // wave end
            isWaveActive = false;
            waveEnemyDeadCounter = 0;
            money += 25 * currentWave;
            currentWave++;
            Cubios::NetworkMessage msg;
            msg.Reset(true);
            msg.WriteBool(isWaveActive);
            msg.WriteInt(currentWave, 16);
            msg.WriteInt(money, 16);
            this->SendNetworkMessage(Commands::cmd_waveStatus, &msg);
        }
        break;
    case Commands::cmd_gameState:
        this->gameState = msg.ReadInt(16);
        break;
    case Commands::cmd_waveStatus:
        this->isWaveActive = msg.ReadBool();
        this->currentWave = msg.ReadInt(16);
        this->money = msg.ReadInt(16);
        if (!isWaveActive) waveEnemyDeadCounter = 0;
        waveLength = 15 + (5 * currentWave);
        enemySpeed = 3 + (currentWave / 5);
        enemyHealth = 2 + (currentWave / 3);
        break;
    case Commands::cmd_tracePath:
        intendedModule = (int)msg.ReadInt(16);
        if (this->Module == intendedModule) {
            intendedScreen = (int)msg.ReadInt(16);
            direction = (int)msg.ReadInt(16);
            tracePath(intendedModule, intendedScreen, direction);
        }
        else if (intendedModule == MODULES_MAX) {
            // path trace complete
            for (int screen = 0; screen < SCREENS_MAX; screen ++) {
                for (int direction = 0; direction < DIRECTIONS_MAX; direction++) {
                    if (!mapInstance->modulePathMap[mapInstance->getPathNum(screen, direction)]->isOnPath) {
                        mapInstance->modulePathMap[mapInstance->getPathNum(screen, direction)]->isActive = false;
                    }
                }
            }
        }
        else if(intendedModule == MODULES_MAX + 1) {
            for (int screen = 0; screen < SCREENS_MAX; screen ++) {
                for (int direction = 0; direction < DIRECTIONS_MAX; direction++) {
                    mapInstance->modulePathMap[mapInstance->getPathNum(screen, direction)]->isOnPath = false;
                }
            }
        }
        // I don't remember what I was planning to do here, lol
        //pathCount = (int)msg.ReadInt(16);
        //for(int i = 0; i < pathCount; i++) {
        //    mapInstance->pathTraceMap.push_back((int)msg.ReadInt(16));
        //}
        break;
    
    default:
        LOG_i("WARNING: Received unknown command: %d", type);
        break;
    }
}

//The external data transfer callback. Gets called when WOWCube module receives a data over BLE from an external source
void TowerDefence::on_ExternalMessage(uint8_t* pkt, u32_t size) 
{
}

//Screen pat callback
void TowerDefence::on_Pat(uint32_t count)
{
    LOG_i("Tap registered %d", count);
    Cubios::TOPOLOGY_place_t homePlace;
    Cubios::TOPOLOGY_getPlace(mapInstance->getHomeModule(), mapInstance->getHomeScreen(), Cubios::TOPOLOGY_orientation_mode_t::ORIENTATION_MODE_MENU, &homePlace);
    for (int screen = 0; screen < SCREENS_MAX; screen ++) {
        if (Cubios::TOPOLOGY_getFaceletOrientation(this->Module, screen) == Cubios::TOPOLOGY_orientation_t::ORIENTATION_UP && Cubios::MS_getFaceAccelX(screen) < 0 && Cubios::MS_getFaceAccelY(screen) < 0) {
            if (isScreenHome(screen)) {
                switch (buildState) {
                case BuildState::bsNone: sendBuildStateMsg(BuildState::bsBuildMenu, money); break;
                default: sendBuildStateMsg(BuildState::bsNone, money); break;
                }
            }
            else if (isScreenHill(screen) && gameState != GameState::gameOver && buildState == BuildState::bsNone && !isWaveActive) {
                //clear unused paths
                for (int screen = 0; screen < SCREENS_MAX; screen ++) {
                    for (int direction = 0; direction < DIRECTIONS_MAX; direction++) {
                        mapInstance->modulePathMap[mapInstance->getPathNum(screen, direction)]->isOnPath = false;
                    }
                }
                Cubios::NetworkMessage msg;
                msg.Reset(true);
                msg.WriteInt(MODULES_MAX + 1, 16);
                this->SendNetworkMessage(Commands::cmd_tracePath, &msg);
                
                // Wave Start
                LOG_i("Selected Hill");
                tracePath(this->Module, screen, -1);

                isWaveActive = true;
                waveLength = 15 + (5 * currentWave);
                enemySpeed = 3 + (currentWave / 5);
                enemyHealth = 2 + (currentWave / 3);
                enemySpawnDelay = 1000 + static_cast<int>(clamp(1050 - currentWave * 50, 0, 1000));
                this->SetTimer(Timers::antSpawnTimer, enemySpawnDelay);

                msg.Reset(true);
                msg.WriteBool(isWaveActive);
                msg.WriteInt(currentWave, 16);
                msg.WriteInt(money, 16);
                this->SendNetworkMessage(Commands::cmd_waveStatus, &msg);
            }
            else if(buildState == BuildState::bsBuildMenu){
                Cubios::TOPOLOGY_place_t currentPlace;
                Cubios::TOPOLOGY_getPlace(this->Module, screen, Cubios::TOPOLOGY_orientation_mode_t::ORIENTATION_MODE_MENU, &currentPlace);
                if (currentPlace.face == homePlace.face && currentPlace.position == (homePlace.position + 1) % 4) {
                    sendBuildStateMsg(BuildState::bsPlaceBowman, money);
                }
            }
            else if (buildState == BuildState::bsPlaceBowman && !isScreenHome(screen) && !isScreenHill(screen)) {
                if (!archerList[screen].isActive && money >= ARCHER_COST) {
                    // buy archer
                    archerList[screen].isActive = true;
                    money -= ARCHER_COST;
                    sendBuildStateMsg(BuildState::bsNone, money);
                }
                else if (archerList[screen].isActive && money >= ARCHER_COST + (archerList[screen].level * ARCHER_COST / 2)) {
                    // upgrade archer
                    archerList[screen].level++; // TODO handle actual leveling
                    archerList[screen].damage = archerList[screen].level;
                    archerList[screen].fireRate = 10 + static_cast<int>(clamp(11 - archerList[screen].level, 0, 10));
                    enemySpawnDelay = 1000 + static_cast<int>(clamp(1050 - currentWave * 50, 0, 1000));
                    money -= ARCHER_COST + (archerList[screen].level * ARCHER_COST / 2);
                    sendBuildStateMsg(BuildState::bsNone, money);
                }
            }
        }
    }
}

void TowerDefence::sendBuildStateMsg(int newBuildState, int argMoney) {
    Cubios::NetworkMessage msg;
    msg.Reset(true);
    buildState = newBuildState;
    msg.WriteInt(newBuildState, 16);
    msg.WriteInt(argMoney, 16);
    this->SendNetworkMessage(Commands::cmd_toggleBuildMenu, &msg);
}

//This callback is called before the application quits
void TowerDefence::on_Close() 
{
}

void sendNetworkMessage(NetworkMessage msg) {
    //this->SendNetworkMessage(msg);
}

void TowerDefence::searchNextPath(int curModule, int curScreen, int fromDirection, bool isAntGen) {
    // find valid paths
    int emergencyLoopDirection;
    vector<int> validDirections;
    for (uint8_t direction = 0; direction < DIRECTIONS_MAX; ++direction) {
        Cubios::TOPOLOGY_faceletInfo_t adjacentScreen;
        Cubios::TOPOLOGY_getAdjacentFacelet(curModule, curScreen, UsableDirections[direction], &adjacentScreen);
        LOG_i("Current place = mod%d, scr%d. Checking direction %d", curModule, curScreen, UsableDirections[direction]);
        if ((adjacentScreen.module == mapInstance->getHillModule() && adjacentScreen.screen == mapInstance->getHillScreen()) 
             && !mapInstance->modulePathMap[mapInstance->getPathNum(curScreen, direction)]->isActive) {
            emergencyLoopDirection = direction;
            LOG_i("Warry of moving towars Hill: %d, %d", adjacentScreen.module, adjacentScreen.screen);
        }
        else if (!mapInstance->modulePathMap[mapInstance->getPathNum(curScreen, direction)]->isActive) {
            validDirections.push_back(direction);
            if (mapInstance->screenPathCountMap[mapInstance->getPathNum(adjacentScreen.module, adjacentScreen.screen)] == 0) {
                validDirections.push_back(direction);
            }
            
        }
    }

    // choose random from valid paths
    int randDirectionNum;
    switch (validDirections.size()) {
    case 0: randDirectionNum = emergencyLoopDirection; LOG_i("Using emergency direction: %d; directions size = %d", emergencyLoopDirection, validDirections.size()); break;
    case 1: randDirectionNum = validDirections[0]; break;
    default: randDirectionNum = validDirections[Cubios::random(0, validDirections.size() - 1)]; break;
    }

    /*
    //get rand num
    int randDirectionNum = Cubios::random(0, 3);
    //check rand direction for valid path
    for (uint8_t direction = 0; direction < DIRECTIONS_MAX; ++direction) {
        if (mapInstance->modulePathMap[mapInstance->getPathNum(curScreen, randDirectionNum)]->isActive) {
            randDirectionNum = (randDirectionNum + 1) % DIRECTIONS_MAX;
        }
        // TODO? add error checking if all 4 paths fail
    }
    //TODO assuming valid now, add more checking later
    */

    //add random path to map
    int curPathNum = mapInstance->getPathNum(curScreen, randDirectionNum);
    int fromPathNum = mapInstance->getPathNum(curScreen, fromDirection);
    mapInstance->modulePathMap[curPathNum]->isActive = true;
    mapInstance->modulePathMap[curPathNum]->isExit = true;
    mapInstance->modulePathMap[curPathNum]->connectedPathNum = fromPathNum;
    if (fromPathNum >= 0) {
        mapInstance->modulePathMap[fromPathNum]->connectedPathNum = curPathNum;
    }
    int screenNum = mapInstance->getScreenNum(curModule, curScreen);
    mapInstance->screenPathCountMap[screenNum] = mapInstance->screenPathCountMap[screenNum] +1;

    //if not at home, searchNextPath / send to next module??
    int randDirection = UsableDirections[randDirectionNum];
    LOG_i("Sending in direction: %d", randDirection);
    Cubios::TOPOLOGY_faceletInfo_t nextScreen;
    Cubios::TOPOLOGY_getAdjacentFacelet(curModule, curScreen, randDirection, &nextScreen);

    LOG_i("Going to module: %d", nextScreen.module);

    // send advertiseScreenPaths
    Cubios::NetworkMessage msg;
    msg.Reset(true);

    msg.WriteInt(screenNum, 32);
    msg.WriteInt(mapInstance->screenPathCountMap[screenNum], 32);
    this->SendNetworkMessage(Commands::cmd_advertiseScreenPaths, &msg);

    if (!isAntGen) {
        // send next path command
        if(nextScreen.module == curModule) {
            // receive path
            nextPathReceived(curModule, curScreen, nextScreen.screen);
        }
        else {
            msg.Reset(true);
            msg.WriteInt(nextScreen.module, 32);
            msg.WriteInt(nextScreen.screen, 32);
            msg.WriteInt(curModule, 32);
            msg.WriteInt(curScreen, 32);
            LOG_i("Printing msg log??");
            msg.Print();
            this->SendNetworkMessage(Commands::cmd_generateNextPath, &msg);
        }
    }

    //renderBridgeLayer(curScreen);
}

void TowerDefence::antForgePath(int curScreen, int direction) {
    // add new path
    int modScreenNum = mapInstance->getScreenNum(this->Module, curScreen);
    mapInstance->modulePathMap[mapInstance->getPathNum(curScreen, direction)]->isActive = true;
    mapInstance->screenPathCountMap[modScreenNum] = mapInstance->screenPathCountMap[modScreenNum] +1;

    // send advertiseScreenPaths
    Cubios::NetworkMessage msg;
    msg.Reset(true);

    msg.WriteInt(modScreenNum, 32);
    msg.WriteInt(mapInstance->screenPathCountMap[modScreenNum], 32);
    this->SendNetworkMessage(Commands::cmd_advertiseScreenPaths, &msg);

    // get next path
    if (mapInstance->screenPathCountMap[modScreenNum] == 1) {
        // forge another new direction
        searchNextPath(this->Module, curScreen, direction, true);
        //Cubios::TOPOLOGY_place_t* result;
        //Cubios::TOPOLOGY_getPlace(mapInstance->getHillModule(), mapInstance->getHomeScreen(), Cubios::TOPOLOGY_orientation_mode_t::ORIENTATION_MODE_GRAVITY, result);
        //result->position;
    }
    else {
        for(int toDirection = 0; toDirection < DIRECTIONS_MAX; toDirection++) {
            if (mapInstance->modulePathMap[mapInstance->getPathNum(curScreen, toDirection)]->isExit){
                mapInstance->modulePathMap[mapInstance->getPathNum(curScreen, direction)]->connectedPathNum = mapInstance->getPathNum(curScreen, toDirection);
            }
        }
    }

    //renderBridgeLayer(curScreen);
}

void TowerDefence::nextPathReceived(int fromModule, int fromScreen, int curScreen) {
    LOG_i("Next Path Module %d; Scr = %d", this->Module, curScreen);
    int modScreenNum = mapInstance->getScreenNum(this->Module, curScreen);
    int8_t direction;
    for (direction = 0; direction < DIRECTIONS_MAX; direction++) {
        Cubios::TOPOLOGY_faceletInfo_t previousScreen;
        Cubios::TOPOLOGY_getAdjacentFacelet(this->Module, curScreen, UsableDirections[direction], &previousScreen);
        if(previousScreen.module == fromModule && previousScreen.screen == fromScreen) {
            // found right direction
            mapInstance->modulePathMap[mapInstance->getPathNum(curScreen, direction)]->isActive = true;
            mapInstance->screenPathCountMap[modScreenNum] = mapInstance->screenPathCountMap[modScreenNum] +1;
            break;
        }
    }

    if (!isScreenHome(curScreen)) {
        searchNextPath(this->Module, curScreen, direction, false);
    }
    else {
        //Cubios::NetworkMessage msg;
        //msg.Reset(true);

        //msg.WriteInt(mapInstance->getHillModule(), 16); // intended module
        //msg.WriteInt(mapInstance->getHillScreen(), 16); // intended screen
        //msg.WriteInt(0, 16); // from module
        //msg.WriteInt(0, 16); // from screen

        //this->SendNetworkMessage(Commands::cmd_startAntWave, &msg);

        //tracePath(mapInstance->getHillModule(), mapInstance->getHillScreen(), -1);
    }

    //renderBridgeLayer(curScreen);
}

void TowerDefence::spawnAnt(int fromModule, int fromScreen, int curScreen, int argHealth) {
    if(isScreenHill(curScreen)) {

        this->Scene.CreateObjectWithID(antCounter, antAtlas->Get(GfxObjects::goblin)->Copy());
        AntSpriteAtlasElement* antObj = dynamic_cast<AntSpriteAtlasElement*>(this->Scene[antCounter]);
        antObj->screen = curScreen;
        int direction;
        for(direction = 0; direction < DIRECTIONS_MAX; direction++) {
            int pathNum = mapInstance->getPathNum(antObj->screen, direction);
            if(mapInstance->modulePathMap[pathNum]->connectedPathNum < 0 && mapInstance->modulePathMap[pathNum]->isActive) {
                antObj->pathNum = pathNum;
                antObj->isMovingIn = false;
                break;
            }
        }
        antObj->speed = enemySpeed;
        antObj->health = argHealth;
        
        this->Scene[antCounter]->SetTransform(Transform(120, 120, directionOrdinalToDegrees(direction)));
        antList.push_back(antCounter);
        LOG_i("Spawning ant %d", antCounter);
        antCounter++;
    }
    else { // if not enemy spawn point
        //OffscreenRenderTarget* rt = new OffscreenRenderTarget(240,240,Cubios::GFX_PixelFormat_t::FORMAT_RGB565);
        //rt->SetPosition(0,0);

        //rt->Begin(true);

        this->Scene.CreateObjectWithID(antCounter, antAtlas->Get(GfxObjects::goblin)->Copy());
        AntSpriteAtlasElement* antObj = dynamic_cast<AntSpriteAtlasElement*>(this->Scene[antCounter]);
        antObj->screen = curScreen;
        antObj->speed = enemySpeed;
        antObj->health = argHealth;

        int8_t direction;
        for (direction = 0; direction < DIRECTIONS_MAX; direction++) {
            Cubios::TOPOLOGY_faceletInfo_t previousScreen;
            Cubios::TOPOLOGY_getAdjacentFacelet(this->Module, curScreen, UsableDirections[direction], &previousScreen);
            //LOG_i("Tring to get direction for scr%d, dir%d", curScreen, UsableDirections[direction]);
            //LOG_i("Adjacent face = mod%d, scr%d", previousScreen.module, previousScreen.screen);
            //LOG_i("Searching for face = mod%d, scr%d", fromModule, fromScreen);
            if(previousScreen.module == fromModule && previousScreen.screen == fromScreen) {
                // found right direction
                //LOG_i("Spawning ant on path %d, derived from scr%d dir%d", mapInstance->getPathNum(curScreen, direction), curScreen, direction);
                antObj->pathNum = mapInstance->getPathNum(curScreen, direction);
                antObj->isMovingIn = true;
                break;
            }
            else {
                //LOG_i("failed to get direction");
            }
        }
        //rt->Add(this->Scene[antCounter]);
        //rt->End();
        
        Vec2 spawnPoint;
        switch (direction) {
            case 0: spawnPoint = Vec2(0, 120); break;
            case 1: spawnPoint = Vec2(120, 0); break;
            case 2: spawnPoint = Vec2(240, 120); break;
            case 3: spawnPoint = Vec2(120, 240); break;
        }

        this->Scene[antCounter]->SetTransform(Transform(spawnPoint.X, spawnPoint.Y, directionOrdinalToDegrees(direction)));
        antList.push_back(antCounter);
        updateTargetForArcher(curScreen);
        antCounter++;
    }
}

bool TowerDefence::isScreenHill(int screen) {
    return this->Module == mapInstance->getHillModule() && screen == mapInstance->getHillScreen();
}

bool TowerDefence::isScreenHome(int screen) {
    return this->Module == mapInstance->getHomeModule() && screen == mapInstance->getHomeScreen();
}

void TowerDefence::updateTargetForArcher(int screen) {
    if (!archerList[screen].isActive) return;

    for (int ant = 0; ant < antList.size(); ant++) {
        if(dynamic_cast<AntSpriteAtlasElement*>(this->Scene[antList[ant]])->screen == screen) {
            archerList[screen].antTarget = antList[ant];
            LOG_i("Archer %d targeting %d", screen, archerList[screen].antTarget);
            return;
        }
    }
    archerList[screen].antTarget = -1;
    LOG_i("Archer %d targeting %d", screen, archerList[screen].antTarget);
}

void TowerDefence::shootAnt(int antId, int damage) {
    dynamic_cast<AntSpriteAtlasElement*>(this->Scene[antId])->health -= damage;
    if (dynamic_cast<AntSpriteAtlasElement*>(this->Scene[antId])->health > 0) {
        // show enemy damaged
        //dynamic_cast<AntSpriteAtlasElement*>(this->Scene[antId])->Color = Cubios::Gfx::Colors::red;
        dynamic_cast<AntSpriteAtlasElement*>(this->Scene[antId])->onDamaged();
    }
    else {
        antDeath(antId);
    }
}

int TowerDefence::directionOrdinalToDegrees(int direction) {
    LOG_i("Direction = %d", direction);
    switch (direction) {
        case(0): return 0; break;
        case(1): return 90; break;
        case(2): return 180; break;
        case(3): return 270; break;
        default: return -1; break;
    }
}

void TowerDefence::damageBase(int damage) {
    this->baseHealth -= damage;
    if (this->baseHealth <= 0) {
        LOG_i("ENDING GAME: %d", this->baseHealth);
        // TODO game over
        this->gameState = GameState::gameOver;
        Cubios::NetworkMessage msg;
        msg.Reset(true);
        msg.WriteInt(this->gameState, 16);
        this->SendNetworkMessage(Commands::cmd_gameState, &msg);
    }
}

void TowerDefence::tracePath(int startModule, int startScreen, int startDirection) {
    if (isScreenHome(startScreen)) {
        //found end, we're done
        LOG_i("tracePath found home");
        mapInstance->modulePathMap[mapInstance->getPathNum(startScreen, startDirection)]->isOnPath = true;

        for (int screen = 0; screen < SCREENS_MAX; screen ++) {
            for (int direction = 0; direction < DIRECTIONS_MAX; direction++) {
                if (!mapInstance->modulePathMap[mapInstance->getPathNum(screen, direction)]->isOnPath) {
                    mapInstance->modulePathMap[mapInstance->getPathNum(screen, direction)]->isActive = false;
                }
            }
        }

        Cubios::NetworkMessage msg;
        msg.Reset(true);
        msg.WriteInt(MODULES_MAX, 16);
        this->SendNetworkMessage(Commands::cmd_tracePath, &msg);
        return;
    }

    // get connect path from current path
    int startPathNum = mapInstance->getPathNum(startScreen, startDirection);
    int leavingPathNum = -1;
    if(startPathNum < 0) { // this is the first path from the hill
        for (int direction = 0; direction < DIRECTIONS_MAX; direction++) {
            int testPath = mapInstance->getPathNum(startScreen, direction);
            if (mapInstance->modulePathMap[testPath]->connectedPathNum == -1 && mapInstance->modulePathMap[testPath]->isActive) {
                leavingPathNum = testPath;
            }
        }
    }
    else {
        leavingPathNum = mapInstance->modulePathMap[startPathNum]->connectedPathNum;
        mapInstance->modulePathMap[startPathNum]->isOnPath = true;
    }

    if(leavingPathNum < 0) {
        LOG_i("SEVERE, tracePath didn't find connected path: mod%d scr%d dir%d", startModule, startScreen, startDirection);
        return;
    }

    // mark paths as on the path
    mapInstance->modulePathMap[leavingPathNum]->isOnPath = true;
    
    // get next face
    Cubios::TOPOLOGY_faceletInfo_t nextScreen;
    Cubios::TOPOLOGY_getAdjacentFacelet(startModule, startScreen, UsableDirections[leavingPathNum % DIRECTIONS_MAX], &nextScreen);

    // get direction for next screen
    for (int direction = 0; direction < DIRECTIONS_MAX; direction++) {
        Cubios::TOPOLOGY_faceletInfo_t previousScreen;
        Cubios::TOPOLOGY_getAdjacentFacelet(nextScreen.module, nextScreen.screen, UsableDirections[direction], &previousScreen);
        if (previousScreen.module == startModule && previousScreen.screen == startScreen) {
            //LOG_i("tracePath found connected path: mod%d scr%d dir%d", nextScreen.module, nextScreen.screen, direction);
            // found path on next screen
            if(nextScreen.module == startModule) {
                tracePath(nextScreen.module, nextScreen.screen, direction);
            }
            else {
                Cubios::NetworkMessage msg;
                msg.Reset(true);
                msg.WriteInt(nextScreen.module, 16);
                msg.WriteInt(nextScreen.screen, 16);
                msg.WriteInt(direction, 16);
                this->SendNetworkMessage(Commands::cmd_tracePath, &msg);
            }
            return;
        }
    }
    LOG_i("SEVERE, tracePath didn't find connected path: mod%d scr%d dir%d", startModule, startScreen, startDirection);
}