
#include "MainMenuScene.hpp"

#include "2D/Layer.hpp"
#include "2D/Color.hpp"
#include "2D/Palette.hpp"
#include "Classes/ScreenSurface.hpp"

#include "ArrayUtil/PtrArray.hpp"

#include "Interfaces/StrManager.hpp"

#include "Views/Components/Button.hpp"
#include "Views/Components/Label.hpp"
#include "Views/Components/CheckBox.hpp"
#include "Views/Components/Choice.hpp"
#include "Views/Components/InputField.hpp"
#include "Views/Components/MouseEvent.hpp"
#include "2D/PackedSurface.hpp"
#include "Actions/ActionManager.hpp"
#include "Interfaces/MouseInterface.hpp"

#ifndef PACKAGE_VERSION
	#define PACKAGE_VERSION "testing"
#endif

static const char npver[] = "NetPanzer " PACKAGE_VERSION;

class BackgroundImageLayer : public Layer
{
private:
    Surface image;
    int hw;
    int hh;
    
public:
    BackgroundImageLayer() : Layer(-1)
    {
        image.loadBMP("pics/backgrounds/defaultMB.bmp");
        hw = image.getWidth()/2;
        hh = image.getHeight()/2;
    }
    
    void draw()
    {
        screen->fill(0);
        
        int dx = (screen->getWidth()/2) - hw;
        int dy = (screen->getHeight()/2) - hh;
        image.blt(*screen, dx, dy);
        
        screen->bltString(10, screen->getHeight()-Surface::getFontHeight() - 4, npver, Color::yellow);
        
        char str[1024];
        snprintf(str, sizeof(str), "%dx%d", screen->getWidth(), screen->getHeight());
        
        screen->bltString(200, screen->getHeight()-Surface::getFontHeight() - 4, str, Color::yellow);
        
    }
};

static void config_button(Button& b, const NPString& label, bool inverted)
{
    Surface bitmap;
    bitmap.loadBMP("pics/buttons/page.bmp");
    if ( inverted )
    {
        bitmap.flipVertical();
    }
    
    b.setLabel(label);
    b.setSize(Surface::getTextLength(label)+20, 25);
    b.setTextColors(Color::gray, Color::lightGray, Color::black, Color::darkGray);

    b.setExtraBorder();
    
    b.borders[0] = Color::gray;
    b.borders[1] = Color::gray;
    b.borders[2] = Color::gray;
    
    PIX bcolor[] = { Color::black, Color::gray, Color::darkGray };

    PtrArray<Surface> bg(3);
    int bgsize = Surface::getTextLength(label)+20;
    
    for ( unsigned int n = 0; n < 3; n++ )
    {
        Surface *s = new Surface( bgsize, 25);
        s->FillRoundRect(s->getRect(), 2, bcolor[n]);
        bitmap.bltTrans(*s, 0, 0);
        bg.push_back(s);
    }
    
    b.setImage(bg);
}

#define MENU_WIDTH  (640)
#define MENU_HEIGHT (480)
#define BUTTONS_START_X (8)
#define BUTTONS_START_Y (0)

enum
{
    CA_MAIN = 1,
    CA_JOIN,
    CA_HOST,
    CA_OPTIONS,
    CA_HELP,
    CA_EXIT
};

class MainMenuLayer : public Layer
{
private:
    Button button_main;
    Button button_join;
    Button button_host;
    Button button_options;
    Button button_help;
    Button button_exit;
    iRect  vrect;
    iXY    prev_screen_size;
    
    PtrArray<Component> visible_components;
    
    CheckBox checkbox;
    bool checkbox_data;
    
    Choice choice;
    
