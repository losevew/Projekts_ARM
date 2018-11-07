//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-����� 
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "stm32f0xx.h"
#include "max7219.h"




//==============================================================================
// ��������� �������������� �����, ������������ ����������� ������ � max7219
// �������������� SPI
// �������������� ��������� ������ � �������� max7219
//==============================================================================
void max7219_init(void)
{
	RCC->AHBENR |= MAX7219_PH ;
	  
	MAX7219_PORT->OTYPER &= ~(DIN_PIN_OTYP | CS_PIN_OTYP | CLK_PIN_OTYP );
	MAX7219_PORT->OSPEEDR |= (DIN_PIN_OSPEED | CS_PIN_OSPEED | CLK_PIN_OSPEED) ;
	MAX7219_PORT->MODER |= (DIN_PIN_MODE | CS_PIN_MODE | CLK_PIN_MODE );
	
	MAX7219_CS1;
	MAX7219_DIN0;
	MAX7219_CLK0;
}
//==============================================================================


//==============================================================================
// ��������� ���������� ������� � ������ � ���� ��� �� ��� max7219 � �������
//==============================================================================
void max7219_send(uint8_t MAX_Idx, uint8_t Cmd, uint8_t Data)
{
  uint16_t max7219_SpiBuff[MAX7219_NUM];
  uint16_t Word = Data | ((uint16_t) Cmd << 8);
  
  for (uint8_t i = 0; i < MAX7219_NUM; i++)
  {
    if (MAX_Idx == 0xFF)  // ����� �������� �� ��� max7219 � �������
      max7219_SpiBuff[i] = Word;
    else                  // ����� �������� ������ � ���� max7219
    {
      if (i == MAX_Idx)         // �� ���������� max7219, � ������� ����� �������� �������/������
        max7219_SpiBuff[i] = Word;
      else                      // max7219, �������� ��� ������ �� ������
        max7219_SpiBuff[i] = 0x00 | ((uint16_t) MAX7219_CMD_NO_OP << 8);
    }
  }
  
  // ����� ����� ���� �� ��� ������������ max7219
  max7219_sendarray(max7219_SpiBuff);
}
//==============================================================================


//==============================================================================
// ��������� ���������� ������ ������ � max7219
//==============================================================================
void max7219_sendarray(uint16_t *pArray)
{
  MAX7219_CS0;
  
  for (uint8_t i = 0; i < MAX7219_NUM; i++)
   { 
		max7219_send16bit(*(pArray++));
   }
  
  MAX7219_CS1;
}
//==============================================================================


//==============================================================================
// ��������� ���������� 16-������ ����� �� SPI
//==============================================================================
void max7219_send16bit(uint16_t Word)
{
		uint16_t i = 0;
		uint8_t temp = hibyte(Word);
			
		while(i<8)
		{
			
			if((temp << i) & 0x80)
			{
				MAX7219_DIN1;
			}
			else
			{
				MAX7219_DIN0;
			}
			
			MAX7219_CLK1;
			MAX7219_CLK0;
			
			i++;
		}
		
		i = 0;
		temp = lobyte(Word);
		
		while(i<8)
		{
			
			if((temp << i) & 0x80)
			{
				MAX7219_DIN1;
			}
			else
			{
				MAX7219_DIN0;
			}
			
			MAX7219_CLK1;
			MAX7219_CLK0;
			
			i++;
		}
		
		
}
//==============================================================================


//==============================================================================
// ��������� ������������� ����� ������������� �������� � 1 ��� �� ���� max7219
//==============================================================================
void max7219_set_decodemode(uint8_t MAX_Idx, uint8_t DecodeMode)
{
  max7219_send(MAX_Idx, MAX7219_CMD_DECODE_MODE, DecodeMode);
}
//==============================================================================


//==============================================================================
// ��������� ������������� ������� � 1 ��� �� ���� max7219
//==============================================================================
void max7219_set_intensity(uint8_t MAX_Idx, uint8_t Intensity)
{
  if (Intensity > 15)
    Intensity = 15;
  
  max7219_send(MAX_Idx, MAX7219_CMD_INTENSITY, Intensity);
}
//==============================================================================


//==============================================================================
// ��������� ������������� ���-�� ������/����� � 1 ��� �� ���� max7219
//==============================================================================
void max7219_set_scan_limit(uint8_t MAX_Idx, uint8_t Limit)
{
  if (Limit > 7)
    Limit = 7;
  
  max7219_send(MAX_Idx, MAX7219_CMD_SCAN_LIMIT, Limit);
}
//==============================================================================


//==============================================================================
// ��������� ��������/��������� max7219. ����� ������ ������� �� ��������
//==============================================================================
void max7219_set_run_onoff(uint8_t MAX_Idx, uint8_t On)
{
  if (On)
    On = 1;
  
  max7219_send(MAX_Idx, MAX7219_CMD_SHUTDOWN, On);
}
//==============================================================================


//==============================================================================
// ��������� ��������/��������� �������� ����� max7219 (����� ��� ����������)
//==============================================================================
void max7219_set_testmode_onoff(uint8_t MAX_Idx, uint8_t On)
{
  if (On)
    On = 1;
  
  max7219_send(MAX_Idx, MAX7219_CMD_DISP_TEST, On);
}
//==============================================================================




