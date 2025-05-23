#include "CustomHotkeys.h"

static std::vector<CustomHotkey> customHotkeys =
{
    {"weapon1", SDLKey::SDLK_1, 1, 8},
    {"weapon2", SDLKey::SDLK_2, 1, 9},
    {"weapon3", SDLKey::SDLK_3, 1, 10},
    {"weapon4", SDLKey::SDLK_4, 1, 11},
    {"drone4", SDLKey::SDLK_8, 1, 15},
    {"drone5", SDLKey::SDLK_9, 1, 16},
    {"drone6", SDLKey::SDLK_0, 1, 17},
    {"console", SDLKey::SDLK_BACKSLASH, 0, -1},
    {"speed", SDLKey::SDLK_BACKQUOTE, 0, -1},
    {"info", SDLKey::SDLK_RALT, 0, 11},
    {"temporal", SDLKey::SDLK_SEMICOLON, 2, 11},
    {"un_temporal", SDLKey::SDLK_UNKNOWN, 2, -1},
    {"temporal_speed", SDLKey::SDLK_PERIOD, 1, 8},
    {"temporal_slow", SDLKey::SDLK_COMMA, 1, 9},
    {"aim_artillery1", SDLKey::SDLK_UNKNOWN, 3, 8},
    {"aim_artillery2", SDLKey::SDLK_UNKNOWN, 3, 9},
    {"aim_artillery3", SDLKey::SDLK_UNKNOWN, 3, -1},
    {"aim_artillery4", SDLKey::SDLK_UNKNOWN, 3, -1}
};

HOOK_METHOD(ControlsScreen, OnInit, () -> void)
{
    LOG_HOOK("HOOK_METHOD -> ControlsScreen::OnInit -> Begin (CustomHotkeys.cpp)\n")
    for (int i = 0; i < 4; i++)
    {
        buttons[i].clear();
    }

    super();

    Point origin = Point(480, 130);
    int btnsPerColumn = 10;
    for (int page = 0; page < 3; page++)
    {
        if (page == 0)
        {
            btnsPerColumn = 12;
        }
        else if (page == 1)
        {
            btnsPerColumn = 10;
        }
        else if (page == 2)
        {
            btnsPerColumn = 12;
        }

        for (int i = 0; i < buttons[page].size(); i++)
        {
            auto& button = buttons[page][i];

            button.rect.x = origin.x + ((i >= btnsPerColumn) ? 440 : 0);
            button.rect.y = origin.y + (40 * (i >= btnsPerColumn ? i - btnsPerColumn : i));
        }

    }
}

HOOK_STATIC(Settings, ResetHotkeys, () -> void)
{
    LOG_HOOK("HOOK_STATIC -> Settings::ResetHotkeys -> Begin (CustomHotkeys.cpp)\n")
    super();

    SettingValues* settings = G_->GetSettings();

    for (auto i : customHotkeys)
    {
        HotkeyDesc hk = HotkeyDesc();
        hk.name = i.hotkeyName;
        hk.key = i.defaultValue;

        for (auto it = settings->hotkeys[i.page].begin(); it != settings->hotkeys[i.page].end(); ++it)
        {
            if (it->name == i.hotkeyName)
            {
                hk = *it;
                settings->hotkeys[i.page].erase(it);
                break;
            }
        }

        if (i.index == -1)
        {
            settings->hotkeys[i.page].push_back(hk);
        }
        else
        {
            settings->hotkeys[i.page].insert(settings->hotkeys[i.page].begin() + i.index, hk);
        }
    }
}

// TO BE REMOVED FOR 2.0
// A mistake made a version ago forced this backward compatibility fix in order to not break text_misc.xml data that should have just been updated with the new ID instead
// beware that the prefix "hotkey_artillery" will always break the power artillery hotkey
HOOK_METHOD(TextLibrary, GetText, (const std::string &name, const std::string &lang) -> std::string)
{
    LOG_HOOK("HOOK_METHOD -> TextLibrary::GetText -> Begin (CustomHotkeys.cpp)\n")

    std::string ret = super(name, lang);
    if (ret.find("Could not find:") != std::string::npos && (name == "hotkey_aim_artillery1" || name == "hotkey_aim_artillery2" || name == "hotkey_aim_artillery3" || name == "hotkey_aim_artillery4"))
        ret = super("hotkey_artillery" + name.substr(20), lang);
    return ret;
}
