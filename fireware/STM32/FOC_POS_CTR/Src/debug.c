#include "debug.h"

debug_data debug;
__IO uint8_t debug_rev_data[DEBUG_REV_MAX_LEN];
__IO uint8_t debug_rev_p = 0;
//__IO uint8_t debug_rev_sta = STATUS_IDLE;
uint32_t PID_FLAG;
debug_data_rev debug_rev;
// CRC 高位字节值表
static const uint8_t s_CRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;
// CRC 低位字节值表
const uint8_t s_CRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

uint16_t CRC16_Calc(uint8_t *_pBuf, uint16_t _usLen)
{
    uint8_t ucCRCHi = 0xFF; /* 高CRC字节初始化 */
    uint8_t ucCRCLo = 0xFF; /* 低CRC 字节初始化 */
    uint16_t usIndex;       /* CRC循环中的索引 */

    while (_usLen--)
    {
        usIndex = ucCRCHi ^ *_pBuf++; /* 计算CRC */
        ucCRCHi = ucCRCLo ^ s_CRCHi[usIndex];
        ucCRCLo = s_CRCLo[usIndex];
    }
    return ((uint16_t)ucCRCHi << 8 | ucCRCLo);
}

void debug_handle(uint8_t *data)
{
    uint8_t temp[DEBUG_REV_MAX_LEN];
    uint8_t i;

    if(debug_rev_p >= DEBUG_REV_MAX_LEN)
    {
        debug_rev_p = 0;
    }
    debug_rev_data[debug_rev_p] = *(data);
    if(*data == DEBUG_DATA_END)
    {
        if(debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 4) % DEBUG_REV_MAX_LEN] == DEBUG_DATA_HEAD)
        {
            for(i=0; i<2; i++)
            {
                temp[i] = debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 4 + i) % DEBUG_REV_MAX_LEN];
            }
#if (EN_CRC > 0)
            if(CRC16_Calc(temp,2) == ((debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 4 + 2) % DEBUG_REV_MAX_LEN] << 8) | \
                                      debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 4 + 3) % DEBUG_REV_MAX_LEN]))
#endif
            {
                switch(debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 4 + 1) % DEBUG_REV_MAX_LEN])
                {
                case CMD_GET_STATUS:
                    debug_upload_data(&debug,TYPE_STATUS);
                    break;
                case CMD_GET_SPEED:
                    debug_upload_data(&debug,TYPE_SPEED);
                    break;
                case CMD_GET_HALL_ENC:
                    debug_upload_data(&debug,TYPE_HAL_ENC);
                    break;
                case CMD_GET_VBUS:
                    debug_upload_data(&debug,TYPE_VBUS);
                    break;
                case CMD_GET_AMP:
                    debug_upload_data(&debug,TYPE_AMP);
                    break;
                case CMD_GET_TEMP:
                    debug_upload_data(&debug,TYPE_TEMP);
                    break;
                case CMD_GET_SUM_LEN:
                    debug_upload_data(&debug,TYPE_SUM_LEN);
                    break;
                case CMD_GET_BEM:
                    debug_upload_data(&debug,TYPE_BEM);
                    break;
                case CMD_GET_MOTOR_CODE:
                    debug_upload_data(&debug,TYPE_MOTOR_CODE);
                    break;
                case CMD_GET_PID:
                    for(i=TYPE_PID1; i<TYPE_PID10; i++)
                    {
                        debug_upload_data(&debug,i);
                    }
                    break;
                }
            }
        }
        if(debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 5) % DEBUG_REV_MAX_LEN] == DEBUG_DATA_HEAD)
        {
            for(i=0; i<3; i++)
            {
                temp[i] = debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 5 + i) % DEBUG_REV_MAX_LEN];
            }
#if (EN_CRC > 0)
            if(CRC16_Calc(temp,3) == ((debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 5 + 3) % DEBUG_REV_MAX_LEN] << 8) | \
                                      debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 5 + 4) % DEBUG_REV_MAX_LEN]))
#endif
            {
                switch(debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 5 + 1) % DEBUG_REV_MAX_LEN])
                {
                case CMD_SET_CTR_CODE:
                    debug_rev.Ctrl_code = debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 5 + 2) % DEBUG_REV_MAX_LEN];
                    break;
                case CMD_SET_CTR_MODE:
                    debug_rev.Ctrl_mode = debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 5 + 2) % DEBUG_REV_MAX_LEN];
                    break;
                }
            }
        }
        if(debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 6) % DEBUG_REV_MAX_LEN] == DEBUG_DATA_HEAD)
        {
            for(i=0; i<4; i++)
            {
                temp[i] = debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 6 + i) % DEBUG_REV_MAX_LEN];
            }
