#include "WS2812B.h"
#include "tim.h"

#define MAX_LED 1    // 灯的数量
#define USE_BRIGHTNESS 1    // 启用亮度控制

uint8_t LED_Data[MAX_LED][4];
uint8_t LED_Mod[MAX_LED][4];  // 用于亮度控制

void Set_LED(int LEDnum, int Red, int Green, int Blue) {
    if (LEDnum < 0 || LEDnum >= MAX_LED) return;

    Red = (Red < 0) ? 0 : (Red > 255) ? 255 : Red;
    Green = (Green < 0) ? 0 : (Green > 255) ? 255 : Green;
    Blue = (Blue < 0) ? 0 : (Blue > 255) ? 255 : Blue;

    LED_Data[LEDnum][0] = LEDnum;
    LED_Data[LEDnum][1] = Green;
    LED_Data[LEDnum][2] = Red;
    LED_Data[LEDnum][3] = Blue;
}

void Set_LED_HEX(int LEDnum, uint32_t colorValue) {
    if (LEDnum < 0 || LEDnum >= MAX_LED) return;

    int Red = (colorValue >> 16) & 0xFF;
    int Green = (colorValue >> 8) & 0xFF;
    int Blue = colorValue & 0xFF;

    LED_Data[LEDnum][0] = LEDnum;
    LED_Data[LEDnum][1] = Green;
    LED_Data[LEDnum][2] = Red;
    LED_Data[LEDnum][3] = Blue;
}

void Set_Brightness(int brightness) {
#if USE_BRIGHTNESS
    brightness = (brightness > 100) ? 100 : (brightness < 0) ? 0 : brightness;

    for (int i = 0; i < MAX_LED; i++) {
        LED_Mod[i][0] = LED_Data[i][0];
        for (int j = 1; j < 4; j++) {
            LED_Mod[i][j] = (LED_Data[i][j] * brightness) / 100;
        }
    }
#endif
}

uint16_t pwmData[(24 * MAX_LED) + 50];

// 发送数据
void WS2812_Send(void) {
    uint32_t indx = 0;
    uint32_t color;

    for (int i = 0; i < MAX_LED; i++) {
#if USE_BRIGHTNESS
        color = ((LED_Mod[i][1] << 16) | (LED_Mod[i][2] << 8) | (LED_Mod[i][3]));
#else
        color = ((LED_Data[i][1] << 16) | (LED_Data[i][2] << 8) | (LED_Data[i][3]));
#endif
        for (int j = 23; j >= 0; j--) {
            if (color & (1 << j)) {
                pwmData[indx] = 43;  // T1H,高电平持续时间
            } else {
                pwmData[indx] = 17;  // T0H,低电平持续时间
            }
            indx++;
        }
    }

    for (int i = 0; i < 50; i++) {
        pwmData[indx++] = 0;				//T1L，T0L持续时间
    }

    HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t *)pwmData, indx);
}
