#include "r/frontend/sdl.h"
#include "r/keys.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace r::frontend {


SDL::SDL(const Engine::Config& config) {
#ifdef _WIN32
    //On Vista or newer windows, let the OS know we are DPI aware, so we won't have odd scaling issues.
    if (void* user_dll = SDL_LoadObject("USER32.DLL")) {
        if (auto SetProcessDPIAware = reinterpret_cast<int(__stdcall *)(void)>(SDL_LoadFunction(user_dll, "SetProcessDPIAware")))
            SetProcessDPIAware();
    }
#endif

    minimal_size = config.minimal_screen_size;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        printf("Failed to initalize SDL2: %s\n", SDL_GetError());
        quit = true;
        return;
    }
    //TODO: Full screen

    SDL_Rect display_mode;
#ifdef __EMSCRIPTEN__
    display_mode.w = EM_ASM_INT({ return window.innerWidth; });
    display_mode.h = EM_ASM_INT({ return window.innerHeight; });
#else
    if (SDL_GetDisplayUsableBounds(0, &display_mode))
    {
        display_mode.w = 640;
        display_mode.h = 480;
    } else {
        display_mode.w *= 0.9;
        display_mode.h *= 0.9;
    }
#endif//__EMSCRIPTEN__

    if (SDL_CreateWindowAndRenderer(display_mode.w, display_mode.h, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        printf("Failed to create SDL2 window or renderer: %s\n", SDL_GetError());
        quit = true;
        return;
    }
    SDL_SetWindowTitle(window, config.title.c_str());
#if SDL_MAJOR_VERSION > 2 || SDL_MINOR_VERSION > 0 || SDL_PATCHLEVEL >= 18
    SDL_RenderSetVSync(renderer, 1);
#endif
    extern unsigned char sdl_font_bmp[];
    extern unsigned int sdl_font_bmp_len;
    auto font_surface = SDL_LoadBMP_RW(SDL_RWFromConstMem(sdl_font_bmp, sdl_font_bmp_len), 1);
    glyph_size.x = font_surface->w / 16;
    glyph_size.y = font_surface->h / 16;
    SDL_SetColorKey(font_surface, 1, 0);
    font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);
    SDL_FreeSurface(font_surface);
}

void SDL::processEvents(GameState& gamestate) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT: quit = true; break;
        case SDL_TEXTINPUT:
            //TODO: UTF-8 decode?
            gamestate.onKey(event.text.text[0]);
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_RETURN: gamestate.onKey(KEY_RETURN); break;
            case SDLK_ESCAPE: gamestate.onKey(KEY_ESCAPE); break;
            case SDLK_BACKSPACE: gamestate.onKey(KEY_BACKSPACE); break;
            case SDLK_DELETE: gamestate.onKey(KEY_DELETE); break;
            case SDLK_TAB: gamestate.onKey(KEY_TAB); break;
            case SDLK_F1: gamestate.onKey(KEY_F1); break;
            case SDLK_F2: gamestate.onKey(KEY_F2); break;
            case SDLK_F3: gamestate.onKey(KEY_F3); break;
            case SDLK_F4: gamestate.onKey(KEY_F4); break;
            case SDLK_F5: gamestate.onKey(KEY_F5); break;
            case SDLK_F6: gamestate.onKey(KEY_F6); break;
            case SDLK_F7: gamestate.onKey(KEY_F7); break;
            case SDLK_F8: gamestate.onKey(KEY_F8); break;
            case SDLK_F9: gamestate.onKey(KEY_F9); break;
            case SDLK_F10: gamestate.onKey(KEY_F10); break;
            case SDLK_F11: gamestate.onKey(KEY_F11); break;
            case SDLK_F12: gamestate.onKey(KEY_F12); break;
            case SDLK_LEFT: gamestate.onKey(KEY_LEFT); break;
            case SDLK_RIGHT: gamestate.onKey(KEY_RIGHT); break;
            case SDLK_UP: gamestate.onKey(KEY_UP); break;
            case SDLK_DOWN: gamestate.onKey(KEY_DOWN); break;

            case SDLK_KP_ENTER: gamestate.onKey(KEY_RETURN); break;
            case SDLK_KP_1: gamestate.onKey(KEY_DOWN_LEFT); break;
            case SDLK_KP_2: gamestate.onKey(KEY_DOWN); break;
            case SDLK_KP_3: gamestate.onKey(KEY_DOWN_RIGHT); break;
            case SDLK_KP_4: gamestate.onKey(KEY_LEFT); break;
            case SDLK_KP_6: gamestate.onKey(KEY_RIGHT); break;
            case SDLK_KP_7: gamestate.onKey(KEY_UP_LEFT); break;
            case SDLK_KP_8: gamestate.onKey(KEY_UP); break;
            case SDLK_KP_9: gamestate.onKey(KEY_UP_RIGHT); break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            gamestate.onMouseDown(mousePositionToTilePosition(event.button.x, event.button.y), event.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            gamestate.onMouseUp(mousePositionToTilePosition(event.button.x, event.button.y), event.button.button);
            break;
        case SDL_MOUSEMOTION:
            for(int n=0; n<5; n++)
                if (event.motion.state & (1 << n))
                    gamestate.onMouseMove(mousePositionToTilePosition(event.motion.x, event.motion.y), n);
            break;
        }
    }
}

