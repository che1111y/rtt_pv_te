/*********************************************************************************************************************
* CH32V307VCT6 Opensourec Library 即（CH32V307VCT6 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是CH32V307VCT6 开源库的一部分
*
* CH32V307VCT6 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          zf_device_ips200
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MounRiver Studio V1.8.1
* 适用平台          CH32V307VCT6
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期                                      作者                             备注
* 2022-09-15        大W            first version
********************************************************************************************************************/
/*********************************************************************************************************************
* 接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   // 双排排针 并口两寸屏 硬件引脚
*                   RD                  查看 zf_device_ips200.h 中 IPS200_RD_PIN_PARALLEL8     宏定义
*                   WR                  查看 zf_device_ips200.h 中 IPS200_WR_PIN_PARALLEL8     宏定义
*                   RS                  查看 zf_device_ips200.h 中 IPS200_RS_PIN_PARALLEL8     宏定义
*                   RST                 查看 zf_device_ips200.h 中 IPS200_RST_PIN_PARALLEL8    宏定义
*                   CS                  查看 zf_device_ips200.h 中 IPS200_CS_PIN_PARALLEL8     宏定义
*                   BL                  查看 zf_device_ips200.h 中 IPS200_BL_PIN_PARALLEL8     宏定义
*                   D0-D7               查看 zf_device_ips200.h 中 IPS200_Dx_PIN_PARALLEL8     宏定义
*                   // 单排排针 SPI 两寸屏 硬件引脚
*                   SCL                 查看 zf_device_ips200.h 中 IPS200_SCL_PIN_SPI  宏定义
*                   SDA                 查看 zf_device_ips200.h 中 IPS200_SDA_PIN_SPI  宏定义
*                   RST                 查看 zf_device_ips200.h 中 IPS200_RST_PIN_SPI  宏定义
*                   DC                  查看 zf_device_ips200.h 中 IPS200_DC_PIN_SPI   宏定义
*                   CS                  查看 zf_device_ips200.h 中 IPS200_CS_PIN_SPI   宏定义
*                   BLk                 查看 zf_device_ips200.h 中 IPS200_BLk_PIN_SPI  宏定义
*                   电源引脚
*                   VCC                 3.3V电源
*                   GND                 电源地
*                   最大分辨率 320 * 240
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _ips200_h_
#define _ips200_h_

#include "main.h"



// --------------------单排两寸屏幕SPI接口引脚定义--------------------//

#define IPS200_USE_SOFT_SPI             (0 )                                    // 默认使用硬件 SPI 方式驱动 建议使用硬件 SPI 方式驱动
//====================================================硬件 SPI 驱动====================================================
// 如果使用的是单排排针的两寸屏幕 SPI 驱动控制引脚 可以修改
#define IPS200_SPI_SPEED                (72 * 1000 * 1000  )                    // 硬件 SPI 速率 这里设置为系统时钟二分频
#define IPS200_SPI                      (SPI1             )                    // 硬件 SPI 号
#define IPS200_SCL_PIN_SPI              (SPI1_MAP0_SCK_B13 )                    // 硬件 SPI SCK 引脚
#define IPS200_SDA_PIN_SPI              (SPI1_MAP0_MOSI_B15)                    // 硬件 SPI MOSI 引脚
//====================================================硬件 SPI 驱动====================================================

// 如果使用的是单排排针的两寸屏幕 SPI 驱动控制引脚 可以修改
#define IPS200_RST_PIN_PORT             GPIOB
#define IPS200_RST_PIN_SPI              GPIO_PIN_4                                    // 液晶复位引脚定义

#define IPS200_DC_PIN_PORT              GPIOE
#define IPS200_DC_PIN_SPI               GPIO_PIN_7                                   // 液晶命令位引脚定义

#define IPS200_CS_PIN_PORT              GPIOE
#define IPS200_CS_PIN_SPI               GPIO_PIN_8 

#define IPS200_BLk_PIN_PORT             GPIOE
#define IPS200_BLk_PIN_SPI              GPIO_PIN_9

// --------------------双排SPI接口两寸屏幕引脚定义--------------------//

#define IPS200_DEFAULT_DISPLAY_DIR      (IPS200_CROSSWISE)                        // 默认的显示方向
#define IPS200_DEFAULT_PENCOLOR         (RGB565_WHITE    )                        // 默认的画笔颜色
#define IPS200_DEFAULT_BGCOLOR          (RGB565_PINK  )                        // 默认的背景颜色
#define IPS200_DEFAULT_DISPLAY_FONT     (IPS200_8X16_FONT)                      // 默认的字体模式

// 控制语句

#define IPS200_RST(x)       ((x) ? (HAL_GPIO_WritePin(IPS200_RST_PIN_PORT, IPS200_RST_PIN_SPI, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(IPS200_RST_PIN_PORT, IPS200_RST_PIN_SPI, GPIO_PIN_RESET)))
#define IPS200_CS(x)        ((x) ? (HAL_GPIO_WritePin(IPS200_CS_PIN_PORT, IPS200_CS_PIN_SPI, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(IPS200_CS_PIN_PORT, IPS200_CS_PIN_SPI, GPIO_PIN_RESET)))
#define IPS200_DC(x)        ((x) ? (HAL_GPIO_WritePin(IPS200_DC_PIN_PORT, IPS200_DC_PIN_SPI, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(IPS200_DC_PIN_PORT, IPS200_DC_PIN_SPI, GPIO_PIN_RESET)))
#define IPS200_BLk(x)       ((x) ? (HAL_GPIO_WritePin(IPS200_BLk_PIN_PORT, IPS200_BLk_PIN_SPI, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(IPS200_BLk_PIN_PORT, IPS200_BLk_PIN_SPI, GPIO_PIN_RESET)))


typedef enum
{
    IPS200_TYPE_SPI,                                                            // SPI 驱动
    IPS200_TYPE_PARALLEL8,                                                      // 并口驱动
}ips200_type_enum;

typedef enum
{
    IPS200_PORTAIT                      = 0,                                    // 竖屏模式
    IPS200_PORTAIT_180                  = 1,                                    // 竖屏模式  旋转180
    IPS200_CROSSWISE                    = 2,                                    // 横屏模式
    IPS200_CROSSWISE_180                = 3,                                    // 横屏模式  旋转180
}ips200_dir_enum;

typedef enum
{
    IPS200_6X8_FONT                     = 0,                                    // 6x8      字体
    IPS200_8X16_FONT                    = 1,                                    // 8x16     字体
    IPS200_16X16_FONT                   = 2,                                    // 16x16    字体 目前不支持
}ips200_font_size_enum;

void    ips200_clear                    (void);
void    ips200_full                     (const uint16_t color);
void    ips200_write_16bit_data     (const uint16_t dat);
void    ips200_set_region           (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void    ips200_write_8bit_data      (const uint8_t dat);
void    ips200_set_dir                  (ips200_dir_enum dir);
void    ips200_set_font                 (ips200_font_size_enum font);
void    ips200_set_color                (const uint16_t pen, const uint16_t bgcolor);
void    ips200_draw_point               (uint16_t x, uint16_t y, const uint16_t color);
void    ips200_draw_line                (uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, const uint16_t color);

void    ips200_show_char                (uint16_t x, uint16_t y, const char dat);
void    ips200_show_string              (uint16_t x, uint16_t y, const char dat[]);
void    ips200_show_int                 (uint16_t x, uint16_t y, const int32_t dat, uint8_t num);
void    ips200_show_uint                (uint16_t x, uint16_t y, const uint32_t dat, uint8_t num);
void    ips200_show_float               (uint16_t x, uint16_t y, const float dat, uint8_t num, uint8_t pointnum);

void    ips200_show_binary_image        (uint16_t x, uint16_t y, const uint8_t *image, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height);
void    ips200_show_gray_image          (uint16_t x, uint16_t y, const uint8_t *image, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t threshold);
void    ips200_show_rgb565_image        (uint16_t x, uint16_t y, const uint16_t *image, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t color_mode);

void    ips200_show_wave                (uint16_t x, uint16_t y, const uint16_t *wave, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max);
void    ips200_show_chinese             (uint16_t x, uint16_t y, uint8_t size, const uint8_t *chinese_buffer, uint8_t number, const uint16_t color);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示小钻风图像
// 参数说明     p               图像数组
// 参数说明     width           显示宽度
// 参数说明     height          显示高度
// 返回参数     void
// 使用示例     ips200_displayimage7725(ov7725_image_binary[0], 80, 60);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
#define ips200_displayimage7725(p, width, height)       (ips200_show_binary_image(0, 0, (p), OV7725_W, OV7725_H, (width), (height)))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示总钻风图像 不带二值化 显示灰度图像
// 参数说明     p               图像数组
// 参数说明     width           显示宽度
// 参数说明     height          显示高度
// 返回参数     void
// 使用示例     ips200_displayimage03x(mt9v03x_image[0], 94, 60);
// 备注信息     如果要显示二值化图像就去调用 ips200_show_gray_image 函数
//-------------------------------------------------------------------------------------------------------------------
#define ips200_displayimage03x(p, width, height)       (ips200_show_gray_image(0, 0, (p), MT9V03X_W, MT9V03X_H, (width), (height), 0))

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示凌瞳图像
// 参数说明     p               图像数组
// 参数说明     width           显示宽度
// 参数说明     height          显示高度
// 返回参数     void
// 使用示例     ips200_displayimage8660(scc8660_image[0], 80, 60);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
#define ips200_displayimage8660(p, width, height)       (ips200_show_rgb565_image(0, 0, (p), SCC8660_W, SCC8660_H, (width), (height), 1))

void    ips200_init                     (void);

#endif
