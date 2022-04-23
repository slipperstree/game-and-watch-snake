/**************************************************
 * File  : embSnakeDevice.c
 * Author: MANDA (slipperstree@gmail.com)
 * Date  : 2019.11
 * Updt  : 2021.07
 * Func  : �豸����ģ�飬������ʵ�ֱ��ļ��ڵĸ���������������豸���ⲿ������Ҫ��ʱ�������Щ����
***************************************************/

#include "embSnakeDevice.h"
#include "boradSupport.h"

// ��ʱ������(sprintf��)
extern u8 buff[128];

u16 drawAreaStartX = 0;
u16 drawAreaStartY = 0;
u16 drawAreaEndX = 0;
u16 drawAreaEndY = 0;
u16 drawAreaCurrX = 0;
u16 drawAreaCurrY = 0;

// =========================================================================================
// ==    ��ʾ�豸���    =====================================================================
// =========================================================================================

// ��ʼ����ʾ�豸
// bgColor : ����ɫ�������ڳ�������
void devDisplayInit(u16 bgColor){
    //TODO:GBA? LCD_Init();
    devScreenOFF(); // �ر�LCD��ʾ
    //TODO:GBA? LCD_Clear(bgColor);
    devScreenON();  // ��LCD��ʾ
    //TODO:GBA? TFT_BL = 0;     // ��Һ���������
}

// ���һ����������
void devFillRectange(u16 x, u16 y, u16 width, u16 height, u16 color){
    //drawRect(x, y, width, height, color);         // for gba_drawing.h
    m3_rect(x, y, x+width, y+height, color);    // for tonc_video.h
}

// Ϊ����һ��������׼�������ⲿ������������������������devPointInDrawArea�����Ƹ���ͼ����������֣�
// һ����˵��������õײ��������ʾ���������Լ���ʼ��ͼ����
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

// ����һ��ָ����ɫ�ĵ㣬���迼������λ�á����ⲿ���ȵ��ú���devPrepareForDrawArea��ȷ����ͼ��Χ��
void devPointInDrawArea(u16 color){

    if (drawAreaCurrX>drawAreaEndX) {
        drawAreaCurrX=drawAreaStartX;
        drawAreaCurrY++;
    }

    // �Ѿ�����ָ����ͼ��Χ��������
    if (drawAreaCurrY>drawAreaEndY || drawAreaCurrY > SCREEN_H || drawAreaCurrX > SCREEN_W) return;

    // �ڵ�ǰλ�û���һ����
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

// ����ֱ��
void devDrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 width, u16 color){
    myDrawLineWidth(x1, y1, x2, y2, width, color);
}

// �ر���Ļ��ʾ(������devScreenON����ʹ�ã����ޱ�Ҫ�ɲ���ʵ�����ռ��ɣ�ʵ���˸��ã����Է�ֹˢ�»���Ĺ��̱��û�����)
void devScreenOFF(){
    //TODO:GBA? LCD_WR_REG(0x28);
}

// ����Ļ��ʾ(������devScreenOFF����ʹ�ã����ޱ�Ҫ�ɲ���ʵ�����ռ��ɣ�ʵ���˸��ã����Է�ֹˢ�»���Ĺ��̱��û�����)
void devScreenON(){
    //TODO:GBA? LCD_WR_REG(0x29);
}

// =========================================================================================
// ==    �������       =====================================================================
// =========================================================================================
// TODO:

// =========================================================================================
// ==    �������       =====================================================================
// =========================================================================================

// ����������EEPROM��ַ
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

//TODO:GBA? ��������
// setting: Ĭ�����á���ֹ�״��ϵ�ʱ����������������Ϸ���ݣ���ʱ�������жϲ����洫���Ĭ�����á�
void devLoadSetting(SaveData_Struct *setting){

    // // ����Ƿ��Ѿ���ʼ������������״��ϵ磬EEPROM�л����������ã���ʱ�����ݲ������ã�
    // // ��鷽���ǿ��������5���ֽڵ������Ƿ���Ԥ��õ����ݡ�
    // EEPROM_Read_Arry(SD_ADDR_PAGE_LAST5, buff, 5);
    // if (buff[0]!='C' || buff[1]!='L' || buff[2]!='I' || buff[3]!='N' || buff[4]!='G')
    // {
    //     // ������ǣ��򱣴洫���Ĭ������
    //     devSaveSetting(setting);
    // } else {
    //     // ������
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

    // // �Ȳ�������
    // EEPROM_Erase_Page(SD_ADDR_PAGE_BASE);
    // // д����
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
// ==    �������       =====================================================================
// =========================================================================================

// maxmod ������Դ����
mm_sound_effect mariodead = {
		{ SFX_MARIODEAD } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};

// sound effect handle (for cancelling it later)
mm_sfxhand amb = 0;

// ����ģ���ʼ��
void devSndInit(){
    // turn sound on
	REG_SNDSTAT= SSTAT_ENABLE;
	// snd1 on left/right ; both full volume
	REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1, 7);
	// DMG ratio to 100%
	REG_SNDDSCNT= SDS_DMG100;

	// no sweep
	REG_SND1SWEEP= SSW_OFF;

    // maxmodģ���ʼ��
    // initialise maxmod with soundbank and 8 channels
    mmInitDefault( (mm_addr)soundbank_bin, 8 );

    // ���ű�������
    //mmStart( MOD_FLATOUTLIES, MM_PLAY_LOOP );
}

void playMaxmodSnd(){
    mmEffectEx(&mariodead);
}

//  �������������ʱ������
//  5444321551
//  .      ...
//  GFFFEDCGGC
//  .      ...
void playGameOver(){
    #define TUNE_TEMPAL_1 180
    #define TUNE_TEMPAL_2 TUNE_TEMPAL_1*1.5
    // envelope: vol=12, decay, max step time (7) ; 50% duty
    // ��������������˥���ȣ��������ʱ�䣨1-7����
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

// ��beep����
void playBeepShort(){
    playBeep(1);
}

// ��������
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
// ==    �������       =====================================================================
// =========================================================================================

// ����������ʱ��������������������Ҫ��ʵ����Ҫ��Ч��(����ر�LED)
// û��Ҫ�������ռ���
void devEnterHomePage(){
    //TODO:GBA? LED_ALL_OFF();
}

// ����Demo����ʱ��������������������Ҫ��ʵ����Ҫ��Ч��
// û��Ҫ�������ռ���
void devEnterDemoPage(){
    
}

// ������Ϸ����ʱ��������������������Ҫ��ʵ����Ҫ��Ч��
// û��Ҫ�������ռ���
void devEnterGamePage(){
    
}

// ����GameOver����ʱ��������������������Ҫ��ʵ����Ҫ��Ч��
// û��Ҫ�������ռ���
void devEnterGameOverPage(){
    
}

// =========================================================================================
// ==    ����          =====================================================================
// =========================================================================================
                                
// ����Ҹı���Ϸ�ٶ�ʱ����������������ʵ����Ҫ��Ч��(���������ͬ��LED)
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

