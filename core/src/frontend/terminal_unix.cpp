#ifndef _WIN32

#include "r/frontend/terminal.h"
#include "r/keys.h"

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <poll.h>
#include <queue>

static struct termios term_orig;


#define ESC "\x1B"
#define CSI "\x1B["


namespace r::frontend {


Terminal::Terminal(const Engine::Config& config) {
    (void)config;
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
    
    //Switch to alternative buffer and clear it, disable the cursor, enable mouse
    printf(CSI "?1049h" CSI "?25l" CSI "?1000;1002;1003h");
    fflush(stdout);
}

Terminal::~Terminal()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &term_orig);
    printf(CSI "?1049l" CSI "?25h" CSI "0m" CSI "?1000;1002;1003l");
    fflush(stdout);
}

void Terminal::processEvents(GameState& gamestate, bool blocking) {
    if (!blocking) {
        struct pollfd p { 0, POLLIN, 0 };
        poll(&p, 1, 16);
        if (!(p.revents & POLLIN))
            return;
    }

    std::deque<char> input_queue;
    char buffer[32];
    auto count = read(STDIN_FILENO, buffer, sizeof(buffer));
    for(int n=0; n<count; n++)
        input_queue.push_back(buffer[n]);
    auto inputNext = [&]() -> char {
        if (input_queue.empty()) return 0;
        char result = input_queue.front();
        input_queue.pop_front();
        return result;
    };
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
            (void)param1;
            (void)param2;
            result = inputNext();
            switch(result) {
            case '~':
                switch(param0) {
                case 1: gamestate.onKey(KEY_UP_LEFT); break;
                case 4: gamestate.onKey(KEY_DOWN_LEFT); break;
                case 5: gamestate.onKey(KEY_UP_RIGHT); break;
                case 6: gamestate.onKey(KEY_DOWN_RIGHT); break;
                }
                break;
            case 'M':
                {
                    int flags = inputNext() - 0x20;
                    int x = (inputNext() & 0xFF) - 0x21;
                    int y = (inputNext() & 0xFF) - 0x21;
                    int button = flags & 3; //0=MB1, 1=MB2, 2=MB3, 3=Release
                    if (flags & 0x40) {
                        gamestate.onMouseWheel({x, y}, button);
                    } else if (flags & 0x20) {
                        gamestate.onMouseMove({x, y}, button);
                    } else if (button == 3) {
                        gamestate.onMouseUp({x, y}, 0);
                    } else {
                        gamestate.onMouseDown({x, y}, button);
                    }
                }
                break;
            case 'A': gamestate.onKey(KEY_UP); break;
            case 'B': gamestate.onKey(KEY_DOWN); break;
            case 'C': gamestate.onKey(KEY_RIGHT); break;
            case 'D': gamestate.onKey(KEY_LEFT); break;
            }
        } else {
            //ALT+key
            result = input_queue.front();
            input_queue.pop_front();
        }
    }
}

ivec2 Terminal::updateSize() {
    struct winsize w;
    ioctl(1, TIOCGWINSZ, &w);
    return {w.ws_col, w.ws_row};
}

}

#endif
