#include "r/frontend/sdl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>


namespace r::frontend {

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *font_texture;
static bool quit = false;

SDL::SDL(const Engine::Config& config) {
#ifdef _WIN32
    //On Vista or newer windows, let the OS know we are DPI aware, so we won't have odd scaling issues.
    if (void* user_dll = SDL_LoadObject("USER32.DLL")) {
        if (auto SetProcessDPIAware = reinterpret_cast<int(__stdcall *)(void)>(SDL_LoadFunction(user_dll, "SetProcessDPIAware")))
            SetProcessDPIAware();
    }
#endif

    minimal_size = config.minimal_screen_size;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    //TODO: Query display size and use that
    //TODO: Full screen
    SDL_CreateWindowAndRenderer(1024, 768, SDL_WINDOW_RESIZABLE, &window, &renderer);
    extern unsigned char sdl_font_bmp[];
    extern unsigned int sdl_font_bmp_len;
    auto font_surface = SDL_LoadBMP_RW(SDL_RWFromConstMem(sdl_font_bmp, sdl_font_bmp_len), 1);
    SDL_SetColorKey(font_surface, 1, 0);
    font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);
    SDL_FreeSurface(font_surface);
}

void SDL::processEvents(GameState& gamestate) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT: quit = true; break;
        case SDL_TEXTINPUT: printf("%s\n", event.text.text); break;
        case SDL_KEYDOWN: gamestate.onKey(event.key.keysym.sym); break;
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
            SDL_Rect src{(t.c & 0x0F) * 8, ((t.c >> 4) & 0x0F) * 8, 8, 8};
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

}