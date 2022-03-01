

// Register addresses and values
#define CLKRC                 0x11 
#define CLKRC_VALUE_VGA       0x01  // Raw Bayer
#define CLKRC_VALUE_QVGA      0x01
#define CLKRC_VALUE_QQVGA     0x01
#define CLKRC_VALUE_NIGHTMODE_FIXED   0x03 // Fixed Frame
#define CLKRC_VALUE_NIGHTMODE_AUTO    0x80 // Auto Frame Rate Adjust

#define COM7                                   0x12 
#define COM7_VALUE_VGA                         0x01   // Raw Bayer
#define COM7_VALUE_VGA_COLOR_BAR               0x03   // Raw Bayer
#define COM7_VALUE_VGA_PROCESSED_BAYER         0x05   // Processed Bayer
#define COM7_VALUE_QVGA                        0x00
#define COM7_VALUE_QVGA_COLOR_BAR              0x02
#define COM7_VALUE_QVGA_PREDEFINED_COLOR_BAR   0x12
#define COM7_VALUE_QQVGA                       0x00
#define COM7_VALUE_QQVGA_COLOR_BAR             0x02   
#define COM7_VALUE_QCIF                        0x08     // Predefined QCIF format
#define COM7_VALUE_COLOR_BAR_QCIF              0x0A     // Predefined QCIF Format with ColorBar
#define COM7_VALUE_RESET                       0x80


#define COM3                            0x0C 
#define COM3_VALUE_VGA                  0x00 // Raw Bayer
#define COM3_VALUE_QVGA                 0x04
#define COM3_VALUE_QQVGA                0x04  // From Docs (0b100)
#define COM3_VALUE_QQVGA_SCALE_ENABLED  0x0C  // Enable Scale and DCW (0b1100)
#define COM3_VALUE_QCIF                 0x0C  // Enable Scaling and enable DCW

#define COM14                            0x3E 
#define COM14_VALUE_VGA                  0x00 // Raw Bayer
#define COM14_VALUE_QVGA                 0x19
#define COM14_VALUE_QQVGA                0x1A
#define COM14_VALUE_MANUAL_SCALING       0x08   // Manual Scaling Enabled
#define COM14_VALUE_NO_MANUAL_SCALING    0x00   // Manual Scaling DisEnabled

#define SCALING_XSC                                  0x70
#define SCALING_XSC_VALUE_VGA                        0x3A  // Raw Bayer
#define SCALING_XSC_VALUE_QVGA                       0x3A
#define SCALING_XSC_VALUE_QQVGA                      0x3A
#define SCALING_XSC_VALUE_QQVGA_SHIFT1               0x3A
#define SCALING_XSC_VALUE_COLOR_BAR                  0xBA  
#define SCALING_XSC_VALUE_QCIF_COLOR_BAR_NO_SCALE    0x80 // Predefined QCIF with Color Bar and NO Scaling

#define SCALING_YSC                                   0x71 
#define SCALING_YSC_VALUE_VGA                         0x35 // Raw Bayer 
#define SCALING_YSC_VALUE_QVGA                        0x35
#define SCALING_YSC_VALUE_QQVGA                       0x35
#define SCALING_YSC_VALUE_COLOR_BAR                   0x35  // 8 bar color bar
#define SCALING_YSC_VALUE_COLOR_BAR_GREY              0xB5  // fade to grey color bar
#define SCALING_YSC_VALUE_COLOR_BAR_SHIFT1            0xB5  // fade to grey color bar
#define SCALING_YSC_VALUE_QCIF_COLOR_BAR_NO_SCALE     0x00  // Predefined QCIF with Color Bar and NO Scaling

#define SCALING_DCWCTR               0x72 
#define SCALING_DCWCTR_VALUE_VGA     0x11  // Raw Bayer
#define SCALING_DCWCTR_VALUE_QVGA    0x11
#define SCALING_DCWCTR_VALUE_QQVGA   0x22  