#if (EN_CRC > 0)
            if(CRC16_Calc(temp,4) == ((debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 6 + 4) % DEBUG_REV_MAX_LEN] << 8) | \
                                      debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 6 + 5) % DEBUG_REV_MAX_LEN]))
#endif
            {
                switch(debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 6 + 1) % DEBUG_REV_MAX_LEN])
                {
                case CMD_SET_SPEED:
                    *(debug_rev.speed) = (debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 6 + 2) % DEBUG_REV_MAX_LEN] << 8) |\
                                         debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 6 + 3) % DEBUG_REV_MAX_LEN];
                    break;
                case CMD_SET_TORQUE:
                    *(debug_rev.torque) = (debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 6 + 2) % DEBUG_REV_MAX_LEN] << 8) |\
                                          debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 6 + 3) % DEBUG_REV_MAX_LEN];
                    break;
                }
            }
        }
        if(debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 16) % DEBUG_REV_MAX_LEN] == DEBUG_DATA_HEAD)
        {
            for(i=0; i<14; i++)
            {
                temp[i] = debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 16 + i) % DEBUG_REV_MAX_LEN];
            }
#if (EN_CRC > 0)
            if(CRC16_Calc(temp,14) == ((debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 16 + 14) % DEBUG_REV_MAX_LEN] << 8) | \
                                       debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 16 + 15) % DEBUG_REV_MAX_LEN]))
#endif
            {
                switch(debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 16 + 1) % DEBUG_REV_MAX_LEN])
                {
                case CMD_SET_PID1:
                case CMD_SET_PID2:
                case CMD_SET_PID3:
                case CMD_SET_PID4:
                case CMD_SET_PID5:
                case CMD_SET_PID6:
                case CMD_SET_PID7:
                case CMD_SET_PID8:
                case CMD_SET_PID9:
                case CMD_SET_PID10:
                    for(i=0; i<12; i++)
                    {
                        debug.pid[debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 16 + 1) % DEBUG_REV_MAX_LEN] - CMD_SET_PID1].pid.pidi8[i] = \
                                debug_rev_data[(debug_rev_p + DEBUG_REV_MAX_LEN - 16 + 2 + i) % DEBUG_REV_MAX_LEN];
                    }
                    break;
                }
            }
        }
    }
    debug_rev_p ++;
}
void debug_obj_init(debug_data *data)
{
    size_t obj_size = sizeof(debug_data);
    memset(data,0,(size_t)obj_size);
    //HAL_UART_Receive_IT(&huart5,uart5_rev_buf,1);
}

uint8_t check_num_len(float para)
{
    int8_t ret_len = 0;
    if(para * 10000 < 1)
    {
        ret_len = -5;
    }
    else if(para * 1000 < 1)
    {
        ret_len = -4;
    }
    else if(para * 100 < 1)
    {
        ret_len = -3;
    }
    else if(para * 10 < 1)
    {
        ret_len = -2;
    }
    else if(para * 1 < 1)
    {
        ret_len = -1;
    }
    else if(para * 0.1f < 1)
    {
        ret_len = 0;
    }
    else if(para * 0.01f < 1)
    {
        ret_len = 1;
    }
    else if(para * 0.001f < 1)
    {
        ret_len = 2;
    }
    else if(para * 0.0001f < 1)
    {
        ret_len = 3;
    }
    else if(para * 0.00001f < 1)
    {
        ret_len = 4;
    }
    else if(para * 0.000001f < 1)
    {
        ret_len = 5;
    }
    else
    {
        ret_len = -128;
    }
    return ret_len;
}

