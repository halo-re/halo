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

#define NUMBER_OF_OUTGOING_OBJECT_FUNCTIONS 4
#define MAXIMUM_REGIONS_PER_OBJECT 8

/// size=0x1A4
typedef struct {
  uint32_t tag_index;       ///< offset=0x00
  uint32_t flags;           ///< offset=0x04  .text:00095B7B                 mov     [esi+4], ecx
  uint32_t unk_8;           ///< offset=0x08  .text:0013EC41                 mov     edx, [esi+8] compared against global object marker
  vector3_t unk_12;         ///< offset=0x0C
  vector3_t unk_24;         ///< offset=0x18
  vector3_t unk_36;         ///< offset=0x24
  vector3_t unk_48;         ///< offset=0x30
  vector3_t unk_60;         ///< offset=0x3C
  uint32_t unk_72;          ///< offset=0x48  .text:00140149                 mov     edx, [ecx+48h] location.???, leaf index?

  // .text:00031FEE                 mov     edx, [eax+4Ch]  
  // .text:00034D1F                 movsx   eax, word ptr [eax+4Ch] object.location.cluster_index
  datum_handle_t unk_76;    ///< offset=0x4C

  float unk_80;             ///< offset=0x50  .text:0009D161                 fsub    dword ptr [ebx+50h]
  float unk_84;             ///< offset=0x54  .text:0009D167                 fsub    dword ptr [ebx+54h]
  float unk_88;             ///< offset=0x58  .text:0009D16D                 fsub    dword ptr [ebx+58h]
  float unk_92;             ///< offset=0x5C  .text:0009D155                 fld     dword ptr [ebx+5Ch]
  float unk_96;             ///< offset=0x60  .text:00141E5B                 fld     dword ptr [esi+60h]
  int16_t type;             ///< offset=0x64  .text:0013D811                 movsx   ecx, word ptr [esi+64h] type enum
  int16_t unk_102;          ///< offset=0x66
  int16_t unk_104;          ///< offset=0x68  .text:00032344                 movsx   eax, word ptr [eax+68h] team-related index
  int16_t unk_106;          ///< offset=0x6A
  int16_t unk_108;          ///< offset=0x6C  .text:000A8741                 cmp     [eax+6Ch], si
  int16_t unk_110;          ///< offset=0x6E  .text:0003EC0B                 cmp     word ptr [edi+6Eh], 64h
  uint32_t unk_112;         ///< offset=0x70  .text:00143FFA                 mov     [edi+70h], edx
  uint32_t unk_116;         ///< offset=0x74  .text:000348B6                 mov     eax, [ecx+74h]
  uint32_t unk_120;         ///< offset=0x78
  uint32_t unk_124;         ///< offset=0x7C  .text:00141C2B                 mov     eax, [esi+7Ch]
  int16_t unk_128;          ///< offset=0x80  .text:00141C3E                 cmp     word ptr [esi+80h], 0FFFFh  animation related, possibly datum index
  int16_t unk_130;          ///< offset=0x82  .text:000FBB39                 mov     word ptr [esi+82h], 0  animation related, possibly datum index
  int16_t unk_132;          ///< offset=0x84  .text:001401F8                 movsx   edx, word ptr [esi+84h]
  int16_t unk_134;          ///< offset=0x86  .text:001401FF                 movsx   ecx, word ptr [esi+86h]
  uint32_t unk_136;         ///< offset=0x88  .text:00136654                 mov     [esi+88h], ecx   float?   body vitality?
  float unk_140;            ///< offset=0x8C  .text:000C9C40                 fmul    dword ptr [ecx+8Ch]  shield vitality?
  float unk_144;            ///< offset=0x90  .text:00136675                 fstp    dword ptr [esi+90h]  shield/vitality related
  float unk_148;            ///< offset=0x94  .text:000C9C46                 fstp    dword ptr [ecx+94h]  shield related, double charge?
  uint32_t unk_152;         ///< offset=0x98  .text:00136BA8                 mov     dword ptr [esi+98h], 0   float? shield
  float unk_156;            ///< offset=0x9C  .text:0001FA9E                 fld     dword ptr [edi+9Ch]
  uint32_t unk_160;         ///< offset=0xA0  .text:00137F25                 cmp     dword ptr [ebx+0A0h], 0FFFFFFFFh   datum_handle?
  float unk_164;            ///< offset=0xA4  .text:00138865                 fld     dword ptr [esi+0A4h]
  float unk_168;            ///< offset=0xA8  .text:001387AF                 fld     dword ptr [esi+0A8h]
  uint32_t unk_172;         ///< offset=0xAC  .text:00143FB0                 mov     [edi+0ACh], eax
  uint32_t unk_176;         ///< offset=0xB0  .text:0013877C                 mov     eax, [esi+0B0h]   datum_handle?

  // 32-bit flags?
  int16_t unk_180;          ///< offset=0xB4  .text:00138775                 mov     [esi+0B4h], ax
  int8_t unk_182;           ///< offset=0xB6  .text:00018832                 or      byte ptr [eax+0B6h], 40h
  int8_t unk_183;           ///< offset=0xB7  .text:0003B35D                 test    byte ptr [eax+0B7h], 1  ranged weapon

  uint32_t unk_184;         ///< offset=0xB8
  uint32_t unk_188;         ///< offset=0xBC  .text:00143F81                 mov     [edi+0BCh], eax
  uint32_t unk_192;         ///< offset=0xC0
  datum_handle_t next_object_index;   ///< offset=0xC4  .text:0014537B                 mov     ecx, [eax+0C4h]
  datum_handle_t unk_200;   ///< offset=0xC8  .text:000320C3                 mov     eax, [edi+0C8h]
  datum_handle_t parent_object_index;   ///< offset=0xCC  .text:00145348                 mov     ecx, [eax+0CCh]
  float unk_208[5];         ///< offset=0xD0  .text:0013E640                 fld     dword ptr [ebx+edx*4+0D0h] Colors related, 5 4-byte elements
  float unk_228[NUMBER_OF_OUTGOING_OBJECT_FUNCTIONS]; ///< offset=0xE4  .text:001403FF                 mov     edx, [esi+ecx*4+0E4h] function stuff
  char unk_244[8];          ///< offset=0xF4
  char unk_252[32];         ///< offset=0xFC  .text:00097B90                 mov     eax, [edx+ecx*4+0FCh] illumination related?
  uint32_t unk_284;         ///< offset=0x11C .text:0013617A                 mov     dword ptr [esi+11Ch], 0FFFFFFFFh widget-related?
  uint32_t unk_288;         ///< offset=0x120 .text:00143F94                 mov     [edi+120h], eax
  uint16_t unk_292;         ///< offset=0x124 .text:001376FF                 movzx   eax, word ptr [ebx+124h] region-related?
  uint16_t unk_294;         ///< offset=0x126 .text:00144015                 mov     [edi+126h], dx
  uint8_t unk_296[MAXIMUM_REGIONS_PER_OBJECT];  ///< offset=0x128 .text:00141B01                 movzx   eax, byte ptr [edx+edi+128h]
  uint8_t unk_304[MAXIMUM_REGIONS_PER_OBJECT];  ///< offset=0x130 .text:00136A62                 mov     [esi+ebx+130h], al  shield/vitality regions?
  char unk_312[0x60];       ///< offset=0x138 .text:0013E21D                 lea     ebx, [edi+138h] color stuff
  uint32_t unk_408;         ///< offset=0x198 .text:001401E2                 lea     edx, [esi+198h] mode related
  uint32_t unk_412;         ///< offset=0x19C .text:001401D1                 lea     ecx, [esi+19Ch] mode related
  uint32_t unk_416;         ///< offset=0x1A0 .text:00140EF1                 add     eax, 1A0h node matrix reference?
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

#define MAXIMUM_WEAPONS_PER_UNIT 4
#define NUMBER_OF_UNIT_GRENADE_TYPES 2

// OBJE -> UNIT
/// size=0x424
typedef struct {
  object_data_t object;               ///< offset=0x000
  datum_handle_t actor_index;         ///< offset=0x1A4 .text:0003EB73                 cmp     dword ptr [edi+1A4h], 0FFFFFFFFh
  datum_handle_t swarm_actor_index;   ///< offset=0x1A8 .text:0003EB9C                 cmp     dword ptr [edi+1A8h], 0FFFFFFFFh
  datum_handle_t unk_428;             ///< offset=0x1AC .text:00031492                 mov     eax, [eax+1ACh]
  datum_handle_t unk_432;             ///< offset=0x1B0 .text:0003AF89                 mov     eax, [esi+1B0h]   datum index?
  uint32_t unk_436;                   ///< offset=0x1B4 .text:001A80D0                 test    dword ptr [esi+1B4h], 400000h   flags
  uint32_t unk_440;                   ///< offset=0x1B8 .text:000D93E7                 mov     ecx, [eax+1B8h] flags
  uint16_t unk_444;                   ///< offset=0x1BC .text:001B3701                 inc     word ptr [ebx+1BCh]
  uint8_t unk_446;                    ///< offset=0x1BE .text:001A8125                 movsx   edx, byte ptr [esi+1BEh]
  uint8_t unk_447;                    ///< offset=0x1BF .text:001AE773                 mov     [esi+1BFh], al    seat index
  uint32_t unk_448;                   ///< offset=0x1C0 .text:001A81DA                 mov     [esi+1C0h], ecx
  uint32_t persistent_control_flags;  ///< offset=0x1C4 .text:001A81D3                 mov     [esi+1C4h], edi
  datum_handle_t unk_456;             ///< offset=0x1C8 .text:00030656                 cmp     dword ptr [ebx+1C8h], 0FFFFFFFFh
  uint16_t unk_460;                   ///< offset=0x1CC .text:0004091B                 cmp     bx, [esi+1CCh]
  uint16_t unk_462;                   ///< offset=0x1CE .text:001A9B5E                 mov     [esi+1CEh], ax
  uint32_t unk_464;                   ///< offset=0x1D0 .text:00040924                 mov     ecx, [esi+1D0h] game time related
  vector3_t unk_468;                  ///< offset=0x1D4 .text:001AF62B                 lea     ecx, [esi+1D4h]
  vector3_t unk_480;                  ///< offset=0x1E0 .text:001AF63E                 lea     edx, [esi+1E0h]
  vector3_t unk_492;                  ///< offset=0x1EC .text:001AF678                 lea     eax, [esi+1ECh]
  vector3_t unk_504;                  ///< offset=0x1F8 .text:001AF7E5                 fld     dword ptr [esi+1F8h]
  vector3_t unk_516;                  ///< offset=0x204 .text:001AF651                 lea     eax, [esi+204h]
  vector3_t unk_528;                  ///< offset=0x210 .text:001AF68B                 add     esi, 210h
  vector3_t unk_540;                  ///< offset=0x21C .text:001AF82F                 fld     dword ptr [esi+21Ch]
  vector3_t unk_552;                  ///< offset=0x228 .text:001B39A3                 lea     edx, [ebx+228h]
  float unk_564;                      ///< offset=0x234 .text:001B387D                 mov     dword ptr [ebx+234h], 3F800000h
  uint8_t unk_568;                    ///< offset=0x238
  uint8_t unk_569;                    ///< offset=0x239 .text:001ABDB1                 mov     cl, [esi+239h]
  uint8_t unk_570;                    ///< offset=0x23A .text:001ABDE5                 mov     cl, [esi+23Ah]
  uint8_t unk_571;                    ///< offset=0x23B .text:001B0EC8                 mov     al, [edi+23Bh]
  uint8_t unk_572;                    ///< offset=0x23C .text:0003D1D3                 mov     [eax+23Ch], bl
  uint8_t unk_573;                    ///< offset=0x23D .text:001AB0FB                 mov     byte ptr [esi+23Dh], 3
  uint8_t unk_574;                    ///< offset=0x23E .text:001AB2F7                 movsx   ecx, word ptr [esi+23Eh]
  uint8_t unk_575;                    ///< offset=0x23F padding?
  uint16_t unk_576;                   ///< offset=0x240 .text:001AB2FE                 movsx   edx, word ptr [esi+240h]
  uint16_t unk_578;                   ///< offset=0x242
  datum_handle_t unk_580;             ///< offset=0x244 .text:001AB147                 mov     edi, [esi+244h]
  uint8_t unk_584;                    ///< offset=0x248 .text:001ACF38                 or      byte ptr [eax+248h], 2
  uint8_t unk_585;                    ///< offset=0x249
  uint16_t unk_586;                   ///< offset=0x24A .text:001A8C5D                 cmp     word ptr [esi+24Ah], 0FFFFh
  uint16_t unk_588;                   ///< offset=0x24C .text:001AD69B                 mov     [esi+24Ch], ax
  uint16_t unk_590;                   ///< offset=0x24E .text:001B2887                 mov     [esi+24Eh], bx
  uint8_t unk_592;                    ///< offset=0x250 .text:001A841D                 movsx   ecx, byte ptr [esi+250h]
  uint8_t unk_593;                    ///< offset=0x251 .text:001A8432                 movsx   ecx, byte ptr [esi+251h]
  uint8_t unk_594;                    ///< offset=0x252 .text:001A8A0C                 movsx   eax, byte ptr [esi+252h]
  uint8_t unk_595;                    ///< offset=0x253 .text:001A8B46                 movsx   eax, byte ptr [esi+253h]
  uint8_t unk_596;                    ///< offset=0x254 .text:001A8AEB                 mov     [esi+254h], cl
  uint8_t unk_597;                    ///< offset=0x255 .text:001A8B31                 movsx   cx, byte ptr [esi+255h]
  uint8_t unk_598;                    ///< offset=0x256 .text:001B0E17                 movsx   eax, byte ptr [edi+256h]
  uint8_t base_seat_index;            ///< offset=0x257 .text:001AE2E8                 movsx   si, byte ptr [esi+257h]
  uint8_t unk_600;                    ///< offset=0x258 .text:001AC08A                 mov     [eax+258h], cl
  uint8_t unk_601;                    ///< offset=0x259 padding? 
  uint16_t unk_602;                   ///< offset=0x25A .text:001AFD69                 mov     ax, [esi+25Ah]   tag block index
  uint16_t unk_604;                   ///< offset=0x25C .text:001AFD7E                 mov     cx, [esi+25Ch]   tag block index
  uint16_t unk_606;                   ///< offset=0x25E .text:001AFDA5                 mov     ax, [esi+25Eh]   tag block index
  uint16_t unk_608;                   ///< offset=0x260 .text:001AFDB4                 mov     cx, [esi+260h]   tag block index
  uint16_t unk_610;                   ///< offset=0x262 .text:001AFDDB                 mov     ax, [esi+262h]   tag block index
  uint16_t unk_612;                   ///< offset=0x264 .text:001AFDEA                 mov     cx, [esi+264h]   tag block index
  uint8_t unk_614;                    ///< offset=0x266 .text:001ADAB2                 mov     bl, [eax+266h]
  uint8_t unk_615;                    ///< offset=0x267 .text:001ADAC0                 mov     bl, [eax+267h]
  float unk_616;                      ///< offset=0x268 .text:001ADAB8                 lea     edi, [eax+268h]  quat?
  float unk_620;                      ///< offset=0x26C .text:001B0225                 fstp    dword ptr [esi+26Ch]
  float unk_624;                      ///< offset=0x270 .text:001B0244                 fstp    dword ptr [esi+270h]
  float unk_628;                      ///< offset=0x274 .text:001B0263                 fstp    dword ptr [esi+274h]
  float unk_632;                      ///< offset=0x278 .text:001ADAC6                 lea     edi, [eax+278h]  quat?
  float unk_636;                      ///< offset=0x27C .text:001B0459                 fstp    dword ptr [esi+27Ch]
  float unk_640;                      ///< offset=0x280 .text:001B0472                 fstp    dword ptr [esi+280h]
  float unk_644;                      ///< offset=0x284 .text:001B0491                 fstp    dword ptr [esi+284h]
  float unk_648;                      ///< offset=0x288
  uint32_t unk_652;                   ///< offset=0x28C
  float unk_656;                      ///< offset=0x290 .text:001AB900                 fstp    dword ptr [esi+290h]   rgb color brightness
  float unk_660;                      ///< offset=0x294 .text:001AB90C                 fstp    dword ptr [esi+294h]   self illumination
  float unk_664;                      ///< offset=0x298 .text:001A80AC                 fld     dword ptr [esi+298h]
  uint32_t unk_668;                   ///< offset=0x29C
  uint16_t unk_672;                   ///< offset=0x2A0 .text:000B6805                 movsx   ecx, word ptr [edi+2A0h]   tag block index
  uint16_t unk_674;                   ///< offset=0x2A2 .text:000B0B06                 mov     ax, [ebx+2A2h]   current weapon index into (0x2A8)
  uint16_t unk_676;                   ///< offset=0x2A4 .text:000B707C                 mov     ax, [ebx+2A4h]   next weapon index
  uint16_t unk_678;                   ///< offset=0x2A6
  datum_handle_t unk_680[MAXIMUM_WEAPONS_PER_UNIT]; ///< offset=0x2A8 .text:001AAD23                 mov     eax, [edi+ecx*4+2A8h]
  datum_handle_t unk_696[MAXIMUM_WEAPONS_PER_UNIT]; ///< offset=0x2B8 .text:001B1E5D                 mov     dword ptr [edi+eax*4+2B8h], 0
  datum_handle_t unk_712;             ///< offset=0x2C8 .text:001AA97E                 mov     eax, [eax+2C8h] current equipment
  uint8_t current_grenade_index;      ///< offset=0x2CC .text:001AAEF1                 mov     al, [esi+2CCh] unit->unit.current_grenade_index
  uint8_t unk_717;                    ///< offset=0x2CD .text:000B7087                 movsx   cx, byte ptr [ebx+2CDh]
  uint8_t unk_718[NUMBER_OF_UNIT_GRENADE_TYPES];  ///< offset=0x2CE .text:001A99E3                 cmp     byte ptr [ecx+ebx+2CEh], 0   grenade counts
  uint8_t zoom_level;                 ///< offset=0x2D0 .text:001A869E                 movsx   ax, byte ptr [eax+2D0h]
  uint8_t unk_721;                    ///< offset=0x2D1 .text:000B7093                 movsx   dx, byte ptr [ebx+2D1h]
  uint8_t unk_722;                    ///< offset=0x2D2
  uint8_t unk_723;                    ///< offset=0x2D3 .text:001A8090                 movzx   edx, byte ptr [esi+2D3h]
  datum_handle_t unk_724;             ///< offset=0x2D4 .text:001AA4DE                 mov     ecx, [eax+2D4h]
  datum_handle_t unk_728;             ///< offset=0x2D8 .text:000D8D8D                 cmp     [eax+2D8h], edi
  uint32_t unk_732;                   ///< offset=0x2DC .text:0003AC65                 mov     ecx, [esi+2DCh]  game time related
  uint32_t unk_736;                   ///< offset=0x2E0 .text:000BC220                 mov     [ebx+2E0h], eax  game time related
  uint16_t unk_740;                   ///< offset=0x2E4 .text:00057E26                 mov     ax, [eax+2E4h]   actor related
  uint16_t unk_742;                   ///< offset=0x2E6 .text:0003DF30                 mov     ax, [edi+2E6h]   squad related
  float unk_744;                      ///< offset=0x2E8 .text:001A8078                 fld     dword ptr [esi+2E8h]
  float unk_748;                      ///< offset=0x2EC .text:001A8085                 fld     dword ptr [esi+2ECh]
  float unk_752;                      ///< offset=0x2F0 .text:000D7F87                 cmp     dword ptr [ecx+2F0h], 3F800000h
  float unk_756;                      ///< offset=0x2F4 .text:000D7FAD                 fld     dword ptr [ecx+2F4h]
  float unk_760;                      ///< offset=0x2F8 .text:001B1337                 mov     dword ptr [edi+2F8h], 0  zoom-related?
  vector3_t unk_764;                  ///< offset=0x2FC .text:001AB4F7                 lea     ecx, [esi+2FCh]
  uint16_t powerup_type;              ///< offset=0x308 .text:001AA9DD                 cmp     word ptr [esi+308h], 6  equipment_definition->equipment.powerup_type
  uint16_t unk_778;                   ///< offset=0x30A .text:001AA9BD                 movsx   eax, word ptr [esi+30Ah]
  float unk_782;                      ///< offset=0x30C .text:001AB528                 fsub    dword ptr [esi+30Ch]
  float unk_786;                      ///< offset=0x310 .text:001AB531                 fsub    dword ptr [esi+310h]
  vector3_t unk_790;                  ///< offset=0x314 .text:001B46F1                 fld     dword ptr [ebx+314h]
  vector3_t unk_800;                  ///< offset=0x320 .text:001AB57C                 fstp    dword ptr [esi+320h]
  float unk_812;                      ///< offset=0x32C .text:0013B79C                 fld     dword ptr [edi+32Ch]
  float unk_816;                      ///< offset=0x330 .text:0003CA20                 fstp    dword ptr [eax+330h]
  uint32_t unk_820;                   ///< offset=0x334 .text:001A698A                 mov     eax, [ecx+334h]  tag index
  uint16_t unk_824;                   ///< offset=0x338 .text:001A6BD3                 cmp     [eax+338h], cx  command type?, also see action_obey_command_perform
  uint16_t unk_826;                   ///< offset=0x33A .text:001A6D9A                 mov     ax, [edi+33Ah]
  uint32_t unk_828;                   ///< offset=0x33C .text:001A6D4B                 mov     eax, [edi+33Ch]  tag index
  uint16_t unk_832;                   ///< offset=0x340 .text:001A6FB3                 mov     ax, [ebx+340h]
  uint16_t unk_834;                   ///< offset=0x342 .text:001A6FC1                 mov     dx, [ebx+342h]
  uint16_t unk_836;                   ///< offset=0x344 .text:001A6FBA                 mov     cx, [ebx+344h]
  uint16_t unk_838;                   ///< offset=0x346
  uint8_t unk_840[0x20];              ///< offset=0x348 .text:001A7913                 lea     eax, [esi+348h]  4th arg to ai_communication_started
  uint16_t unk_872;                   ///< offset=0x368 .text:001A6A73                 mov     dx, [ecx+368h]
  uint8_t unk_874[0x2E];              ///< offset=0x36A
  uint16_t unk_920;                   ///< offset=0x398 .text:001A77D9                 mov     [esi+398h], ax
  uint16_t unk_922;                   ///< offset=0x39A .text:001A77E4                 mov     ax, [esi+39Ah]
  uint16_t unk_924;                   ///< offset=0x39C .text:001A7803                 mov     ax, [esi+39Ch]
  uint16_t unk_926;                   ///< offset=0x39E
  uint32_t unk_928;                   ///< offset=0x3A0 .text:001A6BA2                 mov     edx, [ecx+3A0h]
  uint8_t unk_932;                    ///< offset=0x3A4 .text:001A718B                 mov     byte ptr [esi+3A4h], 1 bool?
  uint8_t unk_933;                    ///< offset=0x3A5 .text:001A6FDF                 mov     byte ptr [ebx+3A5h], 0 bool?
  uint8_t unk_934;                    ///< offset=0x3A6 .text:001A79C5                 mov     byte ptr [esi+3A6h], 1 bool?
  uint8_t unk_935;                    ///< offset=0x3A7
  uint16_t unk_936;                   ///< offset=0x3A8 .text:001A7192                 mov     word ptr [esi+3A8h], 0
  uint16_t unk_938;                   ///< offset=0x3AA .text:00043F5E                 movsx   eax, word ptr [edi+3AAh]   vocalization timer related
  uint16_t unk_940;                   ///< offset=0x3AC .text:001A6FFE                 mov     [ebx+3ACh], dx
  uint16_t unk_942;                   ///< offset=0x3AE .text:001A6B28                 movsx   ebx, word ptr [eax+3AEh]
  uint32_t unk_944;                   ///< offset=0x3B0 .text:001A6FED                 mov     dword ptr [ebx+3B0h], 0FFFFFFFFh
  uint16_t unk_948;                   ///< offset=0x3B4 .text:001B29DF                 mov     [esi+3B4h], di
  uint16_t unk_950;                   ///< offset=0x3B6 .text:001B29E6                 mov     [esi+3B6h], di
  uint32_t unk_952;                   ///< offset=0x3B8 .text:001B29ED                 mov     [esi+3B8h], edi
  uint32_t unk_956;                   ///< offset=0x3BC .text:001B489C                 mov     dword ptr [ebx+3BCh], 0FFFFFFFFh
  datum_handle_t unk_960;             ///< offset=0x3C0 .text:0001C789                 mov     ecx, [eax+3C0h]
  float unk_964;                      ///< offset=0x3C4 .text:001AF3F9                 fsub    dword ptr [edi+3C4h]
  float unk_968;                      ///< offset=0x3C8 .text:001AF451                 fsub    dword ptr [edi+3C8h]
  datum_handle_t unk_972;             ///< offset=0x3CC .text:0002F7DC                 mov     eax, [eax+3CCh]
  uint16_t feign_death_timer;         ///< offset=0x3D0 .text:001B5025                 mov     [esi+3D0h], ax  unit->unit.feign_death_timer
  uint16_t unk_978;                   ///< offset=0x3D2 .text:000BC3C1                 mov     [eax+3D2h], si   datum index only, not a full handle?
  float unk_980;                      ///< offset=0x3D4 .text:000B7471                 fld     dword ptr [eax+3D4h]
  uint16_t unk_984;                   ///< offset=0x3D8
  uint16_t unk_986;                   ///< offset=0x3DA .text:0005BE6E                 movsx   eax, word ptr [eax+3DAh] combat related?
  uint32_t unk_988;                   ///< offset=0x3DC .text:001A90DA                 mov     ecx, [esi+3DCh]

  // array size of 4, struct size of 0x10  .text:0002FB1B                 add     edi, 10h
  // .text:0002FAAB                 lea     edi, [eax+3E0h]
  // .text:001A8F4A                 cmp     dword ptr [eax+edi], 0FFFFFFFFh (0x3E0 + index*16)
  // .text:001A8F05                 lea     eax, [edi+3E4h]
  // .text:001A8F7B                 fcomp   dword ptr [eax+edi+3E4h]
  // .text:0002FAB8                 mov     edx, [edi+8]
  // .text:001A8F6D                 lea     edx, [edi+3F4h]
  char unk_992[0x10 * 4];             ///< offset=0x3E0
  uint32_t unk_1056;                  ///< offset=0x420
} unit_data_t;

// OBJE -> UNIT -> BIPD
/// size=0x480
typedef struct {
  unit_data_t unit;         ///< offset=0x000
  uint32_t flags;           ///< offset=0x424 .text:001A29F5                 test    byte ptr [esi+424h], 1   ; 1A9BEE shows it's 32-bit
  uint8_t unk_1064;         ///< offset=0x428 .text:001A0EDF                 mov     byte ptr [esi+428h], 0
  uint8_t unk_1065;         ///< offset=0x429 .text:001A0EED                 mov     [esi+429h], al
  uint8_t unk_1066;         ///< offset=0x42A .text:001A2567                 movsx   eax, byte ptr [esi+42Ah]
  uint8_t unk_1067;         ///< offset=0x42B .text:001A4A37                 mov     byte ptr [esi+42Bh], 0
  uint32_t unk_1068;        ///< offset=0x42C .text:00095FBE                 mov     edx, [eax+42Ch]
  uint32_t unk_1072;        ///< offset=0x430 .text:001A0874                 mov     [eax+430h], ecx
  uint32_t unk_1076;        ///< offset=0x434 .text:001A087A                 mov     [eax+434h], ecx
  vector3_t unk_1080;       ///< offset=0x438 .text:0003E1D6                 add     eax, 438h
  uint32_t unk_1092;        ///< offset=0x444 .text:001A1C28                 cmp     eax, [esi+444h]  game time related
  uint32_t unk_1096;        ///< offset=0x448 .text:001A0880                 mov     [eax+448h], ecx
  uint32_t unk_1100;        ///< offset=0x44C .text:001A4A0D                 mov     [esi+44Ch], ebx
  uint32_t unk_1104;        ///< offset=0x450 .text:001A0848                 mov     dword ptr [esi+450h], 0FFFFFFFFh
  datum_handle_t unk_1108;  ///< offset=0x454 .text:001A0AB9                 cmp     [esi+454h], edi 
  uint8_t unk_1112;         ///< offset=0x458 .text:001A0B1D                 mov     byte ptr [esi+458h], 0F1h
  uint8_t unk_1113;         ///< offset=0x459 .text:001A1EE2                 cmp     byte ptr [esi+459h], 1Eh
  uint8_t unk_1114;         ///< offset=0x45A .text:001A2B28                 mov     al, [esi+45Ah]
  uint8_t unk_1115;         ///< offset=0x45B .text:001A2576                 mov     byte ptr [esi+45Bh], 1
  uint8_t unk_1116;         ///< offset=0x45C .text:001A2406                 mov     byte ptr [esi+45Ch], 0
  uint8_t unk_1117;         ///< offset=0x45D .text:001A66F4                 mov     [esi+45Dh], bl
  uint8_t unk_1118;         ///< offset=0x45E .text:001A66EE                 mov     [esi+45Eh], dl
  uint8_t unk_1119;         ///< offset=0x45F
  uint16_t unk_1120;        ///< offset=0x460 .text:001A0ED6                 mov     [esi+460h], cx 
  uint16_t unk_1122;        ///< offset=0x462 
  float unk_1124;           ///< offset=0x464 .text:001A0905                 fmul    dword ptr [edi+464h]
  float unk_1128;           ///< offset=0x468 .text:001A4586                 fld     dword ptr [esi+468h]
  vector3_t unk_1132;       ///< offset=0x46C .text:001A0826                 lea     edx, [esi+46Ch]
  uint32_t unk_1144;        ///< offset=0x478 .text:001A5F90                 mov     [edi+478h], edx
  uint8_t unk_1148;         ///< offset=0x47C .text:0019FAF7                 mov     dl, [esi+47Ch]
  uint8_t unk_1149;         ///< offset=0x47D .text:0019FAE0                 mov     cl, [esi+47Dh]
  char unk_1150[2];         ///< offset=0x47E
} biped_data_t;

// OBJE -> UNIT -> VEHI
/// size=0x47C
typedef struct {
  unit_data_t unit;       ///< offset=0x000
  uint16_t unk_1060;      ///< offset=0x424 .text:001B578E                 mov     [esi+424h], bx
  uint16_t unk_1062;      ///< offset=0x426 .text:001B9819                 cmp     word ptr [ebx+426h], 0
  uint8_t unk_1064;       ///< offset=0x428 .text:001A2020                 cmp     byte ptr [ebx+428h], 1Eh
  uint8_t unk_1065;       ///< offset=0x429 .text:001B57A2                 mov     [esi+429h], bl
  uint8_t unk_1066;       ///< offset=0x42A .text:001B57A8                 mov     [esi+42Ah], bl
  uint8_t unk_1067;       ///< offset=0x42B .text:001B572C                 mov     al, [esi+42Bh]
  float unk_1068;         ///< offset=0x42C .text:001B6025                 fld     dword ptr [esi+42Ch]
  float unk_1072;         ///< offset=0x430 .text:001B7B31                 fld     dword ptr [esi+430h]
  float unk_1076;         ///< offset=0x434 .text:001B602B                 fsub    dword ptr [esi+434h]
  float unk_1080;         ///< offset=0x438 .text:001B5BA9                 fld     dword ptr [esi+438h]
  float unk_1084;         ///< offset=0x43C .text:001B604C                 fadd    dword ptr [esi+43Ch]
  float unk_1088;         ///< offset=0x440 .text:001B608F                 fadd    dword ptr [esi+440h]
  float unk_1092;         ///< offset=0x444 .text:0002EAA8                 fld     dword ptr [edi+444h]
  float unk_1096;         ///< offset=0x448 .text:001B6860                 fcomp   dword ptr [edi+448h]
  uint8_t unk_1100[8];    ///< offset=0x44C .text:001B5786                 lea     ecx, [esi+44Ch] & .text:001B5C28                 mov     cl, [edi+esi+44Ch]
  vector3_t unk_1108;     ///< offset=0x454 .text:001B5631                 lea     eax, [esi+454h]
  float unk_1120;         ///< offset=0x460 .text:0015225F                 fadd    dword ptr [edi+460h]
  float unk_1124;         ///< offset=0x464 .text:0015226E                 fadd    dword ptr [edi+464h]
  float unk_1128;         ///< offset=0x468 .text:0015227D                 fadd    dword ptr [edi+468h]
  float unk_1132;         ///< offset=0x46C .text:0015228C                 fadd    dword ptr [edi+46Ch]
  float unk_1136;         ///< offset=0x470 .text:0015229B                 fadd    dword ptr [edi+470h]
  float unk_1140;         ///< offset=0x474 .text:001522AA                 fadd    dword ptr [edi+474h]
  uint32_t unk_1144;      ///< offset=0x478 .text:001B80DA                 test    [ebx+478h], edx
} vehicle_data_t;

// OBJE -> ITEM
/// size=0x1DC
typedef struct {
  object_data_t object;   ///< offset=0x000
  uint32_t flags;         ///< offset=0x1A4   .text:000F6BBB                 mov     edx, [ecx+1A4h]
  uint16_t unk_424;       ///< offset=0x1A8   .text:000F7BC6                 mov     ax, [ebx+1A8h]
  uint16_t unk_426;       ///< offset=0x1AA
  char unk_428[4];        ///< offset=0x1AC
  uint32_t unk_432;       ///< offset=0x1B0   .text:000F693A                 mov     dword ptr [esi+1B0h], 0FFFFFFFFh   datum_handle?
  uint32_t unk_436;       ///< offset=0x1B4   .text:000F6934                 mov     [esi+1B4h], eax game time related
  char unk_440[16];       ///< offset=0x1B8
  float unk_456;          ///< offset=0x1C8   .text:000F6BDE                 fstp    dword ptr [ecx+1C8h]
  float unk_460;          ///< offset=0x1CC   .text:000F6BE9                 fstp    dword ptr [ecx+1CCh]
  float unk_464;          ///< offset=0x1D0   .text:000F6BF2                 fstp    dword ptr [ecx+1D0h]
  float unk_468;          ///< offset=0x1D4   .text:000F6BFC                 fstp    dword ptr [ecx+1D4h]
  float unk_472;          ///< offset=0x1D8   .text:000F6C04                 fstp    dword ptr [ecx+1D8h]
} item_data_t;

/// size=0x24
typedef struct
{
  uint8_t unk_528;                  ///< offset=0x00
  uint8_t unk_529;                  ///< offset=0x01 .text:000FCEA2                 mov     byte ptr [eax+211h], 7 & .text:000FB3D5                 mov     cl, [eax+235h]
  uint16_t unk_530;                 ///< offset=0x02 .text:000FB8FA                 mov     [eax+212h], dx
  char unk_532[12];                 ///< offset=0x04
  float unk_544;                    ///< offset=0x10 .text:000FC17C                 fld     dword ptr [edi+ecx*4+220h]
  float unk_548;                    ///< offset=0x14 .text:000FC156                 fld     dword ptr [edi+eax*4+224h]
  char unk_552[4];                  ///< offset=0x18
  float unk_556;                    ///< offset=0x1C .text:000D137D                 fld     dword ptr [esi+22Ch]
  char unk_560[4];                  ///< offset=0x20
} weapon_trigger_data_t;

/// size=0xC
typedef struct
{
  uint16_t unk_0;                  ///< offset=0x00 .text:000DE270                 cmp     word ptr [edi+258h], 0
  uint16_t unk_2;                  ///< offset=0x02 .text:000DD7F7                 sub     bx, [edx+25Ah]
  uint16_t unk_4;                  ///< offset=0x04 .text:000DD7E7                 mov     bx, [eax+25Ch]
  uint16_t unk_6;                  ///< offset=0x06 .text:000D1228                 movsx   ecx, word ptr [esi+25Eh]
  uint16_t unk_8;                  ///< offset=0x08 .text:000D120A                 movsx   edx, word ptr [esi+260h]
  char unk_10[2];                  ///< offset=0x0A 
} weapon_magazine_data_t;

#define MAXIMUM_NUMBER_OF_TRIGGERS_PER_WEAPON 2
#define MAXIMUM_NUMBER_OF_MAGAZINES_PER_WEAPON 2  // TODO: confirm

// OBJE -> ITEM -> WEAP
/// size=0x27C
typedef struct {
  item_data_t item;                 ///< offset=0x000
  uint32_t unk_476;                 ///< offset=0x1DC .text:000A87F0                 mov     ecx, [eax+1DCh]
  uint8_t unk_480;                  ///< offset=0x1E0 .text:000FDA9E                 test    byte ptr [ebx+1E0h], 40h
  char unk_481[3];                  ///< offset=0x1E1
  float unk_484;                    ///< offset=0x1E4 .text:000D1375                 fld     dword ptr [esi+1E4h]
  uint8_t unk_488;                  ///< offset=0x1E8 .text:000FD158                 mov     al, [eax+1E8h]   state related?
  uint8_t unk_489;                  ///< offset=0x1E9
  uint16_t unk_490;                 ///< offset=0x1EA .text:000FD343                 mov     [edi+1EAh], ax
  float unk_492;                    ///< offset=0x1EC .text:000D121A                 fld     dword ptr [esi+1ECh]
  float unk_496;                    ///< offset=0x1F0 .text:000D1204                 fld     dword ptr [esi+1F0h]
  float unk_500;                    ///< offset=0x1F4 .text:000DD8F9                 fld     dword ptr [edx+1F4h]
  uint32_t integrated_light_power;  ///< offset=0x1F8 .text:000FAEC4                 mov     [eax+1F8h], ecx
  char unk_508[4];                  ///< offset=0x1FC
  uint32_t unk_512;                 ///< offset=0x200 .text:000FD55C                 mov     dword ptr [edi+200h], 0FFFFFFFFh
  char unk_516[8];                  ///< offset=0x204
  uint16_t unk_524;                 ///< offset=0x20C .text:000FD906                 cmp     word ptr [ebx+20Ch], 0
  char unk_526[2];                  ///< offset=0x20E
  weapon_trigger_data_t triggers[MAXIMUM_NUMBER_OF_TRIGGERS_PER_WEAPON];  ///< offset=0x210 .text:000FCFA3                 lea     edi, [edi+eax*4+210h]
  weapon_magazine_data_t magazines[MAXIMUM_NUMBER_OF_MAGAZINES_PER_WEAPON];  ///< offset=0x258 .text:000FBC8D                 lea     edi, [ebx+eax*4]   &v2[3 * magazine_index? + 0x96]; (0x258 is the base address, 12-byte struct due to dword access)
  char unk_624[4];                  ///< offset=0x270
  uint32_t unk_628;                 ///< offset=0x274 .text:000FBD3A                 mov     dword ptr [edi+274h], 0FFFFFFFFh
  char unk_632[4];                  ///< offset=0x278
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
  uint16_t unk_476;       ///< offset=0x1DC .text:000F6833                 dec     word ptr [eax+1DCh]
  char unk_478[0x16];     ///< offset=0x1DE
} garbage_data_t;

// OBJE -> PROJ
/// size=0x228
typedef struct {
  object_data_t object;   ///< offset=0x000
  char unk_420[0x38];     ///< offset=0x1A4
  uint32_t unk_476;       ///< offset=0x1DC .text:000F7CBE                 mov     ecx, [eax+1DCh]
  uint16_t unk_480;       ///< offset=0x1E0 .text:000F7E4B                 cmp     si, [eax+1E0h]   type of some sort, also see projectile_collision
  uint16_t unk_482;       ///< offset=0x1E2 .text:000F8D84                 mov     [esi+1E2h], bx
  datum_handle_t unk_484; ///< offset=0x1E4 .text:000F8D90                 mov     [esi+1E4h], eax
  datum_handle_t unk_488; ///< offset=0x1E8 .text:000F7D44                 mov     [eax+1E8h], ecx
  uint32_t unk_492;       ///< offset=0x1EC .text:000F9CAC                 mov     eax, [ebx+1ECh]  index into [ebx+eax*4+0FCh]
  float unk_496;          ///< offset=0x1F0 .text:000F8A91                 fmul    dword ptr [edi+1F0h]
  float unk_500;          ///< offset=0x1F4 .text:000F8DEC                 fstp    dword ptr [esi+1F4h]
  float unk_504;          ///< offset=0x1F8 .text:000F9DBD                 fld     dword ptr [ebx+1F8h]
  float unk_508;          ///< offset=0x1FC .text:000F8E15                 fstp    dword ptr [esi+1FCh]
  float unk_512;          ///< offset=0x200 .text:000F7F66                 fld     dword ptr [esi+200h]
  float unk_516;          ///< offset=0x204 .text:000F8702                 mov     dword ptr [esi+204h], 3F800000h
  float unk_520;          ///< offset=0x208 .text:000F86AB                 fstp    dword ptr [esi+208h]
  float unk_524;          ///< offset=0x20C .text:000F8677                 fstp    dword ptr [esi+20Ch]
  float unk_528;          ///< offset=0x210 .text:000FA304                 fcomp   dword ptr [ebx+210h]
  float unk_532;          ///< offset=0x214 .text:000F85E7                 fstp    dword ptr [ecx+214h]
  float unk_536;          ///< offset=0x218 .text:000F85F2                 fstp    dword ptr [ecx+218h]
  float unk_540;          ///< offset=0x21C .text:000F85FB                 fstp    dword ptr [ecx+21Ch]
  float unk_544;          ///< offset=0x220 .text:000F8605                 fstp    dword ptr [ecx+220h]
  float unk_548;          ///< offset=0x224 .text:000F860D                 fstp    dword ptr [ecx+224h]
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
  uint8_t flags;          ///< offset=0x1A4   .text:00096784                 test    byte ptr [edi+1A4h], 2
  char unk_421[3];        ///< offset=0x1A5
  uint16_t unk_424;       ///< offset=0x1A8   .text:000960EB                 mov     [esi+1A8h], ax
  uint16_t unk_426;       ///< offset=0x1AA
  float unk_428;          ///< offset=0x1AC   .text:00096182                 fld     dword ptr [edi+1ACh]
  float unk_432;          ///< offset=0x1B0   .text:0009618D                 fld     dword ptr [edi+1B0h]
  uint16_t unk_436;       ///< offset=0x1B4   .text:000960E4                 mov     [esi+1B4h], ax
  uint16_t unk_438;       ///< offset=0x1B6
  float unk_440;          ///< offset=0x1B8   .text:000961BB                 fld     dword ptr [edi+1B8h]
  float unk_444;          ///< offset=0x1BC   .text:000961C6                 fld     dword ptr [edi+1BCh]
  uint16_t unk_448;       ///< offset=0x1C0   .text:000962CD                 movsx   edx, word ptr [edi+1C0h]
  uint16_t unk_450;       ///< offset=0x1C2
} device_data_t;

// OBJE -> DEVI -> MACH
/// size=0x1D8
typedef struct {
  device_data_t device;   ///< offset=0x000
  uint32_t flags;         ///< offset=0x1C4   .text:00096247                 mov     ecx, [esi+1C4h]
  uint32_t unk_456;       ///< offset=0x1C8   .text:00095EB2                 mov     edx, [esi+1C8h]
  vector3_t unk_460;      ///< offset=0x1CC   .text:00095F1E                 fsub    dword ptr [esi+1CCh]
} machine_data_t;

// OBJE -> DEVI -> CTRL
/// size=0x1CC
typedef struct {
  device_data_t device;   ///< offset=0x000
  uint32_t flags;         ///< offset=0x1C4   .text:0009571F                 or      [esi+1C4h], eax
  datum_handle_t unk_456; ///< offset=0x1C8   .text:000D06C1                 cmp     word ptr [esi+1C8h], 0FFFFh    datum_handle?
} control_data_t;

// OBJE -> DEVI -> LIFI
/// size=0x1DC
typedef struct {
  device_data_t device;   ///< offset=0x000
  char unk_452[0x10];     ///< offset=0x1C4
  uint32_t unk_468;       ///< offset=0x1D4 .text:00095A08                 mov     [esi+1D4h], ecx
  uint32_t unk_472;       ///< offset=0x1D8 .text:00095A12                 mov     [esi+1D8h], edx
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
