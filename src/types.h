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
typedef unsigned short wchar_t;
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

typedef union {
  int32_t handle;
  struct {
    int16_t index;
    int16_t salt;
  };
} datum_index_t;

static const datum_index_t INVALID_DATUM_HANDLE = { -1 };

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

/// size=0xd4
typedef struct {
  char unk0[2]; // salt? and this is actually a datum index?
  int16_t local_player_index;

  // .text:000AC599                 add     ecx, 4
  // .text:000BB398                 mov     word ptr [esi+1Ah], 0 ; null terminator
  wchar_t player_name[12];  

  int32_t unk_28; // .text:000BB3A8                 mov     [esi+1Ch], edi

  // .text:00085A85                 cmp     [eax+20h], esi  datum_index_t team? or raw index - .text:000B1719                 mov     ecx, dword_456BA8[eax*4]
  // .text:000AD07B                 mov     ax, [edx+20h]
  // .text:000BB3B9                 mov     dword ptr [esi+20h], 1
  int32_t unk_32;

  int32_t unk_36; // .text:000BAEC6                 mov     dword ptr [eax+24h], 0FFFFFFFFh
  int16_t unk_40; // .text:000BAEC0                 mov     word ptr [eax+28h], 0
  int16_t unk_42; // .text:000BC0C1                 mov     [ebx+2Ah], ax
  int16_t unk_44; // .text:000A8CF3                 mov     eax, [esi+2Ch]

  int16_t unk_46;

  int32_t unk_48; // .text:000AF707                 mov     edx, [esi+30h]
  datum_index_t slave_unit_index; // mov     eax, [esi+34h] 
  datum_index_t last_slave_unit_index;  // .text:00085C51                 mov     eax, [eax+38h]
  
  // .text:000BAD03                 mov     [esi+3Ch], ax
  // .text:000BB951                 mov     word ptr [edi+3Ch], 0FFFFh
  int16_t unk_60;
  int16_t unk_62; // .text:000BD4CD                 mov     [edx+3Eh], al
  int32_t unk_64; // .text:000BB3AF                 mov     [esi+40h], edi

  char unk_68[36];

  // .text:000BBF8E                 lea     ecx, [edi+68h]
  // .text:000BC391                 cmp     word ptr [ebx+esi*2+68h], 0
  // .text:000BC404                 add     [ebx+esi*2+68h], dx
  // .text:000D12A4                 movsx   ecx, word ptr [edi+6Ah]
  int16_t unk_104;
  int16_t unk_106;

  // .text:000A81F8                 mov     dword ptr [eax+6Ch], 3F800000h  speed for score
  // .text:000B4C27                 fld     dword ptr [esi+6Ch]
  // .text:001A5A9C                 fld     dword ptr [eax+6Ch]
  float unk_108;
  
  // .text:000A81FF                 mov     [eax+70h], ecx
  // .text:000AD696                 mov     dword ptr [esi+70h], 0FFFFFFFFh
  uint32_t unk_112;

  // .text:000A998C                 mov     [eax+74h], edx
  // .text:000A81F2                 mov     [eax+74h], ecx
  uint32_t unk_116;

  // .text:000A998F                 mov     [eax+78h], ecx
  // .text:000A81F5                 mov     [eax+78h], ecx
  uint32_t unk_120;

  uint32_t unk_124; // .text:000A8202                 mov     [eax+7Ch], ecx
  uint32_t unk_128; // .text:000AC424                 mov     ecx, [esi+80h]
  uint32_t unk_132; // .text:000A8BFA                 mov     eax, [eax+84h]

  // .text:000B3916                 mov     dword ptr [eax+88h], 0
  // .text:000B4B26                 mov     dword ptr [eax+88h], 0FFFFFFFFh
  uint32_t unk_136; 

  uint16_t unk_140;

  uint16_t unk_142; // .text:000B56A3                 mov     word ptr [esi+8Eh], 1
  uint16_t unk_144; // .text:000B56AE                 inc     word ptr [esi+90h]
  uint16_t unk_146; // .text:000B575C                 mov     [eax+92h], si
  uint16_t unk_148; // .text:000B576C                 mov     [eax+94h], si
  uint16_t unk_150; // .text:000B5763                 mov     word ptr [eax+96h], 0FFFFh
  uint16_t unk_152; // .text:000ABE0C                 movsx   edx, word ptr [ebx+98h]

  char unk_154[6];

  uint16_t unk_160; // .text:000ABE16                 movsx   eax, word ptr [ebx+0A0h]

  char unk_162[6];

  uint16_t unk_168; // .text:000B58E6                 inc     word ptr [esi+0A8h]

  // .text:00086E9C                 cmp     [eax+0AAh], bx
  // .text:000A8C6A                 movsx   edx, word ptr [eax+0AAh]
  uint16_t unk_170;

  uint16_t unk_172; // .text:000B5749                 inc     word ptr [eax+0ACh]

  char unk_174[10];

  uint32_t unk_184; // .text:000B569D                 mov     [esi+0B8h], edx

  uint32_t unk_188;

  // .text:000AE80F                 add     eax, 0C0h memset size of 4 (array of two words?)
  // .text:000B166F                 inc     word ptr [esi+0C0h]
  // .text:000B0F78                 inc     word ptr [esi+0C2h]
  uint16_t unk_192;
  uint16_t unk_194;

  // .text:000B008D                 inc     word ptr [edi+0C4h]
  // .text:000B04FD                 movsx   eax, word ptr [eax+0C4h]  score
  uint16_t unk_196;

  uint16_t unk_198; // padding?

  uint32_t unk_200; // .text:000BDC78                 mov     eax, [esi+0C8h]

  // .text:000A71F0                 mov     eax, [esi+0CCh]
  // .text:000C9AD0                 cmp     [eax+0CCh], edi
  uint32_t unk_204;

  bool unk_208; // .text:000BDC70                 mov     al, [esi+0D0h]

  // .text:000A71FB                 mov     cl, [esi+0D1h]
  // .text:000A720F                 mov     byte ptr [esi+0D1h], 1
  bool unk_209;

  uint16_t unk_210;
} player_data_t;

