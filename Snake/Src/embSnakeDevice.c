/**************************************************
 * File  : embSnakeDevice.c
 * Author: MANDA (slipperstree@gmail.com)
 * Date  : 2019.11
 * Updt  : 2021.07
 * Func  : 设备驱动模块，请自行实现本文件内的各函数以适配你的设备，外部会在需要的时候调用这些函数
***************************************************/

#include "embSnakeDevice.h"
#include "boradSupport.h"

// 临时变量用(sprintf等)
extern u8 buff[128];

u16 drawAreaStartX = 0;
u16 drawAreaStartY = 0;
u16 drawAreaEndX = 0;
u16 drawAreaEndY = 0;
u16 drawAreaCurrX = 0;
u16 drawAreaCurrY = 0;

// =========================================================================================
// ==    显示设备相关    =====================================================================
// =========================================================================================

// 初始化显示设备
// bgColor : 背景色，可用于初期清屏
void devDisplayInit(u16 bgColor){
    //TODO:GBA? LCD_Init();
    devScreenOFF(); // 关闭LCD显示
    //TODO:GBA? LCD_Clear(bgColor);
    devScreenON();  // 打开LCD显示
    //TODO:GBA? TFT_BL = 0;     // 打开液晶屏背光灯
}

// 填充一个矩形区域
void devFillRectange(u16 x, u16 y, u16 width, u16 height, u16 color){
    //drawRect(x, y, width, height, color);         // for gba_drawing.h
    m3_rect(x, y, x+width, y+height, color);    // for tonc_video.h
}

// 为绘制一个区域做准备。（外部调用了这个函数后会批量调用devPointInDrawArea来绘制各种图案或点阵文字）
// 一般来说在这里调用底层的设置显示区域命令以及开始绘图命令
// LCD_SetArea(startX, startY, startX+areaWidth-1, startY+areaHeight-1);
// LCD_WR_REG(0x2C);
void devPrepareForDrawArea(u16 startX, u16 startY, u16 areaWidth, u16 areaHeight){
    drawAreaStartX = startX;
    drawAreaStartY = startY;
    drawAreaEndX = startX+areaWidth-1;
    drawAreaEndY = startY+areaHeight-1;
    drawAreaCurrX = drawAreaStartX;
    drawAreaCurrY = drawAreaStartY;
}

// 绘制一个指定颜色的点，无需考虑坐标位置。（外部会先调用函数devPrepareForDrawArea以确定绘图范围）
void devPointInDrawArea(u16 color){

    if (drawAreaCurrX>drawAreaEndX) {
        drawAreaCurrX=drawAreaStartX;
        drawAreaCurrY++;
    }

    // 已经超出指定绘图范围，不绘制
    if (drawAreaCurrY>drawAreaEndY || drawAreaCurrY > SCREEN_H || drawAreaCurrX > SCREEN_W) return;

    // 在当前位置绘制一个点
    //drawPoint(drawAreaCurrX, drawAreaCurrY, color);   // for gba_drawing.h
    m3_plot(drawAreaCurrX, drawAreaCurrY, color);       // for tonc_video.h

    drawAreaCurrX++;
}

void myDrawLineWidth(u16 x1, u16 y1, u16 x2, u16 y2, u16 width, u16 color){

    //drawLineWidth(x1, y1, x2, y2, width, color);   // for gba_drawing.h

    // for gba_drawing.h
    for ( u16 w = 0; w < width; w++ ) {
        m3_line(x1+w, y1, x2+w, y2, color);
    }
    
}

// 绘制直线
void devDrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 width, u16 color){
    myDrawLineWidth(x1, y1, x2, y2, width, color);
}

// 关闭屏幕显示(跟函数devScreenON搭配使用，如无必要可不用实现留空即可，实现了更好，可以防止刷新画面的过程被用户看见)
void devScreenOFF(){
    //TODO:GBA? LCD_WR_REG(0x28);
}

// 打开屏幕显示(跟函数devScreenOFF搭配使用，如无必要可不用实现留空即可，实现了更好，可以防止刷新画面的过程被用户看见)
void devScreenON(){
    //TODO:GBA? LCD_WR_REG(0x29);
}

// =========================================================================================
// ==    按键相关       =====================================================================
// =========================================================================================
// TODO:

// =========================================================================================
// ==    储存相关       =====================================================================
// =========================================================================================

