#include "Register.h"

// Command and Parameter related Strings
String RawCommandLine = "";
String Command  = "QVGA";
String FPSParam = "ThirtyFPS";
String AWBParam = "SAWB";
String AECParam = "HistAEC";
String YUVMatrixParam = "YUVMatrixOn";
String DenoiseParam = "DenoiseNo";
String EdgeParam = "EdgeNo";
String ABLCParam = "AblcON";


// Parameters:
//   start : Start address, use a define for the register
//   pData : A pointer to the data to write.
//   size  : The number of bytes to write.
//
int OV7670Write(int start, const byte *pData, int size)
{
  int n, error;

  Wire.beginTransmission(OV7670_I2C_ADDRESS);
  n = Wire.write(start);        // write the start address
  if (n != 1)
  {
    return (I2C_ERROR_WRITING_START_ADDRESS);
  }
  
  n = Wire.write(pData, size);  // write data bytes
  if (n != size)
  {
    return (I2C_ERROR_WRITING_DATA);
  }
  
  error = Wire.endTransmission(true); // release the I2C-bus
  if (error != 0)
  {
    return (error);
  }
  
  return 0;         // return : no error
}


byte ReadRegisterValue(int RegisterAddress)
{
  byte data = 0;
  
  Wire.beginTransmission(OV7670_I2C_ADDRESS);         
  Wire.write(RegisterAddress);                        
  Wire.endTransmission();
  Wire.requestFrom(OV7670_I2C_ADDRESS, 1);            
  while(Wire.available() < 1);              
  data = Wire.read(); 

  return data;  
}

int OV7670WriteReg(int reg, byte data)
{
  int error;

  error = OV7670Write(reg, &data, 1);

  return (error);
}


String ParseI2CResult(int result)
{
  String sresult = "";
  switch(result)
  {
    case 0:
     sresult = "I2C Operation OK ...";
    break;
    
    case  I2C_ERROR_WRITING_START_ADDRESS:
     sresult = "I2C_ERROR_WRITING_START_ADDRESS";
    break;
    
    case I2C_ERROR_WRITING_DATA:
     sresult = "I2C_ERROR_WRITING_DATA";
    break;
      
    case DATA_TOO_LONG:
     sresult = "DATA_TOO_LONG";
    break;   
    
    case NACK_ON_TRANSMIT_OF_ADDRESS:
     sresult = "NACK_ON_TRANSMIT_OF_ADDRESS";
    break;
    
    case NACK_ON_TRANSMIT_OF_DATA:
     sresult = "NACK_ON_TRANSMIT_OF_DATA";
    break;
    
    case OTHER_ERROR:
     sresult = "OTHER_ERROR";
    break;
       
    default:
     sresult = "I2C ERROR TYPE NOT FOUND...";
    break;
  }
 
  return sresult;
}


void ResetCameraRegisters()
{
  // Reset Camera Registers
  // Reading needed to prevent error
  byte data = ReadRegisterValue(COM7);
  
  int result = OV7670WriteReg(COM7, COM7_VALUE_RESET );
  String sresult = ParseI2CResult(result);
  //Serial.println("RESETTING ALL REGISTERS BY SETTING COM7 REGISTER to 0x80: " + sresult);

  // Delay at least 500ms 
  delay(500);
}





