#include <stdint.h>
#include <stddef.h>

#define static_assert(cond) _Static_assert(cond, #cond)
#define __noreturn __attribute__((noreturn))

#pragma pack(1)
#define cs(t, s)    static_assert(sizeof(t) == s)
#define co(t, f, o) static_assert(offsetof(t, f) == o)

typedef unsigned int size_t;
typedef unsigned char bool;
cs(long long, 8);
cs(int, 4);
cs(short, 2);
cs(wchar_t, 2);

// FIXME: Normalize
typedef uint32_t _DWORD;
typedef uint16_t _WORD;
typedef uint8_t  _BYTE;
#define __int16 short
#define __int8 char

static const bool true = 1;
static const bool false = 0;

typedef struct {
  uint32_t unk_0;
  uint16_t unk_4;
  uint16_t difficulty; /* Must be < 4 */
  uint32_t random_seed;
  char map_name[256];
} game_options_t;

co(game_options_t, unk_0,       0);
co(game_options_t, unk_4,       4);
co(game_options_t, difficulty,  6);
co(game_options_t, random_seed, 8);
co(game_options_t, map_name,    12);
cs(game_options_t, 0x10C);

typedef struct {
  bool map_loaded;
  bool active;
  char unk_2[6];
  game_options_t game_options;
} game_globals_t;

cs(game_globals_t, 276);
co(game_globals_t, map_loaded,   0);
co(game_globals_t, active,       1);
co(game_globals_t, unk_2,        2);
co(game_globals_t, game_options, 8);

typedef struct {
  char unk_0[104];
} game_variant_t;

cs(game_variant_t, 104);

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

co(game_time_globals_t, initialized, 0);
co(game_time_globals_t, active,      1);
co(game_time_globals_t, unk_2,       2);
co(game_time_globals_t, local_time,  12);
co(game_time_globals_t, elapsed,     16);
co(game_time_globals_t, speed,       24);
co(game_time_globals_t, unk_28,      28);
cs(game_time_globals_t, 0x20);

typedef struct {
  char unk_0[212];
} player_data_t;

cs(player_data_t, 212);

typedef struct {
  char unk_0[64];
} team_data_t;

cs(team_data_t, 64);

typedef struct {
  char unk_0[176];
} players_globals_t;

cs(players_globals_t, 176);

typedef struct {
  char unk_0[272];
} player_control_globals_t;

cs(player_control_globals_t, 272);

typedef struct {
  char name[32];
  int16_t maximum_count;
  int16_t size;
  bool valid;
  bool identifier_zero_invalid;
  char unk_38[2];
  int magic;
  char unk_44[2];
  int16_t current_count;
  int16_t unk_48;
  char unk_50[2];
  void *data;
} data_t;

cs(data_t, 56);
co(data_t, name,                    0);
co(data_t, maximum_count,           32);
co(data_t, size,                    34);
co(data_t, valid,                   36);
co(data_t, identifier_zero_invalid, 37);
co(data_t, unk_38,                  38);
co(data_t, magic,                   40);
co(data_t, unk_44,                  44);
co(data_t, current_count,           46);
co(data_t, unk_48,                  48);
co(data_t, data,                    52);

typedef struct {
  data_t *data;
  uint16_t index;
  char unk_6[2];
  uint32_t datum_handle;
  uint32_t cookie;
} data_iter_t;

cs(data_iter_t, 16);
co(data_iter_t, data,         0);
co(data_iter_t, index,        4);
co(data_iter_t, unk_6,        6);
co(data_iter_t, datum_handle, 8);
co(data_iter_t, cookie,       12);

#undef co
#undef cs
#pragma pack()
