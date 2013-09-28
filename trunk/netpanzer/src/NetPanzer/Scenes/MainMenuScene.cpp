
#include "MainMenuScene.hpp"

#include "2D/Layer.hpp"
#include "2D/ComponentLayer.hpp"
#include "2D/Color.hpp"
#include "2D/Palette.hpp"
#include "Classes/ScreenSurface.hpp"

#include "ArrayUtil/PtrArray.hpp"

#include "Interfaces/StrManager.hpp"

#include "Resources/ResourceManager.hpp"

#include "2D/Components/Button.hpp"
#include "2D/Components/Label.hpp"
#include "2D/Components/CheckBox.hpp"
#include "2D/Components/Choice.hpp"
#include "2D/PackedSurface.hpp"
#include "Actions/ActionManager.hpp"
#include "2D/SceneManager.hpp"
#include "2D/TextRenderingSystem.hpp"
#include "Util/Log.hpp"

#include "Scenes/Common/OptionsLayer.hpp"
#include "Scenes/MainMenu/IntroLayer.hpp"
#include "Scenes/MainMenu/HostGameLayer.hpp"
#include "Scenes/MainMenu/JoinGameLayer.hpp"

#include "2D/Components/Slider.hpp"

//#include "util/FileStream.hpp"
//#include "Config/ConfigGetter.hpp"

#ifndef PACKAGE_VERSION
	#define PACKAGE_VERSION "testing"
#endif


static const char npver[] = "NetPanzer " PACKAGE_VERSION;

class BackgroundImageLayer : public Layer
{
private:
    ImageResource image;
    MImageResource tankbody;
    MImageResource tankturret;
    MImageResource tankturretshadow;
    BlendTableResource darkenalot;
    int hw;
    int hh;

    TextRenderer version_render;
    
public:
    BackgroundImageLayer() : Layer(-1), version_render(npver)
    {
        image = ResourceManager::getResource<ImageResource>("main-background");
        tankbody = ResourceManager::getResource<MImageResource>("heavy-tank-2-body");
        tankturret= ResourceManager::getResource<MImageResource>("heavy-tank-2-turret");
        tankturretshadow= ResourceManager::getResource<MImageResource>("heavy-tank-2-turret-shadow");
        darkenalot = ResourceManager::getResource<BlendTableResource>("darkenalot");

        hw = image.getWidth()/2;
        hh = image.getHeight()/2;
    }
    
    void draw() const
    {
        screen->fill(0);
        
        int dx = (screen->getWidth()/2) - hw;
        int dy = (screen->getHeight()/2) - hh;
        
        image.draw(*screen, dx, dy);
        
        tankbody.draw(*screen, 0, 40, 75);
        tankturretshadow.blend(*screen, 0, 40, 75, darkenalot);
        tankturret.draw(*screen, 0, 40, 75);
        
        version_render.draw(*screen, 10, screen->getHeight() - TextRenderingSystem::line_height() - 4, Color::yellow);
    }
};

class DarkGrayPanel : public Component
{
private:
    ImageFilterResource filter;
    
public:
    DarkGrayPanel()
    {
        filter = ResourceManager::getResource<ImageFilterResource>("darkgray");
    }
    
    void draw(Surface& dest) const
    {
        filter.applyRounded(dest, rect, 10);
        dest.RoundRect(rect, 10, Color::gray);
    }
};

static Button* createMainMenuButton(const NPString& label, const unsigned event, const bool inverted = false)
{
    Button * b = new Button();
    ImageResource image = ResourceManager::getResource<ImageResource>("mainmenu-button-background");
    
    Surface bitmap(image.getWidth(), image.getHeight());
//    bitmap.fill(0);
    image.draw(bitmap, 0, 0);
    
    if ( inverted )
    {
        bitmap.flipVertical();
    }
    
    b->setLabel(label);
    b->setSize(b->getLabelWidth()+20, 25);
    b->setTextColors(Color::gray, Color::lightGray, Color::gray96, Color::darkGray);
    b->setClickEvent(event);

    b->setExtraBorder();
    
    b->borders[0] = Color::gray;
    b->borders[1] = Color::gray;
    b->borders[2] = Color::gray;
    
    PIX bcolor[] = { Color::black, Color::gray, Color::darkGray };

    PtrArray<Surface> bg(3);
    int bgsize = b->getLabelWidth()+20;
    
    for ( unsigned int n = 0; n < 3; n++ )
    {
        Surface *s = new Surface( bgsize, 25);
        s->FillRoundRect(s->getRect(), 2, bcolor[n]);
        bitmap.bltTrans(*s, 0, 0);
        bg.push_back(s);
    }
    
    b->setImage(bg);
    return b;
}