    InputField input_field;
    
public:
    MainMenuLayer() : Layer(0), vrect(0, 26, MENU_WIDTH, MENU_HEIGHT-26-26), prev_screen_size(0,0),
            checkbox(0,0,"Checkbox", &checkbox_data),
            input_field(0)
    {
        config_button( button_main,     _("Main"), false );
        config_button( button_join,     _("Join"), false );
        config_button( button_host,     _("Host"), false );
        config_button( button_options,  _("Options"), false );
        config_button( button_help,     _("Help"), false );
        config_button( button_exit,     _("Exit netPanzer"), false );
        
        button_main.setClickAction(CA_MAIN);
        button_join.setClickAction(CA_JOIN);
        button_host.setClickAction(CA_HOST);
        button_options.setClickAction(CA_OPTIONS);
        button_help.setClickAction(CA_HELP);
        button_exit.setClickAction(CA_EXIT);
        
        visible_components.push_back(&button_main);
        visible_components.push_back(&button_join);
        visible_components.push_back(&button_host);
        visible_components.push_back(&button_options);
        visible_components.push_back(&button_help);
        visible_components.push_back(&button_exit);
        
        mouse_over_component = 0;
        mouse_down_component = 0;
        mouse_down_button = 0;
        
        checkbox_data = false;
        
        choice.addItem("item 1");
        choice.addItem("item 2");
        choice.addItem("item 3");
        choice.addItem("item 4");
        choice.select(0);
        
        input_field.setSize(100,20);
        input_field.setMaxTextLength(20);
        input_field.setText("tteesstt");
        
        
        visible_components.push_back(&checkbox);
        visible_components.push_back(&choice);
        visible_components.push_back(&input_field);
        
        mouse_x = 0; //MouseInterface::getMouseX();
        mouse_y = 0; //MouseInterface::getMouseY();
    }
    
    ~MainMenuLayer()
    {
        
    }
    
    void draw() // const
    {
        if ( (prev_screen_size.x != screen->getWidth())
           ||(prev_screen_size.y != screen->getHeight()) )
        {
            recalculatePositions();
        }
        
        screen->BltRoundRect(vrect, 10, Palette::darkGray256.getColorArray());
        screen->RoundRect(vrect, 10, Color::gray);
        
        drawComponents();
    }
    
    void logic()
    {
        
    }
    
    void handleMouseLocation(int x, int y)
    {
        if ( (x == mouse_x) && (y == mouse_y) )
        {
            return; // nothing to do
        }
        
        Component * c = 0;
        for ( size_t n = 0; n < visible_components.size(); n++ )
        {
            if ( visible_components[n]->contains(x, y) )
            {
                c = visible_components[n];
                break;
            }
        }
        
        if ( c != mouse_over_component )
        {
            if ( mouse_over_component )
            {
                mouse_over_component->actionPerformed(mMouseEvent(
                        mouse_over_component,
                        mMouseEvent::MOUSE_EVENT_EXITED, now(), 0, 
                        x, y, 0, false) );
            }
            
            mouse_over_component = c;
            
            if ( mouse_over_component )
            {
                mouse_over_component->actionPerformed(mMouseEvent(
                        mouse_over_component,
                        mMouseEvent::MOUSE_EVENT_ENTERED, now(), 0, 
                        x, y, 0, false) );
                
                if ( mouse_over_component == mouse_down_component )
                {
                    int bmask = 0;
                    switch ( mouse_down_button )
                    {
                        case 1: bmask = mMouseEvent::BUTTON1_MASK; break;
                        case 2: bmask = mMouseEvent::BUTTON2_MASK; break;
                        case 3: bmask = mMouseEvent::BUTTON3_MASK; break;
                    }

                    mouse_down_component->actionPerformed(mMouseEvent(
                            c, mMouseEvent::MOUSE_EVENT_PRESSED, now(), bmask, 
                            x, y, 0, false) );
                }
            }
        }
        else if ( c )
        {
            // same component but moved
            if ( mouse_over_component == mouse_down_component )
            {
                int bmask = 0;
                switch ( mouse_down_button )
                {
                    case 1: bmask = mMouseEvent::BUTTON1_MASK; break;
                    case 2: bmask = mMouseEvent::BUTTON2_MASK; break;
                    case 3: bmask = mMouseEvent::BUTTON3_MASK; break;
                }

                mouse_down_component->actionPerformed(mMouseEvent(
                        c, mMouseEvent::MOUSE_EVENT_DRAGGED, now(), bmask, 
                        x, y, 0, false) );
            }
        }
        
        mouse_x = x;
        mouse_y = y;
    }
    
    void handleMouseDown(int x, int y, int button)
    {
        if ( mouse_down_button )
        {
            return; // already one button down
        }
        
        Component * c = 0;
        for ( size_t n = 0; n < visible_components.size(); n++ )
        {
            if ( visible_components[n]->contains(x, y) )
            {
                mouse_down_component = visible_components[n];
                mouse_down_button = button;
                
                int bmask = 0;
                switch ( button )
                {
                    case 1: bmask = mMouseEvent::BUTTON1_MASK; break;
                    case 2: bmask = mMouseEvent::BUTTON2_MASK; break;
                    case 3: bmask = mMouseEvent::BUTTON3_MASK; break;
                }
                
                mouse_down_component->actionPerformed(mMouseEvent(
                        c, mMouseEvent::MOUSE_EVENT_PRESSED, now(), bmask, 
                        x, y, 0, false) );
                
                break;
            }
        }
    }
    
