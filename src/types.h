#include <stdint.h>

typedef unsigned int size_t;
typedef unsigned char bool;
typedef short wchar_t;

static const bool true = 1;
static const bool false = 0;

#pragma pack(1)

typedef struct {
    uint32_t unk_0;
    uint16_t unk_4;
    uint16_t difficulty; /* Must be < 4 */
    uint32_t unk_8;
    char map_name[256];
} game_options_t;

static_assert(sizeof(game_options_t) == 0x10C);

typedef union {
    char unk[0x20];
} game_time_globals_t;

static_assert(sizeof(game_time_globals_t) == 0x20);

#pragma pack()