void ReadRegisters()
{
  byte data = 0;
  
  data = ReadRegisterValue(CLKRC);
  Serial.print(F("CLKRC = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM7);
  Serial.print(F("COM7 = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM3);
  Serial.print(F("COM3 = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM14);
  Serial.print(F("COM14 = "));
  Serial.println(data,HEX);
   
  data = ReadRegisterValue(SCALING_XSC);
  Serial.print(F("SCALING_XSC = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(SCALING_YSC);
  Serial.print(F("SCALING_YSC = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(SCALING_DCWCTR);
  Serial.print(F("SCALING_DCWCTR = "));
  Serial.println(data,HEX);
 
  data = ReadRegisterValue(SCALING_PCLK_DIV);
  Serial.print(F("SCALING_PCLK_DIV = "));
  Serial.println(data,HEX);
   
  data = ReadRegisterValue(SCALING_PCLK_DELAY);
  Serial.print(F("SCALING_PCLK_DELAY = "));
  Serial.println(data,HEX);
  
  //data = ReadRegisterValue(COM10);
  //Serial.print(F("COM10 (Vsync Polarity) = "));
  //Serial.println(data,HEX);
  
  // default value D
  data = ReadRegisterValue(TSLB);
  Serial.print(F("TSLB (YUV Order- Higher Bit, Bit[3]) = "));
  Serial.println(data,HEX);
  
  // default value 88
  data = ReadRegisterValue(COM13);
  Serial.print(F("COM13 (YUV Order - Lower Bit, Bit[1]) = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM17);
  Serial.print(F("COM17 (DSP Color Bar Selection) = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM4);
  Serial.print(F("COM4 (works with COM 17) = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM15);
  Serial.print(F("COM15 (COLOR FORMAT SELECTION) = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM11);
  Serial.print(F("COM11 (Night Mode) = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM8);
  Serial.print(F("COM8 (Color Control, AWB) = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(HAECC7);
  Serial.print(F("HAECC7 (AEC Algorithm Selection) = "));
  Serial.println(data,HEX);
 
  data = ReadRegisterValue(GFIX);
  Serial.print(F("GFIX = "));
  Serial.println(data,HEX);
  
   
  // Window Output
  data = ReadRegisterValue(HSTART);
  Serial.print(F("HSTART = "));
  Serial.println(data,HEX);
  //Serial.print(F(", "));
  //Serial.println(data, DEC);
  
  data = ReadRegisterValue(HSTOP);
  Serial.print(F("HSTOP = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(HREF);
  Serial.print(F("HREF = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(VSTRT);
  Serial.print(F("VSTRT = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(VSTOP);
  Serial.print(F("VSTOP = "));
  Serial.println(data,HEX);
 
  data = ReadRegisterValue(VREF);
  Serial.print(F("VREF = "));
  Serial.println(data,HEX);
}


void SetupCameraFor30FPS()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("........... Setting Camera to 30 FPS ........"));
   result = OV7670WriteReg(CLKRC, CLKRC_VALUE_30FPS);
   sresult = ParseI2CResult(result);
   Serial.print(F("CLKRC: "));
   Serial.println(sresult);

   result = OV7670WriteReg(DBLV, DBLV_VALUE_30FPS);
   sresult = ParseI2CResult(result);
   Serial.print(F("DBLV: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(EXHCH, EXHCH_VALUE_30FPS);
   sresult = ParseI2CResult(result);
   Serial.print(F("EXHCH: "));
   Serial.println(sresult);

   result = OV7670WriteReg(EXHCL, EXHCL_VALUE_30FPS);
   sresult = ParseI2CResult(result);
   Serial.print(F("EXHCL: "));
   Serial.println(sresult);
   
   result = OV7670WriteReg(DM_LNL, DM_LNL_VALUE_30FPS);
   sresult = ParseI2CResult(result);
   Serial.print(F("DM_LNL: "));
   Serial.println(sresult);

   result = OV7670WriteReg(DM_LNH, DM_LNH_VALUE_30FPS);
   sresult = ParseI2CResult(result);
   Serial.print(F("DM_LNH: "));
   Serial.println(sresult);

   result = OV7670WriteReg(COM11, COM11_VALUE_30FPS);
   sresult = ParseI2CResult(result);
   Serial.print(F("COM11: "));
   Serial.println(sresult);
   
}

void SetupCameraNightMode()
{
   int result = 0;
   String sresult = "";
   
   //Serial.println(F("......... Turning NIGHT MODE ON ........"));
   result = OV7670WriteReg(CLKRC, CLKRC_VALUE_NIGHTMODE_AUTO);

 
   result = OV7670WriteReg(COM11, COM11_VALUE_NIGHTMODE_AUTO);

}


void SetCameraFPSMode()
{
   // Set FPS for Camera
   if (FPSParam == "ThirtyFPS")
   {
     SetupCameraFor30FPS();
   }    
   else
   if (FPSParam == "NightMode")
   {
     SetupCameraNightMode();
   } 
}


void SetupCameraAverageBasedAECAGC()
{
   int result = 0;
   String sresult = "";
   
   //Serial.println(F("-------------- Setting Camera Average Based AEC/AGC Registers ---------------"));
  
   result = OV7670WriteReg(AEW, AEW_VALUE);



   result = OV7670WriteReg(AEB, AEB_VALUE);

   result = OV7670WriteReg(VPT, VPT_VALUE);
   
   result = OV7670WriteReg(HAECC7, HAECC7_VALUE_AVERAGE_AEC_ON);
}


void SetCameraHistogramBasedAECAGC()
{
   int result = 0;
   String sresult = "";
   
   //Serial.println(F("-------------- Setting Camera Histogram Based AEC/AGC Registers ---------------"));
  
   result = OV7670WriteReg(AEW, AEW_VALUE);

   result = OV7670WriteReg(AEB, AEB_VALUE);
  
   result = OV7670WriteReg(HAECC1, HAECC1_VALUE);

   result = OV7670WriteReg(HAECC2, HAECC2_VALUE);

   result = OV7670WriteReg(0xA1, 0x03); // Reserved register?

   result = OV7670WriteReg(HAECC3, HAECC3_VALUE);

   result = OV7670WriteReg(HAECC4, HAECC4_VALUE);

   result = OV7670WriteReg(HAECC5, HAECC5_VALUE);

   result = OV7670WriteReg(HAECC6, HAECC6_VALUE);
 
   result = OV7670WriteReg(HAECC7, HAECC7_VALUE_HISTOGRAM_AEC_ON);

}

void SetCameraAEC()
{
    // Process AEC
   if (AECParam == "AveAEC")
   {
     // Set Camera's Average AEC/AGC Parameters  
     SetupCameraAverageBasedAECAGC();  
   }
   else
   if (AECParam == "HistAEC")
   { 
     // Set Camera AEC algorithim to Histogram
     SetCameraHistogramBasedAECAGC();
   }
}


void SetupCameraAdvancedAutoWhiteBalanceConfig()
{
   int result = 0;
   String sresult = "";
  
   result = OV7670WriteReg(AWBC1, AWBC1_VALUE);
 
   result = OV7670WriteReg(AWBC2, AWBC2_VALUE);
  
   result = OV7670WriteReg(AWBC3, AWBC3_VALUE);
  
   result = OV7670WriteReg(AWBC4, AWBC4_VALUE);
 
   result = OV7670WriteReg(AWBC5, AWBC5_VALUE);

   result = OV7670WriteReg(AWBC6, AWBC6_VALUE);
  
   result = OV7670WriteReg(AWBC7, AWBC7_VALUE);

   result = OV7670WriteReg(AWBC8, AWBC8_VALUE);

   result = OV7670WriteReg(AWBC9, AWBC9_VALUE);

   result = OV7670WriteReg(AWBC10, AWBC10_VALUE);

   result = OV7670WriteReg(AWBC11, AWBC11_VALUE);

   result = OV7670WriteReg(AWBC12, AWBC12_VALUE);

   result = OV7670WriteReg(AWBCTR3, AWBCTR3_VALUE);

   result = OV7670WriteReg(AWBCTR2, AWBCTR2_VALUE);

   result = OV7670WriteReg(AWBCTR1, AWBCTR1_VALUE);

}

void SetupCameraSimpleAutomaticWhiteBalance()
{
 /*
   i2c_salve_Address = 0x42;
   write_i2c(0x13, 0xe7); //AWB on
   write_i2c(0x6f, 0x9f); // Simple AWB
 */
 
   int result = 0;
   String sresult = "";
   
   result = OV7670WriteReg(COM8, COM8_VALUE_AWB_ON);

   // AWBCTR0
   //result = OV7670WriteReg(0x6f, 0x9f);
   result = OV7670WriteReg(AWBCTR0, AWBCTR0_VALUE_NORMAL);

}

void SetupCameraAdvancedAutomaticWhiteBalance()
{
   int result = 0;
   String sresult = "";
   
  // Serial.println(F("........... Setting Camera to Advanced AWB ........"));
  
   // AGC, AWB, and AEC Enable
   result = OV7670WriteReg(0x13, 0xE7);

   // AWBCTR0 
   result = OV7670WriteReg(0x6f, 0x9E);

}

void SetupCameraGain()
{
   int result = 0;
   String sresult = "";

   // Set Maximum Gain
   //result = OV7670WriteReg(COM9, COM9_VALUE_MAX_GAIN_128X);
   result = OV7670WriteReg(COM9, COM9_VALUE_4XGAIN);

   
   // Set Blue Gain
   //{ REG_BLUE, 0x40 },
   result = OV7670WriteReg(BLUE, BLUE_VALUE);

   // Set Red Gain
   //{ REG_RED, 0x60 },
   result = OV7670WriteReg(RED, RED_VALUE);

   // Set Green Gain
   //{ 0x6a, 0x40 }, 
   result = OV7670WriteReg(GGAIN, GGAIN_VALUE);

   // Enable AWB Gain
   // REG_COM16  0x41  /* Control 16 */
   // COM16_AWBGAIN   0x08    /* AWB gain enable */
   // { REG_COM16, COM16_AWBGAIN }, 
   result = OV7670WriteReg(COM16, COM16_VALUE);
   
}

void SetCameraSaturationControl()
{
  int result = 0;
  String sresult = "";
  
 // Serial.println(F("........... Setting Camera Saturation Level ........"));
  result = OV7670WriteReg(SATCTR, SATCTR_VALUE);

}




void SetupCameraAWB()
{
   // Set AWB Mode
   if (AWBParam == "SAWB")
   {
     // Set Simple Automatic White Balance
     SetupCameraSimpleAutomaticWhiteBalance(); // OK
      
     // Set Gain Config
     SetupCameraGain();
   }
   else
   if (AWBParam == "AAWB")
   {
     // Set Advanced Automatic White Balance
     SetupCameraAdvancedAutomaticWhiteBalance(); // ok
   
     // Set Camera Automatic White Balance Configuration
     SetupCameraAdvancedAutoWhiteBalanceConfig(); // ok
     
     // Set Gain Config
     SetupCameraGain();
   }
}


void SetupCameraDenoise()
{  
   int result = 0;
   String sresult = "";
   
   //Serial.println(F("........... Setting Camera Denoise  ........"));
  
   result = OV7670WriteReg(DNSTH, DNSTH_VALUE);

   result = OV7670WriteReg(REG77, REG77_VALUE);

}


void SetupCameraEdgeEnhancement()
{
   int result = 0;
   String sresult = "";
   
 //  Serial.println(F("........... Setting Camera Edge Enhancement  ........"));
  
   result = OV7670WriteReg(EDGE, EDGE_VALUE);


   result = OV7670WriteReg(REG75, REG75_VALUE);

   result = OV7670WriteReg(REG76, REG76_VALUE);

}



void SetupCameraDenoiseEdgeEnhancement()
{
   int result = 0;
   String sresult = "";
   
   if ((DenoiseParam == "DenoiseYes")&& 
       (EdgeParam == "EdgeYes"))
      {
        SetupCameraDenoise();
        SetupCameraEdgeEnhancement();
        result = OV7670WriteReg(COM16, COM16_VALUE_DENOISE_ON__EDGE_ENHANCEMENT_ON__AWBGAIN_ON);

      }
      else
      if ((DenoiseParam == "DenoiseYes")&& 
          (EdgeParam == "EdgeNo"))
       {
         SetupCameraDenoise();
         result = OV7670WriteReg(COM16, COM16_VALUE_DENOISE_ON__EDGE_ENHANCEMENT_OFF__AWBGAIN_ON);
       }
       else
       if ((DenoiseParam == "DenoiseNo")&& 
          (EdgeParam == "EdgeYes"))
          {
            SetupCameraEdgeEnhancement();
            result = OV7670WriteReg(COM16, COM16_VALUE_DENOISE_OFF__EDGE_ENHANCEMENT_ON__AWBGAIN_ON);
          }
}



void SetupCameraABLC()
{
   int result = 0;
   String sresult = "";
   
   // If ABLC is off then return otherwise
   // turn on ABLC.
   if (ABLCParam == "AblcOFF")
   {
     return;
   }
   
  // Serial.println(F("........ Setting Camera ABLC ......."));
   
   result = OV7670WriteReg(ABLC1, ABLC1_VALUE);
  
   result = OV7670WriteReg(THL_ST, THL_ST_VALUE);
}

void SetupCameraArrayControl()
{
   int result = 0;
   String sresult = "";
   
   //Serial.println(F("........... Setting Camera Array Control  ........"));
  
   result = OV7670WriteReg(CHLF, CHLF_VALUE);

   result = OV7670WriteReg(ARBLM, ARBLM_VALUE);
}

void SetupCameraUndocumentedRegisters()
{ 
   // Write(0xb0,0x84); //adding this improve the color a little bit
   int result = 0;
   String sresult = "";
   
   //Serial.println(F("........... Setting Camera Undocumented Registers ........"));
   result = OV7670WriteReg(0xB0, 0x84);
}

void SetupCameraADCControl()
{
   int result = 0;
   String sresult = "";
   
  // Serial.println(F("........... Setting Camera ADC Control  ........"));
  
   result = OV7670WriteReg(ADCCTR1, ADCCTR1_VALUE);

   result = OV7670WriteReg(ADCCTR2, ADCCTR2_VALUE);

   result = OV7670WriteReg(ADC, ADC_VALUE);
 
   result = OV7670WriteReg(ACOM, ACOM_VALUE);

   result = OV7670WriteReg(OFON, OFON_VALUE);

}