#define MENU_WIDTH  (640)
#define MENU_HEIGHT (480)
#define BUTTONS_START_X (8)
#define BUTTONS_START_Y (0)

class MainMenuLayer : public ComponentLayer
{
private:
    DarkGrayPanel * panel_darkgray;
    Button * button_main;
    Button * button_join;
    Button * button_host;
    Button * button_options;
    Button * button_help;
    Button * button_exit;

    IntroLayer    * intro_layer;
    OptionsLayer  * options_layer;
    HostGameLayer * hostgame_layer;
    JoinGameLayer * joingame_layer;

    struct Events {
        enum
        {
            ShowMain = 1,
            ShowJoin,
            ShowHost,
            ShowOptions,
            ShowHelp,
            ExitNetpanzer

        };
    };
    
public:
    MainMenuLayer() : ComponentLayer(0)
    {
        options_layer = 0;
        hostgame_layer = 0;
        joingame_layer = 0;

        panel_darkgray = new DarkGrayPanel();
        panel_darkgray->setSize(MENU_WIDTH, MENU_HEIGHT-26-26);

        button_main =    createMainMenuButton( _("Main"),           Events::ShowMain );
        button_join =    createMainMenuButton( _("Join"),           Events::ShowJoin );
        button_host =    createMainMenuButton( _("Host"),           Events::ShowHost );
        button_options = createMainMenuButton( _("Options"),        Events::ShowOptions );
        button_help =    createMainMenuButton( _("Help"),           Events::ShowHelp );
        button_exit =    createMainMenuButton( _("Exit netPanzer"), Events::ExitNetpanzer );
        
        addComponent(panel_darkgray);

        addComponent(button_main);
        addComponent(button_join);
        addComponent(button_host);
        addComponent(button_options);
        addComponent(button_help);
        addComponent(button_exit);
        
        intro_layer = new IntroLayer();
        setSubLayer(intro_layer);

    }
    
    ~MainMenuLayer()
    {
        delete intro_layer;
        intro_layer = 0;
        
        if ( options_layer )
        {
            delete options_layer;
        }
        
        if ( hostgame_layer )
        {
            delete hostgame_layer;
        }
        
        if ( joingame_layer )
        {
            delete joingame_layer;
        }
    }
    
private:
    void handleComponentEvents()
    {
        int event;
        while ( (event = component_events.nextEvent()) ) switch ( event )
        {
            case Events::ShowMain:
                setSubLayer(intro_layer);
                break;
                
            case Events::ShowJoin:
                if ( joingame_layer == 0 )
                {
                    joingame_layer = new JoinGameLayer();
                }
                
                setSubLayer(joingame_layer);
                break;

            case Events::ShowHost:
                if ( hostgame_layer == 0 )
                {
                    hostgame_layer = new HostGameLayer();
                }

                setSubLayer( hostgame_layer );
                break;

            case Events::ShowOptions:
                if ( options_layer == 0 )
                {
                    options_layer = new OptionsLayer();
                }

                setSubLayer( options_layer );
                
                break;

            case Events::ShowHelp:
                LOGGER.warning("Clicked on Help!!!");
                break;

            case Events::ExitNetpanzer:
                ActionManager::runAction("quit");
                break;
        }
    }

    void recalculateComponentLocations()
    {
        int sw = screen->getWidth();
        int sh = screen->getHeight();
        
        panel_darkgray->setLocation((sw/2) - (panel_darkgray->getWidth()/2), (sh/2) - (panel_darkgray->getHeight()/2));
        
        int bx = panel_darkgray->getLocationX() + BUTTONS_START_X;
        int by = panel_darkgray->getLocationY() + BUTTONS_START_Y - 26;
        
        button_main->setLocation( bx, by);
        bx += button_main->getWidth();
        
        button_join->setLocation( bx, by);
        bx += button_join->getWidth();
        
        button_host->setLocation( bx, by);
        bx += button_host->getWidth();
        
        button_options->setLocation( bx, by);
        bx += button_options->getWidth();
        
        button_help->setLocation( bx, by);
        
        bx = panel_darkgray->getLocationX() + MENU_WIDTH - button_exit->getWidth() - BUTTONS_START_X;
        button_exit->setLocation( bx, by);

        bx = panel_darkgray->getLocationX()+10;

    }
    
};


MainMenuScene::MainMenuScene()
{
    addLayer( new BackgroundImageLayer() );
    
    menu_layer = new MainMenuLayer();
    addLayer( menu_layer );
}

MainMenuScene::~MainMenuScene()
{
    
}

void MainMenuScene::logic()
{
    
}

void MainMenuScene::onBecomeActive()
{
    
}
