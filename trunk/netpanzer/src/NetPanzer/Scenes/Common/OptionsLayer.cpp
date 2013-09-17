
#include "OptionsLayer.hpp"

#include "ModalMessageLayer.hpp"

#include "Classes/ScreenSurface.hpp"

#include "2D/Components/AreaComponent.hpp"
#include "2D/Components/Separator.hpp"
#include "2D/Components/CheckBox.hpp"
#include "2D/Components/Choice.hpp"
#include "2D/Components/Slider.hpp"
#include "2D/Components/Label.hpp"
#include "Views/Components/ViewGlobals.hpp"

#include "Interfaces/StrManager.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/GameManager.hpp"

#include "System/Sound.hpp"
#include "System/SDLSound.hpp"
#include "System/DummySound.hpp"

#include "Util/Log.hpp"

#include "2D/Color.hpp"
#include "2D/TextRenderingSystem.hpp"

#define MENU_WIDTH  (640)
#define MENU_HEIGHT (480-26-26)

struct Events
{
    enum
    {
        ChangeFullScreen = 1,
        ChangeShadows,
        ChangeBlendSmoke,
        ChangeScrollRate,
        ChangeSound,
        ChangeSoundVolume,
        ChangeMusic,
        ChangeMusicVolume,
        
        
        
        CloseMessageLayer,
        UseDirectX
    };
};

class ScreenResolutionChoice : public Choice
{
public:
    ScreenResolutionChoice(int w) : Choice()
    {
        addItem(_("current"));

        SDL_Rect** modes = SDL_ListModes(0, SDL_FULLSCREEN);
        int cur_mode = 0;
        char res_str[20];
        if ( modes && modes != (SDL_Rect**)-1 )
        {
            while ( modes[cur_mode] )
            {
                if ((modes[cur_mode]->w > 799) && (modes[cur_mode]->h > 479) ) 
                {
                    snprintf(res_str,sizeof(res_str),"%dx%d", modes[cur_mode]->w, modes[cur_mode]->h);
                    res_str[sizeof(res_str)-1] = 0;
                    addItem(res_str);
                }
                ++cur_mode;
            }
        }

        setMinWidth(w);
        
        select(0);
    }
    
    void onSelectionChanged()
    {
        int sel_index = getSelectedIndex()-1;
        if ( sel_index < 0 )
        {
            return;
        }

        SDL_Rect** modes = SDL_ListModes(0, SDL_FULLSCREEN);
        SDL_Rect* mode = 0;
        if ( modes && modes != (SDL_Rect**)-1 )
        {
            mode = modes[sel_index];
        }

        if ( mode )
        {
            gameconfig->video.setWidth(mode->w);
            gameconfig->video.setHeight(mode->h);
        }

        if ( sel_index == 0 && ! gameconfig->video.useFullScreen() )
        {
            // on Mac crash if we are in window and we select the biggest
            // resolution (the first one in theory), we make it smaller so it
            // wont crash, it is a SDL error.
            gameconfig->video.setHeight(gameconfig->video.getHeight() - 50);
        }

        GameManager::setVideoMode();
    }
};

class MiniMapUnitSizeChoice : public Choice
{
public:
    MiniMapUnitSizeChoice(int w) : Choice()
    {
        addItem(_("Small"));
        addItem(_("Large"));
        setMinWidth(w);
        
        if ( gameconfig->gameinterface.minimap.useBigUnitSize() )
        {
            select(1);
        }
        else
        {
            select(0);
        }
    }
    
    void onSelectionChanged()
    {
        gameconfig->gameinterface.minimap.setBigUnitSize(getSelectedIndex() == 1);
    }
};


