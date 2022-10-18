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
#define true 1
#define false 0 
#define NONE -1

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
typedef unsigned short wchar_t;
typedef unsigned int size_t;

// FIXME: Normalize
typedef uint32_t _DWORD;
typedef uint16_t _WORD;
typedef uint8_t _BYTE;
#define __int16 short
#define __int8 char

#pragma pack(1)

/// size=0x0C
typedef struct {
  float x; ///< offset=0x00
  float y; ///< offset=0x04
  float z; ///< offset=0x08
} vector3_t;

/// size=0x04
typedef union {
  int32_t value;   ///< offset=0x00
  struct {
    int16_t index; ///< offset=0x00
    int16_t salt;  ///< offset=0x02
  };
} datum_handle_t;

static const datum_handle_t INVALID_DATUM_HANDLE = { -1 };

/// size=0x10c
typedef struct {
  uint32_t unk_0;         ///< offset=0x00
  uint16_t unk_4;         ///< offset=0x04
  int16_t  difficulty;    ///< offset=0x06
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
  bool     unk_2;       ///< offset=0x02
  char     unk_3[9];    ///< offset=0x03
  uint32_t local_time;  ///< offset=0x0c
  uint16_t elapsed;     ///< offset=0x10
  char     unk_18[6];   ///< offset=0x12
  float    speed;       ///< offset=0x18
  uint32_t unk_28;      ///< offset=0x1c
} game_time_globals_t;

/// size=0x98
typedef struct {
  bool object_is_being_placed;                ///< offset=0x00    see .text:0013F060 _objects_place
  bool object_marker_initialized;             ///< offset=0x01    see .text:0013EB70 _object_marker_begin
  bool garbage_collect_now;                   ///< offset=0x02    see .text:0013DB50 _garbage_collect_now
  char unk_3;                                 ///< offset=0x03    padding?
  uint16_t unk_4;                             ///< offset=0x04    see .text:00144B50 _objects_garbage_collection & .text:001444F0 _object_update
  uint16_t unk_6;                             ///< offset=0x06    padding?
  datum_handle_t unk_8;                       ///< offset=0x08    see .text:0013D939 mov     eax, [ecx+8]   datum handle for object_header_data
  char combined_pvs[64];                      ///< offset=0x0C    see .text:0013F9A1 add     edx, 0Ch
  char combined_pvs_local[64];                ///< offset=0x4C    see .text:0013F9B2 add     eax, 4Ch
  uint32_t last_garbage_collection_tick;      ///< offset=0x8C    see .text:00144EF6 mov     edx, [ecx+8Ch]
  uint16_t pvs_activator_type;                ///< offset=0x90    see .text:0013DBE0 _object_pvs_set_object
  uint16_t unk_146;                           ///< offset=0x92    padding?
  datum_handle_t pvs_activator_object_index;  ///< offset=0x94    see .text:0013DBE0 _object_pvs_set_object & .text:0013DCE4 mov     ecx, [ecx+94h]
} object_globals_t;

/// size=0x1A4
typedef struct {
  uint32_t tag_index;
  char unk_4[0x1A0];
} object_data_t;

/// size=0xc
typedef struct {
  uint16_t unk_0;         ///< offset=0x00
  uint8_t unk_2;          ///< offset=0x02  see .text:0013FF78                 or      byte ptr [esi+2], 40h  flags
  uint8_t type;           ///< offset=0x03  see .text:000F68C3                 movzx   ax, byte ptr [eax+3]
  uint16_t unk_4;         ///< offset=0x04  cluster_index?
  uint16_t data_size;     ///< offset=0x06  see .text:0013E015                 movsx   eax, word ptr [edi+6]
  object_data_t* object;  ///< offset=0x08  see .text:0013D80E                 mov     esi, [eax+8]
} object_header_data_t;

// OBJE -> UNIT
/// size=0x424
typedef struct {
  object_data_t object;   ///< offset=0x000
  char unk_420[0x280];    ///< offset=0x1A4
} unit_data_t;

// OBJE -> UNIT -> BIPD
/// size=0x480
typedef struct {
  unit_data_t unit;       ///< offset=0x000
  char unk_1060[0x5C];    ///< offset=0x424
} biped_data_t;

// OBJE -> UNIT -> VEHI
/// size=0x47C
typedef struct {
  unit_data_t unit;       ///< offset=0x000
  char unk_1060[0x58];    ///< offset=0x424
} vehicle_data_t;

