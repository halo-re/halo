#include <stdint.h>
#include <stddef.h>

typedef unsigned int size_t;
typedef unsigned char bool;
static_assert(sizeof(wchar_t) == 2);
static const bool true = 1;
static const bool false = 0;

#pragma pack(1)
#define cs(s, e)    static_assert(sizeof(s) == e)
#define co(s, f, o) static_assert(offsetof(s, f) == o)

typedef struct {
    uint32_t unk_0;
    uint16_t unk_4;
    uint16_t difficulty; /* Must be < 4 */
    uint32_t unk_8;
    char map_name[256];
} game_options_t;

co(game_options_t, unk_0,      0);
co(game_options_t, unk_4,      4);
co(game_options_t, difficulty, 6);
co(game_options_t, unk_8,      8);
co(game_options_t, map_name,   12);
cs(game_options_t, 0x10C);

typedef struct {
    bool initialized;
    bool active;
    char unk_2[10];
    uint32_t local_time;
    uint16_t elapsed;
    char unk_17[6];
    float speed;
    uint32_t unk_28;
} game_time_globals_t;
#pragma pack()

co(game_time_globals_t, initialized, 0);
co(game_time_globals_t, active,      1);
co(game_time_globals_t, unk_2,       2);
co(game_time_globals_t, local_time,  12);
co(game_time_globals_t, elapsed,     16);
co(game_time_globals_t, speed,       24);
co(game_time_globals_t, unk_28,      28);
cs(game_time_globals_t, 0x20);

#undef co
#undef cs
#pragma pack()
