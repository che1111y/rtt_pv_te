/*********************************************************************************************************************
* CH32V307VCT6 Opensourec Library ����CH32V307VCT6 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ���CH32V307VCT6 ��Դ���һ����
*
* CH32V307VCT6 ��Դ�� ���������
* �����Ը���������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          zf_common_function
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          MounRiver Studio V1.8.1
* ����ƽ̨          CH32V307VCT6
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����                                      ����                             ��ע
* 2022-09-15        ��W            first version
********************************************************************************************************************/

#include "SEEKFREE_FUNCTION.h"

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ�����������Լ�� ��������֮���������
// ����˵��     num1            ����1
// ����˵��     num2            ����2
// ���ز���     uint32          ���Լ��
// ʹ��ʾ��     return func_get_greatest_common_divisor(144, 36);               // ��ȡ 144 �� 36 �����Լ��
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
uint32_t func_get_greatest_common_divisor (uint32_t num1, uint32_t num2)
{
    while(num1 != num2)
    {
        if(num1 > num2)
        {
            num1 = num1 - num2;
        }
        if(num1 < num2)
        {
            num2 = num2 - num1;
        }
    }
    return num1;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ʱ
// ����˵��     t               ��ʱʱ��
// ���ز���     void
// ʹ��ʾ��     func_soft_delay(100);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void func_soft_delay (volatile long t)
{
    while(t --);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �ַ���ת�������� ���ݷ�Χ�� [-32768,32767]
// ����˵��     *str            �����ַ��� �ɴ�����
// ���ز���     int32           ת���������          
// ʹ��ʾ��     int32 dat = func_str_to_int("-100");
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
int32_t func_str_to_int (char *str)
{

    uint8_t sign = 0;                                                             // ��Ƿ��� 0-���� 1-����
    int32_t temp = 0;                                                             // ��ʱ�������
    do
    {
        if(NULL == str)
        {
            break;
        }

        if('-' == *str)                                                         // �����һ���ַ��Ǹ���
        {
            sign = 1;                                                           // ��Ǹ���
            str ++;
        }
        else if('+' == *str)                                                    // �����һ���ַ�������
        {
            str ++;
        }

        while(('0' <= *str) && ('9' >= *str))                                   // ȷ�����Ǹ�����
        {
            temp = temp * 10 + ((uint8_t)(*str) - 0x30);                          // ������ֵ
            str ++;
        }

        if(sign)
        {
            temp = -temp;
        }
    }while(0);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��������ת�ַ��� ���ݷ�Χ�� [-32768,32767]
// ����˵��     *str            �ַ���ָ��
// ����˵��     number          ���������
// ���ز���     void
// ʹ��ʾ��     func_int_to_str(data_buffer, -300);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void func_int_to_str (char *str, int32_t number)
{
    uint8_t data_temp[16];                                                        // ������
    uint8_t dat_bit = 0;                                                              // ����λ��
    int32_t number_temp = 0;

    do
    {
        if(NULL == str)
        {
            break;
        }

        if(0 > number)                                                          // ����
        {
            *str ++ = '-';
            number = -number;
        }
        else if(0 == number)                                                    // �������Ǹ� 0
        {
            *str = '0';
            break;
        }

        while(0 != number)                                                      // ѭ��ֱ����ֵ����
        {
            number_temp = number % 10;
            data_temp[dat_bit ++] = func_abs(number_temp);                          // ������ֵ��ȡ����
            number /= 10;                                                       // ��������ȡ�ĸ�λ��
        }
        while(0 != dat_bit)                                                         // ��ȡ�����ָ����ݼ�����
        {
            *str ++ = (data_temp[dat_bit - 1] + 0x30);                              // �����ִӵ��������е���ȡ�� �����������ַ���
            dat_bit --;
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �ַ���ת�������� ���ݷ�Χ�� [0,65535]
// ����˵��     *str            �����ַ��� �޷���
// ���ز���     uint32          ת���������          
// ʹ��ʾ��     uint32 dat = func_str_to_uint("100");
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
uint32_t func_str_to_uint (char *str)
{
    uint32_t temp = 0;                                                            // ��ʱ�������

    do
    {
        if(NULL == str)
        {
            break;
        }

        while(('0' <= *str) && ('9' >= *str))                                  // ȷ�����Ǹ�����
        {
            temp = temp * 10 + ((uint8_t)(*str) - 0x30);                         // ������ֵ
            str ++;
        }
    }while(0);

    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��������ת�ַ��� ���ݷ�Χ�� [0,65535]
// ����˵��     *str            �ַ���ָ��
// ����˵��     number          ���������
// ���ز���     void
// ʹ��ʾ��     func_uint_to_str(data_buffer, 300);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void func_uint_to_str (char *str, uint32_t number)
{

    int8_t data_temp[16];                                                         // ������
    uint8_t dat_bit = 0;                                                              // ����λ��

    do
    {
        if(NULL == str)
        {
            break;
        }

        if(0 == number)                                                         // ���Ǹ� 0
        {
            *str = '0';
            break;
        }

        while(0 != number)                                                      // ѭ��ֱ����ֵ����
        {
            data_temp[dat_bit ++] = (number % 10);                                  // ������ֵ��ȡ����
            number /= 10;                                                       // ��������ȡ�ĸ�λ��
        }
        while(0 != dat_bit)                                                         // ��ȡ�����ָ����ݼ�����
        {
            *str ++ = (data_temp[dat_bit - 1] + 0x30);                              // �����ִӵ��������е���ȡ�� �����������ַ���
            dat_bit --;
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �ַ���ת������ ��Ч�ۼƾ���ΪС�������λ
// ����˵��     *str            �����ַ��� �ɴ�����
// ���ز���     float           ת���������          
// ʹ��ʾ��     float dat = func_str_to_float("-100.2");
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
float func_str_to_float (char *str)
{
    uint8_t sign = 0;                                                             // ��Ƿ��� 0-���� 1-����
    float temp = 0.0;                                                           // ��ʱ������� ��������
    float temp_point = 0.0;                                                     // ��ʱ������� С������
    float point_bit = 1;                                                        // С���ۼƳ���

    do
    {
        if(NULL == str)
        {
            break;
        }

        if('-' == *str)                                                         // ����
        {
            sign = 1;                                                           // ��Ǹ���
            str ++;
        }
        else if('+' == *str)                                                    // �����һ���ַ�������
        {
            str ++;
        }

        // ��ȡ��������
        while(('0' <= *str) && ('9' >= *str))                                   // ȷ�����Ǹ�����
        {
            temp = temp * 10 + ((uint8_t)(*str) - 0x30);                          // ����ֵ��ȡ����
            str ++;
        }
        if('.' == *str)
        {
            str ++;
            while(('0' <= *str) && ('9' >= *str) && point_bit < 1000000.0)      // ȷ�����Ǹ����� ���Ҿ��ȿ��ƻ�û����λ
            {
                temp_point = temp_point * 10 + ((uint8_t)(*str) - 0x30);          // ��ȡС��������ֵ
                point_bit *= 10;                                                // �����ⲿ��С���ĳ���
                str ++;
            }
            temp_point /= point_bit;                                            // ����С��
        }
        temp += temp_point;                                                     // ����ֵƴ��

        if(sign)
        {
            temp = -temp;
        }
    }while(0);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��������ת�ַ���
// ����˵��     *str            �ַ���ָ��
// ����˵��     number          ���������
// ����˵��     point_bit       С���㾫��
// ���ز���     void
// ʹ��ʾ��     func_float_to_str(data_buffer, 3.1415, 2);                      // ������ data_buffer = "3.14"
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void func_float_to_str (char *str, float number, uint8_t point_bit)
{

    int data_int = 0;                                                           // ��������
    int data_float = 0.0;                                                       // С������
    int data_temp[8];                                                           // �����ַ�����
    int data_temp_point[6];                                                     // С���ַ�����
    uint8_t dat_bit = point_bit;                                                      // ת������λ��

    do
    {
        if(NULL == str)
        {
            break;
        }

        // ��ȡ��������
        data_int = (int)number;                                                 // ֱ��ǿ��ת��Ϊ int
        if(0 > number)                                                          // �ж�Դ�������������Ǹ���
        {
            *str ++ = '-';
        }
        else if(0.0 == number)                                                  // ����Ǹ� 0
        {
            *str ++ = '0';
            *str ++ = '.';
            *str = '0';
            break;
        }

        // ��ȡС������
        number = number - data_int;                                             // ��ȥ�������ּ���
        while(dat_bit --)
        {
            number = number * 10;                                               // ����Ҫ��С��λ����ȡ����������
        }
        data_float = (int)number;                                               // ��ȡ�ⲿ����ֵ

        // ��������תΪ�ַ���
        dat_bit = 0;
        do
        {
            data_temp[dat_bit ++] = data_int % 10;                                  // ���������ֵ���д���ַ�������
            data_int /= 10;
        }while(0 != data_int);
        while(0 != dat_bit)
        {
            *str ++ = (func_abs(data_temp[dat_bit - 1]) + 0x30);                    // �ٵ��򽫵������ֵд���ַ��� �õ�������ֵ
            dat_bit --;
        }

        // С������תΪ�ַ���
        if(point_bit != 0)
        {
            dat_bit = 0;
            *str ++ = '.';
            if(0 == data_float)
            {
                *str = '0';
            }
            else
            {
                while(0 != point_bit)                                           // �ж���Чλ��
                {
                    data_temp_point[dat_bit ++] = data_float % 10;                  // ����д���ַ�������
                    data_float /= 10;
                    point_bit --;                                                
                }
                while(0 != dat_bit)
                {
                    *str ++ = (func_abs(data_temp_point[dat_bit - 1]) + 0x30);      // �ٵ��򽫵������ֵд���ַ��� �õ�������ֵ
                    dat_bit --;
                }
            }
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �ַ���ת������ ��Ч�ۼƾ���ΪС������λ
// ����˵��     str             �����ַ��� �ɴ�����
// ���ز���     double          ת���������          
// ʹ��ʾ��     double dat = func_str_to_double("-100.2");
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
double func_str_to_double (char *str)
{

    uint8_t sign = 0;                                                             // ��Ƿ��� 0-���� 1-����
    double temp = 0.0;                                                          // ��ʱ������� ��������
    double temp_point = 0.0;                                                    // ��ʱ������� С������
    double point_bit = 1;                                                       // С���ۼƳ���

    do
    {
        if(NULL == str)
        {
            break;
        }

        if('-' == *str)                                                         // ����
        {
            sign = 1;                                                           // ��Ǹ���
            str ++;
        }
        else if('+' == *str)                                                    // �����һ���ַ�������
        {
            str ++;
        }

        // ��ȡ��������
        while(('0' <= *str) && ('9' >= *str))                                   // ȷ�����Ǹ�����
        {
            temp = temp * 10 + ((uint8_t)(*str) - 0x30);                          // ����ֵ��ȡ����
            str ++;
        }
        if('.' == *str)
        {
            str ++;
            while(('0' <= *str) && ('9' >= *str) && point_bit < 1000000000.0)   // ȷ�����Ǹ����� ���Ҿ��ȿ��ƻ�û����λ
            {
                temp_point = temp_point * 10 + ((uint8_t)(*str) - 0x30);          // ��ȡС��������ֵ
                point_bit *= 10;                                                // �����ⲿ��С���ĳ���
                str ++;
            }
            temp_point /= point_bit;                                            // ����С��
        }
        temp += temp_point;                                                     // ����ֵƴ��

        if(sign)
        {
            temp = -temp;
        }
    }while(0);
    return temp;

}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��������ת�ַ���
// ����˵��     *str            �ַ���ָ��
// ����˵��     number          ���������
// ����˵��     point_bit       С���㾫��
// ���ز���     void
// ʹ��ʾ��     func_double_to_str(data_buffer, 3.1415, 2);                     // ������ data_buffer = "3.14"
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void func_double_to_str (char *str, double number, uint8_t point_bit)
{

    int data_int = 0;                                                           // ��������
    int data_float = 0.0;                                                       // С������
    int data_temp[12];                                                          // �����ַ�����
    int data_temp_point[9];                                                     // С���ַ�����
    uint8_t dat_bit = point_bit;                                                      // ת������λ��

    do
    {
        if(NULL == str)
        {
            break;
        }

        // ��ȡ��������
        data_int = (int)number;                                                 // ֱ��ǿ��ת��Ϊ int
        if(0 > number)                                                          // �ж�Դ�������������Ǹ���
        {
            *str ++ = '-';
        }
        else if(0.0 == number)                                                  // ����Ǹ� 0
        {
            *str ++ = '0';
            *str ++ = '.';
            *str = '0';
            break;
        }

        // ��ȡС������
        number = number - data_int;                                             // ��ȥ�������ּ���
        while(dat_bit --)
        {
            number = number * 10;                                               // ����Ҫ��С��λ����ȡ����������
        }
        data_float = (int)number;                                               // ��ȡ�ⲿ����ֵ

        // ��������תΪ�ַ���
        dat_bit = 0;
        do
        {
            data_temp[dat_bit ++] = data_int % 10;                                  // ���������ֵ���д���ַ�������
            data_int /= 10;
        }while(0 != data_int);
        while(0 != dat_bit )
        {
            *str ++ = (func_abs(data_temp[dat_bit - 1]) + 0x30);                    // �ٵ��򽫵������ֵд���ַ��� �õ�������ֵ
            dat_bit --;
        }

        // С������תΪ�ַ���
        if(point_bit != 0)
        {
            dat_bit = 0;
            *str ++ = '.';
            if(0 == data_float)
                *str = '0';
            else
            {
                while(0 != point_bit)                                           // �ж���Чλ��
                {
                    data_temp_point[dat_bit ++] = data_float % 10;                  // ����д���ַ�������
                    data_float /= 10;
                    point_bit --;                                                
                }
                while(0 != dat_bit )
                {
                    *str ++ = (func_abs(data_temp_point[dat_bit - 1]) + 0x30);      // �ٵ��򽫵������ֵд���ַ��� �õ�������ֵ
                    dat_bit --;
                }
            }
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �ַ���ת Hex
// ����˵��     str             �����ַ��� �޷���
// ���ز���     uint32          ת���������
// ʹ��ʾ��     uint32 dat = func_str_to_hex("0x11");
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
uint32_t func_str_to_hex (char *str)
{

    uint32_t str_len = strlen(str);                                               // �ַ�����
    uint32_t result_data = 0;                                                     // �������
    uint8_t temp = 0;                                                             // �������
    uint8_t flag = 0;                                                             // ��־λ

    do
    {
        if(NULL == str)
        {
            break;
        }

        if(flag)
        {
            if(('a' <= *str) && ('f' >= *str))
            {
                temp = (*str - 87);
            }
            else if(('A' <= *str) && ('F' >= *str))
            {
                temp = (*str - 55);
            }
            else if(('0' <= *str) && ('9' >= *str))
            {
                temp = (*str - 48);
            }
            else
            {
                break;
            }
            result_data = ((result_data << 4) | (temp & 0x0F));
        }
        else
        {
//            if(strncmp("0x", str, 2))
            if((*str == '0') && (*(str + 1) == 'x'))
            {
                str ++;
                flag = 1;
            }
        }
        str ++;
    }while(str_len --);

    return result_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     Hex ת�ַ���
// ����˵��     *str            �ַ���ָ��
// ����˵��     number          ���������
// ���ز���     void
// ʹ��ʾ��     func_hex_to_str(data_buffer, 0x11);                             // ������ data_buffer = "0x11"
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void func_hex_to_str (char *str, uint32_t number)
{

    const char hex_index[16] = {
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'A', 'B',
        'C', 'D', 'E', 'F'};
    int8_t data_temp[12];                                                         // ������
    uint8_t dat_bit = 0;                                                              // ����λ��

    *str++ = '0';
    *str++ = 'x';
    do
    {
        if(NULL == str)
        {
            break;
        }

        if(0 == number)                                                         // ���Ǹ� 0
        {
            *str = '0';
            break;
        }

        while(0 != number)                                                      // ѭ��ֱ����ֵ����
        {
            data_temp[dat_bit ++] = (number & 0xF);                                 // ������ֵ��ȡ����
            number >>= 4;                                                       // ��������ȡ�ĸ�λ��
        }
        while(0 != dat_bit)                                                         // ��ȡ�����ָ����ݼ�����
        {
            *str ++ = hex_index[data_temp[dat_bit - 1]];                            // �����ִӵ��������е���ȡ�� �����������ַ���
            dat_bit --;
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ����ת��Ϊ ASCII ֵ
// ����˵��     dat             ���������
// ����˵��     *p              ���ݻ���
// ����˵��     neg_type        ��������
// ����˵��     radix           ����
// ���ز���     uint8_t           ����
// ʹ��ʾ��     number_conversion_ascii((uint32)ival, vstr, 1, 10);
// ��ע��Ϣ     ���������ļ��ڲ����� �û����ù�ע Ҳ�����޸�
//-------------------------------------------------------------------------------------------------------------------
static uint8_t number_conversion_ascii (uint32_t dat, int8_t *p, uint8_t neg_type, uint8_t radix)
{
    int32_t   neg_dat;
    uint32_t  pos_dat;
    uint8_t   temp_data = 0;
    uint8_t   valid_num = 0;

    if(neg_type)
    {
        neg_dat = (int32_t)dat;
        if(0 > neg_dat)
        {
            neg_dat = -neg_dat;
        }
        while(1)
        {
            *p = neg_dat%radix + '0';
            neg_dat = neg_dat/radix;
            valid_num ++;

            if(!neg_dat)
            {
                break;
            }
            p ++;
        }
    }
    else
    {
        pos_dat = dat;
        while(1)
        {
            temp_data = pos_dat%radix;
            if(10 <= temp_data)
            {
                temp_data += 'A'-10;
            }
            else
            {
                temp_data += '0';
            }

            *p = temp_data;

            pos_dat = pos_dat/radix;
            valid_num ++;

            if(!pos_dat)
            {
                break;
            }
            p ++;
        }
    }
    return valid_num;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     printf ��ʾת��
// ����˵��     *d_buff         ������
// ����˵��     len             ����
// ���ز���     void
// ʹ��ʾ��     printf_reverse_order(vstr, vlen);
// ��ע��Ϣ     ���������ļ��ڲ����� �û����ù�ע Ҳ�����޸�
//-------------------------------------------------------------------------------------------------------------------
static void printf_reverse_order (int8_t *d_buff, uint32_t len)
{
    uint32_t i;
    int8_t  temp_data;
    for(i = 0; len / 2 > i; i ++)
    {
        temp_data = d_buff[len - 1 - i];
        d_buff[len - 1 -i ] = d_buff[i];
        d_buff[i] = temp_data; 
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     sprintf ����ʵ��
// ����˵��     *buff           ������
// ����˵��     *format         Դ�ַ���
// ����˵��     ...             �ɱ�����б�
// ���ز���     uint32          ���������ݳ�
// ʹ��ʾ��     zf_sprintf(buff, "Data : %d", 100);
// ��ע��Ϣ     ���������ļ��ڲ����� �û����ù�ע Ҳ�����޸�
//-------------------------------------------------------------------------------------------------------------------
uint32_t zf_sprintf(char  *buff, const int8_t *format, ...)
{
    uint32_t buff_len=0;
    va_list arg;
	va_start(arg, format);
 
	while (*format)
	{
		int8_t ret = *format;
		if (ret == '%')
		{
			switch (*++format)
			{
                case 'a':// ʮ������p��������������� ��δʵ��
                {
                    
                    
                }break;
                
                
                case 'c':// һ���ַ�
                {
                    int8_t ch = (int8_t)va_arg(arg, uint32_t);
                    *buff = ch;
                    buff++;
                    buff_len++;
                            
                }break;
                
                
                case 'd':
                case 'i':// �з���ʮ��������
                {
                    int8_t vstr[33];
                    int32_t ival = (int32_t)va_arg(arg, int32_t);
                    uint8_t vlen = number_conversion_ascii((uint32_t)ival, vstr, 1, 10);
                    
                    if(ival<0)  
                    {
                        vstr[vlen] = '-';
                        vlen++;
                    }
                    printf_reverse_order(vstr,vlen);
                    memcpy(buff,vstr,vlen);
                    buff += vlen;
                    buff_len += vlen;
                    
                    
                }break;
                
                case 'f':// �����������С�������λ  ����ָ���������
                case 'F':// �����������С�������λ  ����ָ���������
                {
                    int8_t vstr[33];
                    double ival = (double)va_arg(arg, double);
                    uint8_t vlen = number_conversion_ascii((uint32_t)(int32_t)ival, vstr, 1, 10);
                    
                    if(ival<0)  
                    {
                        vstr[vlen] = '-';
                        vlen++;
                    }
                    printf_reverse_order(vstr,vlen);
                    memcpy(buff,vstr,vlen);
                    buff += vlen;
                    buff_len += vlen;

                    ival = ((double)ival - (int32_t)ival)*1000000;
                    if(ival)
                    {
                        vlen = number_conversion_ascii((uint32_t)(int32_t)ival, vstr, 1, 10);
                    }
                    else
                    {
                        vstr[0] = vstr[1] = vstr[2] = vstr[3] = vstr[4] = vstr[5] = '0';
                        vlen = 6;
                    }
					
                    while(6>vlen)
                    {
                        vstr[vlen] = '0';
                        vlen++;
                    }
					
                    vstr[vlen] = '.';
                    vlen++;
                    
                    printf_reverse_order(vstr,vlen);
                    memcpy(buff,vstr,vlen);
					buff += vlen;
                    buff_len += vlen;

                    break;
                }
                
                case 'u':// �޷���ʮ��������
                {
                    int8_t vstr[33];
                    uint32_t ival = (uint32_t)va_arg(arg, uint32_t);
                    uint8_t vlen = number_conversion_ascii(ival, vstr, 0, 10);
                    
                    printf_reverse_order(vstr,vlen);
                    memcpy(buff,vstr,vlen);
                    buff += vlen;
                    buff_len += vlen;
                }break;
                
                case 'o':// �޷��Ű˽������� 
                {
                    int8_t vstr[33];
                    uint32_t ival = (uint32_t)va_arg(arg, uint32_t);
                    uint8_t vlen = number_conversion_ascii(ival, vstr, 0, 8);
                    
                    printf_reverse_order(vstr,vlen);
                    memcpy(buff,vstr,vlen);
                    buff += vlen;
                    buff_len += vlen;
                    
                }break;
                
                case 'x':// �޷���ʮ����������
                case 'X':// �޷���ʮ����������
                {
                    int8_t vstr[33];
                    uint32_t ival = (uint32_t)va_arg(arg, uint32_t);
                    uint8_t vlen = number_conversion_ascii(ival, vstr, 0, 16);
                    
                    printf_reverse_order(vstr,vlen);
                    memcpy(buff,vstr,vlen);
                    buff += vlen;
                    buff_len += vlen;
                }break;
                
                
                case 's':// �ַ���
                {
                    int8_t *pc = va_arg(arg, int8_t *);
                    while (*pc)
                    {
                        *buff = *pc;
                        buff++;
                        buff_len++;
                        pc++;
                    }
                }break;
                
                case 'p':// ��16������ʽ���ָ��
                {
                    int8_t vstr[33];
                    uint32_t ival = (uint32_t)va_arg(arg, uint32_t);
                    //uint8_t vlen = number_conversion_ascii(ival, vstr, 0, 16);
					number_conversion_ascii(ival, vstr, 0, 16);
                    
                    printf_reverse_order(vstr,8);
                    memcpy(buff,vstr,8);
                    buff += 8;
                    buff_len += 8;
                            
                }break;
                
                
                case '%':// ����ַ�% 
                {
                    *buff = '%';
                    buff++;
                    buff_len++;
                }break;

                default:break;
			}
		}
		else
		{
            *buff = (int8_t)(*format);
            buff++;
            buff_len++;
		}
		format++;
	}
	va_end(arg);

    return buff_len;
}


