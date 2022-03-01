// https://forum.arduino.cc/t/why-the-sketch-size-and-amount-of-ram-used-by-due-is-more-than-mega/180148/9
// memory
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
extern char _end;
extern "C" char *sbrk(int i);
char *ramstart = (char *)0x20070000;
char *ramend = (char *)0x20088000;

// para tensor flow lite
#include <EloquentTinyML.h>
#include "data.h"



// para la camara
#include <Wire.h>
#include "Camara.h"
#include "Register.h"


//#define DEBUG
//#define IMG


// Globales, utilizados para compatibilidad con Sketch de  Arduino.
namespace {
const tflite::Model* model = nullptr;

tflite::MicroInterpreter* interpreter = nullptr;
constexpr int kTensorArenaSize = 40 * 1024; // para 79280
uint8_t tensor_arena[kTensorArenaSize];
tflite::ErrorReporter* error_reporter = nullptr;

TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
}
// estas variables son para la inferencia
float input_scale;
int input_zero_point;

bool inference = true;
  
String out = "";

const int BUFFERLENGTH = 255;
char IncomingByte[BUFFERLENGTH];
String CommandLine = "";
// constantes y variables para camara:

int PHOTO_WIDTH = 60;
int PHOTO_HEIGHT = 60;
int PHOTO_BYTES_PER_PIXEL = 2;

//byte data[60 * 60 * 2];
//byte sandro[6];

// Camera input/output pin connection to Arduino
#define WRST  22      // Output Write Pointer Reset
#define RRST  23      // Output Read Pointer Reset
#define WEN   24      // Output Write Enable
#define VSYNC 25      // Input Vertical Sync marking frame capture
#define RCLK  26      // Output FIFO buffer output clock
// set OE to low gnd

// FIFO Ram input pins
#define DO7   28
#define DO6   29
#define DO5   30
#define DO4   31
#define DO3   32
#define DO2   33
#define DO1   34
#define DO0   35


String rawCommandLine = "";

void InitializeOV7670Camera()
{

  //Set WRST to 0 and RRST to 0 , 0.1ms after power on.
  int DurationMicroSecs =  1;

  // Set mode for pins wither input or output
  pinMode(WRST , OUTPUT);
  pinMode(RRST , OUTPUT);
  pinMode(WEN  , OUTPUT);
  pinMode(VSYNC, INPUT);
  pinMode(RCLK , OUTPUT);

  // FIFO Ram output pins
  pinMode(DO7 , INPUT);
  pinMode(DO6 , INPUT);
  pinMode(DO5 , INPUT);
  pinMode(DO4 , INPUT);
  pinMode(DO3 , INPUT);
  pinMode(DO2 , INPUT);
  pinMode(DO1 , INPUT);
  pinMode(DO0 , INPUT);

  // Delay 1 ms
  delay(1);

  PulseLowEnabledPin(WRST, DurationMicroSecs);

  //PulseLowEnabledPin(RRST, DurationMicroSecs);
  // Need to clock the fifo manually to get it to reset
  digitalWrite(RRST, LOW);
  PulsePin(RCLK, DurationMicroSecs);
  PulsePin(RCLK, DurationMicroSecs);
  digitalWrite(RRST, HIGH);
}

// Main Call to Setup the ov7670 Camera
void SetupCamera()
{
  //Serial.println(F("In SetupCamera() ..."));
  InitializeOV7670Camera();
}

void PulseLowEnabledPin(int PinNumber, int DurationMicroSecs)
{
  // For Low Enabled Pins , 0 = on and 1 = off
  digitalWrite(PinNumber, LOW);            // Sets the pin on
  delayMicroseconds(DurationMicroSecs);    // Pauses for DurationMicroSecs microseconds

  digitalWrite(PinNumber, HIGH);            // Sets the pin off
  delayMicroseconds(DurationMicroSecs);     // Pauses for DurationMicroSecs microseconds
}

void PulsePin(int PinNumber, int DurationMicroSecs)
{
  digitalWrite(PinNumber, HIGH);           // Sets the pin on
  delayMicroseconds(DurationMicroSecs);    // Pauses for DurationMicroSecs microseconds

  digitalWrite(PinNumber, LOW);            // Sets the pin off
  delayMicroseconds(DurationMicroSecs);    // Pauses for DurationMicroSecs microseconds
}