// OBJE -> ITEM
/// size=0x1DC
typedef struct {
  object_data_t object;   ///< offset=0x000
  char unk_420[0x38];     ///< offset=0x1A4
} item_data_t;

// OBJE -> ITEM -> WEAP
/// size=0x27C
typedef struct {
  item_data_t item;       ///< offset=0x000
  char unk_476[0xA0];     ///< offset=0x1DC
} weapon_data_t;

// OBJE -> ITEM -> EQUI
/// size=0x1F4
typedef struct {
  item_data_t item;       ///< offset=0x000
  char unk_476[0x18];     ///< offset=0x1DC
} equipment_data_t;

// OBJE -> ITEM -> GARB
/// size=0x1F4
typedef struct {
  item_data_t item;       ///< offset=0x000
  char unk_476[0x18];     ///< offset=0x1DC
} garbage_data_t;

// OBJE -> PROJ
/// size=0x228
typedef struct {
  object_data_t object;   ///< offset=0x000
  char unk_420[0x84];     ///< offset=0x1A4
} projectile_data_t;

// OBJE -> SCEN
/// size=0x1A8
typedef struct {
  object_data_t object;   ///< offset=0x000
  char unk_420[4];        ///< offset=0x1A4
} scenery_data_t;

// OBJE -> DEVI
/// size=0x1C4
typedef struct {
  object_data_t object;   ///< offset=0x000
  char unk_420[0x20];     ///< offset=0x1A4
} device_data_t;

// OBJE -> DEVI -> MACH
/// size=0x1D8
typedef struct {
  device_data_t device;   ///< offset=0x000
  char unk_452[0x14];     ///< offset=0x1C4
} machine_data_t;

// OBJE -> DEVI -> CTRL
/// size=0x1CC
typedef struct {
  device_data_t device;   ///< offset=0x000
  char unk_452[8];        ///< offset=0x1C4
} control_data_t;

// OBJE -> DEVI -> LIFI
/// size=0x1DC
typedef struct {
  device_data_t device;   ///< offset=0x000
  char unk_452[0x18];     ///< offset=0x1C4
} light_fixture_data_t;

// OBJE -> PLAC
/// size=0x1FC
typedef struct {
  object_data_t object;   ///< offset=0x000
  char unk_420[0x58];     ///< offset=0x1A4
} placeholder_data_t;

// OBJE -> SSCE
/// size=0x1A8
typedef struct {
  object_data_t object;   ///< offset=0x000
  char unk_420[4];        ///< offset=0x1A4
} sound_scenery_data_t;

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

/// size=0x54
typedef struct {
  vector3_t         unk_0;                  ///< offset=0x00
  vector3_t         unk_12;                 ///< offset=0x0c
  vector3_t         unk_24;                 ///< offset=0x18
  uint8_t           unk_36;                 ///< offset=0x24
  char              unk_37[3];              ///< offset=0x25
  float             vertical_field_of_view; ///< offset=0x28
  viewport_bounds_t viewport_bounds;        ///< offset=0x2c
  viewport_bounds_t unk_52;                 ///< offset=0x34
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

/// size=0xAC
typedef struct
{
  __int16           player;      ///< offset=0x00
  bool              unk_2;       ///< offset=0x02
  char              unk_3[129];  ///< offset=0x03
  viewport_bounds_t unk_132;     ///< offset=0x84
  viewport_bounds_t unk_140;     ///< offset=0x8c
  char              unk_148[24]; ///< offset=0x94
} window_t;

// FIXME: Structure size
/// size=0xF0
typedef struct
{
  _BYTE unk_0[60];   ///< offset=0x00
  _WORD type;        ///< offset=0x3C
  _BYTE unk_62[174]; ///< offset=0x3E
  int   unk_236;     ///< offset=0xEC
} scenario_t;

// FIXME: Merge adjacent globals into this structure
/// size=0x01
typedef struct
{
  bool main_menu_scenario_loaded; ///< offset=0x00
} main_globals_t;

/// size=0x10C
typedef struct
{
  uint32_t magic;      ///< offset=0x00
  char     unk_4[2];   ///< offset=0x04
  int16_t  unk_6;      ///< offset=0x06
  char     unk_8[260]; ///< offset=0x08
} file_ref_t;

/// size=0x1C
typedef struct
{
  char unk_0[10];      ///< offset=0x00
  bool can_be_skipped; ///< offset=0x0A
  char unk_11[17];     ///< offset=0x0B
} cinematic_globals_t;

#pragma pack()
