/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-24                  the first version
 */

#include <rthw.h>
#include <rtthread.h>

#include "main.h"

struct PID **fan_pid_vector;
struct PID **fuzzy_pid_vector;

// int control_id = 5;
int control_fan, control_teg, fan_out, teg_out;
float idea_temp_cold;
float idea_temp_hot;
bool direct[DOF] = {false, false, true, true, true, true};

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
/*
 * Please modify RT_HEAP_SIZE if you enable RT_USING_HEAP
 * the RT_HEAP_SIZE max value = (sram size - ZI size), 1024 means 1024 bytes
 */
#define RT_HEAP_SIZE (15*1024)
static rt_uint8_t rt_heap[RT_HEAP_SIZE];

RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

void SysTick_Handler(void)
{
    rt_interrupt_enter();
    
    rt_tick_increase();

    rt_interrupt_leave();
}

#ifdef RT_USING_FINSH
#include <finsh.h>
static void reboot(uint8_t argc, char **argv)
{
    rt_hw_cpu_reset();
}
MSH_CMD_EXPORT(reboot, Reboot System);
#endif /* RT_USING_FINSH */

/**
 * This function will initial your board.
 */
void rt_hw_board_init(void)
{
    extern void SystemClock_Config(void);             
    HAL_Init();
    SystemClock_Config();
    SystemCoreClockUpdate();
    /* 
     * 1: OS Tick Configuration
     * Enable the hardware timer and call the rt_os_tick_callback function
     * periodically with the frequency RT_TICK_PER_SECOND. 
     */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/RT_TICK_PER_SECOND);

    // MX_GPIO_Init();
    //ads1256 init
    ADS1256_GPIO_Init();
    // delay_ms (500);
    ADS1256_CfgADC(PGA_1, DATARATE_100);

    MX_USART1_UART_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_SPI1_Init();

    ips200_init ();

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_RESET);

    control_fan = 0; 
    control_teg = 1;
    idea_temp_cold = 15.0f;
    idea_temp_hot = idea_temp_cold*1.2f;

    // Default fuzzy rule base of delta kp/ki/kd
    int rule_base[][qf_default] = {
          //delta kp rule base
          {PB, PB, PM, PM, PS, ZO, ZO},
          {PB, PB, PM, PS, PS, ZO, NS},
          {PM, PM, PM, PS, ZO, NS, NS},
          {PM, PM, PS, ZO, NS, NM, NM},
          {PS, PS, ZO, NS, NS, NM, NM},
          {PS, ZO, NS, NM, NM, NM, NB},
          {ZO, ZO, NM, NM, NM, NB, NB},
          //delta ki rule base
          {NB, NB, NM, NM, NS, ZO, ZO},
          {NB, NB, NM, NS, NS, ZO, ZO},
          {NB, NM, NS, NS, ZO, PS, PS},
          {NM, NM, NS, ZO, PS, PM, PM},
          {NM, NS, ZO, PS, PS, PM, PB},
          {ZO, ZO, PS, PS, PM, PB, PB},
          {ZO, ZO, PS, PM, PM, PB, PB},
          //delta kd rule base
          {PS, NS, NB, NB, NB, NM, PS},
          {PS, NS, NB, NM, NM, NS, ZO},
          {ZO, NS, NM, NM, NS, NS, ZO},
          {ZO, NS, NS, NS, NS, NS, ZO},
          {ZO, ZO, ZO, ZO, ZO, ZO, ZO},
          {PB, PS, PS, PS, PS, PS, PB},
          {PB, PM, PM, PM, PS, PS, PB}};
  // Default parameters of membership function
  int mf_params[4 * qf_default] = {-8, -8, -4,  0,
                                   -8, -4, -1,  0,
                                   -4, -1,  0,  0,
                                   -1,  0,  1,  0,
                                    0,  1,  4,  0,
                                    1,  4,  8,  0,
                                    4,  8,  8,  0};
  // Default parameters of pid controller
  float fuzzy_pid_params[DOF][pid_params_count] = {{20.4f,   0,     0,    5,     0.1f, 0, 1},//kp, ki, kd, integral_limit, dead_zone, feed_forward,
                                                   {4.0f,   0,     0,    5,     0.1f, 0, 1},
                                                   {1.1f,   0,     0,    0,     0,    0, 1},
                                                   {2.4f,   0,     0,    0,     0,    0, 1},
                                                   {1.2f,   0,     0,    0,     0,    0, 1},
                                                   {1.2f,   0,     0,    0,     0,    0, 1}};
  float pid_params[DOF][pid_params_count] = {{10.2f,   0,     0,    5,     0.1f, 0, 1}};
  // Obtain the PID controller vector according to the parameters
  fuzzy_pid_vector = fuzzy_pid_vector_init(
                    fuzzy_pid_params,  
                    1.0f, 
                    4, 
                    1, 
                    0, 
                    mf_params, 
                    rule_base, 
                    DOF
                    );//delta_kPID参数的调整幅度比例, mf_type隶属度函数类型, fo_type模糊化方法, df_type解模糊化方法
  
  fan_pid_vector = pid_vector_init(
                    pid_params, 
                    DOF
                    );

    //测试用
    // while (1);

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

#ifdef RT_USING_CONSOLE

static UART_HandleTypeDef UartHandle;
static int uart_init(void)
{
    /* TODO: Please modify the UART port number according to your needs */
    UartHandle.Instance = USART1;
    UartHandle.Init.BaudRate = 115200;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits = UART_STOPBITS_1;
    UartHandle.Init.Parity = UART_PARITY_NONE;
    UartHandle.Init.Mode = UART_MODE_TX_RX;
    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        while (1);
    }
    return 0;
}
INIT_BOARD_EXPORT(uart_init);

void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = 0;
    char a = '\r';

    __HAL_UNLOCK(&UartHandle);

    size = rt_strlen(str);

    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            HAL_UART_Transmit(&UartHandle, (uint8_t *)&a, 1, 1);
        }
        HAL_UART_Transmit(&UartHandle, (uint8_t *)(str + i), 1, 1);
    }
}
#endif

#ifdef RT_USING_FINSH
char rt_hw_console_getchar(void)
{
    /* Note: the initial value of ch must < 0 */
    int ch = -1;

    if (__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_RXNE) != RESET)
    {
        ch = UartHandle.Instance->DR & 0xff;
    }
    else
    {
        rt_thread_mdelay(10);
    }
    return ch;
}
#endif
