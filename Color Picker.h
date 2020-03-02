
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

/*************** GUI�ե�ID ******************/

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

/***************** �y���� ********************/

#define TXT_COLORWHEEL_HELP		GetLocalString( 0, "�C�⪺���", "Hue")
#define TXT_COLORSQUARE_HELP	GetLocalString( 0, "�C�⪺���פι��M��", "Saturation & Value")
#define TXT_COLORSWITCH_HELP	GetLocalString( 0, "�洫�e����έI���� \n�k���I���N�e����έI����]���P��", "Switch A/B pen \nRight click: set A color to B pen")
#define TXT_COLORABBTN_HELP		GetLocalString( 0, "�e����έI���� \n�I���i�l��Τ����e����έI����", "A/B pen \nClick to pick color or change current pen")
#define TXT_BLACKWHITEBTN_HELP	GetLocalString( 0, "�N�e����έI����]�w���¥�", "Set A/B pen black & white")
#define TXT_GEAR_HELP			GetLocalString( 0, "��L��T", "Infomation")
#define TXT_POPUP_HELP			GetLocalString( 0, "�����ܼҦ�", "Change mode")
#define TXT_POPUP_0				GetLocalString( 0, " RGB+HSV ", "RGB+HSV")
#define TXT_POPUP_1				GetLocalString( 0, " �L ", "None")
#define TXT_COLORGET			GetLocalString( 0, "�P�B", "Sync")
#define TXT_COLORGET_HELP		GetLocalString( 0, "�Y���^��TVPaint�t���C��A�æP�B��s \n�}�ҮɡA�b�C���X���A�U \n������|����A�����v�T�\��ϥ� \n\n*�n�ϥΦⲼ�B�C��O���A�ж}�Ҧ��\�� \n*�w�]������", "Get TVPaint's A/B pen in realtime \nThere's some bug with low saturation \nBut the function still work perfectly")

#define TXT_NUM_R_HELP			GetLocalString( 0, "�ثe�C�⪺����� \n��J�ƭȨ��ܧ�����", "Red \nInput number to set Red")
#define TXT_SLIDER_R_HELP		GetLocalString( 0, "��ԷƱ���ܧ�����", "Drag slider to change Red")
#define TXT_NUM_G_HELP			GetLocalString( 0, "�ثe�C�⪺���� \n��J�ƭȨ��ܧ����", "Green \nInput number to set Green")
#define TXT_SLIDER_G_HELP		GetLocalString( 0, "��ԷƱ���ܧ����", "Drag slider to change Green")
#define TXT_NUM_B_HELP			GetLocalString( 0, "�ثe�C�⪺�Ŧ�� \n��J�ƭȨ��ܧ��Ŧ��", "Blue \nInput number to set Blue")
#define TXT_SLIDER_B_HELP		GetLocalString( 0, "��ԷƱ���ܧ��Ŧ�", "Drag slider to change Blue")
#define TXT_NUM_H_HELP			GetLocalString( 0, "�ثe�C�⪺��� \n��J�ƭȨ��ܧ���", "Hue \nInput number to set Hue")
#define TXT_SLIDER_H_HELP		GetLocalString( 0, "��ԷƱ���ܧ���", "Drag slider to change Hue")
#define TXT_NUM_S_HELP			GetLocalString( 0, "�ثe�C�⪺���M�� \n��J�ƭȨ��ܧ󹡩M��", "Saturation \nInput number to set Saturation")
#define TXT_SLIDER_S_HELP		GetLocalString( 0, "��ԷƱ���ܧ󹡩M��", "Drag slider to change Saturation")
#define TXT_NUM_V_HELP			GetLocalString( 0, "�ثe�C�⪺���� \n��J�ƭȨ��ܧ����", "Value \nInput number to set Value")
#define TXT_SLIDER_V_HELP		GetLocalString( 0, "��ԷƱ���ܧ����", "Drag slider to change Value")

#define TXT_R					GetLocalString( 1, "R", NULL)
#define TXT_G					GetLocalString( 1, "G", NULL)
#define TXT_B					GetLocalString( 1, "B", NULL)
#define TXT_H					GetLocalString( 1, "H", NULL)
#define TXT_S					GetLocalString( 1, "S", NULL)
#define TXT_V					GetLocalString( 1, "V", NULL)

#define TXT_NAME				GetLocalString( 1, "Color Picker", NULL)

#define TXT_REQUESTER			GetLocalString( 1, "Color Picker", NULL)

#define TXT_ERROR				GetLocalString( 0, "�L�k�}�� !", "Error!")

#define TXT_INFO				GetLocalString( 0, "�}�o��: \nYanun Yang\n\n ����: \n1.0.2020\n \n�p��Email: \na1256212562@gmail.com \n\n\n�w������õ����X�B��Yanun Yang \n", "Developer\nYanun Yang\n\n version: \n2020.1.0\n \nEmail: \na1256212562@gmail.com \n\n\n*Please cite the source if reprint* \n")

#define TXT_INFO_CLOSE			GetLocalString( 0, "����", "Close")

#define TXT_INFO_MORE			GetLocalString( 0, "������s��T", "Update Infomation")

#define TXT_INFO_T1				GetLocalString( 0, "������s�Ц�FaceBook�Q�װϬd��", "Please check the update in our FacebBook forum")

#define TXT_INFO_T2				GetLocalString( 1, "https://www.facebook.com/groups/1092485080952166/", NULL)

#endif