void debug_upload_data(debug_data * data, uint8_t upload_type)
{
    uint8_t cur_data,i;
//    int8_t num_len;
    uint8_t upload_data[52];
    upload_data[0] = DEBUG_DATA_HEAD;
    cur_data = 2;
    switch(upload_type)
    {
    case TYPE_STATUS:
        upload_data[1] = upload_type;
        upload_data[cur_data++] = data->status;
        break;
    case TYPE_SPEED:
        upload_data[1] = upload_type;
        upload_data[cur_data++] = (data->speed >> 8) & 0xFF;
        upload_data[cur_data++] = data->speed & 0xFF;
        break;
    case TYPE_HAL_ENC:
        upload_data[1] = upload_type;
        upload_data[cur_data++] = (data->hall_p) & 0x08;
        upload_data[cur_data++] = (data->encode_p >> 8) & 0xFF;
        upload_data[cur_data++] = (data->encode_p) & 0xFF;
        break;
    case TYPE_VBUS:
        upload_data[1] = upload_type;
        upload_data[cur_data++] = ((uint8_t)data->bus_vol) % 101;
        upload_data[cur_data++] = ((uint16_t)(data->bus_vol * 100)) % 100;
        break;
    case TYPE_AMP:
        upload_data[1] = upload_type;
        upload_data[cur_data++] = (((int16_t)(data->amp[0] * 1000)) >> 8) & 0xFF;
        upload_data[cur_data++] = ((int16_t)(data->amp[0] * 1000)) & 0xFF;
        upload_data[cur_data++] = (((int16_t)(data->amp[1] * 1000)) >> 8) & 0xFF;
        upload_data[cur_data++] = ((int16_t)(data->amp[1] * 1000)) & 0xFF;
        upload_data[cur_data++] = (((int16_t)(data->amp[2] * 1000)) >> 8) & 0xFF;
        upload_data[cur_data++] = ((int16_t)(data->amp[2] * 1000)) & 0xFF;
        break;
    case TYPE_TEMP:
        upload_data[1] = upload_type;
        upload_data[cur_data++] = (uint8_t)(data->temp[0] + 50);
        upload_data[cur_data++] = (uint8_t)(data->temp[1] + 50);
        break;
    case TYPE_SUM_LEN:
        upload_data[1] = upload_type;
        upload_data[cur_data++] = (data->sum_len >> 56) & 0xFF;
        upload_data[cur_data++] = (data->sum_len >> 48) & 0xFF;
        upload_data[cur_data++] = (data->sum_len >> 40) & 0xFF;
        upload_data[cur_data++] = (data->sum_len >> 32) & 0xFF;
        upload_data[cur_data++] = (data->sum_len >> 24) & 0xFF;
        upload_data[cur_data++] = (data->sum_len >> 16) & 0xFF;
        upload_data[cur_data++] = (data->sum_len >> 8) & 0xFF;
        upload_data[cur_data++] = (data->sum_len >> 0) & 0xFF;
        break;
    case TYPE_BEM:
        upload_data[1] = upload_type;
        upload_data[cur_data++] = (int8_t)data->bem[0];
        upload_data[cur_data++] = ((int16_t)(data->bem[0] * 100)) % 100;
        upload_data[cur_data++] = (int8_t)data->bem[1];
        upload_data[cur_data++] = ((int16_t)(data->bem[1] * 100)) % 100;
        upload_data[cur_data++] = (int8_t)data->bem[2];
        upload_data[cur_data++] = ((int16_t)(data->bem[2] * 100)) % 100;
        break;
    case TYPE_MOTOR_CODE:
        upload_data[1] = upload_type;
        upload_data[cur_data++] = data->motor_code;
        break;
    case TYPE_TORQUE:
        upload_data[1] = upload_type;
        upload_data[cur_data++] = (((int16_t)(data->torque * 1000)) >> 8) & 0xFF;
        upload_data[cur_data++] = ((int16_t)(data->torque * 1000)) & 0xFF;
        break;
    case TYPE_POWER:
        upload_data[1] = upload_type;
        upload_data[cur_data++] = (((int16_t)(data->power * 100)) >> 8) & 0xFF;
        upload_data[cur_data++] = ((int16_t)(data->power * 100)) & 0xFF;
        break;

    case TYPE_PID1:
    case TYPE_PID2:
    case TYPE_PID3:
    case TYPE_PID4:
    case TYPE_PID5:
    case TYPE_PID6:
    case TYPE_PID7:
    case TYPE_PID8:
    case TYPE_PID9:
    case TYPE_PID10:
        upload_data[1] = upload_type;
        for(i=0; i<3; i++)
        {
#if 0
            num_len = check_num_len(data->pid[upload_type - TYPE_PID1].para[i]);
            if(num_len != (-128))
            {
                upload_data[cur_data++] = (uint16_t)(data->pid[upload_type - TYPE_PID1].para[i] * (pow(10,-num_len))) >> 8;
                upload_data[cur_data++] = (uint16_t)data->pid[upload_type - TYPE_PID1].para[i] * (pow(10,-num_len));
                upload_data[cur_data++] = num_len;
            }
            else
            {
                upload_data[cur_data++] = 0;
                upload_data[cur_data++] = 0;
                upload_data[cur_data++] = 0;
            }
#else
            upload_data[cur_data++] = data->pid[upload_type-TYPE_PID1].pid.pidi8[i * 4 + 0];
            upload_data[cur_data++] = data->pid[upload_type-TYPE_PID1].pid.pidi8[i * 4 + 1];
            upload_data[cur_data++] = data->pid[upload_type-TYPE_PID1].pid.pidi8[i * 4 + 2];
            upload_data[cur_data++] = data->pid[upload_type-TYPE_PID1].pid.pidi8[i * 4 + 3];
#endif
        }
        break;

    case TYPE_USER_DATA:
        upload_data[1] = upload_type;
        for(i=0; i<16; i++)
        {
            upload_data[cur_data++] = (data->user_data[i] >> 8) & 0xFF;
            upload_data[cur_data++] = data->user_data[i] & 0xFF;
        }
        break;
    default :
        upload_data[1] = 0xFE;
        break;
    }
    if(upload_data[1] == 0xFE)
    {
        return;
    }
    else
    {
        uint16_t crc_res = CRC16_Calc(&(upload_data[0]),cur_data);
        upload_data[cur_data++] = (crc_res >> 8) & 0xFF;
        upload_data[cur_data++] = (crc_res) & 0xFF;
        upload_data[cur_data++] = DEBUG_DATA_END;
        HAL_UART_Transmit(&huart5,upload_data,cur_data,0xFFFF);
    }
}
/**************************************用户应用层*************************************/
void Debug_Init(void)
{
    debug_obj_init(&debug);
}
void Debug_Send_InitData(upload_type PIDx,float *SetPoint,float P,float I,float D)
{
    debug_rev.speed=(float*)(SetPoint);/*保持目标设置和上位机同步*/
    /*保持PID参数和上位机同步*/
    debug.pid[PIDx-TYPE_PID1].pid.pidf[0]=P;//发送P
    debug.pid[PIDx-TYPE_PID1].pid.pidf[1]=I;//发送I
    debug.pid[PIDx-TYPE_PID1].pid.pidf[2]=D;//发送D
    debug_upload_data(&debug,PIDx);/*执行发送 */
}
void Debug_Send_Wave_data(uint8_t chx,int16_t wave)
{
    debug.user_data[chx-1]=wave;//选择通道1 发送实际速度
    debug_upload_data(&debug,TYPE_USER_DATA);//发送用户数据（波形数据）
}
void Debug_Send_Current(float U_I,float V_I,float W_I)
{
    debug.amp[0]=U_I;//U相电流
    debug.amp[1]=V_I;//V相电流
    debug.amp[2]=W_I;//W相电流
    debug_upload_data(&debug,TYPE_AMP);
}
void Debug_Send_Valtage(float valtage)
{
    debug.bus_vol=valtage;	//电压
    debug_upload_data(&debug, TYPE_VBUS);
}
void Debug_Send_Power(float power)
{
    debug.power=power;	//功率
    debug_upload_data(&debug,TYPE_POWER);
}
void Debug_Send_Speed(float speed)
{
    debug.speed=(int16_t)(speed);//速度
    debug_upload_data(&debug,TYPE_SPEED);
}
void Debug_Send_Distance(uint64_t len)
{
    debug.sum_len=len;//总里程;
    debug_upload_data(&debug,TYPE_SUM_LEN);
}
void Debug_Send_Temp(float motor_temp,float board_temp)
{
    debug.temp[0]=board_temp;//驱动板温度
    debug.temp[1]=motor_temp;//电机温度
    debug_upload_data(&debug,TYPE_TEMP);
}