// 保存配置用EEPROM地址
#define SD_ADDR_PAGE_BASE       0x000
#define SD_ADDR_HSCORE_H        SD_ADDR_PAGE_BASE
#define SD_ADDR_HSCORE_L        SD_ADDR_HSCORE_H+1
#define SD_ADDR_SOUND           SD_ADDR_HSCORE_L+1
#define SD_ADDR_COLOR_BG_H      SD_ADDR_SOUND+1
#define SD_ADDR_COLOR_BG_L      SD_ADDR_COLOR_BG_H+1
#define SD_ADDR_COLOR_FO_H      SD_ADDR_COLOR_BG_L+1
#define SD_ADDR_COLOR_FO_L      SD_ADDR_COLOR_FO_H+1
#define SD_ADDR_COLOR_SNAKE_H   SD_ADDR_COLOR_FO_L+1
#define SD_ADDR_COLOR_SNAKE_L   SD_ADDR_COLOR_SNAKE_H+1
#define SD_ADDR_COLOR_APPLE_H   SD_ADDR_COLOR_SNAKE_L+1
#define SD_ADDR_COLOR_APPLE_L   SD_ADDR_COLOR_APPLE_H+1
#define SD_ADDR_COLOR_FRAME_H   SD_ADDR_COLOR_APPLE_L+1
#define SD_ADDR_COLOR_FRAME_L   SD_ADDR_COLOR_FRAME_H+1
#define SD_ADDR_PAGE_LAST5      0x1FB

#define H8(DATA16)              (u8) ((DATA16>>8) & 0xFF)
#define L8(DATA16)              (u8) (DATA16 & 0xFF)
#define HL16(DATA16H8,DATA16L8) (u16)  ((DATA16H8)*256 + (DATA16L8 & 0xFF))
#define READ8(ADDR)             EEPROM_Read_Byte(ADDR)
#define READ16(ADDR)            HL16(EEPROM_Read_Byte(ADDR), EEPROM_Read_Byte(ADDR+1))
#define WRITE8(ADDR,DATA8)      EEPROM_Write_Byte(ADDR, DATA8)
#define WRITE16(ADDR,DATA16)    EEPROM_Write_Byte(ADDR, H8(DATA16));EEPROM_Write_Byte(ADDR+1, L8(DATA16))

//TODO:GBA? 保存设置
// setting: 默认设置。防止首次上电时储存区还不存在游戏数据，此时请自行判断并保存传入的默认设置。
void devLoadSetting(SaveData_Struct *setting){

    // // 检查是否已经初始化过（如果是首次上电，EEPROM中还不存在配置，此时的数据不可以用）
    // // 检查方法是看扇区最后5个字节的内容是否是预设好的内容。
    // EEPROM_Read_Arry(SD_ADDR_PAGE_LAST5, buff, 5);
    // if (buff[0]!='C' || buff[1]!='L' || buff[2]!='I' || buff[3]!='N' || buff[4]!='G')
    // {
    //     // 如果不是，则保存传入的默认设置
    //     devSaveSetting(setting);
    // } else {
    //     // 读配置
    //     setting->hiScore         = READ16(SD_ADDR_HSCORE_H);
    //     setting->soundOnOff      = READ8 (SD_ADDR_SOUND);
    //     setting->colorBackGround = READ16(SD_ADDR_COLOR_BG_H);
    //     setting->colorFront      = READ16(SD_ADDR_COLOR_FO_H);
    //     setting->colorSnake      = READ16(SD_ADDR_COLOR_SNAKE_H);
    //     setting->colorApple      = READ16(SD_ADDR_COLOR_APPLE_H);
    //     setting->colorFrame      = READ16(SD_ADDR_COLOR_FRAME_H);
    // }
}

//TODO:GBA? 
void devSaveSetting(SaveData_Struct *setting){

    // // 先擦除扇区
    // EEPROM_Erase_Page(SD_ADDR_PAGE_BASE);
    // // 写数据
    // WRITE16(SD_ADDR_HSCORE_H,       setting->hiScore);
    // WRITE8 (SD_ADDR_SOUND,          setting->soundOnOff);
    // WRITE16(SD_ADDR_COLOR_BG_H,     setting->colorBackGround);
    // WRITE16(SD_ADDR_COLOR_FO_H,     setting->colorFront);
    // WRITE16(SD_ADDR_COLOR_SNAKE_H,  setting->colorSnake);
    // WRITE16(SD_ADDR_COLOR_APPLE_H,  setting->colorApple);
    // WRITE16(SD_ADDR_COLOR_FRAME_H,  setting->colorFrame);
    // EEPROM_Write_Str(SD_ADDR_PAGE_LAST5, "CLING");

}

// =========================================================================================
// ==    声音相关       =====================================================================
// =========================================================================================

// maxmod 声音资源定义
mm_sound_effect mariodead = {
		{ SFX_MARIODEAD } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};

// sound effect handle (for cancelling it later)
mm_sfxhand amb = 0;

// 声音模块初始化
void devSndInit(){
    // turn sound on
	REG_SNDSTAT= SSTAT_ENABLE;
	// snd1 on left/right ; both full volume
	REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1, 7);
	// DMG ratio to 100%
	REG_SNDDSCNT= SDS_DMG100;

	// no sweep
	REG_SND1SWEEP= SSW_OFF;

    // maxmod模块初始化
    // initialise maxmod with soundbank and 8 channels
    mmInitDefault( (mm_addr)soundbank_bin, 8 );

    // 播放背景音乐
    //mmStart( MOD_FLATOUTLIES, MM_PLAY_LOOP );
}

