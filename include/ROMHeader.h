#ifndef ROMHEADER_H
#define ROMHEADER_H

struct s_ROMHeaderInfo {
    uint8_t MBCType; //What MBC chip does the cart have?      @0x0147
    uint8_t ROMSize; //What size is our ROM?                  @0x0148
    uint8_t RAMSize; //How many ram Banks do we have?         @0x0149
    uint8_t region; //0 = Japanese; 1 = Non-Japanese         @0x014A
};

#define MBC_ROM             0x00
#define MBC_1               0x01
#define MBC_1_RAM           0x02
#define MBC_1_RAM_BAT       0x03
#define MBC_2               0x05
#define MBC_2_BAT           0x06
#define MBC_ROM_RAM         0x08
#define MBC_ROM_RAM_BAT     0x09
#define MBC_MMM_01          0x0B
#define MBC_MMM_01_RAM      0x0C
#define MBC_MMM_01_RAM_BAT  0x0D
#define MBC_3_TIMER_BAT     0x0F
#define MBC_3_TIMER_RAM_BAT 0x10
#define MBC_3               0x11
#define MBC_3_RAM           0x12
#define MBC_3_RAM_BAT       0x13
#define MBC_4               0x15
#define MBC_4_RAM           0x16
#define MBC_4_RAM_BAT       0x17
#define MBC_5               0x19
#define MBC_5_RAM           0x1A
#define MBC_5_RAM_BAT       0x1B
#define MBC_5_RUM           0x1C
#define MBC_5_RUM_RAM       0x1D
#define MBC_5_RUM_RAM_BAT   0x1E
#define MBC_POCKETCAM       0xFC
#define MBC_BANDAITAMA5     0xFD
#define MBC_HUC3            0xFE
#define MBC_HUC1_RAM_BAT    0xFF

#define ROM_32K     0x00
#define ROM_64K     0x01
#define ROM_128K    0x02
#define ROM_256K    0x03
#define ROM_512K    0x04
#define ROM_1M      0x05
#define ROM_2M      0x06
#define ROM_4M      0x07
#define ROM_1.1M    0x52
#define ROM_1.2M    0x53
#define ROM_1.5M    0x54

#define RAM_NONE    0x00
#define RAM_2K      0x01
#define RAM_8K      0x02
#define RAM_32K     0x03

#endif