void CaptureOV7670Frame()
{
  unsigned long DurationStart = 0;
  unsigned long DurationStop = 0;
  unsigned long TimeForCaptureStart = 0;
  unsigned long TimeForCaptureEnd = 0;
  unsigned long ElapsedTime = 0;

  TimeForCaptureStart = millis();

  // 1. Wait for VSync to pulse to indicate the start of the image
  DurationStart = pulseIn(VSYNC, HIGH);

  // 2. Reset Write Pointer to 0. Which is the beginning of frame
  PulseLowEnabledPin(WRST, 6); // 3 microseconds + 3 microseconds for error factor on Arduino

  // 3. Set FIFO Write Enable to active (high) so that image can be written to ram
  digitalWrite(WEN, HIGH);

  // 4. Wait for VSync to pulse again to indicate the end of the frame capture
  DurationStop = pulseIn(VSYNC, HIGH);

  // 5. Set FIFO Write Enable to nonactive (low) so that no more images can be written to the ram
  digitalWrite(WEN, LOW);

  // 6. Print out Stats
  TimeForCaptureEnd = millis();
  ElapsedTime = TimeForCaptureEnd - TimeForCaptureStart;

  // 7. WAIT so that new data can appear on output pins Read new data.
  delay(2);
}

// Converts pin HIGH/LOW values on pins at positions 0-7 to a corresponding byte value
byte ConvertPinValueToByteValue(int PinValue, int PinPosition)
{
  byte ByteValue = 0;
  if (PinValue == HIGH)
  {
    ByteValue = 1 << PinPosition;
  }

  return ByteValue;
}


unsigned char ReadFrameByte()
{
  // Read captured frame from FIFO memory and send each byte as it is read to the Android controller
  // via bluetooth.

  // Set Output Enable OE to active (low).
  // * Make sure to connect the OE output to ground.

  // Reset the FIFO read pointer by setting RRST to active (low) then bringing it back to high.
  // *Done from previous CaptureOV7670Frame() assuming WRST and RRST are tied together.

  // Read in the QQVGA image that is captured in the camera buffer by reading in the 38400 bytes that make up the
  //   YUV photo

  byte PixelData = 0;
  byte PinVal7 = 0;
  byte PinVal6 = 0;
  byte PinVal5 = 0;
  byte PinVal4 = 0;
  byte PinVal3 = 0;
  byte PinVal2 = 0;
  byte PinVal1 = 0;
  byte PinVal0 = 0;


  // Set Read Buffer Pointer to start of frame
  digitalWrite(RRST, LOW);
  PulsePin(RCLK, 1);
  PulsePin(RCLK, 1);
  PulsePin(RCLK, 1);
  digitalWrite(RRST, HIGH);
  byte gray = 0;

  //uint8_t* b = (uint8_t*)data;

  int i = 0;
  unsigned long  ByteCounter = 0;

  // para rgb
  uint8_t pix_lo;
  uint8_t pix_hi;
  int r;
  int v;
  int a;
  for (int height = 0; height < PHOTO_HEIGHT; height++)
  {
    for (int width = 0; width < PHOTO_WIDTH; width++)
    {
      for (int bytenumber = 0; bytenumber < PHOTO_BYTES_PER_PIXEL; bytenumber++)
      {
        // Pulse the read clock RCLK to bring in new byte of data.
        // 7ns for RCLK High Pulse Width and Low Pulse Width .007 micro secs
        PulsePin(RCLK, 1);

        // Convert Pin values to byte values for pins 0-7 of incoming pixel byte
        PinVal7 = ConvertPinValueToByteValue(digitalRead(DO7), 7);
        PinVal6 = ConvertPinValueToByteValue(digitalRead(DO6), 6);
        PinVal5 = ConvertPinValueToByteValue(digitalRead(DO5), 5);
        PinVal4 = ConvertPinValueToByteValue(digitalRead(DO4), 4);
        PinVal3 = ConvertPinValueToByteValue(digitalRead(DO3), 3);
        PinVal2 = ConvertPinValueToByteValue(digitalRead(DO2), 2);
        PinVal1 = ConvertPinValueToByteValue(digitalRead(DO1), 1);
        PinVal0 = ConvertPinValueToByteValue(digitalRead(DO0), 0);

        // Combine individual data from each pin into composite data in the form of a single byte
        PixelData = PinVal7 | PinVal6 | PinVal5 | PinVal4 | PinVal3 | PinVal2 | PinVal1 | PinVal0;

        Serial.write(PixelData);
         
        float data = 0.0;
        int8_t quantized;
        if (bytenumber == 0){
          pix_hi = PixelData;
        }else{
          pix_lo = PixelData;
          r = (0xF8 & pix_hi); 
          v = ((0x07 & pix_hi) << 5) | ((0xE0 & pix_lo) >> 3);
          a = (0x1F & pix_lo) << 3;
          data = r / 255.0;
          quantized = data / input_scale + input_zero_point;
          input->data.int8[i] = quantized;
          i++;
  
          data = v / 255.0;
          quantized = data / input_scale + input_zero_point;
          input->data.int8[i] = quantized;
          i++;

          data = a / 255.0;
          quantized = data / input_scale + input_zero_point;
          input->data.int8[i] = quantized;
          i++;
 
        }
      }
    }
  } 
}