#define SCALING_PCLK_DIV              0x73  
#define SCALING_PCLK_DIV_VALUE_VGA    0xF0 // Raw Bayer
#define SCALING_PCLK_DIV_VALUE_QVGA   0xF1
#define SCALING_PCLK_DIV_VALUE_QQVGA  0xF2

#define SCALING_PCLK_DELAY              0xA2
#define SCALING_PCLK_DELAY_VALUE_VGA    0x02 // Raw Bayer
#define SCALING_PCLK_DELAY_VALUE_QVGA   0x02
#define SCALING_PCLK_DELAY_VALUE_QQVGA  0x02


// Controls YUV order Used with COM13
// Need YUYV format for Android Decoding- Default value is 0xD
#define TSLB                                         0x3A
#define TSLB_VALUE_YUYV_AUTO_OUTPUT_WINDOW_ENABLED   0x01 // No custom scaling
#define TSLB_VALUE_YUYV_AUTO_OUTPUT_WINDOW_DISABLED  0x00 // For adjusting HSTART, etc. YUYV format
#define TSLB_VALUE_UYVY_AUTO_OUTPUT_WINDOW_DISABLED  0x08 
#define TSLB_VALUE_TESTVALUE                         0x04 // From YCbCr Reference 


// Default value is 0x88
// ok if you want YUYV order, no need to change
#define COM13                      0x3D
#define COM13_VALUE_DEFAULT        0x88
#define COM13_VALUE_NOGAMMA_YUYV   0x00
#define COM13_VALUE_GAMMA_YUYV     0x80
#define COM13_VALUE_GAMMA_YVYU     0x82
#define COM13_VALUE_YUYV_UVSATAUTOADJ_ON 0x40



// Works with COM4
#define COM17                                 0x42
#define COM17_VALUE_AEC_NORMAL_NO_COLOR_BAR   0x00
#define COM17_VALUE_AEC_NORMAL_COLOR_BAR      0x08 // Activate Color Bar for DSP

#define COM4   0x0D

// RGB Settings and Data format
#define COM15    0x40

/* Control 10 */
#define   REG_COM10  0x15  
#define   COM10_HSYNC   0x40    /* HSYNC instead of HREF */
#define   COM10_PCLK_HB   0x20    /* Suppress PCLK on horiz blank */
#define   COM10_HREF_REV  0x08    /* Reverse HREF */
#define   COM10_VS_LEAD   0x04    /* VSYNC on clock leading edge */
#define   COM10_VS_NEG    0x02    /* VSYNC negative */
#define   COM10_HS_NEG    0x01    /* HSYNC negative */

// Night Mode
#define COM11                             0x3B
#define COM11_VALUE_NIGHTMODE_ON          0x80     // Night Mode
#define COM11_VALUE_NIGHTMODE_OFF         0x00 
#define COM11_VALUE_NIGHTMODE_ON_EIGHTH   0xE0     // Night Mode 1/8 frame rate minimum
#define COM11_VALUE_NIGHTMODE_FIXED       0x0A 
#define COM11_VALUE_NIGHTMODE_AUTO        0xEA     // Night Mode Auto Frame Rate Adjust


// Color Matrix Control YUV
#define MTX1      0x4f 
#define MTX1_VALUE  0x80

#define MTX2      0x50 
#define MTX2_VALUE  0x80

#define MTX3      0x51 
#define MTX3_VALUE  0x00

#define MTX4      0x52 
#define MTX4_VALUE  0x22

#define MTX5      0x53 
#define MTX5_VALUE  0x5e

#define MTX6      0x54 
#define MTX6_VALUE  0x80

#define CONTRAS     0x56 
#define CONTRAS_VALUE 0x40

#define MTXS      0x58 
#define MTXS_VALUE  0x9e



// COM8
#define COM8                    0x13
#define COM8_VALUE_AWB_OFF      0xE5
#define COM8_VALUE_AWB_ON       0xE7

