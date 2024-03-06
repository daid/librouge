#include "r/frontend/terminal.h"
#include "r/keys.h"


#define ESC "\x1B"
#define CSI "\x1B["


namespace r::frontend {


void Terminal::present() {
    auto result = updateSize();
    if (result != tiles.size()) {
        tiles.resize(result);
        cursor_pos = {-1, -1};
    }

    Color cur_fg = {-1, -1, -1};
    Color cur_bg = {-1, -1, -1};
    cur_fg.r = cur_bg.r = -1;
    for(int y=0; y<tiles.size().y; y++) {
        for(int x=0; x<tiles.size().x; x++) {
            auto pos = ivec2{x, y};
            auto& tile = tiles[pos];
            if (tile.c != tile.previous_c || tile.forground_color != tile.previous_forground_color || tile.background_color != tile.previous_background_color) {
                tile.previous_c = tile.c;
                tile.previous_forground_color = tile.forground_color;
                tile.previous_background_color = tile.background_color;
                if (cursor_pos != pos) {
                    cursor_pos = pos;
                    printf(CSI "%d;%dH", pos.y + 1, pos.x + 1);
                }
                if (tile.forground_color != cur_fg)
                    printf(CSI "38;2;%d;%d;%dm", int(tile.forground_color.r * 255), int(tile.forground_color.g * 255), int(tile.forground_color.b * 255));
                if (tile.background_color != cur_bg)
                    printf(CSI "48;2;%d;%d;%dm", int(tile.background_color.r * 255), int(tile.background_color.g * 255), int(tile.background_color.b * 255));
                printf("%c", tile.c);
                cur_fg = tile.forground_color;
                cur_bg = tile.background_color;
                cursor_pos.x += 1;
            }
        }
    }
    if (cursor_pos != ivec2{0, 0}) {
        cursor_pos = {0, 0};
        printf(CSI "1;1H"); //Put the cursor in the top left corner
        printf(CSI "?25l"); //WSL sometimes enables the cursor again, stupid WSL, so disable it every refresh
        fflush(stdout);
    }
}

bool Terminal::isOpen() {
    return !quit;
}

ivec2 Terminal::size() {
    return tiles.size();
}

void Terminal::draw(ivec2 position, char c, Color forground_color) {
    if (position.x < 0 || position.y < 0) return;
    if (position.x >= tiles.size().x || position.y >= tiles.size().y) return;

    auto& e = tiles(position);
    e.c = c;
    e.forground_color = forground_color;
}

void Terminal::draw(ivec2 position, char c, Color forground_color, Color background_color) {
    if (position.x < 0 || position.y < 0) return;
    if (position.x >= tiles.size().x || position.y >= tiles.size().y) return;

    auto& e = tiles(position);
    e.c = c;
    e.forground_color = forground_color;
    e.background_color = background_color;
}

void Terminal::draw(ivec2 position, Color background_color) {
    if (position.x < 0 || position.y < 0) return;
    if (position.x >= tiles.size().x || position.y >= tiles.size().y) return;

    auto& e = tiles(position);
    e.background_color = background_color;
}

}
