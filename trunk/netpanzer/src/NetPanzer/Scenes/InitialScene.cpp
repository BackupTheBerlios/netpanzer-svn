
#include "InitialScene.hpp"

#include "2D/Layer.hpp"
#include "Classes/ScreenSurface.hpp"
#include "2D/Color.hpp"
#include "Interfaces/StrManager.hpp"
#include "MainMenuScene.hpp"
#include "2D/SceneManager.hpp"

class InitialLayer : public Layer
{
public:
    InitialLayer() : Layer(0) {}
    
    void draw() // const
    {
        screen->fill(Color::black);
        screen->bltStringCenter(_("Loading..."), Color::white);
    }
    
};

InitialScene::InitialScene()
{
    addLayer( new InitialLayer() );
}

InitialScene::~InitialScene()
{
    
}

void InitialScene::logic()
{
    MainMenuScene * mms = new MainMenuScene();
    SceneManager::setNextScene(mms);
}