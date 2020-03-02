
#ifndef __Color_Picker_H
#define __Color_Picker_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
//#include <inttypes.h>

#include "plugdllx.h"
#include "plugx.h"


/**************************************************************************************/
/*all define call*/


#ifdef _MSC_VER 
#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define FIXME_MESSAGE(x) __pragma( message (__FILE__ "(" STRING(__LINE__) ") : FIXME: " x))
#else
#define FIXME_MESSAGE(x) 
#endif

#ifdef WIN32
#pragma warning(disable : 4996)
#endif // WIN32 


/**************************************************************************************/
// all data needed by the plugin

#define csPI				3.14159265358979323846
#define csPIR				(csPI/ 180)

/*************** GUI組件ID ******************/

#define ID_COLORWHEEL			10
#define ID_COLORSQUARE			11

#define ID_COLORSWITCHBTN		12

#define ID_COLORABBTN			13
#define ID_BLACKWHITEBTN		14
#define ID_PICKGUIBTN			15

#define ID_GEAR					16

#define ID_POPUP				17
#define ID_PIPOPUP				18

#define ID_COLORGET				19

#define ID_LSK_TXT				20

#define ID_INFO_OK				22


#define ID_NAME_R				300
#define ID_NUM_R				301
#define ID_SLIDER_R				302
#define ID_NAME_G				303
#define ID_NUM_G				304
#define ID_SLIDER_G				305
#define ID_NAME_B				306
#define ID_NUM_B				307
#define ID_SLIDER_B				308
#define ID_NAME_H				310
#define ID_NUM_H				311
#define ID_SLIDER_H				312
#define ID_NAME_S				313
#define ID_NUM_S				314
#define ID_SLIDER_S				315
#define ID_NAME_V				316
#define ID_NUM_V				317
#define ID_SLIDER_V				318

/***************** 語言化 ********************/

#define TXT_COLORWHEEL_HELP		GetLocalString( 0, "顏色的色相", "Hue")
#define TXT_COLORSQUARE_HELP	GetLocalString( 0, "顏色的明度及飽和度", "Saturation & Value")
#define TXT_COLORSWITCH_HELP	GetLocalString( 0, "交換前景色及背景色 \n右鍵點擊將前景色及背景色設為同色", "Switch A/B pen \nRight click: set A color to B pen")
#define TXT_COLORABBTN_HELP		GetLocalString( 0, "前景色及背景色 \n點擊可吸色及切換前景色及背景色", "A/B pen \nClick to pick color or change current pen")
#define TXT_BLACKWHITEBTN_HELP	GetLocalString( 0, "將前景色及背景色設定為黑白", "Set A/B pen black & white")
#define TXT_GEAR_HELP			GetLocalString( 0, "其他資訊", "Infomation")
#define TXT_POPUP_HELP			GetLocalString( 0, "選擇顯示模式", "Change mode")
#define TXT_POPUP_0				GetLocalString( 0, " RGB+HSV ", "RGB+HSV")
#define TXT_POPUP_1				GetLocalString( 0, " 無 ", "None")
#define TXT_COLORGET			GetLocalString( 0, "同步", "Sync")
#define TXT_COLORGET_HELP		GetLocalString( 0, "即時擷取TVPaint系統顏色，並同步更新 \n開啟時，在低飽合狀態下 \n色相環會跳位，但不影響功能使用 \n\n*要使用色票、顏色記錄，請開啟此功能 \n*預設為關閉", "Get TVPaint's A/B pen in realtime \nThere's some bug with low saturation \nBut the function still work perfectly")

#define TXT_NUM_R_HELP			GetLocalString( 0, "目前顏色的紅色值 \n輸入數值來變更紅色值", "Red \nInput number to set Red")
#define TXT_SLIDER_R_HELP		GetLocalString( 0, "拖拉滑桿來變更紅色值", "Drag slider to change Red")
#define TXT_NUM_G_HELP			GetLocalString( 0, "目前顏色的綠色值 \n輸入數值來變更綠色值", "Green \nInput number to set Green")
#define TXT_SLIDER_G_HELP		GetLocalString( 0, "拖拉滑桿來變更綠色值", "Drag slider to change Green")
#define TXT_NUM_B_HELP			GetLocalString( 0, "目前顏色的藍色值 \n輸入數值來變更藍色值", "Blue \nInput number to set Blue")
#define TXT_SLIDER_B_HELP		GetLocalString( 0, "拖拉滑桿來變更藍色", "Drag slider to change Blue")
#define TXT_NUM_H_HELP			GetLocalString( 0, "目前顏色的色相 \n輸入數值來變更色相", "Hue \nInput number to set Hue")
#define TXT_SLIDER_H_HELP		GetLocalString( 0, "拖拉滑桿來變更色相", "Drag slider to change Hue")
#define TXT_NUM_S_HELP			GetLocalString( 0, "目前顏色的飽和度 \n輸入數值來變更飽和度", "Saturation \nInput number to set Saturation")
#define TXT_SLIDER_S_HELP		GetLocalString( 0, "拖拉滑桿來變更飽和度", "Drag slider to change Saturation")
#define TXT_NUM_V_HELP			GetLocalString( 0, "目前顏色的明度 \n輸入數值來變更明度", "Value \nInput number to set Value")
#define TXT_SLIDER_V_HELP		GetLocalString( 0, "拖拉滑桿來變更明度", "Drag slider to change Value")

#define TXT_R					GetLocalString( 1, "R", NULL)
#define TXT_G					GetLocalString( 1, "G", NULL)
#define TXT_B					GetLocalString( 1, "B", NULL)
#define TXT_H					GetLocalString( 1, "H", NULL)
#define TXT_S					GetLocalString( 1, "S", NULL)
#define TXT_V					GetLocalString( 1, "V", NULL)

#define TXT_NAME				GetLocalString( 1, "Color Picker", NULL)

#define TXT_REQUESTER			GetLocalString( 1, "Color Picker", NULL)

#define TXT_ERROR				GetLocalString( 0, "無法開啟 !", "Error!")

#define TXT_INFO				GetLocalString( 0, "開發者: \nYanun Yang\n\n 版本: \n1.0.2020\n \n聯絡Email: \na1256212562@gmail.com \n\n\n歡迎轉載並註明出處為Yanun Yang \n", "Developer\nYanun Yang\n\n version: \n2020.1.0\n \nEmail: \na1256212562@gmail.com \n\n\n*Please cite the source if reprint* \n")

#define TXT_INFO_CLOSE			GetLocalString( 0, "關閉", "Close")

#define TXT_INFO_MORE			GetLocalString( 0, "版本更新資訊", "Update Infomation")

#define TXT_INFO_T1				GetLocalString( 0, "版本更新請至FaceBook討論區查看", "Please check the update in our FacebBook forum")

#define TXT_INFO_T2				GetLocalString( 1, "https://www.facebook.com/groups/1092485080952166/", NULL)

#endif