void TakePhoto()
{
  // Take Photo using the ov7670 camera and transmit the image to the Android controller via
  // Bluetooth
  unsigned long StartTime   = 0;
  unsigned long EndTime     = 0;
  unsigned long ElapsedTime = 0;

  StartTime = millis();

  CaptureOV7670Frame();

  ReadFrameByte();

  EndTime = millis();
  ElapsedTime = (EndTime - StartTime) / 1000; // Convert to seconds

}

// fin de constantes y variables para cámara


void setup() {
  Serial.begin(115200);
  Wire.begin();
  ResetCameraRegisters();
  SetupCamera();

  model = tflite::GetModel(mi_data);


#ifdef DEBUG
  Serial.print("Model provided is schema version: ");
  Serial.print(model->version());
  Serial.print("\nTensorFlow Lite Library version: ");
  Serial.println(TFLITE_SCHEMA_VERSION);
#endif


  static tflite::ops::micro::AllOpsResolver resolver;


  static tflite::MicroInterpreter static_interpreter( model,
      resolver,
      tensor_arena,
      kTensorArenaSize,
      error_reporter);

  interpreter = &static_interpreter;
  
  TfLiteStatus allocate_status = interpreter->AllocateTensors();

  input = interpreter->input(0);
  output = interpreter->output(0);

#ifdef DEBUG
  Serial.println("input:");
  Serial.print("input->dims->data[1] : ");
  Serial.println(input->dims->data[1]);
  Serial.print("input->dims->data[2] : ");
  Serial.println(input->dims->data[2]);
  Serial.print("input->dims->data[3] : ");
  Serial.println(input->dims->data[3]);
  Serial.print("input->type : ");
  Serial.println(input->type);
  Serial.print("input->dims->size : ");
  Serial.println(input->dims->size);
  Serial.print("input->bytes : ");
  Serial.println(input->bytes);

#endif
  //  if (input->type != kTfLiteInt8) {
  //    Serial.println("No es kTfLiteInt8");
  //  } else {
  //    Serial.println("Sí es kTfLiteInt8");
  //  }
#ifdef DEBUG
  Serial.println("------- this is the key-------");

  Serial.print("input->params.zero_point : ");
  Serial.println(input->params.zero_point);
  Serial.print("input->params.scale : ");
  Serial.println(input->params.scale, 8);


  Serial.println("--------------");
  Serial.println("output:");
  Serial.print("output->dims->data[1] : ");
  Serial.println(output->dims->data[1]);
  Serial.print("output->type : ");
  Serial.println(output->type);
  Serial.print("output->dims->size : ");
  Serial.println(output->dims->size);
  Serial.println("------- this is the key-------");

  Serial.print("output->params.zero_point : ");
  Serial.println(output->params.zero_point);
  Serial.print("output->params.scale : ");
  Serial.println(output->params.scale, 8);
#endif

  SquareRGB565();
}

void inferenciaFinal(){
   // Get the output quantization parameters
  float output_scale = output->params.scale;
  int output_zero_point = output->params.zero_point;

  TfLiteStatus invoke_status = interpreter->Invoke();

  float out_0 = (output->data.int8[0] - output_zero_point) * output_scale;
  float out_1 = (output->data.int8[1] - output_zero_point) * output_scale;

  //Serial.print("Naranja : ");
  Serial.print(out_0 * 100, 3);
  Serial.print(" , ");
  //Serial.print("Manzana : ");
  Serial.print(out_1 * 100, 3);
  Serial.println("");
}
void inferencia_imagen() {

  // Get the input quantization parameters
  input_scale = input->params.scale;
  input_zero_point = input->params.zero_point;

  TakePhoto();

}