void playMaxmodSnd(){
    mmEffectEx(&mariodead);
}

//  超级马里奥死亡时的音乐
//  5444321551
//  .      ...
//  GFFFEDCGGC
//  .      ...
void playGameOver(){
    #define TUNE_TEMPAL_1 180
    #define TUNE_TEMPAL_2 TUNE_TEMPAL_1*1.5
    // envelope: vol=12, decay, max step time (7) ; 50% duty
    // 设置音量，混响衰减度，混响持续时间（1-7）等
	REG_SND1CNT= SSQR_ENV_BUILD(12, 0, 1) | SSQR_DUTY1_2;
	REG_SND1FREQ= 0;

    //REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_G, 1); My_delay_ms(TUNE_TEMPAL_1);
    //REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_G, 1); My_delay_ms(TUNE_TEMPAL_2*2);

    REG_SND1CNT= SSQR_ENV_BUILD(12, 0, 2) | SSQR_DUTY1_2;
    REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_G, 0); My_delay_ms(TUNE_TEMPAL_1);
    REG_SND1CNT= SSQR_ENV_BUILD(12, 0, 1) | SSQR_DUTY1_2;
    REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_F, 1); My_delay_ms(TUNE_TEMPAL_2);
    REG_SND1CNT= SSQR_ENV_BUILD(12, 0, 2) | SSQR_DUTY1_2;
    REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_F, 1); My_delay_ms(TUNE_TEMPAL_1);
    REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_F, 1); My_delay_ms(TUNE_TEMPAL_1);
    REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_E, 1); My_delay_ms(TUNE_TEMPAL_1);
    REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_D, 1); My_delay_ms(TUNE_TEMPAL_1);
    REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_C, 1); My_delay_ms(TUNE_TEMPAL_1);
    REG_SND1CNT= SSQR_ENV_BUILD(12, 0, 1) | SSQR_DUTY1_2;
    REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_G, 0); My_delay_ms(TUNE_TEMPAL_2);
    REG_SND1CNT= SSQR_ENV_BUILD(12, 0, 2) | SSQR_DUTY1_2;
    REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_G, 0); My_delay_ms(TUNE_TEMPAL_1);

    REG_SND1CNT= SSQR_ENV_BUILD(12, 0, 4) | SSQR_DUTY1_2;
    REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_C, 0);
}

void playBeep(u8 beepLen){
    // envelope: vol=12, decay, max step time (7) ; 50% duty
	REG_SND1CNT= SSQR_ENV_BUILD(12, 0, beepLen) | SSQR_DUTY1_2;
	REG_SND1FREQ= 0;

    REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_A, 0);
}

// 短beep声音
void playBeepShort(){
    playBeep(1);
}

// 播放声音
void devPlaySound(Sound_Type soundType){

    switch (soundType)
    {
    case SOUND_EAT_APPLE:
        playBeepShort();
        break;
    case SOUND_HISCORE:
        break;
    case SOUND_MENU:
        break;
    case SOUND_DEAD:
        break;
    case SOUND_GAMEOVER:
        //playGameOver();
        playMaxmodSnd();
        break;
    default:
        break;
    }
}

// =========================================================================================
// ==    画面相关       =====================================================================
// =========================================================================================

// 进入主画面时会调用这个函数，如有需要请实现想要的效果(比如关闭LED)
// 没有要做的留空即可
void devEnterHomePage(){
    //TODO:GBA? LED_ALL_OFF();
}

// 进入Demo画面时会调用这个函数，如有需要请实现想要的效果
// 没有要做的留空即可
void devEnterDemoPage(){
    
}

// 进入游戏画面时会调用这个函数，如有需要请实现想要的效果
// 没有要做的留空即可
void devEnterGamePage(){
    
}

// 进入GameOver画面时会调用这个函数，如有需要请实现想要的效果
// 没有要做的留空即可
void devEnterGameOverPage(){
    
}

// =========================================================================================
// ==    其他          =====================================================================
// =========================================================================================
                                
// 当玩家改变游戏速度时会调用这个函数，请实现想要的效果(比如点亮不同的LED)
void devSpeedChanged(u16 speed){

    switch (speed)
    {
    case SPEED_DEMO_L:
        LED_ALL_OFF();
        break;
    case SPEED_DEMO_M:
        LED_R_ON();
        LED_Y_OFF();
        LED_G_OFF();
        break;
    case SPEED_DEMO_H:
        LED_R_ON();
        LED_Y_ON();
        LED_G_OFF();
        break;
    case SPEED_DEMO_S:
        LED_ALL_ON();
        break;
    default:
        break;
    }
}