#define   COM8_FASTAEC    0x80    // Enable fast AGC/AEC
#define   COM8_AECSTEP    0x40    // Unlimited AEC step size
#define   COM8_BFILT    0x20    // Band filter enable
#define   COM8_AGC    0x04    // Auto gain enable
#define   COM8_AWB    0x02    // White balance enable
#define   COM8_AEC    0x01    // Auto exposure enable


// Automatic White Balance
#define AWBC1     0x43 
#define AWBC1_VALUE 0x14

#define AWBC2     0x44 
#define AWBC2_VALUE 0xf0

#define AWBC3     0x45 
#define AWBC3_VALUE   0x34

#define AWBC4     0x46 
#define AWBC4_VALUE 0x58

#define AWBC5         0x47 
#define AWBC5_VALUE 0x28

#define AWBC6     0x48 
#define AWBC6_VALUE 0x3a

#define AWBC7           0x59
#define AWBC7_VALUE     0x88

#define AWBC8          0x5A
#define AWBC8_VALUE    0x88

#define AWBC9          0x5B
#define AWBC9_VALUE    0x44

#define AWBC10         0x5C
#define AWBC10_VALUE   0x67

#define AWBC11         0x5D
#define AWBC11_VALUE   0x49

#define AWBC12         0x5E
#define AWBC12_VALUE   0x0E

#define AWBCTR3        0x6C
#define AWBCTR3_VALUE  0x0A

#define AWBCTR2        0x6D
#define AWBCTR2_VALUE  0x55

#define AWBCTR1        0x6E
#define AWBCTR1_VALUE  0x11

#define AWBCTR0                0x6F
#define AWBCTR0_VALUE_NORMAL   0x9F
#define AWBCTR0_VALUE_ADVANCED 0x9E


// Gain
#define COM9                        0x14
#define COM9_VALUE_MAX_GAIN_128X    0x6A
#define COM9_VALUE_4XGAIN           0x10    // 0001 0000

#define BLUE          0x01    // AWB Blue Channel Gain
#define BLUE_VALUE    0x40

#define RED            0x02    // AWB Red Channel Gain
#define RED_VALUE      0x40

#define GGAIN            0x6A   // AWB Green Channel Gain
#define GGAIN_VALUE      0x40

#define COM16     0x41 
#define COM16_VALUE 0x08 // AWB Gain on

#define GFIX      0x69 
#define GFIX_VALUE  0x00

// Edge Enhancement Adjustment
#define EDGE      0x3f 
#define EDGE_VALUE  0x00

#define REG75     0x75 
#define REG75_VALUE 0x03

#define REG76     0x76 
#define REG76_VALUE 0xe1

// DeNoise 
#define DNSTH     0x4c 
#define DNSTH_VALUE 0x00

#define REG77     0x77 
#define REG77_VALUE 0x00

// Denoise and Edge Enhancement
#define COM16_VALUE_DENOISE_OFF_EDGE_ENHANCEMENT_OFF_AWBGAIN_ON     0x08 // Denoise off, AWB Gain on
#define COM16_VALUE_DENOISE_ON__EDGE_ENHANCEMENT_OFF__AWBGAIN_ON    0x18
#define COM16_VALUE_DENOISE_OFF__EDGE_ENHANCEMENT_ON__AWBGAIN_ON    0x28
#define COM16_VALUE_DENOISE_ON__EDGE_ENHANCEMENT_ON__AWBGAIN_ON     0x38 // Denoise on,  Edge Enhancement on, AWB Gain on


// 30FPS Frame Rate , PCLK = 24Mhz
#define CLKRC_VALUE_30FPS  0x80

#define DBLV               0x6b
#define DBLV_VALUE_30FPS   0x0A

#define EXHCH              0x2A
#define EXHCH_VALUE_30FPS  0x00

#define EXHCL              0x2B
#define EXHCL_VALUE_30FPS  0x00