void inferencia() {
  
  // Get the input quantization parameters
  input_scale = input->params.scale;
  input_zero_point = input->params.zero_point;
  
  int contador = 0;
  float data = 0.0;
  int8_t quantized;

  float output_scale = output->params.scale;
  int output_zero_point = output->params.zero_point;

  TfLiteStatus invoke_status = interpreter->Invoke();

  float out_0 = (output->data.int8[0] - output_zero_point) * output_scale;
  float out_1 = (output->data.int8[1] - output_zero_point) * output_scale;

  int decimales = 5;
  Serial.print(" Orange  : ");
  Serial.println(out_0, decimales);
  Serial.print(" Apple   : ");
  Serial.println(out_1, decimales);


}
void loop() {
 while (1)
  {
    if (Serial.available() > 0)
    {
      int NumberCharsRead = Serial.readBytesUntil('\n', IncomingByte, BUFFERLENGTH);
      for (int i = 0; i < NumberCharsRead; i++)
      {
        CommandLine += IncomingByte[i];
      }
      break;
    }
  }

  if (CommandLine == "c")
  {
    inferencia_imagen();
    //Serial.write(data, 1800); //para probar
    CommandLine= "";

  }
  if (CommandLine == "i")
  {

    inferenciaFinal();
    //Serial.write(data, 1800); //para probar
    CommandLine= "";

  }
  if (CommandLine == "fm")
  {
     char *heapend=sbrk(0);
     register char * stack_ptr asm ("sp");
     struct mallinfo mi=mallinfo();
     printf("\nDynamic ram usada: %d\n",mi.uordblks);
     printf("RAM estática del usada:  %d\n",&_end - ramstart); 
     printf("Stack ram usado: %d\n",ramend - stack_ptr); 
     printf("Memoria libre: %d\n\n",stack_ptr - heapend + mi.fordblks);
     
     CommandLine= "";
  }
}
void SquareRGB565() 
{
  int result = 0;
  String sresult = "";
  
  PHOTO_WIDTH  = 60;
  PHOTO_HEIGHT = 60;
  PHOTO_BYTES_PER_PIXEL = 2;
 // data[PHOTO_WIDTH * PHOTO_HEIGHT * 2];

  // Night Modes
  result = OV7670WriteReg(CLKRC, 0x40);  // //DEDAULD = 0x80


  result = OV7670WriteReg(COM11, 0x12);

  // Basic RGB 565
  
  result = OV7670WriteReg(COM7, 0x04 );    /* bits 0 and 2 - RGB format */

  sresult = ParseI2CResult(result);

  result = OV7670WriteReg(REG_RGB444, 0 );    //REG_RGB444 = 0x8c


  result = OV7670WriteReg( REG_COM15, 0x10 | 0xC0  );    // COM15_RGB565 y debe ser 0xD0


 // generales

  result = OV7670WriteReg(TSLB, 0x04);


  result = OV7670WriteReg(REG_COM10, COM10_VS_NEG);

  
  //gama();

  result = OV7670WriteReg(COM8, 0x80 | 0x40 |0x20);

  result = OV7670WriteReg(GAIN, GAIN_VALUE); //0x00     

  result = OV7670WriteReg(COM2_SSLEEP, 0x00); 

  result = OV7670WriteReg(REG_COM4, 0x00);   

  result = OV7670WriteReg(REG_COM9, 0x20); // Max AGC value

  result = OV7670WriteReg(REG_BD50MAX, 0x05);

  result = OV7670WriteReg(REG_BD60MAX, 0x07);

  SetCameraHistogramBasedAECAGC();

  result = OV7670WriteReg(COM8, 0x80 | 0x40 |0x20 |0x04 | 0x01 );   //AWB on COM8_VALUE_AWB_ON
 
  
  result = OV7670WriteReg(REG_COM5, 0x61);

  result = OV7670WriteReg(REG_COM6, 0x4B);

  result = OV7670WriteReg(0x16, 0x02); // Reserved register?

  result = OV7670WriteReg(REG_MVFP, 0x07);

  result = OV7670WriteReg(ADCCTR1, 0x02);
  
  result = OV7670WriteReg(ADCCTR2, 0x91);

  result = OV7670WriteReg(0x29, 0x07); // Reserved register?

  result = OV7670WriteReg(CHLF, 0x0B);

  result = OV7670WriteReg(0x35, 0x0B); // Reserved register?

  result = OV7670WriteReg(ADC, 0x1D);

  result = OV7670WriteReg(ACOM, 0x71);
  
  result = OV7670WriteReg(OFON, 0x2A);

  result = OV7670WriteReg(OFON, 0x2A);

  result = OV7670WriteReg(REG_COM12, 0x78); // HASTA AQUI FUNCIONA
  
  result = OV7670WriteReg(0x4D, 0x40); // Reserved register?

  result = OV7670WriteReg(0x4E, 0x20); // Reserved register?

  result = OV7670WriteReg(REG_GFIX, 0x5D);

  result = OV7670WriteReg(REG_GFIX, 0x5D);

  result = OV7670WriteReg(0x74, 0x19);  //


  result = OV7670WriteReg(0x8D, 0x4F); // Reserved register?
  result = OV7670WriteReg(0x8E, 0x00); // Reserved register?
  result = OV7670WriteReg(0x8F, 0x00); // Reserved register?
  result = OV7670WriteReg(0x90, 0x00); // Reserved register?
  result = OV7670WriteReg(0x91, 0x00); // Reserved register?

  result = OV7670WriteReg(DM_LNL, 0x00);

  result = OV7670WriteReg(0x96, 0x00); // Reserved register?
  result = OV7670WriteReg(0x9A, 0x80); // Reserved register?
  result = OV7670WriteReg(0xB0, 0x84); // Reserved register?

  result = OV7670WriteReg(ABLC1, 0x0C);

  result = OV7670WriteReg(0xB2, 0x0E); // Reserved register?
 
  result = OV7670WriteReg(THL_ST, 0x82);

  result = OV7670WriteReg(0xB8, 0x0A); // Reserved register?

  SetupCameraAdvancedAutoWhiteBalanceConfig();

  // Lens correction option

  result = OV7670WriteReg(0x64, 0x04); // Lens correction option 3

  result = OV7670WriteReg(0x65, 0x20);

  result = OV7670WriteReg(0x66, 0x05); 

  result = OV7670WriteReg(0x94, 0x04); //Lens correction option 6

  result = OV7670WriteReg(0x95, 0x08); //Lens correction option 6


//  result = OV7670WriteReg(AWBCTR3, 0x0A);
//
//  result = OV7670WriteReg(AWBCTR2, 0x55);
//
//  result = OV7670WriteReg(AWBCTR1, 0x11);

  SetCameraColorMatrixYUV();
  //SetCameraColorMatrixRGB565();



  result = OV7670WriteReg(AWBCTR0, AWBCTR0_VALUE_NORMAL );   // Simple AWB = 0x9F

  result = OV7670WriteReg(AWBCTR1, 0x11 );

  result = OV7670WriteReg(REG_BRIGHT, 0x00);

  result = OV7670WriteReg(REG_CONTRAS, 0x40);

  result = OV7670WriteReg(0x57, 0x80); //REG_CONTRAS_CENTER
    
  
  
  SetupQQQVGA();


  //SetupCameraUndocumentedRegisters();
 
  SquareFRame();


}
void gama(){

  int result = 0;
  
  result = OV7670WriteReg( SLOP, 0x20 ); //0x7a

  result = OV7670WriteReg( GAM1, 0x1C ); //0x7b

  result = OV7670WriteReg( GAM2, 0x28 ); //0x7c
  
  result = OV7670WriteReg( GAM3, 0x3C);
  
  result = OV7670WriteReg( GAM4, 0x55);
  
  result = OV7670WriteReg( GAM5, 0x68);
  
  result = OV7670WriteReg( GAM6, 0x76);
  
  result = OV7670WriteReg( GAM7, 0x80);
  
  result = OV7670WriteReg( GAM8, 0x88);
  
  result = OV7670WriteReg( GAM9, 0x8F);
  
  result = OV7670WriteReg( GAM10, 0x96);
  
  result = OV7670WriteReg( GAM11, 0xA3);
  
  result = OV7670WriteReg( GAM12, 0xAF);
  
  result = OV7670WriteReg( GAM13, 0xC4);
  
  result = OV7670WriteReg( GAM14, 0xD7);

  result = OV7670WriteReg( GAM15, 0xE8);

  
  
}
void SquareFRame() { // only for square

  int result = 0;
  /*
       Horizontal positioning.

       Each frame is equivalent to 784*510 pixels of which only 640*480 pixels are dislayed.
       With the default HSTART, HSTOP, & HREF values, the image has a black band down left
       hand side. The image moves left when we increase the HSTART value.

           0                          784                          784
         0 +----------------------------+----------------------------+------//--
           |                            |                            |
           |         320                  16      320                  16
        10 |            +----------------+           +----------------+
           |            |                |           |                |
           |            |                |           |                |
           |            |     Image 1    |           |     Image 2    |
           |            |     480*480    |           |     480*480    |   (320+480-784=16)
           |            |                |           |                |
           |            |                |           |                |
       490 |            +----------------+           +----------------+
           |                            |                            |
       510 +----------------------------+----------------------------+------//--
                       Frame 1                      Frame 2

      The image sensor array is 656*488 pixels

      192 -->  192+640-784 = 48
      320 -->  320+480-784 = 16 
  */


  // here is where i have to work
  int hStart = 320;// 192 , 320
  int hStop = 16; //48 ,16
  
  // Change Window Output parameters after custom scaling
  result = OV7670WriteReg(HSTART, hStart >> 3); //0x13 HSTART_VALUE_QVGA


  result = OV7670WriteReg(HSTOP, hStop >> 3 ); //HSTOP_VALUE_QVGA


  result = OV7670WriteReg(HREF, ((hStop & 0b111) << 3) | (hStart & 0b111));  //HREF_VALUE_QVGA


  result = OV7670WriteReg(VSTRT, 8 >> 2); //VSTRT_VALUE_QVGA


  result = OV7670WriteReg(VSTOP, 488 >> 2 ); //VSTOP_VALUE_QVGA


  result = OV7670WriteReg(VREF, ((488 & 0b11) << 2) | (8 & 0b11)); //VREF_VALUE_QVGA

}