OptionsLayer::OptionsLayer() : ComponentLayer(0)
{
    char number[50];
    message_layer = 0;

    area = new AreaComponent( MENU_WIDTH, MENU_HEIGHT );
    
    separator_visual       = new Separator( _("VISUAL"),     area->getWidth() - 20, componentActiveTextColor);
    label_screenres        = new Label( UString(_("Resolution")),         Color::white);
    label_minimapunitsize  = new Label( UString(_("Mini Map Unit Size")), Color::white);
    // @todo redo the checks
    checkbox_fullscreen    = new CheckBox( _("Full screen"),      gameconfig->video.useFullScreen(), Events::ChangeFullScreen);
    checkbox_showshadows   = new CheckBox( _("Draw All Shadows"), gameconfig->video.useShadows()   , Events::ChangeShadows);
    checkbox_blendsmoke    = new CheckBox( _("Blend Smoke"),      gameconfig->video.useBlendSmoke(), Events::ChangeBlendSmoke);
    choice_screenres       = new ScreenResolutionChoice( 200 );
    choice_minimapunitsize = new MiniMapUnitSizeChoice ( 200 );
    
    addComponent(area);
    addComponent(separator_visual);
    addComponent(label_screenres);
    addComponent(label_minimapunitsize);
    addComponent(checkbox_fullscreen);
    addComponent(checkbox_showshadows);
    addComponent(checkbox_blendsmoke);
    addComponent(choice_minimapunitsize);
    addComponent(choice_screenres);
    
    
    separator_interface = new Separator( _("INTERFACE"), area->getWidth() - 20, componentActiveTextColor);
    label_scrollrate    = new Label( UString(_("Scroll Rate:")), Color::white );
    slider_scrollrate   = new Slider( 500, 1000, gameconfig->gameinterface.getScrollRate(), 200, Events::ChangeScrollRate);
    
    snprintf(number, sizeof(number), "%d", gameconfig->gameinterface.getScrollRate());
    label_scrollrate_value = new Label( UString(number), Color::white);
    
    addComponent(separator_interface);
    addComponent(label_scrollrate);
    addComponent(slider_scrollrate);
    addComponent(label_scrollrate_value);
    
    
    separator_sound    = new Separator( _("SOUND"),      area->getWidth() - 20, componentActiveTextColor);
    label_sound        = new Label( UString(_("Sound")), Color::white);
    checkbox_sound     = new CheckBox( gameconfig->sound.useSound() ? _("On") : _("Off"), gameconfig->sound.useSound(), Events::ChangeSound);
    slider_soundvolume = new Slider( 0, 100, gameconfig->sound.getEffectsVol(), 200, Events::ChangeSoundVolume);
    label_music        = new Label( UString(_("Music")), Color::white);
    checkbox_music     = new CheckBox( gameconfig->sound.useMusic() ? _("On") : _("Off"), gameconfig->sound.useMusic(), Events::ChangeMusic);
    slider_musicvolume = new Slider( 0, 100, gameconfig->sound.getMusicVol(), 200, Events::ChangeMusicVolume);
    
    snprintf(number, sizeof(number), "%d", gameconfig->sound.getEffectsVol());
    label_soundvolume_value = new Label( UString(number), Color::white);
    snprintf(number, sizeof(number), "%d", gameconfig->sound.getMusicVol());
    label_musicvolume_value = new Label( UString(number), Color::white);
    
    addComponent(separator_sound);
    addComponent(label_sound);
    addComponent(checkbox_sound);
    addComponent(slider_soundvolume);
    addComponent(label_soundvolume_value);
    addComponent(label_music);
    addComponent(checkbox_music);
    addComponent(slider_musicvolume);
    addComponent(label_musicvolume_value);

#ifdef _WIN32
    // @todo redo the check
    checkbox_usedirectx = new CheckBox( _("Use DirectX"), gameconfig->video.useDirectX(), Events::UseDirectX);
    addComponent(checkbox_usedirectx);
#endif
    
}

OptionsLayer::~OptionsLayer()
{
    if ( message_layer )
    {
        delete message_layer;
        message_layer = 0;
    }
}