#define DM_LNL               0x92
#define DM_LNL_VALUE_30FPS   0x00

#define DM_LNH               0x93
#define DM_LNH_VALUE_30FPS   0x00

#define COM11_VALUE_30FPS    0x0A   


// Saturation Control
#define SATCTR      0xc9 
#define SATCTR_VALUE  0x60


// AEC/AGC - Automatic Exposure/Gain Control
#define GAIN    0x00 
#define GAIN_VALUE  0x00

#define AEW     0x24 
#define AEW_VALUE 0x95

#define AEB     0x25 
#define AEB_VALUE 0x33

#define VPT     0x26 
#define VPT_VALUE 0xe3



// Gamma
#define SLOP      0x7a 
#define SLOP_VALUE  0x20

#define GAM1      0x7b 
#define GAM1_VALUE  0x10

#define GAM2      0x7c 
#define GAM2_VALUE      0x1e

#define GAM3      0x7d 
#define GAM3_VALUE  0x35

#define GAM4      0x7e 
#define GAM4_VALUE  0x5a

#define GAM5      0x7f 
#define GAM5_VALUE  0x69

#define GAM6      0x80 
#define GAM6_VALUE  0x76

#define GAM7      0x81 
#define GAM7_VALUE  0x80

#define GAM8      0x82 
#define GAM8_VALUE  0x88

#define GAM9      0x83 
#define GAM9_VALUE  0x8f

#define GAM10     0x84 
#define GAM10_VALUE 0x96

#define GAM11     0x85 
#define GAM11_VALUE 0xa3

#define GAM12     0x86 
#define GAM12_VALUE 0xaf

#define GAM13     0x87 
#define GAM13_VALUE 0xc4

#define GAM14     0x88 
#define GAM14_VALUE 0xd7

#define GAM15     0x89 
#define GAM15_VALUE 0xe8




// AEC/AGC Control- Histogram
#define HAECC1      0x9f 
#define HAECC1_VALUE  0x78

#define HAECC2      0xa0 
#define HAECC2_VALUE  0x68

#define HAECC3      0xa6 
#define HAECC3_VALUE  0xd8

#define HAECC4      0xa7 
#define HAECC4_VALUE  0xd8

#define HAECC5      0xa8 
#define HAECC5_VALUE  0xf0

#define HAECC6      0xa9 
#define HAECC6_VALUE  0x90

#define HAECC7                          0xaa  // AEC Algorithm selection
#define HAECC7_VALUE_HISTOGRAM_AEC_ON 0x94 
#define HAECC7_VALUE_AVERAGE_AEC_ON     0x00




// Gamma
#define SLOP      0x7a 
#define SLOP_VALUE  0x20

#define GAM1      0x7b 
#define GAM1_VALUE  0x10

#define GAM2      0x7c 
#define GAM2_VALUE      0x1e

#define GAM3      0x7d 
#define GAM3_VALUE  0x35

#define GAM4      0x7e 
#define GAM4_VALUE  0x5a

#define GAM5      0x7f 
#define GAM5_VALUE  0x69

#define GAM6      0x80 
#define GAM6_VALUE  0x76

#define GAM7      0x81 
#define GAM7_VALUE  0x80

#define GAM8      0x82 
#define GAM8_VALUE  0x88

#define GAM9      0x83 
#define GAM9_VALUE  0x8f

#define GAM10     0x84 
#define GAM10_VALUE 0x96

#define GAM11     0x85 
#define GAM11_VALUE 0xa3

#define GAM12     0x86 
#define GAM12_VALUE 0xaf

#define GAM13     0x87 
#define GAM13_VALUE 0xc4

#define GAM14     0x88 
#define GAM14_VALUE 0xd7

#define GAM15     0x89 
#define GAM15_VALUE 0xe8



// Array Control
#define CHLF      0x33 
#define CHLF_VALUE  0x0b

#define ARBLM     0x34 
#define ARBLM_VALUE 0x11



