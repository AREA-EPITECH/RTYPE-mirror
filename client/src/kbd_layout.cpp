/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** kbd_layout.cpp
*/

#include "raylib/kbd_layout.h"
#include <spdlog/spdlog.h>
// 30 layout, 300 key with 4 int (input,base,shift,alt)
// input is on qwerty us keyboard 
#define MAX_LAYOUT 30
#define MAX_KEY 300
const int KBD_LAYOUT[MAX_LAYOUT][MAX_KEY][4] = {
    { // EN US, id 0
        {KEY_ONE,KEY_ONE,33,0},
        {KEY_TWO,KEY_TWO,64,0},
        {KEY_THREE,KEY_THREE,35,0},
        {KEY_FOUR,KEY_FOUR,36,0},
        {KEY_FIVE,KEY_FIVE,37,0},
        {KEY_SIX,KEY_SIX,0,94},
        {KEY_SEVEN,KEY_SEVEN,38,0},
        {KEY_EIGHT,KEY_EIGHT,42,0},
        {KEY_NINE,KEY_NINE,40,0},
        {KEY_ZERO,KEY_ZERO,41,0},
        {KEY_MINUS,KEY_MINUS,95,0},
        {KEY_EQUAL,KEY_EQUAL,43,0},

        {KEY_Q,KEY_Q+32,KEY_Q,0},
        {KEY_W,KEY_W+32,KEY_W,0},
        {KEY_E,KEY_E+32,KEY_E,0},
        {KEY_R,KEY_R+32,KEY_R,0},
        {KEY_T,KEY_T+32,KEY_T,0},
        {KEY_Y,KEY_Y+32,KEY_Y,0},
        {KEY_U,KEY_U+32,KEY_U,0},
        {KEY_I,KEY_I+32,KEY_I,0},
        {KEY_O,KEY_O+32,KEY_O,0},
        {KEY_P,KEY_P+32,KEY_P,0},
        {91,91,123,0},//[
        {93,93,125,0},//]
        {KEY_A,KEY_A+32,KEY_A,0},
        {KEY_S,KEY_S+32,KEY_S,0},
        {KEY_D,KEY_D+32,KEY_D,0},
        {KEY_F,KEY_F+32,KEY_F,0},
        {KEY_G,KEY_G+32,KEY_G,0},
        {KEY_H,KEY_H+32,KEY_H,0},
        {KEY_J,KEY_J+32,KEY_J,0},
        {KEY_K,KEY_K+32,KEY_K,0},
        {KEY_L,KEY_L+32,KEY_L,0},
        {59,59,58,0},//;
        {39,0,37,39},//'


        {KEY_Z,KEY_Z+32,KEY_Z,0},
        {KEY_X,KEY_X+32,KEY_X,0},
        {KEY_C,KEY_C+32,KEY_C,0},
        {KEY_V,KEY_V+32,KEY_V,0},
        {KEY_B,KEY_B+32,KEY_B,0},
        {KEY_N,KEY_N+32,KEY_N,0},
        {KEY_M,KEY_M+32,KEY_M,0},
        {KEY_COMMA,KEY_COMMA,60,0},
        {KEY_PERIOD,KEY_PERIOD,62,0},
        {47,47,63,0},// /
        {161,0,0,0}
    },
    {// EN UK
        {KEY_ONE,KEY_ONE,33,0},
        {KEY_TWO,KEY_TWO,34,0},
        {KEY_THREE,KEY_THREE,163,0},
        {KEY_FOUR,KEY_FOUR,36,0},
        {KEY_FIVE,KEY_FIVE,37,0},
        {KEY_SIX,KEY_SIX,94,94},
        {KEY_SEVEN,KEY_SEVEN,38,0},
        {KEY_EIGHT,KEY_EIGHT,42,0},
        {KEY_NINE,KEY_NINE,40,0},
        {KEY_ZERO,KEY_ZERO,41,0},
        {KEY_MINUS,KEY_MINUS,95,0},
        {KEY_EQUAL,KEY_EQUAL,43,0},

        {KEY_Q,KEY_Q+32,KEY_Q,0},
        {KEY_W,KEY_W+32,KEY_W,0},
        {KEY_E,KEY_E+32,KEY_E,0},
        {KEY_R,KEY_R+32,KEY_R,0},
        {KEY_T,KEY_T+32,KEY_T,0},
        {KEY_Y,KEY_Y+32,KEY_Y,0},
        {KEY_U,KEY_U+32,KEY_U,0},
        {KEY_I,KEY_I+32,KEY_I,0},
        {KEY_O,KEY_O+32,KEY_O,0},
        {KEY_P,KEY_P+32,KEY_P,0},
        {91,91,123,0},//[
        {93,93,125,0},//]
        {KEY_A,KEY_A+32,KEY_A,0},
        {KEY_S,KEY_S+32,KEY_S,0},
        {KEY_D,KEY_D+32,KEY_D,0},
        {KEY_F,KEY_F+32,KEY_F,0},
        {KEY_G,KEY_G+32,KEY_G,0},
        {KEY_H,KEY_H+32,KEY_H,0},
        {KEY_J,KEY_J+32,KEY_J,0},
        {KEY_K,KEY_K+32,KEY_K,0},
        {KEY_L,KEY_L+32,KEY_L,0},
        {59,59,58,0},//;
        {39,39,64,39},//'
        {92,35,126,126},/* \ */

        {KEY_Z,KEY_Z+32,KEY_Z,0},
        {KEY_X,KEY_X+32,KEY_X,0},
        {KEY_C,KEY_C+32,KEY_C,0},
        {KEY_V,KEY_V+32,KEY_V,0},
        {KEY_B,KEY_B+32,KEY_B,0},
        {KEY_N,KEY_N+32,KEY_N,0},
        {KEY_M,KEY_M+32,KEY_M,0},
        {KEY_COMMA,KEY_COMMA,60,0},
        {KEY_PERIOD,KEY_PERIOD,62,0},
        {47,47,63,0},// /
        {161,0,0,0}
    },
    {// FR
        {KEY_ONE,   38,     49,     126},
        {KEY_TWO,   233,    50,     35 },
        {KEY_THREE, 34,     51,     123},
        {KEY_FOUR,  39,     52,     91 },
        {KEY_FIVE,  40,     53,     124},
        {KEY_SIX,   45,     54,     96 },
        {KEY_SEVEN, 232,    55,     92 },
        {KEY_EIGHT, 95,     56,     94 },
        {KEY_NINE,  231,    57,     64 },
        {KEY_ZERO,  224,    48,     93 },
        {KEY_MINUS, 41,     176,    125},
        {KEY_EQUAL, 61,     43,     0  },

        {KEY_Q, KEY_A+32, KEY_A, 0},
        {KEY_W, KEY_Z+32, KEY_Z, 0},
        {KEY_E, KEY_E+32, KEY_E, 0},
        {KEY_R, KEY_R+32, KEY_R, 0},
        {KEY_T, KEY_T+32, KEY_T, 0},
        {KEY_Y, KEY_Y+32, KEY_Y, 0},
        {KEY_U, KEY_U+32, KEY_U, 0},
        {KEY_I, KEY_I+32, KEY_I, 0},
        {KEY_O, KEY_O+32, KEY_O, 0},
        {KEY_P, KEY_P+32, KEY_P, 0},
        {91,     94,  0,   0},
        {93,     36,  0,   0},

        {KEY_A, KEY_Q+32, KEY_Q, 0},
        {KEY_S, KEY_S+32, KEY_S, 0},
        {KEY_D, KEY_D+32, KEY_D, 0},
        {KEY_F, KEY_F+32, KEY_F, 0},
        {KEY_G, KEY_G+32, KEY_G, 0},
        {KEY_H, KEY_H+32, KEY_G, 0},
        {KEY_J, KEY_J+32, KEY_J, 0},
        {KEY_K, KEY_K+32, KEY_K, 0},
        {KEY_L, KEY_L+32, KEY_L, 0},
        {KEY_SEMICOLON, KEY_M+32, KEY_M, 0},
        {KEY_APOSTROPHE, 249, 37, 0},

        {KEY_Z, KEY_W+32, KEY_W, 0},
        {KEY_X, KEY_X+32, KEY_X, 0},
        {KEY_C, KEY_C+32, KEY_C, 0},
        {KEY_V, KEY_V+32, KEY_V, 0},
        {KEY_B, KEY_B+32, KEY_B, 0},
        {KEY_N, KEY_N+32, KEY_N, 0},
        {KEY_M, ',', 63, 0},
        {KEY_COMMA,  ';',  '.',  0},
        {KEY_PERIOD, ':',  '/',  0},
        {KEY_SLASH,  '!',  0,    0},
        {161, 0, 0, 0}
    },
    {//FR_BEL_VAR , id 2
        {KEY_ZERO,0,KEY_ZERO,125},
        {KEY_ONE,38,KEY_ONE,124},
        {KEY_TWO,0,KEY_TWO,64},
        {KEY_THREE,34,KEY_THREE,35},
        {KEY_FOUR,39,KEY_FOUR,0},
        {KEY_FIVE,40,KEY_FIVE,0},
        {KEY_SIX,0,KEY_SIX,0},
        {KEY_SEVEN,0,KEY_SEVEN,0},
        {KEY_EIGHT,33,KEY_EIGHT,0},
        {KEY_NINE,0,KEY_NINE,123},
        {KEY_MINUS,41,167,0},
        {KEY_EQUAL,KEY_MINUS,95,0},

        {KEY_Q,KEY_A+32,KEY_A,0},
        {KEY_W,KEY_Z+32,KEY_Z,0},
        {KEY_E,KEY_E+32,KEY_E,0},
        {KEY_R,KEY_R+32,KEY_R,0},
        {KEY_T,KEY_T+32,KEY_T,0},
        {KEY_Y,KEY_Y+32,KEY_Y,0},
        {KEY_U,KEY_U+32,KEY_U,0},
        {KEY_I,KEY_I+32,KEY_I,0},
        {KEY_O,KEY_O+32,KEY_O,0},
        {KEY_P,KEY_P+32,KEY_P,0},
        {91,0,0,91},//[
        {93,36,42,93},//]

        {KEY_A,KEY_Q+32,KEY_Q,0},
        {KEY_S,KEY_S+32,KEY_S,0},
        {KEY_D,KEY_D+32,KEY_D,0},
        {KEY_F,KEY_F+32,KEY_F,0},
        {KEY_G,KEY_G+32,KEY_G,0},
        {KEY_H,KEY_H+32,KEY_H,0},
        {KEY_J,KEY_J+32,KEY_J,0},
        {KEY_K,KEY_K+32,KEY_K,0},
        {KEY_L,KEY_L+32,KEY_L,0},
        {KEY_SEMICOLON,KEY_M+32,KEY_M,0},//;
        {39,0,37,39},//'

        {KEY_Z,KEY_W+32,KEY_W,0},
        {KEY_X,KEY_X+32,KEY_X,0},
        {KEY_C,KEY_C+32,KEY_C,0},
        {KEY_V,KEY_V+32,KEY_V,0},
        {KEY_B,KEY_B+32,KEY_B,0},
        {KEY_N,KEY_N+32,KEY_N,0},
        {KEY_M,KEY_COMMA,63,0},
        {44,KEY_SEMICOLON,KEY_PERIOD,0},//,
        {46,58,47,0},//.
        {47,61,43,126},

        {161,60,62,92}//alt gr
    }
};