    void handleMouseUp(int x, int y, int button)
    {
        if ( button != mouse_down_button )
        {
            return; // ignore other buttons
        }
        
        if ( ! mouse_down_button || ! mouse_down_component )
        {
            mouse_down_button = 0;
            mouse_down_component = 0;
            return; // nothing down;
        }
        
        Component * c = 0;
        for ( size_t n = 0; n < visible_components.size(); n++ )
        {
            if ( visible_components[n]->contains(x, y) )
            {
                Component * c = visible_components[n];
                
                if ( c != mouse_down_component )
                {
                    // ignore other component
                    break;
                }
                    
                int bmask = 0;
                switch ( button )
                {
                    case 1: bmask = mMouseEvent::BUTTON1_MASK; break;
                    case 2: bmask = mMouseEvent::BUTTON2_MASK; break;
                    case 3: bmask = mMouseEvent::BUTTON3_MASK; break;
                }
                
                c->actionPerformed(mMouseEvent(
                        c, mMouseEvent::MOUSE_EVENT_CLICKED, now(), bmask, 
                        x, y, 0, false) );
                
                c->actionPerformed(mMouseEvent(
                        c, mMouseEvent::MOUSE_EVENT_RELEASED, now(), bmask, 
                        x, y, 0, false) );
                
                if ( c->getClickAction() )
                {
                    handleClickAction( c->getClickAction() );
                }
                
                break;
            }
        }
        
        mouse_down_button = 0;
        mouse_down_component = 0;
    }
    
private:
    Component * mouse_over_component;
    Component * mouse_down_component;
    int         mouse_down_button;
    int         mouse_x;
    int         mouse_y;
    
    void recalculatePositions()
    {
        int sw = screen->getWidth();
        int sh = screen->getHeight();
        
        vrect.setLocation((sw/2) - (vrect.getWidth()/2), (sh/2) - (vrect.getHeight()/2));
        
        int bx = vrect.getLocationX() + BUTTONS_START_X;
        int by = vrect.getLocationY() + BUTTONS_START_Y - 26;
        
        button_main.setLocation( bx, by);
        bx += button_main.getWidth();
        
        button_join.setLocation( bx, by);
        bx += button_join.getWidth();
        
        button_host.setLocation( bx, by);
        bx += button_host.getWidth();
        
        button_options.setLocation( bx, by);
        bx += button_options.getWidth();
        
        button_help.setLocation( bx, by);
        
        bx = vrect.getLocationX() + MENU_WIDTH - button_exit.getWidth() - 8;
        button_exit.setLocation( bx, by);
        
        checkbox.setLocation(vrect.getLocationX() + 10, vrect.getLocationY()+20);
        choice.setLocation(vrect.getLocationX() + 10, vrect.getLocationY()+40);
        input_field.setLocation(vrect.getLocationX() + 10, vrect.getLocationY()+60);
        
        prev_screen_size.x = screen->getWidth();
        prev_screen_size.y = screen->getHeight();
    }
    
    void drawComponents()
    {
        for ( size_t n = 0; n < visible_components.size(); n++ )
        {
            visible_components[n]->draw(*screen);
        }
    }
    
    void handleClickAction( int action )
    {
        switch ( action )
        {
            case CA_MAIN:
                break;
            
            case CA_JOIN:
                break;
                
            case CA_HOST:
                break;
                
            case CA_OPTIONS:
                break;
                
            case CA_HELP:
                break;
                
            case CA_EXIT:
                ActionManager::runAction("quit");
                break;
        }

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

void MainMenuScene::handleMouseLocation(int x, int y)
{
    menu_layer->handleMouseLocation(x, y);
}

void MainMenuScene::handleMouseDown(int x, int y, int button)
{
    menu_layer->handleMouseDown(x, y, button);
}

void MainMenuScene::handleMouseUp(int x, int y, int button)
{
    menu_layer->handleMouseUp(x, y, button);
    
}