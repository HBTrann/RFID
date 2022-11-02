//#ifndef MFRC522_H
#define MFRC522_H

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdint.h> //khai bao thu vien uint8_t
#include <unistd.h>

#define channel 0
#define MAX_LEN 18

#define PCD_IDLE              0x00               
#define PCD_AUTHENT           0x0E              
#define PCD_RECEIVE           0x08               
#define PCD_TRANSMIT          0x04               
#define PCD_TRANSCEIVE        0x0C               
#define PCD_RESETPHASE        0x0F            
#define PCD_CALCCRC           0x03              


#define PICC_REQIDL           0x26               
#define PICC_REQALL           0x52               
#define PICC_ANTICOLL         0x93            
#define PICC_ANTICOLL1        0x93            
#define PICC_ANTICOLL2        0x95             
#define PICC_AUTHENT1A        0x60              
#define PICC_AUTHENT1B        0x61              
#define PICC_READ             0x30               
#define PICC_WRITE            0xA0          
#define PICC_DECREMENT        0xC0              
#define PICC_INCREMENT        0xC1               
#define PICC_RESTORE          0xC2               
#define PICC_TRANSFER         0xB0            
#define PICC_HALT             0x50              


#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte

// PAGE 0
#define     RFU00                 0x00   
#define     CommandReg            0x01   
#define     CommIEnReg            0x02   
#define     DivlEnReg             0x03   
#define     CommIrqReg            0x04   
#define     DivIrqReg             0x05
#define     ErrorReg              0x06   
#define     Status1Reg            0x07   
#define     Status2Reg            0x08   
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1    
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2   
#define     RFU20                 0x20 
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3     
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39 
#define     TestDAC2Reg           0x3A  
#define     TestADCReg            0x3B  
#define     RFU3C                 0x3C  
#define     RFU3D                 0x3D  
#define     RFU3E                 0x3E  
#define     RFU3F    0x3F

#define MI_OK                          0
#define MI_NOTAGERR                    (-1)
#define MI_ERR                         (-2)
 
void RFID_antennaOn(void)
{
  unsigned char temp;
 
  temp = readMFRC522(TxControlReg);
  if (!(temp & 0x03))
  {
    RFID_setBitMask(TxControlReg, 0x03);
  }
}

void MFRC522_init()
{
  //SPI
  wiringPiSPISetup(channel, 8000000); //SPI0, 13MHz

  writeMFRC522(CommandReg, PCD_RESETPHASE);
  writeMFRC522(TModeReg, 0x8D);   //Tauto=1; f(Timer) = 6.78MHz/TPreScaler
  writeMFRC522(TPrescalerReg, 0x3E);  //TModeReg[3..0] + TPrescalerReg
  writeMFRC522(TReloadRegL, 30);
  writeMFRC522(TReloadRegH, 0);
  writeMFRC522(TxAutoReg, 0x40);    //100%ASK
  writeMFRC522(ModeReg, 0x3D);    // CRC valor inicial de 0x6363
 
  RFID_antennaOn();    
}
 
void writeMFRC522(unsigned char Address, unsigned char value)
{
    unsigned char buff[2];
 
    buff[0] = (char)((Address<<1)&0x7E);
    buff[1] = (char)value;
     
    wiringPiSPIDataRW(channel, buff, 2);  
}
 
unsigned char readMFRC522(unsigned char Address)
{
    unsigned char buff[2];
    buff[0] = ((Address<<1)&0x7E)|0x80;
    wiringPiSPIDataRW(channel, buff, 2);
    return (uint8_t)buff[1];
}


void RFID_setBitMask(unsigned char reg, unsigned char mask)
{
  unsigned char tmp;
  tmp = readMFRC522(reg);
  writeMFRC522(reg, tmp | mask);  // set bit mask
}

void RFID_clearBitMask(unsigned char reg, unsigned char mask)
{
  unsigned char tmp;
  tmp = readMFRC522(reg);
  writeMFRC522(reg, tmp & (~mask));  // clear bit mask
}



void RFID_antennaOff(void)
{
  unsigned char temp;
 
  temp = readMFRC522(TxControlReg);
  if (!(temp & 0x03))
  {
    RFID_clearBitMask(TxControlReg, 0x03);
  }
}