// ADC Control
#define ADCCTR1     0x21 
#define ADCCTR1_VALUE 0x02

#define ADCCTR2     0x22 
#define ADCCTR2_VALUE 0x91

#define ADC     0x37 
#define ADC_VALUE       0x1d

#define ACOM      0x38 
#define ACOM_VALUE  0x71

#define OFON      0x39 
#define OFON_VALUE  0x2a


// Black Level Calibration
#define ABLC1     0xb1 
#define ABLC1_VALUE 0x0c

#define THL_ST    0xb3 
#define THL_ST_VALUE  0x82


// Window Output 
#define HSTART               0x17
#define HSTART_VALUE_DEFAULT 0x11
#define HSTART_VALUE_VGA     0x13     
#define HSTART_VALUE_QVGA    0x13   
#define HSTART_VALUE_QQVGA   0x13   // Works

#define HSTOP                0x18
#define HSTOP_VALUE_DEFAULT  0x61
#define HSTOP_VALUE_VGA      0x01   
#define HSTOP_VALUE_QVGA     0x01  
#define HSTOP_VALUE_QQVGA    0x01   // Works 

#define HREF                  0x32
#define HREF_VALUE_DEFAULT    0x80
#define HREF_VALUE_VGA        0xB6   
#define HREF_VALUE_QVGA       0x24
#define HREF_VALUE_QQVGA      0xA4  

#define VSTRT                0x19
#define VSTRT_VALUE_DEFAULT  0x03
#define VSTRT_VALUE_VGA      0x02
#define VSTRT_VALUE_QVGA     0x02
#define VSTRT_VALUE_QQVGA    0x02  
 
#define VSTOP                0x1A
#define VSTOP_VALUE_DEFAULT  0x7B
#define VSTOP_VALUE_VGA      0x7A
#define VSTOP_VALUE_QVGA     0x7A
#define VSTOP_VALUE_QQVGA    0x7A  

#define VREF                 0x03
#define VREF_VALUE_DEFAULT   0x03
#define VREF_VALUE_VGA       0x0A   
#define VREF_VALUE_QVGA      0x0A
#define VREF_VALUE_QQVGA     0x0A  


// I2C 
#define OV7670_I2C_ADDRESS                 0x21
//#define OV7670_I2C_ADDRESS                 0x42
#define I2C_ERROR_WRITING_START_ADDRESS      11
#define I2C_ERROR_WRITING_DATA               22

#define DATA_TOO_LONG                  1      // data too long to fit in transmit buffer 
#define NACK_ON_TRANSMIT_OF_ADDRESS    2      // received NACK on transmit of address 
#define NACK_ON_TRANSMIT_OF_DATA       3      // received NACK on transmit of data 
#define OTHER_ERROR                    4      // other error 

#define I2C_READ_START_ADDRESS_ERROR        33
#define I2C_READ_DATA_SIZE_MISMATCH_ERROR   44


