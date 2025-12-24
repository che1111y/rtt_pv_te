/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern struct PID **fan_pid_vector;
extern struct PID **fuzzy_pid_vector;
extern int control_fan, control_teg, fan_out, teg_out;
extern float idea_temp_cold;
extern float idea_temp_hot;
extern bool direct[DOF];
/* 定义线程控制块指针 */
static  rt_thread_t adc_thread = RT_NULL;
static  rt_thread_t wifi_thread = RT_NULL;
static  rt_thread_t scrn_thread = RT_NULL;
static  rt_thread_t pid_thread = RT_NULL;
static  rt_thread_t pwm_thread = RT_NULL;

/* 定义邮箱控制块 */
static rt_mailbox_t temp_mail = RT_NULL;
static rt_mailbox_t pwm_mail = RT_NULL;
static rt_mailbox_t display_mail = RT_NULL;

//任务
static void adc_thread_entry(void *parameter);
static void wifi_thread_entry(void *parameter);
static void scrn_thread_entry(void *parameter);
static void pid_thread_entry(void *parameter);
static void pwm_thread_entry(void *parameter);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
float temp[8], R[8], V[8];

/* 邮箱数据结构 */
typedef struct {
    float temp_data[8];  // 存储8个温度值
} temp_data_t;

// PWM数据结构
typedef struct {
    int fan_output;
    int teg_output;
} pwm_data_t;