void Debug_Send_MotorState(motor_state motor_codestae)
{
    debug.status=motor_codestae;
    debug_upload_data(&debug,TYPE_STATUS);
}
void Debug_Updata_MotorState(int16_t speed,uint8_t flag_state)
{
    switch(flag_state)
    {
    case 0:
        Debug_Set_Point_Range(0,0);
        Debug_Send_MotorState(ERROR_STATE);/*电机停机*/
        break;
    case 1:
    {
        if(speed==0)/*实时更新电机状态*/
            Debug_Send_MotorState(IDLE_STATE);/*电机空闲*/
        else
            Debug_Send_MotorState(RUN_STATE);/*电机运行*/
    }
    break;
    case 2:
        Debug_Send_MotorState(BREAKED_STATE);/*电机刹车*/
        break;
    }
}
void Debug_Send_MotorCode(motor_code motorcode)
{
    debug.motor_code=motorcode;//电机类型为直流电机
    debug_upload_data(&debug,TYPE_MOTOR_CODE);
}

void Debug_Receive_PID(upload_type PIDx,float *P,float *I,float *D)
{
    *P=debug.pid[PIDx-TYPE_PID1].pid.pidf[0];/*接收PID助手设置的P参数*/
    *I=debug.pid[PIDx-TYPE_PID1].pid.pidf[1];/*接收PID助手设置的I参数*/
    *D=debug.pid[PIDx-TYPE_PID1].pid.pidf[2]; /*接收PID助手设置的D参数*/
}
void Debug_Set_Point_Range(float max_limit,float min_limit)
{
    if(*debug_rev.speed>=max_limit)
        *debug_rev.speed=max_limit;
    if(*debug_rev.speed<=min_limit)
        *debug_rev.speed=min_limit;
}
uint8_t Debug_Receive_Ctrl_Code(void)
{
    static uint8_t rec_r=0;
    if(debug_rev.Ctrl_code>=0x01&&debug_rev.Ctrl_code<=0x03)
    {
        rec_r++;
        if(rec_r>=2)
        {
            rec_r=0;
            debug_rev.Ctrl_code=0;
        }
        return debug_rev.Ctrl_code;
    }
    return 0;
}