/* Registers */
#define REG_GAIN  0x00  /* Gain lower 8 bits (rest in vref) */
#define REG_BLUE  0x01  /* blue gain */
#define REG_RED   0x02  /* red gain */
#define REG_VREF  0x03  /* Pieces of GAIN, VSTART, VSTOP */
#define REG_COM1  0x04  /* Control 1 */
#define  COM1_CCIR656   0x40  /* CCIR656 enable */
#define REG_BAVE  0x05  /* U/B Average level */
#define REG_GbAVE 0x06  /* Y/Gb Average level */
#define REG_AECHH 0x07  /* AEC MS 5 bits */
#define REG_RAVE  0x08  /* V/R Average level */
#define REG_COM2  0x09  /* Control 2 */
#define  COM2_SSLEEP    0x10  /* Soft sleep mode */
#define REG_PID   0x0a  /* Product ID MSB */
#define REG_VER   0x0b  /* Product ID LSB */
#define REG_COM3  0x0c  /* Control 3 */
#define  COM3_SWAP    0x40    /* Byte swap */
#define  COM3_SCALEEN   0x08    /* Enable scaling */
#define  COM3_DCWEN   0x04    /* Enable downsamp/crop/window */
#define REG_COM4  0x0d  /* Control 4 */
#define REG_COM5  0x0e  /* All "reserved" */
#define REG_COM6  0x0f  /* Control 6 */
#define REG_AECH  0x10  /* More bits of AEC value */
#define REG_CLKRC 0x11  /* Clocl control */
#define   CLK_EXT   0x40    /* Use external clock directly */
#define   CLK_SCALE   0x3f    /* Mask for internal clock scale */
#define REG_COM7  0x12  /* Control 7 */
#define   COM7_RESET    0x80    /* Register reset */
#define   COM7_FMT_MASK   0x38
#define   COM7_FMT_VGA    0x00
#define   COM7_FMT_CIF    0x20    /* CIF format */
#define   COM7_FMT_QVGA   0x10    /* QVGA format */
#define   COM7_FMT_QCIF   0x08    /* QCIF format */
#define   COM7_RGB    0x04    /* bits 0 and 2 - RGB format */
#define   COM7_YUV    0x00    /* YUV */
#define   COM7_BAYER    0x01    /* Bayer format */
#define   COM7_PBAYER   0x05    /* "Processed bayer" */
#define REG_COM8  0x13  /* Control 8 */
#define   COM8_FASTAEC    0x80    /* Enable fast AGC/AEC */
#define   COM8_AECSTEP    0x40    /* Unlimited AEC step size */
#define   COM8_BFILT    0x20    /* Band filter enable */
#define   COM8_AGC    0x04    /* Auto gain enable */
#define   COM8_AWB    0x02    /* White balance enable */
#define   COM8_AEC    0x01    /* Auto exposure enable */
#define REG_COM9  0x14  /* Control 9  - gain ceiling */
#define REG_COM10 0x15  /* Control 10 */
#define   COM10_HSYNC   0x40    /* HSYNC instead of HREF */
#define   COM10_PCLK_HB   0x20    /* Suppress PCLK on horiz blank */
#define   COM10_HREF_REV  0x08    /* Reverse HREF */
#define   COM10_VS_LEAD   0x04    /* VSYNC on clock leading edge */
#define   COM10_VS_NEG    0x02    /* VSYNC negative */
#define   COM10_HS_NEG    0x01    /* HSYNC negative */
#define REG_HSTART  0x17  /* Horiz start high bits */
#define REG_HSTOP 0x18  /* Horiz stop high bits */
#define REG_VSTART  0x19  /* Vert start high bits */
#define REG_VSTOP 0x1a  /* Vert stop high bits */
#define REG_PSHFT 0x1b  /* Pixel delay after HREF */
#define REG_MIDH  0x1c  /* Manuf. ID high */
#define REG_MIDL  0x1d  /* Manuf. ID low */
#define REG_MVFP  0x1e  /* Mirror / vflip */
#define   MVFP_MIRROR   0x20    /* Mirror image */
#define   MVFP_FLIP   0x10    /* Vertical flip */