void RFID_calculateCRC(unsigned char *pIndata, unsigned char len, unsigned char *pOutData)
{
  unsigned char i, n;
 
  RFID_clearBitMask(DivIrqReg, 0x04);      //CRCIrq = 0
  RFID_setBitMask(FIFOLevelReg, 0x80);     
  //Write_MFRC522(CommandReg, PCD_IDLE);
 
  for (i=0; i<len; i++)
    writeMFRC522(FIFODataReg, *(pIndata+i));
  writeMFRC522(CommandReg, PCD_CALCCRC);
 
  i = 0xFF;
  do
  {
    n = readMFRC522(DivIrqReg);
    i--;
  }
  while ((i!=0) && !(n&0x04));      //CRCIrq = 1
 
  pOutData[0] = readMFRC522(CRCResultRegL);
  pOutData[1] = readMFRC522(CRCResultRegM);
}

unsigned char RFID_MFRC522ToCard(unsigned char command, unsigned char *sendData, unsigned char sendLen, unsigned char *backData, unsigned int *backLen)
{
  unsigned char status = MI_ERR;
  unsigned char irqEn = 0x00;
  unsigned char waitIRq = 0x00;
  unsigned char lastBits;
  unsigned char n;
  unsigned int i;
 
  switch (command)
  {
    case PCD_AUTHENT:   
    {
      irqEn = 0x12;
      waitIRq = 0x10;
      break;
    }
    case PCD_TRANSCEIVE:  
    {
      irqEn = 0x77;
      waitIRq = 0x30;
      break;
    }
    default:
      break;
  }
 
  writeMFRC522(CommIEnReg, irqEn|0x80); 
  RFID_clearBitMask(CommIrqReg, 0x80);       
  RFID_setBitMask(FIFOLevelReg, 0x80);       
 
  writeMFRC522(CommandReg, PCD_IDLE);  
 
  for (i=0; i<sendLen; i++)
    writeMFRC522(FIFODataReg, sendData[i]);
 
  writeMFRC522(CommandReg, command);
  if (command == PCD_TRANSCEIVE)
    RFID_setBitMask(BitFramingReg, 0x80);    //StartSend=1,transmission of data starts
 
  
  i = 2000; 
  do
  {
    //CommIrqReg[7..0]
    //Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
    n = readMFRC522(CommIrqReg);
    i--;
  }
  while ((i!=0) && !(n&0x01) && !(n&waitIRq));
 
  RFID_clearBitMask(BitFramingReg, 0x80);      //StartSend=0
 
  if (i != 0)
  {
    if(!(readMFRC522(ErrorReg) & 0x1B)) //BufferOvfl Collerr CRCErr ProtecolErr
    {
      status = MI_OK;
      if (n & irqEn & 0x01)
        status = MI_NOTAGERR;     //??
 
      if (command == PCD_TRANSCEIVE)
      {
        n = readMFRC522(FIFOLevelReg);
        lastBits = readMFRC522(ControlReg) & 0x07;
        if (lastBits)
          *backLen = (n-1)*8 + lastBits;
        else
          *backLen = n*8;
 
        if (n == 0)
          n = 1;
        if (n > MAX_LEN)
          n = MAX_LEN;
 
        
        for (i=0; i<n; i++)
          backData[i] = readMFRC522(FIFODataReg);
      }
    }
    else
      status = MI_ERR;
  }
 
  //SetBitMask(ControlReg,0x80);           //timer stops
  //Write_MFRC522(CommandReg, PCD_IDLE);
 
  return status;
}

unsigned char RFID_findCard(unsigned char reqMode, unsigned char *TagType)
{
  unsigned char status;
  unsigned int backBits;      
 
  writeMFRC522(BitFramingReg, 0x07);    //TxLastBists = BitFramingReg[2..0] ???
 
  TagType[0] = reqMode;
  status = RFID_MFRC522ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);
 
  if ((status != MI_OK) || (backBits != 0x10))
    status = MI_ERR;
 
  return status;
}

