
#include "InitialScene.hpp"

#include "2D/Layer.hpp"
#include "2D/Color.hpp"
#include "2D/SceneManager.hpp"
#include "2D/Palette.hpp"
#include "2D/TextRenderer.hpp"

#include "Classes/ScreenSurface.hpp"
#include "Interfaces/StrManager.hpp"
#include "MainMenuScene.hpp"

//#define TEST_TEXT

class InitialLayer : public Layer
{
    TextRenderer loading_text;

#ifdef TEST_TEXT
    TextRenderer t1, t2, t3;
#endif

public:
    InitialLayer() : Layer(0), loading_text(_("Loading..."))
    {
#ifdef TEST_TEXT
        t1.setText("JljKygL. L我L");
        t2.setText("这是什么东西");
        t3.setText("Êtes-vous sûr de vouloir quitter netPanzer ?");
#endif
    }
    
    void draw() // const
    {
        screen->fill(Color::black);
        loading_text.drawCenter(*screen);

#ifdef TEST_TEXT
        t1.draw(*screen, 100, 100);
        t2.draw(*screen, 100, 120);
        t3.draw(*screen, 100, 140);
#endif
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
#ifndef TEST_TEXT
    MainMenuScene * mms = new MainMenuScene();
    SceneManager::setNextScene(mms);
#endif
}