#define REG_AEW   0x24  /* AGC upper limit */
#define REG_AEB   0x25  /* AGC lower limit */
#define REG_VPT   0x26  /* AGC/AEC fast mode op region */
#define REG_HSYST 0x30  /* HSYNC rising edge delay */
#define REG_HSYEN 0x31  /* HSYNC falling edge delay */
#define REG_HREF  0x32  /* HREF pieces */
#define REG_TSLB  0x3a  /* lots of stuff */
#define   TSLB_YLAST    0x04    /* UYVY or VYUY - see com13 */
#define REG_COM11 0x3b  /* Control 11 */
#define   COM11_NIGHT   0x80    /* NIght mode enable */
#define   COM11_NMFR    0x60    /* Two bit NM frame rate */
#define   COM11_HZAUTO    0x10    /* Auto detect 50/60 Hz */
#define   COM11_50HZ    0x08    /* Manual 50Hz select */
#define   COM11_EXP   0x02
#define REG_COM12 0x3c  /* Control 12 */
#define   COM12_HREF    0x80    /* HREF always */
#define REG_COM13 0x3d  /* Control 13 */
#define   COM13_GAMMA   0x80    /* Gamma enable */
#define   COM13_UVSAT   0x40    /* UV saturation auto adjustment */
#define   COM13_UVSWAP    0x01    /* V before U - w/TSLB */
#define REG_COM14 0x3e  /* Control 14 */
#define   COM14_DCWEN   0x10    /* DCW/PCLK-scale enable */
#define REG_EDGE  0x3f  /* Edge enhancement factor */
#define REG_COM15 0x40  /* Control 15 */
#define   COM15_R10F0   0x00    /* Data range 10 to F0 */
#define   COM15_R01FE   0x80    /*            01 to FE */
#define   COM15_R00FF   0xc0    /*            00 to FF */
#define   COM15_RGB565    0x10    /* RGB565 output */
#define   COM15_RGB555    0x30    /* RGB555 output */
#define REG_COM16 0x41  /* Control 16 */
#define   COM16_AWBGAIN   0x08    /* AWB gain enable */
#define REG_COM17 0x42  /* Control 17 */
#define   COM17_AECWIN    0xc0    /* AEC window - must match COM4 */
#define   COM17_CBAR    0x08    /* DSP Color bar */

/*
 * This matrix defines how the colors are generated, must be
 * tweaked to adjust hue and saturation.
 *
 * Order: v-red, v-green, v-blue, u-red, u-green, u-blue
 *
 * They are nine-bit signed quantities, with the sign bit
 * stored in 0x58.  Sign for v-red is bit 0, and up from there.
 */
#define REG_CMATRIX_BASE 0x4f
#define   CMATRIX_LEN 6
#define REG_CMATRIX_SIGN 0x58


#define REG_BRIGHT  0x55  /* Brightness */
#define REG_CONTRAS 0x56  /* Contrast control */

#define REG_GFIX  0x69  /* Fix gain control */

#define REG_DBLV  0x6b  /* PLL control an debugging */
#define   DBLV_BYPASS   0x0a    /* Bypass PLL */
#define   DBLV_X4   0x4a    /* clock x4 */
#define   DBLV_X6   0x8a    /* clock x6 */
#define   DBLV_X8   0xca    /* clock x8 */

#define REG_SCALING_XSC 0x70  /* Test pattern and horizontal scale factor */
#define   TEST_PATTTERN_0 0x80
#define REG_SCALING_YSC 0x71  /* Test pattern and vertical scale factor */
#define   TEST_PATTTERN_1 0x80

#define REG_REG76 0x76  /* OV's name */
#define   R76_BLKPCOR   0x80    /* Black pixel correction enable */
#define   R76_WHTPCOR   0x40    /* White pixel correction enable */

#define REG_RGB444  0x8c  /* RGB 444 control */
#define   R444_ENABLE   0x02    /* Turn on RGB444, overrides 5x5 */
#define   R444_RGBX   0x01    /* Empty nibble at end */

#define REG_HAECC1  0x9f  /* Hist AEC/AGC control 1 */
#define REG_HAECC2  0xa0  /* Hist AEC/AGC control 2 */

#define REG_BD50MAX 0xa5  /* 50hz banding step limit */
#define REG_HAECC3  0xa6  /* Hist AEC/AGC control 3 */
#define REG_HAECC4  0xa7  /* Hist AEC/AGC control 4 */
#define REG_HAECC5  0xa8  /* Hist AEC/AGC control 5 */
#define REG_HAECC6  0xa9  /* Hist AEC/AGC control 6 */
#define REG_HAECC7  0xaa  /* Hist AEC/AGC control 7 */
#define REG_BD60MAX 0xab  /* 60hz banding step limit */
