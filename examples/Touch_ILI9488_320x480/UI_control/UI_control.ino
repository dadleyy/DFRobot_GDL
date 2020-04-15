/*!
 * @file UI_contal.ino
 * @brief 在屏幕上创建一个开关，文本框和滑条控件，用户可以这些控件的参数，也可以使用默认的参数
 * @n 当用户使用控件时，会在文本框显示你当前的操作
 * @n 本示例支持的主板有Arduino Uno, Mega2560, FireBeetle-ESP32, FireBeetle-SP8266, FireBeetle-M0
 * @n 需要文本框显示时，需要点击文本框以使光标移到文本框内
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-6
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_GDL/src/DFRpbot_UI
*/

/*!
 * @file UI_contal.ino
 * @brief creates a switch, text box and slider control on the screen, the user can use the parameters of these controls, or use the default parameters
 * @n When the user uses the control, it will display your current operation in the text box
 * @n The motherboards supported by this example are Arduino Uno, Mega2560, FireBeetle-ESP32, FireBeetle-SP8266, FireBeetle-M0
 * @n When the text box needs to be displayed, you need to click the text box to move the cursor into the text box
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [fengli] (li.feng@dfrobot.com)
 * @version V1.0
 * @date 2019-12-6
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_GDL/src/DFRpbot_UI
*/
#include "DFRobot_UI.h"
#include "Arduino.h"
#include "DFRobot_GDL.h"
#include "DFRobot_Touch.h"
/*M0*/
#if defined ARDUINO_SAM_ZERO
#define TFT_DC  7
#define TFT_CS  5
#define TFT_RST 6
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define TFT_DC  D3
#define TFT_CS  D4
#define TFT_RST D5
/*AVR系列主板*/  /* AVR series motherboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#endif

/**
   @brief Constructor  当触摸采用gt系列芯片时，可以调用此构造函数 When the touch uses the gt series chip, you can call this constructor
*/
DFRobot_Touch_GT911 touch;

/**
   @brief Constructor  当屏采用硬件SPI通信，驱动IC是st7789，屏幕分辨率是240x320时，可以调用此构造函数
   @param dc  SPI通信的命令/数据线引脚
   @param cs  SPI通信的片选引脚
   @param rst  屏的复位引脚
*/
/**
   @brief Constructor When the screen uses hardware SPI communication, the driver IC is st7789, and the screen resolution is 240x320, this constructor can be called
   @param dc Command / data line pin for SPI communication
   @param cs Chip select pin for SPI communication
   @param rst Reset pin of the screen
*/
DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/*M0主板下DMA传输*/ /* M0 motherboard DMA transfer */
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


/**
   @brief 构造函数
   @param gdl 屏幕对象
   @param touch 触摸对象
*/
/**
   @brief constructor
   @param gdl screen object
   @param touch touch object
*/
DFRobot_UI ui(&screen, &touch);
//滑条控件的回调函数  Callback function of slider control
void changeColor(DFRobot_UI::sSlider_t &slider, DFRobot_UI::sTextBox_t &textBox) {
  String s(slider.value);
  textBox.setText("slider's value is " + s);
}

//开关控件的回调函数 Callback function of switch control
void switchCallBack(DFRobot_UI::sSwitch_t &sw, DFRobot_UI::sTextBox_t &textBox) {
  if (sw.state == 0) {
    textBox.setText("you have turn off the Switch!");

  }
  else {
    textBox.setText("you have turn on the Switch!");

  }

}
void setup()
{

  Serial.begin(9600);
  ui.begin();
  // 设置UI的主题，有两种主题可供选择 1.CLASSIC ，2.MODERN。  Set the theme of the UI, there are two themes to choose from 1.CLASSIC, 2.MODERN.
  ui.setTheme(DFRobot_UI::MODERN);

  //创建一个文本框控件 Create a text box control
  DFRobot_UI::sTextBox_t &tb = ui.creatText();
  //在屏幕上创建一个文本框控件，根据自定义或初始化的参数绘制文本框 Create a text box control on the screen and draw the text box according to the customized or initialized parameters
  ui.draw(&tb);
  //初始化滑条控件，对滑条的参数进行初始化赋值 Initialize the slider control, initialize and assign the parameters of the slider
  DFRobot_UI::sSlider_t &slider = ui.creatSlider();
  slider.setCallback(changeColor);
  //  slider.bgColor = COLOR_RGB565_GREEN;
  //设置滑条的输出文本框 Set the output text box of the slider
  slider.setOutput(&tb);
  //在指定位置绘制滑条  Draw a slider at a specified position
  ui.draw(&slider,/*x = */(screen.width() - slider.width) / 2,/*y = */(screen.height() - tb.posy - tb.height) / 3 * 1 + tb.posy + tb.height);

  DFRobot_UI::sSwitch_t &swh = ui.creatSwitch();
  swh.bgColor = COLOR_RGB565_RED;
  swh.setCallback(switchCallBack);
  //设置开关的输出文本框  Set the output text box of the switch
  swh.setOutput(&tb);
  //在指定位置绘制开关  Draw switch at specified position
  ui.draw(&swh,/*x = */(screen.width() - swh.width - 40) / 2,/*y = */(screen.height() - tb.posy - tb.height) / 3 * 2 + tb.posy + tb.height,/*width=*/55,/*height=*/20);


}


void loop()
{
  ui.refresh();
}