/// size=0x40
typedef struct {
  char unk_0[0x40];
} team_data_t;

/// size=0xb0
typedef struct {
  int unk_0;                                                         //< offset=0x00 see local_player_get_next, players_compute_local_player_count
  datum_index_t local_players[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];      //< offset=0x04
  datum_index_t local_dead_players[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS]; //< offset=0x14
  int16_t local_player_count;                                        //< offset=0x24
  int16_t double_speed_ticks_remaining;                              //< offset=0x26
  bool are_all_dead;                                                 //< offset=0x28
  bool input_disabled;                                               //< offset=0x29
  int16_t unk_tag_index;                                             //< offset=0x2A bsp index? see players_reconnect_to_structure_bsp pulling from tag block at scnr+0x39C
  int16_t respawn_failure;                                           //< offset=0x2C enum, see _players_get_respawn_failure
  bool teleported;                                                   //< offset=0x2E
  char unk_flags;                                                    //< offset=0x2F bsp-related flags? see players_update_after_game
  char combined_pvs[0x40];                                           //< offset=0x30
  char combined_pvs_local[0x40];                                     //< offset=0x70
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
  data_t        *data;        ///< offset=0x00
  uint16_t      index;        ///< offset=0x04
  char          unk_6[2];     ///< offset=0x06
  datum_index_t datum_index;  ///< offset=0x08
  uint32_t      cookie;       ///< offset=0x0c
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
  __int16           local_player_index; ///< offset=0x00
  bool              unk_2;              ///< offset=0x02
  char              unk_3[129];         ///< offset=0x03
  viewport_bounds_t unk_132;            ///< offset=0x84
  viewport_bounds_t unk_140;            ///< offset=0x8c
  char              unk_148[24];        ///< offset=0x94
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