unsigned char RFID_anticoll(unsigned char *serNum)
{
  unsigned char status;
  unsigned char i;
  unsigned char serNumCheck=0;
  unsigned int unLen;
 
  RFID_clearBitMask(Status2Reg, 0x08);   //TempSensclear
  RFID_clearBitMask(CollReg,0x80);     //ValuesAfterColl
  writeMFRC522(BitFramingReg, 0x00);    //TxLastBists = BitFramingReg[2..0]
 
  serNum[0] = PICC_ANTICOLL;
  serNum[1] = 0x20;
 
  status = RFID_MFRC522ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);
 
  if (status == MI_OK)
  {
    
    for (i=0; i<4; i++){
      *(serNum+i)  = serNum[i];
      serNumCheck ^= serNum[i];
    }
    if (serNumCheck != serNum[i]){
      status = MI_ERR;
    }
  }
 
  RFID_setBitMask(CollReg, 0x80);    //ValuesAfterColl=1
 
  return status;
}

unsigned char RFID_auth(unsigned char authMode, unsigned char BlockAddr, unsigned char *Sectorkey, unsigned char *serNum)
{
  unsigned char status;
  unsigned int recvBits;
  unsigned char i;
  unsigned char buff[12];
 
  
  buff[0] = authMode;
  buff[1] = BlockAddr;
  for (i=0; i<6; i++)
    buff[i+2] = *(Sectorkey+i);
  for (i=0; i<4; i++)
    buff[i+8] = *(serNum+i);
     
  status = RFID_MFRC522ToCard(PCD_AUTHENT, buff, 12, buff, &recvBits);
  if ((status != MI_OK) || (!(readMFRC522(Status2Reg) & 0x08)))
    status = MI_ERR;
 
  return status;
}
 
unsigned char RFID_read(unsigned char blockAddr, unsigned char *recvData)
{
  unsigned char status;
  unsigned int unLen;
 
  recvData[0] = PICC_READ;
  recvData[1] = blockAddr;
  RFID_calculateCRC(recvData,2, &recvData[2]);
  status = RFID_MFRC522ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);
 
  if ((status != MI_OK) || (unLen != 0x90))
    status = MI_ERR;
 
  return status;
}
 
unsigned char RFID_write(unsigned char blockAddr, unsigned char *writeData)
{
  unsigned char status;
  unsigned int recvBits;
  unsigned char i;
  unsigned char buff[18];
 
  buff[0] = PICC_WRITE;
  buff[1] = blockAddr;
  RFID_calculateCRC(buff, 2, &buff[2]);
  status = RFID_MFRC522ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);
 
  if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
    status = MI_ERR;
 
  if (status == MI_OK)
  {
    for (i=0; i<16; i++)    //?FIFO?16Byte?? Datos a la FIFO 16Byte escribir
      buff[i] = *(writeData+i);
       
    RFID_calculateCRC(buff, 16, &buff[16]);
    status = RFID_MFRC522ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);
 
    if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
      status = MI_ERR;
  }
 
  return status;
}

unsigned char RFID_selectTag(unsigned char *serNum)
{
  unsigned char i;
  unsigned char status;
  unsigned char size;
  unsigned int recvBits;
  unsigned char buffer[9];
 
  //ClearBitMask(Status2Reg, 0x08);                        //MFCrypto1On=0
 
  //buffer[0] = PICC_SElECTTAG;
  buffer[0] = PICC_ANTICOLL1;
  buffer[1] = 0x70;
 
  for (i=0; i<5; i++)
    buffer[i+2] = *(serNum+i);
 
  RFID_calculateCRC(buffer, 7, &buffer[7]);
   
  status = RFID_MFRC522ToCard(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);
  if ((status == MI_OK) && (recvBits == 0x18))
    size = buffer[i];
  else
    size = 0;
  return size;
}

void RFID_halt()
{
  unsigned char status;
  unsigned int unLen;
  unsigned char buff[4];
 
  buff[0] = PICC_HALT;
  buff[1] = 0;
  RFID_calculateCRC(buff, 2, &buff[2]);
 
  status = RFID_MFRC522ToCard(PCD_TRANSCEIVE, buff, 4, buff,&unLen);
}
 
//#endif
