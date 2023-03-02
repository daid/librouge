#include "r/frontend/terminal.h"
#include "r/keys.h"

#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#  define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#  define DISABLE_NEWLINE_AUTO_RETURN        0x0008
# endif
#else
# include <poll.h>
#endif


#define ESC "\x1B"
#define CSI "\x1B["


namespace r::frontend {


Terminal::Terminal(const Engine::Config& config) {
    (void)config;
#ifdef _WIN32
    DWORD mode{};
    if (!GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode))
    {
        printf("Failed to get stdin...");
        exit(1);
    }
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT);
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN);
#else
    struct termios term;
    if (tcgetattr(STDIN_FILENO, &term_orig)) {
        printf("tcgetattr failed\n");
        ::exit(-1);
    }

    term = term_orig;

    cfmakeraw(&term);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 1;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term)) {
        printf("tcsetattr failed\n");
        ::exit(-1);
    }
#endif
    
    //Switch to alternative buffer and clear it, disable the cursor, enable mouse
    printf(CSI "?1049h" CSI "?25l" CSI "?1000;1015h");
    fflush(stdout);
}

void Terminal::processEvents(GameState& gamestate) {
#ifdef _WIN32
    while(1) {
        INPUT_RECORD input_record;
        DWORD count = 0;
        GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &count);
        if (count == 0)
            break;
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &input_record, 1, &count);
        switch(input_record.EventType)
        {
        case KEY_EVENT:
            if (input_record.Event.KeyEvent.bKeyDown) {
                if (input_record.Event.KeyEvent.uChar.AsciiChar != 0) {
                    gamestate.onKey(input_record.Event.KeyEvent.uChar.AsciiChar);
                } else {
                    switch(input_record.Event.KeyEvent.wVirtualKeyCode) {
                    case VK_RETURN: gamestate.onKey(KEY_RETURN); break;
                    case VK_ESCAPE: gamestate.onKey(KEY_ESCAPE); break;
                    case VK_BACK: gamestate.onKey(KEY_BACKSPACE); break;
                    case VK_DELETE: gamestate.onKey(KEY_DELETE); break;
                    case VK_TAB: gamestate.onKey(KEY_TAB); break;
                    case VK_F1: gamestate.onKey(KEY_F1); break;
                    case VK_F2: gamestate.onKey(KEY_F2); break;
                    case VK_F3: gamestate.onKey(KEY_F3); break;
                    case VK_F4: gamestate.onKey(KEY_F4); break;
                    case VK_F5: gamestate.onKey(KEY_F5); break;
                    case VK_F6: gamestate.onKey(KEY_F6); break;
                    case VK_F7: gamestate.onKey(KEY_F7); break;
                    case VK_F8: gamestate.onKey(KEY_F8); break;
                    case VK_F9: gamestate.onKey(KEY_F9); break;
                    case VK_F10: gamestate.onKey(KEY_F10); break;
                    case VK_F11: gamestate.onKey(KEY_F11); break;
                    case VK_F12: gamestate.onKey(KEY_F12); break;
                    case VK_LEFT: gamestate.onKey(KEY_LEFT); break;
                    case VK_RIGHT: gamestate.onKey(KEY_RIGHT); break;
                    case VK_UP: gamestate.onKey(KEY_UP); break;
                    case VK_DOWN: gamestate.onKey(KEY_DOWN); break;

                    case VK_NUMPAD1: gamestate.onKey(KEY_DOWN_LEFT); break;
                    case VK_NUMPAD2: gamestate.onKey(KEY_DOWN); break;
                    case VK_NUMPAD3: gamestate.onKey(KEY_DOWN_RIGHT); break;
                    case VK_NUMPAD4: gamestate.onKey(KEY_LEFT); break;
                    case VK_NUMPAD6: gamestate.onKey(KEY_RIGHT); break;
                    case VK_NUMPAD7: gamestate.onKey(KEY_UP_LEFT); break;
                    case VK_NUMPAD8: gamestate.onKey(KEY_UP); break;
                    case VK_NUMPAD9: gamestate.onKey(KEY_UP_RIGHT); break;
                    }
                }
            }
            /*
            printf("KEY: %d %d %d %d %d %d %d\n",
                input_record.Event.KeyEvent.bKeyDown,
                input_record.Event.KeyEvent.wRepeatCount,
                input_record.Event.KeyEvent.wVirtualKeyCode,
                input_record.Event.KeyEvent.wVirtualScanCode,
                input_record.Event.KeyEvent.uChar.UnicodeChar,
                input_record.Event.KeyEvent.uChar.AsciiChar,
                input_record.Event.KeyEvent.dwControlKeyState);
            */
            break;
        case WINDOW_BUFFER_SIZE_EVENT:
            //printf("SIZE: %d %d\n", input_record.Event.WindowBufferSizeEvent.dwSize.X, input_record.Event.WindowBufferSizeEvent.dwSize.Y);
            break;
        }
    }