void SetupQQQVGA() { // 80 x 60 pixeles
  int result = 0;
  String sresult = "";
  
  result = OV7670WriteReg(COM3, 0x0C ); //QQVGA_SCALE_ENABLED

  result = OV7670WriteReg(COM14, 0x1b );  // pixel clock divided by 8, manual scaling enable, DCW and PCLK controlled by register


  result = OV7670WriteReg(SCALING_XSC, SCALING_XSC_VALUE_QQVGA); //0x3A

  result = OV7670WriteReg(SCALING_YSC, SCALING_YSC_VALUE_QQVGA); //0x35

  result = OV7670WriteReg(SCALING_DCWCTR, 0x22); //   downsample by 8 (0x33)!= 0b100010???

  result = OV7670WriteReg(SCALING_PCLK_DIV, 0xf3); // pixel clock divided by 8

  result = OV7670WriteReg(SCALING_PCLK_DELAY, SCALING_PCLK_DELAY_VALUE_QQVGA ); //0x02
}

void SetCameraColorMatrixYUV()
{
  int result = 0;
  String sresult = "";  
    
  result = OV7670WriteReg(MTX1, MTX1_VALUE); //0x80

 
  result = OV7670WriteReg(MTX2, MTX2_VALUE); //0x80

 
  result = OV7670WriteReg(MTX3, MTX3_VALUE); //0x00

 
  result = OV7670WriteReg(MTX4, MTX4_VALUE); //0x22

 
  result = OV7670WriteReg(MTX5, MTX5_VALUE); //0x5e

 
  result = OV7670WriteReg(MTX6, MTX6_VALUE); //0x80

 
  //result = OV7670WriteReg(CONTRAS, CONTRAS_VALUE); //0x40

 
  result = OV7670WriteReg(MTXS, MTXS_VALUE); //0x58

//  result = OV7670WriteReg(REG_BRIGHT, 0x00);
//
//  result = OV7670WriteReg(REG_CONTRAS, 0x40);
//
//  result = OV7670WriteReg(0x57, 0x80);

}



void SetCameraColorMatrixRGB565()
{
  int result = 0;
  String sresult = "";

  int s = 2;

  result = OV7670WriteReg(MTX1, 0x80 + 0x20 * s); //0x80 0x80 + 0x20 * s



  result = OV7670WriteReg(MTX2, 0x80 + 0x20 * s); //0x80



  result = OV7670WriteReg(MTX3, 0); //0x00



  result = OV7670WriteReg(MTX4, 0x22 + (0x11 * s) / 2); //0x22



  result = OV7670WriteReg(MTX5, 0x22 + (0x11 * s) / 2); //0x5e



  result = OV7670WriteReg(MTX6, 0x80 + 0x20 * s); //0x80



//  result = OV7670WriteReg(CONTRAS, CONTRAS_VALUE);
//  sresult = ParseI2CResult(result);

  result = OV7670WriteReg(MTXS, MTXS_VALUE);

}
//by sandro ormeño
