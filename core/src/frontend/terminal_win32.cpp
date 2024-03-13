#ifdef _WIN32

#include "r/frontend/terminal.h"
#include "r/keys.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#  define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#  define DISABLE_NEWLINE_AUTO_RETURN        0x0008
#endif


#define ESC "\x1B"
#define CSI "\x1B["


static DWORD last_mouse_buttons = 0;

namespace r::frontend {


Terminal::Terminal(const Engine::Config& config) {
    (void)config;

    DWORD mode{};
    if (!GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode))
    {
        printf("Failed to get stdin console mode: %lx\n", GetLastError());
        exit(1);
    }
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_EXTENDED_FLAGS);
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN);
    
    fflush(stdout);
}

Terminal::~Terminal()
{
    fflush(stdout);
}

void Terminal::processEvents(GameState& gamestate, bool blocking) {
    while(1) {
        INPUT_RECORD input_record;
        DWORD count = 0;
        if (!blocking) {
            GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &count);
            if (count == 0)
                break;
        }
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &input_record, 1, &count);
        blocking = false;
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
        case MOUSE_EVENT: {
            auto& me = input_record.Event.MouseEvent;
            if (me.dwEventFlags == 0) {
                for(int n=0; n<5; n++) {
                    if ((me.dwButtonState & (1 << n)) != (last_mouse_buttons & (1 << n))) {
                        if (me.dwButtonState & (1 << n))
                            gamestate.onMouseDown({me.dwMousePosition.X, me.dwMousePosition.Y}, n);
                        else
                            gamestate.onMouseUp({me.dwMousePosition.X, me.dwMousePosition.Y}, n);
                    }
                }
                last_mouse_buttons = me.dwButtonState;
            } else if (me.dwEventFlags == 1)
                for(int n=0; n<5; n++) {
                    if (me.dwButtonState & (1 << n))
                        gamestate.onMouseMove({me.dwMousePosition.X, me.dwMousePosition.Y}, n);
                }
            else if (me.dwEventFlags == 4)
                gamestate.onMouseWheel({me.dwMousePosition.X, me.dwMousePosition.Y}, me.dwButtonState);
            } break;
        }
    }
}

ivec2 Terminal::updateSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    glm::ivec2 result{80, 24};
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        result.x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        result.y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    return result;
}

}

#endif
