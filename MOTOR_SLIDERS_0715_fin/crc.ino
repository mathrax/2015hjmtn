/*The function KIRSP_cal_CRC16 takes two parameters:
 Please do not modify
 Revision 1.0b 6/26/09
 unsigned char *CRC16_Data_Array     = A pointer to the data array containing binary data to be used for generating the CRC
 unsigned short CRC16_Data_Array_Len = The number of bytes in the data array.
 
 This optimized CRC-16 algorithm calculation uses no tables or EEPROM
 Polynomial: x^16 + x^15 + x^2 + 1 (0xA001) <- The Key value 
 Initial value: 0xFFFF
 CRC-16 is normally used in disk-drive controllers. 
 Modified for the Audrion/Freeduino 
 The sending system will calculate a CRC and append it to the message. The receiving system will calculate a new
 CRC based on the entire message – including the appended CRC bytes. The resulting CRC should be 0x0000. <-------------- 
 If the CRC-16 calculated by the receiving system is not equal to zero, then an error occurred in the transmission and all
 data should be ignored. */

// KIRSP Global Variables for CRC-16 Checking
unsigned char KIRSP_CRC16_Hi_Byte  = 0xFF;  // Do not modify
unsigned char KIRSP_CRC16_Low_Byte = 0xFF;  // Do not modify


unsigned int KIRSP_cal_CRC16(unsigned char *CRC16_Data_Array, unsigned short CRC16_Data_Array_Len) 
{ 
  unsigned int x = 0xFFFF; 
  unsigned int y;
  int i; 
  x ^= *CRC16_Data_Array;
  for (i = 0; i < 8; ++i)
  {
    if (x & 1)
      x = (x >> 1) ^ 0xA001; // <----The key
    else
      x = (x >> 1);
  }
  KIRSP_CRC16_Hi_Byte = highByte(x);
  KIRSP_CRC16_Low_Byte = lowByte(x);
  y = x;   
  CRC16_Data_Array_Len--; 
  *CRC16_Data_Array++;  
  while (CRC16_Data_Array_Len--)
  {
    y ^= *CRC16_Data_Array;
    for (i = 0; i < 8; ++i)
    {
      if (y & 1)
        y = (y >> 1) ^ 0xA001; // <---The Key
      else
        y = (y >> 1);
    }
    KIRSP_CRC16_Hi_Byte = highByte(y);
    KIRSP_CRC16_Low_Byte = lowByte(y);   
    *CRC16_Data_Array++;
  }  
  KIRSP_CRC16_Hi_Byte = lowByte(y);   // write to global variable
  KIRSP_CRC16_Low_Byte = highByte(y); // write to global variable
  return y;
} // end of KIRSP_cal_CRC16 function