#else
    struct pollfd p { 0, POLLIN, 0 };
    poll(&p, 1, 0);
    if (!(p.revents & POLLIN))
        return;

    std::deque<char> input_queue;
    char buffer[32];
    auto count = read(STDIN_FILENO, buffer, sizeof(buffer));
    for(int n=0; n<count; n++)
        input_queue.push_back(buffer[n]);
    
    auto inputExpect = [&](char c) {
        if (input_queue.empty()) return false;
        if (input_queue.front() != c) return false;
        input_queue.pop_front();
        return true;
    };
    auto inputNumber = [&]() {
        int result = 0;
        while(!input_queue.empty() && input_queue.front() >= '0' && input_queue.front() <= '9') {
            result = result * 10 + (input_queue.front() - '0');
            input_queue.pop_front();
        }
        return result;
    };

    while(!input_queue.empty())
    {
        int result = input_queue.front();
        input_queue.pop_front();
        if (result == 3) { // CTRL+C
            quit = true;
            continue;
        }

        if (result != 0x1B) {
            gamestate.onKey(result);
            continue;
        }
        // ESC, this is where things get wild
        if (input_queue.empty()) { // No more keys, actual escape key
            gamestate.onKey(result);
            continue;
        }
        if (input_queue.front() == 0x1B) { // Actual escape, but then properly escaped
            input_queue.pop_front();
            gamestate.onKey(result);
            continue;
        }
        if (inputExpect('[')) { // CSI, most "special" keys
            if (input_queue.empty()) {
                //ALT+[
                continue;
            }
            int param0 = inputNumber();
            int param1 = -1;
            int param2 = -1;
            if (inputExpect(';')) {
                param1 = inputNumber();
            }
            if (inputExpect(';')) {
                param2 = inputNumber();
            }
            result = inputNext();
            if (result == '~') {
                continue;
            }
            if (result == 'M') { // Mouse tracking
                if (param0 >= 0x20) {
                    auto mouse_pos = ivec2{param1 - 1, param2 - 1};
                    auto mouse_button = param0;
                    continue;
                }
                int button = inputNext() - 0x20;
                auto mouse_pos = ivec2{((unsigned char)inputNext()) - 0x21, ((unsigned char)inputNext()) - 0x21};
                continue;
            }
            //result, param0
            continue;
        }
        //ALT+key
        result = input_queue.front();
        input_queue.pop_front();
    }
#endif
}

void Terminal::present() {
#ifdef WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    glm::ivec2 result{80, 24};
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        result.x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        result.y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
#else
    struct winsize w;
    ioctl(1, TIOCGWINSZ, &w);
    glm::ivec2 result{w.ws_col, w.ws_row};
#endif
    if (result != tiles.size()) {
        tiles.resize(result);
        cursor_pos = {-1, -1};
    }

    printf(CSI "?25l"); //WSL sometimes enables the cursor again, stupid WSL, so disable it every refresh
    Color cur_fg = {-1, -1, -1};
    Color cur_bg = {-1, -1, -1};
    for(int y=0; y<tiles.size().y; y++) {
        for(int x=0; x<tiles.size().x; x++) {
            auto pos = glm::ivec2{x, y};
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
    cursor_pos = {0, 0};
    printf(CSI "1;1H");//Put the cursor in the top left corner
    fflush(stdout);
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

}