void OptionsLayer::recalculateComponentLocations()
{
    int sw = screen->getWidth();
    int sh = screen->getHeight();

    area->setLocation((sw/2) - (area->getWidth()/2), ((sh/2) - (area->getHeight()/2)) );

    int x = area->getLocationX() + 10;
    int y = area->getLocationY();

    // -- VISUAL --
    
    separator_visual->setLocation(x, y);
    y += TextRenderingSystem::line_height();
    
    label_screenres->setLocation(x, y);
    choice_screenres->setLocation( x + 150, y);
    checkbox_fullscreen->setLocation(x + 375, y);
    
    y += TextRenderingSystem::line_height() + 4;
    
    label_minimapunitsize->setLocation(x, y);
    choice_minimapunitsize->setLocation(x + 150, y  );
    
    checkbox_showshadows->setLocation(x + 375, y);
    
    y += TextRenderingSystem::line_height() + 4 ;
    
    checkbox_blendsmoke->setLocation(x + 375, y);
    
#ifdef _WIN32
    y += TextRenderingSystem::line_height() + 4 ;
    checkbox_usedirectx->setLocation(x+375, y);
#endif
    
    // -- INTERFACE --
    
    y += 40;
    
    separator_interface->setLocation(x, y);
    y += TextRenderingSystem::line_height();
    
    label_scrollrate->setLocation(x, y);
    slider_scrollrate->setLocation(x + 150, y);
    label_scrollrate_value->setLocation(x + 375, y);
    
    // -- SOUND --
    
    y += 40;

    separator_sound->setLocation(x, y);
    y += TextRenderingSystem::line_height();
    
    label_sound->setLocation(x, y);
    checkbox_sound->setLocation(x + 80, y);
    slider_soundvolume->setLocation(x + 150, y);
    label_soundvolume_value->setLocation(x + 375, y);
    
    y += TextRenderingSystem::line_height() + 4;
    
    label_music->setLocation(x, y);
    checkbox_music->setLocation(x + 80, y);
    slider_musicvolume->setLocation(x + 150, y);
    label_musicvolume_value->setLocation(x + 375, y);
    
    
}

void OptionsLayer::handleComponentEvents()
{
    int event;
    while ( (event = component_events.nextEvent()) ) switch ( event )
    {
        case Events::ChangeFullScreen:
            gameconfig->video.setFullScreen(checkbox_fullscreen->getState());
            GameManager::setVideoMode();
            break;
            
        case Events::ChangeShadows:
            gameconfig->video.setShadows(checkbox_showshadows->getState());
            break;
            
        case Events::ChangeBlendSmoke:
            gameconfig->video.setBlendSmoke(checkbox_blendsmoke->getState());
            break;
            
        case Events::ChangeScrollRate:
        {
            char number[50];
            const int sr = slider_scrollrate->getValue();
            gameconfig->gameinterface.setScrollRate(sr);
            snprintf(number, sizeof(number), "%d", sr);
            label_scrollrate_value->setText(number);
        }
            break;
            
        case Events::ChangeSound:
            delete sound;

            gameconfig->sound.setSound(checkbox_sound->getState());
            if ( gameconfig->sound.useSound() )
            {
                sound = new SDLSound();
                checkbox_sound->setLabel(_("On"));

                if ( GameControlRulesDaemon::getGameState() )
                {
                    sound->playTankIdle();
                }

                if ( gameconfig->sound.useMusic() )
                {
                    sound->playMusic("sound/music/");
                }
            }
            else
            {
                sound = new DummySound();
                checkbox_sound->setLabel(_("Off"));
            }
            break;
            
        case Events::ChangeSoundVolume:
        {
            char number[50];
            const int sv = slider_soundvolume->getValue();
            gameconfig->sound.setEffectsVol(sv);
            sound->setSoundVolume(sv);
            snprintf(number, sizeof(number), "%d", sv);
            label_soundvolume_value->setText(number);
        }
            break;
            
        case Events::ChangeMusic:
            gameconfig->sound.setMusic(checkbox_music->getState());
            if ( gameconfig->sound.useMusic() )
            {
                sound->playMusic("sound/music/");
                checkbox_music->setLabel(_("On"));
            }
            else
            {
                sound->stopMusic();
                checkbox_music->setLabel(_("Off"));
            }
            break;
            
        case Events::ChangeMusicVolume:
        {
            char number[50];
            const int mv = slider_musicvolume->getValue();
            gameconfig->sound.setMusicVol(mv);
            sound->setMusicVolume(mv);
            snprintf(number, sizeof(number), "%d", mv);
            label_musicvolume_value->setText(number);
        }
            break;
            
        case Events::CloseMessageLayer:
            setSubLayer(0);
            break;
            
        case Events::UseDirectX:
        {
            gameconfig->video.setDirectX(checkbox_usedirectx->getState());
            if ( ! message_layer )
            {
                message_layer = new ModalMessageLayer(_("You need to restart NetPanzer for this change to work"),
                                                      &component_events, Events::CloseMessageLayer);
            }
            
            setSubLayer(message_layer);
        }
    }
}