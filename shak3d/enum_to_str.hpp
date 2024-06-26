#pragma once
#include <SDL2/SDL.h>
#include <string>

inline std::string enum_to_str(SDL_EventType event)
{
    switch (event)
    {
    case SDL_FIRSTEVENT:
        return "SDL_FIRSTEVENT ";
        break;
    case SDL_QUIT:
        return "SDL_QUIT ";
        break;
    case SDL_APP_TERMINATING:
        return "SDL_APP_TERMINATING";
        break;
    case SDL_APP_LOWMEMORY:
        return "SDL_APP_LOWMEMORY";
        break;
    case SDL_APP_WILLENTERBACKGROUND:
        return "SDL_APP_WILLENTERBACKGROUND";
        break;
    case SDL_APP_DIDENTERBACKGROUND:
        return "SDL_APP_DIDENTERBACKGROUND";
        break;
    case SDL_APP_WILLENTERFOREGROUND:
        return "SDL_APP_WILLENTERFOREGROUND";
        break;
    case SDL_APP_DIDENTERFOREGROUND:
        return "SDL_APP_DIDENTERFOREGROUND";
        break;
    case SDL_LOCALECHANGED:
        return "SDL_LOCALECHANGED";
        break;
    case SDL_DISPLAYEVENT:
        return "SDL_DISPLAYEVENT ";
        break;
    case SDL_WINDOWEVENT:
        return "SDL_WINDOWEVENT ";
        break;
    case SDL_SYSWMEVENT:
        return "SDL_SYSWMEVENT";
        break;
    case SDL_KEYDOWN:
        return "SDL_KEYDOWN ";
        break;
    case SDL_KEYUP:
        return "SDL_KEYUP";
        break;
    case SDL_TEXTEDITING:
        return "SDL_TEXTEDITING";
        break;
    case SDL_TEXTINPUT:
        return "SDL_TEXTINPUT";
        break;
    case SDL_KEYMAPCHANGED:
        return "SDL_KEYMAPCHANGED";
        break;
    case SDL_TEXTEDITING_EXT:
        return "SDL_TEXTEDITING_EXT";
        break;
    case SDL_MOUSEMOTION:
        return "SDL_MOUSEMOTION ";
        break;
    case SDL_MOUSEBUTTONDOWN:
        return "SDL_MOUSEBUTTONDOWN";
        break;
    case SDL_MOUSEBUTTONUP:
        return "SDL_MOUSEBUTTONUP";
        break;
    case SDL_MOUSEWHEEL:
        return "SDL_MOUSEWHEEL";
        break;
    case SDL_JOYAXISMOTION:
        return "SDL_JOYAXISMOTION ";
        break;
    case SDL_JOYBALLMOTION:
        return "SDL_JOYBALLMOTION";
        break;
    case SDL_JOYHATMOTION:
        return "SDL_JOYHATMOTION";
        break;
    case SDL_JOYBUTTONDOWN:
        return "SDL_JOYBUTTONDOWN";
        break;
    case SDL_JOYBUTTONUP:
        return "SDL_JOYBUTTONUP";
        break;
    case SDL_JOYDEVICEADDED:
        return "SDL_JOYDEVICEADDED";
        break;
    case SDL_JOYDEVICEREMOVED:
        return "SDL_JOYDEVICEREMOVED";
        break;
    case SDL_JOYBATTERYUPDATED:
        return "SDL_JOYBATTERYUPDATED";
        break;
    case SDL_CONTROLLERAXISMOTION:
        return "SDL_CONTROLLERAXISMOTION ";
        break;
    case SDL_CONTROLLERBUTTONDOWN:
        return "SDL_CONTROLLERBUTTONDOWN";
        break;
    case SDL_CONTROLLERBUTTONUP:
        return "SDL_CONTROLLERBUTTONUP";
        break;
    case SDL_CONTROLLERDEVICEADDED:
        return "SDL_CONTROLLERDEVICEADDED";
        break;
    case SDL_CONTROLLERDEVICEREMOVED:
        return "SDL_CONTROLLERDEVICEREMOVED";
        break;
    case SDL_CONTROLLERDEVICEREMAPPED:
        return "SDL_CONTROLLERDEVICEREMAPPED";
        break;
    case SDL_CONTROLLERTOUCHPADDOWN:
        return "SDL_CONTROLLERTOUCHPADDOWN";
        break;
    case SDL_CONTROLLERTOUCHPADMOTION:
        return "SDL_CONTROLLERTOUCHPADMOTION";
        break;
    case SDL_CONTROLLERTOUCHPADUP:
        return "SDL_CONTROLLERTOUCHPADUP";
        break;
    case SDL_CONTROLLERSENSORUPDATE:
        return "SDL_CONTROLLERSENSORUPDATE";
        break;
    case SDL_FINGERDOWN:
        return "SDL_FINGERDOWN ";
        break;
    case SDL_FINGERUP:
        return "SDL_FINGERUP";
        break;
    case SDL_FINGERMOTION:
        return "SDL_FINGERMOTION";
        break;
    case SDL_DOLLARGESTURE:
        return "SDL_DOLLARGESTURE ";
        break;
    case SDL_DOLLARRECORD:
        return "SDL_DOLLARRECORD";
        break;
    case SDL_MULTIGESTURE:
        return "SDL_MULTIGESTURE";
        break;
    case SDL_CLIPBOARDUPDATE:
        return "SDL_CLIPBOARDUPDATE ";
        break;
    case SDL_DROPFILE:
        return "SDL_DROPFILE ";
        break;
    case SDL_DROPTEXT:
        return "SDL_DROPTEXT";
        break;
    case SDL_DROPBEGIN:
        return "SDL_DROPBEGIN";
        break;
    case SDL_DROPCOMPLETE:
        return "SDL_DROPCOMPLETE";
        break;
    case SDL_AUDIODEVICEADDED:
        return "SDL_AUDIODEVICEADDED ";
        break;
    case SDL_AUDIODEVICEREMOVED:
        return "SDL_AUDIODEVICEREMOVED";
        break;
    case SDL_SENSORUPDATE:
        return "SDL_SENSORUPDATE ";
        break;
    case SDL_RENDER_TARGETS_RESET:
        return "SDL_RENDER_TARGETS_RESET ";
        break;
    case SDL_RENDER_DEVICE_RESET:
        return "SDL_RENDER_DEVICE_RESET";
        break;
    case SDL_POLLSENTINEL:
        return "SDL_POLLSENTINEL ";
        break;
    case SDL_USEREVENT:
        return "SDL_USEREVENT ";
        break;
    default:
        return "";
        break;
    }
}