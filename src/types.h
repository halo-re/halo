#ifdef MSVC
#define __noreturn
#define offsetof(t, f) ( (int) &((t*)0)->f )
#define static_assert(cond) static_assert(cond, #cond)
#else
#define __noreturn __attribute__((noreturn))
#define offsetof(t, f) __builtin_offsetof(t, f)
#define static_assert(cond) _Static_assert(cond, #cond)
#endif
#define NULL ((void*)0)

#define cs(t, s)    static_assert(sizeof(t) == s)
#define co(t, f, o) static_assert(offsetof(t, f) == o)

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
#ifdef MSVC
typedef __int64 int64_t;
#else
typedef signed long long int64_t;
#endif

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef unsigned char bool;
typedef signed short wchar_t;
typedef unsigned int size_t;

// FIXME: Normalize
typedef uint32_t _DWORD;
typedef uint16_t _WORD;
typedef uint8_t _BYTE;
#define __int16 short
#define __int8 char

static const bool true = 1;
static const bool false = 0;

#pragma pack(1)

/// size=0x10c
typedef struct {
  uint32_t unk_0;         ///< offset=0x00
  uint16_t unk_4;         ///< offset=0x04
  uint16_t difficulty;    ///< offset=0x06
  uint32_t random_seed;   ///< offset=0x08
  char     map_name[256]; ///< offset=0x0c
} game_options_t;

/// size=0x114
typedef struct {
  bool           map_loaded;   ///< offset=0x00
  bool           active;       ///< offset=0x01
  char           unk_2[6];     ///< offset=0x02
  game_options_t game_options; ///< offset=0x08
} game_globals_t;

/// size=0x68
typedef struct {
  char unk_0[104];
} game_variant_t;

/// size=0x20
typedef struct {
  bool     initialized; ///< offset=0x00
  bool     active;      ///< offset=0x01
  char     unk_2[10];   ///< offset=0x02
  uint32_t local_time;  ///< offset=0x0c
  uint16_t elapsed;     ///< offset=0x10
  char     unk_18[6];   ///< offset=0x12
  float    speed;       ///< offset=0x18
  uint32_t unk_28;      ///< offset=0x1c
} game_time_globals_t;

/// size=0xd4
typedef struct {
  char unk_0[0xd4];
} player_data_t;

/// size=0x40
typedef struct {
  char unk_0[0x40];
} team_data_t;

/// size=0xb0
typedef struct {
  char unk_0[0xb0];
} players_globals_t;

/// size=0x110
typedef struct {
  char unk_0[0x110];
} player_control_globals_t;

/// size=0x38
typedef struct {
  char    name[32];                ///< offset=0x00
  int16_t maximum_count;           ///< offset=0x20
  int16_t size;                    ///< offset=0x22
  bool    valid;                   ///< offset=0x24
  bool    identifier_zero_invalid; ///< offset=0x25
  char    unk_38[2];               ///< offset=0x26
  int     magic;                   ///< offset=0x28
  char    unk_44[2];               ///< offset=0x2c
  int16_t current_count;           ///< offset=0x2e
  int16_t unk_48;                  ///< offset=0x30
  char    unk_50[2];               ///< offset=0x32
  void    *data;                   ///< offset=0x34
} data_t;

/// size=0x10
typedef struct {
  data_t   *data;        ///< offset=0x00
  uint16_t index;        ///< offset=0x04
  char     unk_6[2];     ///< offset=0x06
  uint32_t datum_handle; ///< offset=0x08
  uint32_t cookie;       ///< offset=0x0c
} data_iter_t;

/// size=8
typedef struct
{
  int16_t y0; ///< offset=0x00
  int16_t x0; ///< offset=0x02
  int16_t y1; ///< offset=0x04
  int16_t x1; ///< offset=0x06
} viewport_bounds_t;

/// size=0x0C
typedef struct {
  float x; ///< offset=0x00
  float y; ///< offset=0x04
  float z; ///< offset=0x08
} vector3_t;

/// size=0x54
typedef struct {
  vector3_t         unk_0;                  ///< offset=0x00
  vector3_t         unk_12;                 ///< offset=0x0c
  vector3_t         unk_24;                 ///< offset=0x18
  uint8_t           unk_36;                 ///< offset=0x24
  char              unk_37[3];              ///< offset=0x25
  float             vertical_field_of_view; ///< offset=0x28
  viewport_bounds_t viewport_bounds;        ///< offset=0x2c
  _WORD             unk_52[4];              ///< offset=0x34
  float             z_near;                 ///< offset=0x3c
  float             z_far;                  ///< offset=0x40
  char              unk_68[16];             ///< offset=0x44
} camera_t;

/// size=0xac
typedef struct {
  int16_t  unk_0; ///< offset=0x00
  int8_t   unk_2; ///< offset=0x02
  int8_t   unk_3; ///< offset=0x03
  camera_t cam0;  ///< offset=0x04
  camera_t cam1;  ///< offset=0x58
} pregame_render_info_t;

/// size=0x258
typedef struct {
  __int16  unk_0[4];     ///< offset=0x00
  camera_t camera;       ///< offset=0x08
  float    frustum[127]; ///< offset=0x5c
} window_parameters_t;

/// size=0x28
typedef struct {
  uint32_t unk_0;  ///< offset=0x00
  uint32_t unk_4;  ///< offset=0x04
  int64_t  unk_8;  ///< offset=0x08
  int64_t  unk_16; ///< offset=0x10
  int64_t  unk_24; ///< offset=0x18
  int64_t  unk_32; ///< offset=0x20
} unk_time_globals_t;

#pragma pack()