void SDL::present() {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    int scale = std::min(w / minimal_size.x, h / minimal_size.y);
    int tw = (w + scale - 1) / scale;
    int th = (h + scale - 1) / scale;
    if (tiles.size() != ivec2{tw, th}) {
        tiles.resize({tw, th});
    }
    int xoffset = (w - tw * scale) / 2;
    int yoffset = (h - th * scale) / 2;

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
    auto size = tiles.size();
    for(int y=0; y<size.y; y++) {
        for(int x=0; x<size.x; x++) {
            auto& t = tiles(x, y);
            SDL_SetTextureColorMod(font_texture, t.forground_color.r * 255, t.forground_color.g * 255, t.forground_color.b * 255);
            SDL_Rect src{(t.c & 0x0F) * glyph_size.x, ((t.c >> 4) & 0x0F) * glyph_size.y, glyph_size.x, glyph_size.y};
            SDL_Rect dst{x * scale + xoffset, y * scale + yoffset, scale, scale};
            SDL_SetRenderDrawColor(renderer, t.background_color.r * 255, t.background_color.g * 255, t.background_color.b * 255, 255);
            SDL_RenderFillRect(renderer, &dst);
            SDL_RenderCopy(renderer, font_texture, &src, &dst);
        }
    }
    SDL_RenderPresent(renderer);
}

bool SDL::isOpen() {
    return !quit;
}

ivec2 SDL::size() {
    return tiles.size();
}

void SDL::draw(ivec2 position, char c, Color forground_color) {
    if (position.x < 0 || position.y < 0) return;
    if (position.x >= tiles.size().x || position.y >= tiles.size().y) return;

    auto& e = tiles(position);
    e.c = c;
    e.forground_color = forground_color;
}

void SDL::draw(ivec2 position, char c, Color forground_color, Color background_color) {
    if (position.x < 0 || position.y < 0) return;
    if (position.x >= tiles.size().x || position.y >= tiles.size().y) return;

    auto& e = tiles(position);
    e.c = c;
    e.forground_color = forground_color;
    e.background_color = background_color;
}

ivec2 SDL::mousePositionToTilePosition(int x, int y)
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    int scale = std::min(w / minimal_size.x, h / minimal_size.y);
    int tw = (w + scale - 1) / scale;
    int th = (h + scale - 1) / scale;
    int xoffset = (w - tw * scale) / 2;
    int yoffset = (h - th * scale) / 2;

    return {std::floor((x - xoffset) / scale), std::floor((y - yoffset) / scale)};
}

}