// 屏幕显示数据结构
typedef struct {
    float temp_values[8];  // 存储8个温度值
    int fan_pwm;
    int teg_pwm;
} display_data_t;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//printf 重定义
int fputc(int ch, FILE *f)
{
  uint8_t temp[1] = {ch};
  HAL_UART_Transmit(&huart1, temp, 1, 2);
  return ch;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* 创建temp邮箱 */
  temp_mail = rt_mb_create("temp_mail", /* 邮箱名字 */
                            20,         /* 邮箱大小 */
                            RT_IPC_FLAG_FIFO);/* 信号量模式 FIFO(0x00)*/
  if (temp_mail != RT_NULL)
    rt_kprintf("Temperature data mailbox created successfully\n\r");

  /* 创建pwm邮箱 */
  pwm_mail = rt_mb_create("pwm_mail", /* 邮箱名字 */
                            20,         /* 邮箱大小 */
                            RT_IPC_FLAG_FIFO);/* 信号量模式 FIFO(0x00)*/
  if (pwm_mail != RT_NULL)
    rt_kprintf("PWM data mailbox created successfully\n\r");

  /* 创建屏幕显示邮箱 */
  display_mail = rt_mb_create("display_mail", /* 邮箱名字 */
                            20,         /* 邮箱大小 */
                            RT_IPC_FLAG_FIFO);/* 信号量模式 FIFO(0x00)*/
  if (display_mail != RT_NULL)
    rt_kprintf("Display data mailbox created successfully\n\r");

  //初始化adc线程
	adc_thread =                           /* 线程控制块指针 */   
    rt_thread_create( "adc",               /* 线程名字 */        
                adc_thread_entry,          /* 线程入口函数 */       
                RT_NULL,                   /* 线程入口函数参数 */      
                512,                       /* 线程栈大小 */        
                2,                         /* 线程的优先级 */         
                20);                       /* 线程时间片 */          
	//开启线程调度
	if (adc_thread != RT_NULL)
        rt_thread_startup(adc_thread);     /* 启动线程，开启调度 */
    else
        return -1;

  //初始化wifi线程
  wifi_thread =                           /* 线程控制块指针 */   
    rt_thread_create( "wifi",               /* 线程名字 */        
                wifi_thread_entry,          /* 线程入口函数 */       
                RT_NULL,                   /* 线程入口函数参数 */      
                512,                       /* 线程栈大小 */        
                3,                         /* 线程的优先级 */         
                20);                       /* 线程时间片 */          
	//开启线程调度
	if (wifi_thread != RT_NULL)
        rt_thread_startup(wifi_thread);     /* 启动线程，开启调度 */
    else
        return -1;

  //初始化pwm线程
  pwm_thread =                           /* 线程控制块指针 */   
    rt_thread_create( "pwm",               /* 线程名字 */        
                pwm_thread_entry,          /* 线程入口函数 */       
                RT_NULL,                   /* 线程入口函数参数 */      
                512,                       /* 线程栈大小 */        
                0,                         /* 线程的优先级 */         
                20);                       /* 线程时间片 */          
	//开启线程调度
	if (pwm_thread != RT_NULL)
        rt_thread_startup(pwm_thread);     /* 启动线程，开启调度 */
    else
        return -1;
  
  //初始化pid线程
  pid_thread =                           /* 线程控制块指针 */   
    rt_thread_create( "pid",               /* 线程名字 */        
                pid_thread_entry,          /* 线程入口函数 */       
                RT_NULL,                   /* 线程入口函数参数 */      
                512,                       /* 线程栈大小 */        
                1,                         /* 线程的优先级 */         
                20);                       /* 线程时间片 */          
	//开启线程调度
	if (pid_thread != RT_NULL)
        rt_thread_startup(pid_thread);     /* 启动线程，开启调度 */
    else
        return -1;

  //初始化屏幕显示线程
  scrn_thread =                           /* 线程控制块指针 */   
    rt_thread_create( "scrn",               /* 线程名字 */        
                scrn_thread_entry,          /* 线程入口函数 */       
                RT_NULL,              /* 线程入口函数参数 */      
                512,                       /* 线程栈大小 */        
                4,                         /* 线程的优先级 */         
                20);                       /* 线程时间片 */          
	//开启线程调度
	if (scrn_thread != RT_NULL)
        rt_thread_startup(scrn_thread);     /* 启动线程，开启调度 */
    else
        return -1;

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//ADC
static void adc_thread_entry(void *parameter)
{
  temp_data_t *temp_msg;
	while(1)
	{
    // 分配内存用于存储温度数据
    temp_msg = (temp_data_t*)rt_malloc(sizeof(temp_data_t));
    if(temp_msg == RT_NULL)
    {
      rt_kprintf("Memory allocation failed!\n");
      rt_thread_mdelay(100);
      continue;
    }
    for(int i=0;i<8;i++)
    {
      // rt_kprintf("adc0:%d\r\n",(int32_t)ADS1256_GetAdc(0<<4 | 0x08));
      // V[i] = ADS1256_GetVolt(i, 0);
      // rt_kprintf("Voltage%d: %d.%dV\r\n",i, (int)V[i], (int)(V[i]*1000)%1000);
      // R[i] = Get_Rntc(i, 10);
      // rt_kprintf("Rntc%d: %d.%dO\r\n",i, (int)R[i], (int)(R[i]*100)%100);
      temp_msg->temp_data[i] = GetAccuraryTemperature(Get_Rntc(i, 10));
		  rt_kprintf("Temperature%d: %d.%dC\r\n",i, (int)temp_msg->temp_data[i], (int)(temp_msg->temp_data[i]*1000)%1000);
      // rt_thread_mdelay(20);
    }
    if(rt_mb_send(temp_mail, (rt_uint32_t)temp_msg) != RT_EOK)
    {
      rt_kprintf("ADC Mailbox send failed!\n");
      rt_free(temp_msg);  // 发送失败时释放内存
    }
    // rt_thread_mdelay(50);
	}
}

//WIFI
static void wifi_thread_entry(void *parameter)
{
  rt_kprintf("WiFi thread started!\n\r");  // 添加启动确认
	while(1)
	{
    rt_thread_mdelay(10);
	}
}

//屏幕显示
static void scrn_thread_entry(void *parameter)
{
  rt_kprintf("scrn thread started!\n\r");  // 添加启动确认
  display_data_t *disp_data;
  rt_ubase_t msg;
    
  // 画好所有的标签（只画一次）
  ips200_show_string(10, 10, "Temp0:");
  ips200_show_string(10, 30, "Temp1:");
  ips200_show_string(10, 50, "Temp2:");
  ips200_show_string(10, 70, "Temp3:");
  ips200_show_string(10, 90, "Temp4:");
  ips200_show_string(10, 110, "Temp5:");
  ips200_show_string(10, 130, "Temp6:");
  ips200_show_string(10, 150, "Temp7:");

  ips200_show_string(10, 180, "Fan PWM:");
  ips200_show_string(10, 200, "TEG PWM:");

	while(1)
	{
    // 从邮箱接收显示数据
    if(rt_mb_recv(display_mail, &msg, RT_WAITING_FOREVER) == RT_EOK)
    {
      disp_data = (display_data_t*)msg;

      //显示温度数据
      char temp_str[32];
      for(int i = 0; i < 8; i++) {
        snprintf(temp_str, sizeof(temp_str), "Temp%d: %.2f C", i, disp_data->temp_values[i]);
        ips200_show_string(10, 10 + i*20, temp_str);
      }
      
      //显示PWM数据
      char fan_str[32], teg_str[32];
      snprintf(fan_str, sizeof(fan_str), "Fan PWM: %d", disp_data->fan_pwm);
      snprintf(teg_str, sizeof(teg_str), "TEG PWM: %d", disp_data->teg_pwm);
      ips200_show_string(10, 180, fan_str);
      ips200_show_string(10, 200, teg_str);
      
      // 释放内存
      rt_free(disp_data);
    }
	}
}

static void pid_thread_entry(void *parameter){
  rt_kprintf("PID thread started!\n\r");  // 添加启动确认信息
  
  temp_data_t *received_temp;
  rt_ubase_t msg;
  pwm_data_t *pwm_msg;

	while(1){
    // 从邮箱接收温度数据
    if(rt_mb_recv(temp_mail, &msg, RT_WAITING_FOREVER) == RT_EOK){
      received_temp = (temp_data_t*)msg;
        
      // 使用接收到的温度数据进行PID计算
      teg_out = fuzzy_pid_motor_pwd_output(received_temp->temp_data[0], idea_temp_cold, 
                                          direct[control_teg], fuzzy_pid_vector[control_teg]);
      fan_out = pid_motor_pwd_output(received_temp->temp_data[1], idea_temp_hot, 
                                          direct[control_fan], fan_pid_vector[control_fan]); 

      rt_kprintf("fanOutput:%d,Real:%d.%d\n\r", (int)(fan_out), 
                                                (int)received_temp->temp_data[1], 
                                                (int)(received_temp->temp_data[1]*1000)%1000);
      rt_kprintf("tegOutput:%d,Real:%d.%d\n\r", (int)(teg_out), 
                                                (int)received_temp->temp_data[0], 
                                                (int)(received_temp->temp_data[0]*1000)%1000);

      // 创建并发送PWM数据到pwm线程
      pwm_msg = (pwm_data_t*)rt_malloc(sizeof(pwm_data_t));
      if(pwm_msg != RT_NULL){
        pwm_msg->fan_output = fan_out;
        pwm_msg->teg_output = teg_out;
        
        if(rt_mb_send(pwm_mail, (rt_uint32_t)pwm_msg) != RT_EOK){
          rt_kprintf("PWM mailbox send failed!\n\r");
          rt_free(pwm_msg);
        }
        else{
          rt_kprintf("PWM mailbox sent successfully!\n\r");
        }

        // 发送数据到屏幕显示线程
        display_data_t *disp_data = (display_data_t*)rt_malloc(sizeof(display_data_t));
        if (disp_data != RT_NULL) {
          // 复制温度数据
          for (int i = 0; i < 8; i++) {
            disp_data->temp_values[i] = received_temp->temp_data[i];
          }
          // 设置PWM值
          disp_data->fan_pwm = fan_out;
          disp_data->teg_pwm = teg_out;
          
          // 发送到显示邮箱
          if (rt_mb_send(display_mail, (rt_uint32_t)disp_data) != RT_EOK) {
            rt_kprintf("Display mailbox send failed!\n\r");
            rt_free(disp_data);
          }
          else{
            rt_kprintf("Display mailbox send successfully!\n\r");
          }
        }

        // 释放温度数据内存
        rt_free(received_temp);
      }
      else{
        rt_kprintf("PWM memory allocation failed!\n\r");
      }
	  }
  }
}

static void pwm_thread_entry(void *parameter)
{
  pwm_data_t *pwm_data;
  rt_ubase_t msg;

	while(1)
	{
    // 从邮箱接收PWM数据
    if(rt_mb_recv(pwm_mail, &msg, RT_WAITING_FOREVER) == RT_EOK)
    {
      pwm_data = (pwm_data_t*)msg;
      
      // 使用接收到的PWM数据设置比较值
      __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1, pwm_data->fan_output);//min:14  max:50
      __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2, pwm_data->teg_output);//  

      // 释放内存
      rt_free(pwm_data);
    }
    else
    {
      rt_kprintf("PWM mailbox receive failed!\n\r");
    }
	}
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