int Kbd_GetKeyPressed(KBD_Layout layout, bool is_shift_default)
{
	int c = GetKeyPressed();
    bool shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    bool alt = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);
    if (is_shift_default) {
        shift = true;
    }
    if (c>255) return c;
    for(int i=0;i<MAX_KEY;i++)
    {
        if(KBD_LAYOUT[layout][i][0]==c)
        {
            if(shift) return KBD_LAYOUT[layout][i][2];
            else if(alt) return KBD_LAYOUT[layout][i][3];
            else return KBD_LAYOUT[layout][i][1];
        }
    }
	return c;
}

bool Kbd_IsKeyDown(KBD_Layout layout, int key)
{
    for(int i=0;i<MAX_KEY;i++)
    {
        if(KBD_LAYOUT[layout][i][0]==key)
        {
            int c = KBD_LAYOUT[layout][i][1];
            if(c>=97 && c<=122)
                return IsKeyDown(c-32);
            else
                return IsKeyDown(c);
        }
    }
    return IsKeyDown(key);
}
bool Kbd_IsKeyPressed(KBD_Layout layout, int key)
{
    for(int i=0;i<MAX_KEY;i++)
    {
        if(KBD_LAYOUT[layout][i][0]==key)
        {
            int c = KBD_LAYOUT[layout][i][1];
            if(c>=97 && c<=122)
                return IsKeyPressed(c-32);
            else
                return IsKeyPressed(c);
        }
    }
    return IsKeyPressed(key);
}
