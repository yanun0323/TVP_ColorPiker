/**************************************************************************************/
/**************************************************************************************/
/*                                                                                    */
/*                                     COLOR PICKER                                   */
/*                                                                                    */
/**************************************************************************************/
/**************************************************************************************/


#include "Color Picker.h"



////////////////////////////////////////////////////////////////////////////////////////


typedef struct RGBPixel
{
	float r;
	float g;
	float b;
	int a;
} RGBPixel;

typedef struct HSVPixel
{
	float h;
	float s;
	float v;
	int a;
} HSVPixel;


static RGBPixel mRGB;
static RGBPixel mRGB2;
static RGBPixel mRGBtmp;

static RGBPixel mRGBreq;

static HSVPixel mHSV;
static HSVPixel mHSV2;
static HSVPixel mHSVtmp;

static PIPixel mPIP;
static PIPixel mPIPtmp;

/**************************************************************************************/
// all data needed by the plugin


static struct Data
{

	DWORD       mReq;				// Identification of the requester.  (=0 closed, !=0 requester ID)

	DWORD       mReqPick;			// Identification of the requester.  (=0 closed, !=0 requester ID)

	DWORD       mReqOther;			// Identification of the requester.  (=0 closed, !=0 requester ID)

	void*       mLocalFile;			// file containing localized messages

	char		mChar[256];

	int 		mRequestX;

	int 		mRequestY;				//#5

	float		mHueClickX;

	float		mHueClickY;

	float		mHueBtnX;

	float		mHueBtnY;

	float		mSquaClickX;		//#10

	float		mSquaClickY;

	float		mSquaBtnX;

	float		mSquaBtnY;

	PIBlock*	mHueBlock;

	PIBlock*	mSquareBlock;		//#15

	int			mSlideHueBtn;

	int			mIsHueClick;

	int			mSlideSquaBtn;

	int			mIsSquaClick;

	int			mIsColorSwitchOn;	//#20

	int			mIsRefresh;			/*單純更新GUI*/

	PIBlock*	mBlockR;

	PIBlock*	mBlockG;

	PIBlock*	mBlockB;

	PIBlock*	mBlockH;			//#25

	PIBlock*	mBlockS;

	PIBlock*	mBlockV;

	PIBlock*	mABColorBlock;

	PIBlock*	mBlackWhiteBlock;

	PIBlock*	mCSwitchBlock;		//#30

	int			mABPenSelected;		/* 0 = a; 1 = b */

	int			mIsPickMode;		/*判斷是否吸色*/

	int			mIsPickRelease;		/*判斷吸色是否釋放*/

	int			mBlackWhiteRefresh;	/*黑白顏色指定*/

	int			mIsSliderHide;		/*滑桿是否隱藏*/

	int			mInputColorNum;		/*判斷是否輸入顏色，輸入哪個顏色 0=無輸入*/

	PIBlock*	mPickBlock;			/*吸取顏色浮動GUI*/

	PIBlock*	mGearBlock;			/*齒輪按鈕*/

	int			mIsColorGet;		/*即時更新顏色是否開啟 0=關閉 1=開啟*/

	int			mModeSelect;		/*下半面板模式 0~N*/

	int			mIsGearSelect;		/*齒輪是否選取 0=無選取 1=選取*/

	int			mThemeSelect;		/*主題模式 0=無 1=淺色模式 2=深色模式*/

	char		mRoom[256];			/*目前工作區*/

	int			mRequestWidth;		/*視窗寬度暫存*/

	int			mRequestHeight;		/*視窗寬度暫存*/

	int			mLanguage;			/*語言 0=中文語系 1=其他語言*/

}
Data =
{
	0,		//mReq
	0,		//mReqPick
	0,		//mReqOther
	NULL,	/*mLocalFile*/
	0,		//mChar
	0,		//mX
	0,		//mY
	0,		//mHueClickX
	0,		//mHueClickY
	150,	//mHueBtnX
	150,	//mHueBtnY
	0,		//mSquaClickX
	0,		//mSquaClickY
	0,		//mSquaBtnX
	0,		//mSquaBtnY
	NULL,	/*mHueBlock*/
	NULL,	/*mSquareBlock*/
	0,		//mSlideHueBtn
	0,		//mIsHueClick
	0,		//mSlideSquaBtn
	0,		//mIsSquaClick
	0,		//mIsColorSwitchOn
	0,		//mIsSliderClick
	NULL,	/*mBlockR*/
	NULL,	/*mBlockG*/
	NULL,	/*mBlockB*/
	NULL,	/*mBlockH*/
	NULL,	/*mBlockS*/
	NULL,	/*mBlockV*/
	NULL,	/*mAColorBlock*/
	NULL,	/*mBColorBlock*/
	NULL,	/*mCSwitchBlock*/
	0,		//mABPenSelected
	0,		//mIsPickClick
	0,		//mPickCoord
	0,		//mBlackWhiteRefresh
	0,		//mTabSelected
	0,		//mRefreshBtn
	NULL,	/*mPickBlock*/
	NULL,	/*mGearBlock*/
	1,		//mIsRatioLock
	0,		//mModeSelect;
	0,		//mIsGearSelect
	0,		//mThemeSelect
	0,		//mRoom
	0,		//mRequestWidth;
	0,		//mRequestHeight;
	0,		//mLanguage
};


static struct Gui
{
	/***************** GUI大小 ********************/

	float	GUI_RATIO;			/*GUI比例*/
	int		REQUESTER_W;
	int		REQUESTER_H;
	int		REQUESTER_W_MIN;
	int		REQUESTER_H_MIN;
	int		GUI_W;
	int		GUI_H;

	/***************** 色相環 ********************/
	int		CW_SIZE;			//色相環大小
	float	CW_RADIUS;			//色相環半徑、中心點
	int		CW_GAP;				//色相環寬度
	int		CW_X;				//色相環起始X	
	int		CW_Y;				//色相環起始Y
	int		CW_BTN_SIZE;		//色相環按鈕
	float	CW_BTN_BLOD;
	float	CW_BTN_RADIUS;

	/**************** 色相方塊 *******************/
	int		CS_SIZE;			//色相方塊實際大小
	int		CS_X;				//色相方塊起始X
	int		CS_Y;				//色相方塊起始Y
	int		CS_PXSIZE;			//色相方塊像素大小
	int		CS_PXGAP;			//色相方塊像素 跟 實際外框的間距
	int		CS_BTN_SIZE;		//色相方塊按鈕
	float	CS_BTN_BLOD;
	float	CS_BTN_GAP;

								/************** 顏色區塊按鈕 ****************/
	int		C_A_SIZE;			//顏色A 區塊按鈕大小
	float	C_A_RADIUS;			//顏色A 半徑
	int		C_A_X;				//顏色A 區塊按鈕起始X
	int		C_A_Y;				//顏色A 區塊按鈕起始Y
	int		C_B_SIZE;			//顏色B 區塊按鈕大小
	float	C_B_RADIUS;			//顏色B 半徑

								/************** 顏色交換按鈕 ****************/
	int		CSWITCH_SIZE;		//顏色交換按鈕大小
	int		CSWITCH_X;			//顏色交換按鈕起始X 
	int		CSWITCH_Y;			//顏色交換按鈕起始Y 	

								/************** 顏色黑白按鈕 ****************/
	int		BLACKWHITE_SIZE;	//顏色黑白按鈕大小
	int		BLACKWHITE_X;		//顏色黑白按鈕起始X 
	int		BLACKWHITE_Y;		//顏色黑白按鈕起始Y 	

								/************** 顏色數值區塊 ****************/
	int SLIDER_GAP_X;			//顏色數值滑桿左右間距
	int SLIDER_GAP_Y;			//顏色數值滑桿上下間距
	int SLIDER_Y;				//顏色數值區塊起始Y
	int SLIDER_H;				//顏色數值區塊高度

								/************** 顏色數值字母 ****************/
	int SLIDER_NAME_W;			//顏色數值字母大小
	int SLIDER_NAME_X;			//顏色數值字母起始X

								/************** 顏色數值數字 ****************/
	int SLIDER_NUM_W;			//顏色數值數字大小
	int SLIDER_NUM_X;			//顏色數值數字起始X

								/************** 顏色數值滑桿 ****************/
	int SLIDER_W;				//顏色數值滑桿大小
	int SLIDER_X;				//顏色數值滑桿起始X
	int SLIDER_OFFSET;			//顏色數值滑桿起始Y偏移

								/************** 顏色吸取 GUI ****************/
	int PICK_W;					//顏色吸取GUI寬
	int PICK_H;					//顏色吸取GUI高
	int PICK_X;					//顏色吸取GUI起始X
	int PICK_Y;					//顏色吸取GUI起始Y

								/**************   齒輪按鈕   ****************/
	int OTHER_SIZE;				//齒輪按鈕大小
	int OTHER_X;				//齒輪按鈕起始X
	int OTHER_Y;				//齒輪按鈕起始Y

								/************** 鎖定比例按鈕 ****************/
	int LOCKRATIO_SIZE;			//鎖定比例按鈕大小
	int LOCKRATIO_X;			//鎖定比例按鈕起始X
	int LOCKRATIO_Y;			//鎖定比例按鈕起始Y

								/************** 模式彈出按鈕 ****************/
	int POPUP_W;				//模式彈按鈕寬
	int POPUP_H;				//模式彈按鈕高
	int POPUP_X;				//模式彈按鈕起始X
	int POPUP_Y;				//模式彈按鈕起始Y


	float	TEMP_;
}Gui;



/**************************************************************************************/
/**************************************************************************************/
/*****************************						***********************************/
/*****************************		自訂函數區		***********************************/
/*****************************						***********************************/
/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/


/**************************************************************************************/
/*重新計算GUI*/
int TVResizeGUI(PIFilter* iFilter, int width, int height) {
	/***************** GUI大小 ********************/

	Gui.GUI_RATIO = 3 / (float)7;

	Gui.REQUESTER_W_MIN = 180;

	float ratio = 5 / (float)7;

	if (Data.mIsSliderHide >> 1)
		Gui.REQUESTER_H_MIN = Gui.REQUESTER_W_MIN / Gui.GUI_RATIO;
	else
		Gui.REQUESTER_H_MIN = (!Data.mIsSliderHide) ? Gui.REQUESTER_W_MIN / Gui.GUI_RATIO : Gui.REQUESTER_W_MIN / ratio;

	int w = (width < Gui.REQUESTER_W_MIN) ? Gui.REQUESTER_W_MIN : width;
	int h = (height < Gui.REQUESTER_H_MIN) ? Gui.REQUESTER_H_MIN : height;


	char tmp[256];
	int tmpW, tmpH;

	//tmpW = h * ratio;
	//tmpH = w / ratio;
	//Data.mRequestWidth, Data.mRequestHeight
	tmpW = abs(w - Data.mRequestWidth);
	tmpH = abs(h - Data.mRequestHeight);

	//sprintf(tmp, "tv_infobar tmpW/H: %d %d Data.W/H: %d %d w/h: %d %d", tmpW, tmpH, Data.mRequestWidth, Data.mRequestHeight, w, h);
	//sprintf(tmp, "tmpW/H: %d %d Data.W/H: %d %d w/h: %d %d mw/mh: %d %d", tmpW, tmpH, Data.mRequestWidth, Data.mRequestHeight, width, height, w, h);



	if (Data.mIsSliderHide >> 1) {
		Gui.REQUESTER_W = Data.mRequestWidth;
		Gui.REQUESTER_H = Data.mRequestWidth / Gui.GUI_RATIO;
	}
	else if (Data.mIsSliderHide) {
		//if (width == Data.mRequestWidth && height == Data.mRequestHeight) {
		if (tmpW == 0 && tmpH == 0) {
			Gui.REQUESTER_W = w;
			Gui.REQUESTER_H = w / ratio;

			//TVSendCmd(iFilter, "tv_infobar A", NULL);
		}
		else if (tmpW >= tmpH) {
			Gui.REQUESTER_W = w;
			Gui.REQUESTER_H = w / ratio;
			//TVSendCmd(iFilter, "tv_infobar B", NULL);
		}
		else {
			Gui.REQUESTER_W = h * ratio;
			Gui.REQUESTER_H = h;
			//TVSendCmd(iFilter, "tv_infobar C", NULL);
		}
	}
	else {
		if (tmpW >= tmpH) {
			Gui.REQUESTER_W = w;
			Gui.REQUESTER_H = w / Gui.GUI_RATIO;
		}
		else {
			Gui.REQUESTER_W = h * Gui.GUI_RATIO;
			Gui.REQUESTER_H = h;
		}
	}
	Gui.GUI_W = Gui.REQUESTER_W;
	Gui.GUI_H = Gui.GUI_W / Gui.GUI_RATIO;
	
	if (Data.mIsSliderHide >> 1) Data.mIsSliderHide = 0;

	/***************** 色相環 ********************/
	//色相環大小
	Gui.CW_SIZE = Gui.GUI_W - 10;
	//色相環半徑、中心點
	Gui.CW_RADIUS = Gui.CW_SIZE / (float)2;
	//色相環寬度
	Gui.CW_GAP = floor(Gui.CW_SIZE / (float)8);
	//色相環起始XY	
	Gui.CW_X = floor((Gui.REQUESTER_W - Gui.CW_SIZE) / (float)2);
	Gui.CW_Y = floor(Gui.GUI_H / (float)6);
	//色相環按鈕
	Gui.CW_BTN_SIZE = Gui.CW_GAP;
	Gui.CW_BTN_BLOD = (float)1.2;
	Gui.CW_BTN_RADIUS = Gui.CW_RADIUS - Gui.CW_GAP + (Gui.CW_BTN_SIZE / (float)2);

	/**************** 色相方塊 *******************/
	//色相方塊實際大小
	Gui.CS_SIZE = floor(Gui.GUI_W / (float)2);
	//色相方塊起始XY
	Gui.CS_X = floor((Gui.CW_SIZE - Gui.CS_SIZE) / (float)2 + Gui.CW_X);
	Gui.CS_Y = floor((Gui.CW_SIZE - Gui.CS_SIZE) / (float)2 + Gui.CW_Y);
	//色相方塊像素大小
	Gui.CS_PXSIZE = floor((Gui.GUI_W - Gui.CW_GAP) / (float)2);
	//色相方塊像素 跟 實際外框的間距
	Gui.CS_PXGAP = floor((Gui.CS_SIZE - Gui.CS_PXSIZE) / (float)2);
	//色相方塊按鈕
	Gui.CS_BTN_SIZE = floor(Gui.CS_SIZE / (float)8);
	Gui.CS_BTN_BLOD = (float)1.1;
	//色相方塊按鈕 與 像素邊的間距
	Gui.CS_BTN_GAP = (float)5;

	/************** 顏色區塊按鈕 ****************/
	//顏色A 區塊按鈕大小
	Gui.C_A_SIZE = floor(Gui.GUI_H / (float)7);
	//顏色A 半徑
	Gui.C_A_RADIUS = Gui.C_A_SIZE / (float)2;
	//顏色A 區塊按鈕起始XY
	Gui.C_A_X = Gui.CW_X;
	Gui.C_A_Y = floor(Gui.GUI_H * 3 / (float)140);
	//顏色B 區塊按鈕大小
	Gui.C_B_SIZE = floor(Gui.C_A_SIZE / (float)2);
	//顏色B 半徑
	Gui.C_B_RADIUS = Gui.C_B_SIZE / (float)2;

	/************** 顏色交換按鈕 ****************/
	//顏色交換按鈕大小
	Gui.CSWITCH_SIZE = floor((Gui.C_A_SIZE - Gui.C_B_SIZE) * 3 / (float)5);
	//顏色交換按鈕起始XY 
	Gui.CSWITCH_X = floor(Gui.C_A_X + Gui.C_A_SIZE - Gui.GUI_W / (float)18);
	Gui.CSWITCH_Y = floor(Gui.C_A_Y / 2 + Gui.CSWITCH_SIZE * 4 / (float)10);

	/************** 顏色黑白按鈕 ****************/
	//顏色黑白按鈕大小
	Gui.BLACKWHITE_SIZE = floor((Gui.C_A_SIZE - Gui.C_B_SIZE) * 2 / (float)5);
	//顏色黑白按鈕起始XY
	Gui.BLACKWHITE_X = floor(Gui.CW_X + Gui.C_A_SIZE / (float)5);
	Gui.BLACKWHITE_Y = floor(Gui.GUI_H / (float)8 + Gui.CSWITCH_Y);

	/************** 顏色數值區塊 ****************/
	//顏色數值區塊高度
	Gui.SLIDER_H = 17;
	//顏色數值區塊間距
	Gui.SLIDER_GAP_X = floor(Gui.GUI_W / (float)40);
	Gui.SLIDER_GAP_Y = floor((Gui.GUI_H - Gui.CW_SIZE - Gui.CW_Y - (6 * Gui.SLIDER_H)) / (float)9);
	//顏色數值區塊起始X
	Gui.SLIDER_Y = Gui.CW_Y + Gui.CW_SIZE + 2 * Gui.SLIDER_GAP_Y;

	/************** 顏色數值字母 ****************/
	Gui.SLIDER_NAME_W = 10;
	Gui.SLIDER_NAME_X = Gui.CW_X + Gui.SLIDER_GAP_X;

	/************** 顏色數值數字 ****************/
	Gui.SLIDER_NUM_W = 30;
	Gui.SLIDER_NUM_X = Gui.SLIDER_NAME_X + Gui.SLIDER_NAME_W + Gui.SLIDER_GAP_X;

	/************** 顏色數值滑桿 ****************/
	Gui.SLIDER_W = Gui.CW_SIZE - (Gui.SLIDER_NAME_W + Gui.SLIDER_NUM_W + 4 * Gui.SLIDER_GAP_X);
	Gui.SLIDER_X = Gui.SLIDER_NUM_X + Gui.SLIDER_NUM_W + Gui.SLIDER_GAP_X;
	Gui.SLIDER_OFFSET = 3;

	/************** 顏色吸取 GUI ****************/
	Gui.PICK_W = 100;
	Gui.PICK_H = 50;
	Gui.PICK_X = 20;
	Gui.PICK_Y = 15;

	/************** 選項齒輪按鈕 ****************/
	Gui.OTHER_SIZE = 20;	//floorf(Gui.GUI_W / 10);
	Gui.OTHER_X = Gui.CW_X + Gui.CW_SIZE - Gui.OTHER_SIZE - 2;
	Gui.OTHER_Y = floorf(Gui.GUI_H * 3 / (float)100);

	/************** 鎖定比例按鈕 ****************/
	Gui.LOCKRATIO_SIZE = 20;
	//Gui.LOCKRATIO_X = Gui.CW_X + Gui.CW_SIZE - Gui.LOCKRATIO_SIZE - 2;
	Gui.LOCKRATIO_X	= Gui.OTHER_X - Gui.LOCKRATIO_SIZE - Gui.GUI_W * 1 / (float)20;
	Gui.LOCKRATIO_Y = Gui.OTHER_Y + 2;


	/************** 模式彈出按鈕 ****************/
	Gui.POPUP_W = Gui.CW_SIZE / (float)2 - 2;
	Gui.POPUP_H = 20;
	Gui.POPUP_X = Gui.CW_X + Gui.CW_SIZE / (float)2;
	Gui.POPUP_Y = Gui.OTHER_Y + Gui.OTHER_SIZE + (Gui.CW_Y - Gui.OTHER_Y - Gui.OTHER_SIZE - Gui.POPUP_H) / (float)2;

	return 1;
}

/**************************************************************************************/
//  Localisation

static char* FAR PASCAL
GetLocalString(int i, char* iDefault1, char* iDefault2)
{
	if (!Data.mLocalFile)
		return  iDefault1; 
	else if (Data.mLocalFile) 
		return  (i) ? iDefault1 : iDefault2;
}

/**************************************************************************************/
/* RGB、HSV、HSL顏色轉換 */

//RGB -> HSV
HSVPixel
csRGB2HSV(PIFilter *iFilter, RGBPixel rgb)
{ // lazy man's HUE (widely accepted but very computerish HUE)
	HSVPixel hsv;

	float var_R, var_G, var_B;
	float del_R, del_G, del_B;
	float var_Min, var_Max, del_Max;

	var_R = ((float)rgb.r / 255);                     //RGB values = 0 ?255
	var_G = ((float)rgb.g / 255);
	var_B = ((float)rgb.b / 255);

	var_Min = min(var_R, min(var_G, var_B));    //Min. value of RGB
	var_Max = max(var_R, max(var_G, var_B));    //Max. value of RGB
	del_Max = var_Max - var_Min;             //Delta RGB value

	hsv.v = var_Max;

	if (del_Max == 0)                     //This is a gray, no chroma...
	{
		hsv.h = 0;                                //HSV results = 0 ?1
		hsv.s = 0;
	}
	else                                    //Chromatic data...
	{
		hsv.s = del_Max / var_Max;

		del_R = (((var_Max - var_R) / (float)6) + (del_Max / (float)2)) / del_Max;
		del_G = (((var_Max - var_G) / (float)6) + (del_Max / (float)2)) / del_Max;
		del_B = (((var_Max - var_B) / (float)6) + (del_Max / (float)2)) / del_Max;

		if (var_R == var_Max)
			hsv.h = del_B - del_G;
		else if (var_G == var_Max)
			hsv.h = (float)(1 / (float)3) + del_R - del_B;
		else if (var_B == var_Max)
			hsv.h = (float)(2 / (float)3) + del_G - del_R;
		if (hsv.h < 0)
			hsv.h += 1;
		if (hsv.h > 1)
			hsv.h -= 1;
		hsv.h = 1 - hsv.h;
	}

	hsv.a = rgb.a;
	hsv.h = 1 - hsv.h;

	if (hsv.h == 1) hsv.h = 0;

	return hsv;
}


HSVPixel
csRGB2HSV2(PIFilter *iFilter, RGBPixel rgb, HSVPixel tmp)
{ // lazy man's HUE (widely accepted but very computerish HUE)
	HSVPixel hsv;

	float var_R, var_G, var_B;
	float del_R, del_G, del_B;
	float var_Min, var_Max, del_Max;

	var_R = ((float)rgb.r / 255);                     //RGB values = 0 ?255
	var_G = ((float)rgb.g / 255);
	var_B = ((float)rgb.b / 255);

	var_Min = min(var_R, min(var_G, var_B));    //Min. value of RGB
	var_Max = max(var_R, max(var_G, var_B));    //Max. value of RGB
	del_Max = var_Max - var_Min;             //Delta RGB value

	hsv.v = var_Max;

	if (del_Max == 0)                     //This is a gray, no chroma...
	{
		hsv.h = 0;                                //HSV results = 0 ?1
		hsv.s = 0;
	}
	else                                    //Chromatic data...
	{
		hsv.s = del_Max / var_Max;

		del_R = (((var_Max - var_R) / (float)6) + (del_Max / (float)2)) / del_Max;
		del_G = (((var_Max - var_G) / (float)6) + (del_Max / (float)2)) / del_Max;
		del_B = (((var_Max - var_B) / (float)6) + (del_Max / (float)2)) / del_Max;

		if (var_R == var_Max)
			hsv.h = del_B - del_G;
		else if (var_G == var_Max)
			hsv.h = (1 / (float)3) + del_R - del_B;
		else if (var_B == var_Max)
			hsv.h = (2 / (float)3) + del_G - del_R;
		if (hsv.h < 0)
			hsv.h += 1;
		if (hsv.h > 1)
			hsv.h -= 1;
		hsv.h = 1 - hsv.h;
	}

	hsv.a = rgb.a;
	hsv.h = 1 - hsv.h;

	if (hsv.h == tmp.h) hsv.h = tmp.h;
	if (hsv.h == 1) hsv.h = 0;

	return hsv;
}

//HSV -> RGB
RGBPixel
csHSV2RGB(PIFilter *iFilter, HSVPixel hsv)
{// lazy man's HUE (widely accepted but very computerish HUE)
	float var_r, var_g, var_b, var_h, var_1, var_2, var_3;
	int var_i;

	RGBPixel rgb;


	if (hsv.s == 0)                       //HSV values = 0 ?1
	{
		rgb.r = min((float)1, hsv.v) * (float)255;
		rgb.g = min((float)1, hsv.v) * (float)255;
		rgb.b = min((float)1, hsv.v) * (float)255;
		rgb.a = hsv.a;
	}
	else
	{
		hsv.h = fmod(hsv.h, (float)1);
		hsv.h = hsv.h < 0 ? hsv.h + 1 : hsv.h;
		hsv.s = min((float)1, hsv.s);
		var_h = (hsv.h) * (float)6;
		if (var_h == 6)
			var_h = 0;      //H must be < 1
		var_i = (int)floorf(var_h);             //Or ... var_i = floorf( var_h )
		var_1 = hsv.v * (1 - hsv.s);
		var_2 = hsv.v * (1 - hsv.s * (var_h - var_i));
		var_3 = hsv.v * (1 - hsv.s * (1 - (var_h - var_i)));

		if (var_i == 0) { var_r = hsv.v; var_g = var_3; var_b = var_1; }
		else if (var_i == 1) { var_r = var_2; var_g = hsv.v; var_b = var_1; }
		else if (var_i == 2) { var_r = var_1; var_g = hsv.v; var_b = var_3; }
		else if (var_i == 3) { var_r = var_1; var_g = var_2; var_b = hsv.v; }
		else if (var_i == 4) { var_r = var_3; var_g = var_1; var_b = hsv.v; }
		else { var_r = hsv.v; var_g = var_1; var_b = var_2; }

		rgb.r = (var_r > (float)1 ? 1 : var_r) * (float)255;
		rgb.g = (var_g > (float)1 ? 1 : var_g) * (float)255;
		rgb.b = (var_b > (float)1 ? 1 : var_b) * (float)255;
		rgb.a = hsv.a;
	}

	return rgb;
}

//PIP-> RGB
RGBPixel
csPIP2RGB(PIFilter *iFilter, PIPixel pip)
{
	RGBPixel rgb;

	int  r = iFilter->PlaneOffsetRed;
	int  g = iFilter->PlaneOffsetGreen;
	int  b = iFilter->PlaneOffsetBlue;
	int  a = iFilter->PlaneOffsetAlpha;

	rgb.r = pip.c[r];
	rgb.g = pip.c[g];
	rgb.b = pip.c[b];
	rgb.a = pip.c[a];

	rgb.r = rgb.a != 0 ? ((float)rgb.r * (float)255 / (float)rgb.a) : 0;
	rgb.g = rgb.a != 0 ? ((float)rgb.g * (float)255 / (float)rgb.a) : 0;
	rgb.b = rgb.a != 0 ? ((float)rgb.b * (float)255 / (float)rgb.a) : 0;

	return rgb;
}

//PIP-> HSV
HSVPixel
csPIP2HSV(PIFilter *iFilter, PIPixel pip)
{
	HSVPixel hsv;
	hsv = csRGB2HSV(iFilter, csPIP2RGB(iFilter, pip));

	return hsv;
}

//HEX-> RGB
RGBPixel
csHEX2RGB(PIFilter *iFilter, DWORD hex)
{
	RGBPixel rgb;

	rgb.r = ((hex >> 16) & 0xFF);
	rgb.g = ((hex >> 8) & 0xFF);
	rgb.b = ((hex) & 0xFF);

	rgb.a = 255;

	return rgb;
}



/**************************************************************************************/
/* 取得顏色 */

//取得A顏色RGB
RGBPixel
GetAPenRGB(PIFilter* iFilter) {
	RGBPixel rgb;
	char result[256];
	int r, g, b, a;

	TVSendCmd(iFilter, "tv_GetAPen", result);
	sscanf(result, "%d %d %d %d", &r, &g, &b, &a);
	rgb.r = r;
	rgb.g = g;
	rgb.b = b;
	rgb.a = 255;

	return rgb;
}


//取得A顏色HSV
HSVPixel
GetAPenHSV(PIFilter* iFilter) {
	char tmp[256];
	RGBPixel rgb;
	int r, g, b, a;
	HSVPixel hsv;

	TVSendCmd(iFilter, "tv_GetAPen", tmp);
	sscanf(tmp, "%d %d %d %d", &r, &g, &b, &a);
	rgb.r = r;
	rgb.g = g;
	rgb.b = b;
	rgb.a = 255;

	hsv = csRGB2HSV(iFilter, rgb);

	if (hsv.h == 1)	hsv.s = 0;
	if (hsv.s == 0 || hsv.v == 0)	hsv.h = mHSV.h;
	if (hsv.v == 0)	hsv.s = mHSV.s;


	return hsv;
}

//取得B顏色RGB
RGBPixel
GetBPenRGB(PIFilter* iFilter) {
	RGBPixel rgb;
	char result[256];
	int r, g, b, a;

	TVSendCmd(iFilter, "tv_GetBPen", result);
	sscanf(result, "%d %d %d %d", &r, &g, &b, &a);
	rgb.r = r;
	rgb.g = g;
	rgb.b = b;
	rgb.a = 255;

	return rgb;
}

//取得B顏色HSV
HSVPixel
GetBPenHSV(PIFilter* iFilter) {
	char tmp[256];
	HSVPixel hsv;
	int r, g, b, a;

	TVSendCmd(iFilter, "tv_GetBPen", tmp);
	sscanf(tmp, "%d %d %d %d", &r, &g, &b, &a);
	hsv = csRGB2HSV(iFilter, mRGB);

	if (hsv.s == 0 || hsv.v == 0)	hsv.h = mHSV.h;
	if (hsv.v == 0)	hsv.s = mHSV.s;


	return hsv;
}

// 取得像素色RGB
RGBPixel
GetPixelRGB(PIFilter* iFilter, int x, int y) {
	char tmp[256];
	RGBPixel rgb;

	TVSendCmd(iFilter, "tv_GetPixelDisplay", tmp);
	sscanf(tmp, "%f %f %f %d", &rgb.r, &rgb.g, &rgb.b, &rgb.a);

	if (rgb.a == 0) {
		rgb.r = (float)255;
		rgb.g = (float)255;
		rgb.b = (float)255;
	}

	rgb.a = 255;

	return rgb;
}


/**************************************************************************************/
/* 設定顏色 */

/*設定A顏色*/
void FAR PASCAL
TVSetAPen(PIFilter* iFilter, RGBPixel rgb)
{
	char  cmd[256];
	int   r, g, b;

	r = (int)floorf(rgb.r);
	g = (int)floorf(rgb.g);
	b = (int)floorf(rgb.b);

	mRGB = rgb;

	sprintf(cmd, "tv_SetAPen %d %d %d", r, g, b);
	TVSendCmd(iFilter, cmd, NULL);
}

/*設定B顏色*/
void FAR PASCAL
TVSetBPen(PIFilter* iFilter, RGBPixel rgb)
{
	char  cmd[256];
	int   r, g, b;

	r = (int)floorf(rgb.r);
	g = (int)floorf(rgb.g);
	b = (int)floorf(rgb.b);

	mRGB2 = rgb;

	sprintf(cmd, "tv_SetBPen %d %d %d", r, g, b);
	TVSendCmd(iFilter, cmd, NULL);
}


//交換AB RGB顏色
void
SwitchABPen(PIFilter* iFilter) {

	mRGBtmp = mRGB;
	mRGB = mRGB2;
	mRGB2 = mRGBtmp;

	mHSVtmp = mHSV;
	mHSV = mHSV2;
	mHSV2 = mHSVtmp;

	TVSetAPen(iFilter, mRGB);
	TVSetBPen(iFilter, mRGB2);
}


/**************************************************************************************/
/* 顏色及座標轉換 */

//RGB -> Hue座標
void
csHueBtn(PIFilter* iFilter) {
	float drgree = (mHSV.h * 360);
	float val = csPI / (float)180;

	Data.mHueBtnX = (Gui.CW_SIZE / (float)2) + Gui.CW_BTN_RADIUS * cos((drgree - 90)*val);
	Data.mHueBtnY = (Gui.CW_SIZE / (float)2) + Gui.CW_BTN_RADIUS * sin((drgree - 90)*val);
}

//RGB -> Suare座標
void
csSquaBtn(PIFilter* iFilter) {
	Data.mSquaBtnX = (float)Gui.CS_PXGAP + mHSV.s * (float)Gui.CS_PXSIZE;
	Data.mSquaBtnY = (float)Gui.CS_PXGAP + (1 - mHSV.v) * (float)Gui.CS_PXSIZE;
}



/**************************************************************************************/
/* 判斷 */


//判斷色相環是否可更新
int IsHueRefresh(PIFilter* iFilter) {
	HSVPixel hsv = GetAPenHSV(iFilter);
	if (hsv.h == mHSV.h)
		return 1;
	else
		return 0;
}

//判斷色相方塊是否可更新
int IsSquareRefresh(PIFilter* iFilter) {
	RGBPixel rgb = GetAPenRGB(iFilter);
	if (rgb.r == mRGB.r && rgb.g == mRGB.g && rgb.b == mRGB.b)
		return 1;
	else
		return 0;
}

//判斷角度(r = 中心點 , x & y = 目前點座標)
float getDegreeHue(PIFilter* iFilter, float r, float x, float y)
{
	int degree = floorf((atan2(y - r, x - r) * 180 / atan2((float)0, (float)-1)) + 180);
	if (degree > 360)
		degree = degree - 360;
	if (degree < 0)
		degree = degree + 360;
	return degree;
}

/**************************************************************************************/
/* 色相環繪製與刷新 */


/* 色相環刷新程式碼 */
void
HueBlock(PIFilter *iFilter, PIBlock *image, RGBPixel rgb) {


	float Rx, Ry, tmp, tmp2;
	int radius = Gui.CW_BTN_SIZE / 2;

	for (int x = 0; x<Gui.CW_SIZE; x++)
	{
		for (int y = 0; y < Gui.CW_SIZE; y++) {

			Rx = (float)x - Data.mHueBtnX;
			Ry = (float)y - Data.mHueBtnY;
			tmp = (Rx * Rx) + (Ry * Ry);
			tmp2 = (x - Gui.CW_RADIUS) * (x - Gui.CW_RADIUS) + (y - Gui.CW_RADIUS) * (y - Gui.CW_RADIUS);

			//色環按鈕白邊

			if (tmp < pow(radius, 2) && tmp >= (pow((radius - (float)0.22 * Gui.CW_BTN_BLOD), 2))) {
				image->Img[y][x].c[image->r] = (rgb.r + ((255 - rgb.r) * 7) / 10);
				image->Img[y][x].c[image->g] = (rgb.g + ((255 - rgb.g) * 7) / 10);
				image->Img[y][x].c[image->b] = (rgb.b + ((255 - rgb.b) * 7) / 10);
				image->Img[y][x].c[image->a] = 255;
			}
			else if (tmp < pow((radius - (float)0.22 * Gui.CW_BTN_BLOD), 2) && tmp >= pow((radius - (float)0.88 * Gui.CW_BTN_BLOD), 2)) {
				image->Img[y][x].c[image->r] = 255;
				image->Img[y][x].c[image->g] = 255;
				image->Img[y][x].c[image->b] = 255;
				image->Img[y][x].c[image->a] = 255;
			}
			else if (tmp < pow((radius - (float)0.88 * Gui.CW_BTN_BLOD), 2) && tmp >= pow((radius - Gui.CW_BTN_BLOD), 2)) {
				image->Img[y][x].c[image->r] = (rgb.r + ((255 - rgb.r) * 7) / 10);
				image->Img[y][x].c[image->g] = (rgb.g + ((255 - rgb.g) * 7) / 10);
				image->Img[y][x].c[image->b] = (rgb.b + ((255 - rgb.b) * 7) / 10);
				image->Img[y][x].c[image->a] = 255;
			}
			//色環按鈕內色
			else if (tmp < pow((radius - Gui.CW_BTN_BLOD), 2) && tmp >= 0) {
				image->Img[y][x].c[image->r] = rgb.r;
				image->Img[y][x].c[image->g] = rgb.g;
				image->Img[y][x].c[image->b] = rgb.b;
				image->Img[y][x].c[image->a] = 255;
			}



			else {
				//色環圈圈邊
				if (tmp2 < pow(Gui.CW_RADIUS, 2) && tmp2 >= pow((Gui.CW_RADIUS - (float)0.005 * Gui.CW_GAP), 2)) {
					float degree = ((getDegreeHue(iFilter, Gui.CW_RADIUS, x, y) - 90) / 360);
					HSVPixel hsv2 = { degree, 1, 1, 255 };
					RGBPixel rgb2 = csHSV2RGB(iFilter, hsv2);
					image->Img[y][x].c[image->r] = floorf(rgb2.r / 4);
					image->Img[y][x].c[image->g] = floorf(rgb2.g / 4);
					image->Img[y][x].c[image->b] = floorf(rgb2.b / 4);
					image->Img[y][x].c[image->a] = 64;
				}
				else if (tmp2 < pow(Gui.CW_RADIUS - (float)0.005, 2) && tmp2 >= pow((Gui.CW_RADIUS - (float)0.015 * Gui.CW_GAP), 2)) {
					float degree = ((getDegreeHue(iFilter, Gui.CW_RADIUS, x, y) - 90) / 360);
					HSVPixel hsv2 = { degree, 1, 1, 255 };
					RGBPixel rgb2 = csHSV2RGB(iFilter, hsv2);
					image->Img[y][x].c[image->r] = floorf(rgb2.r / 2);
					image->Img[y][x].c[image->g] = floorf(rgb2.g / 2);
					image->Img[y][x].c[image->b] = floorf(rgb2.b / 2);
					image->Img[y][x].c[image->a] = 128;
				}
				//色環圈圈內色

				else if (tmp2 < pow((Gui.CW_RADIUS - (float)0.015 * Gui.CW_GAP), 2) && tmp2 > pow((Gui.CW_RADIUS - (float)0.985 * Gui.CW_GAP), 2)) {
					float degree = ((getDegreeHue(iFilter, Gui.CW_RADIUS, x, y) - 90) / 360);
					HSVPixel hsv2 = { degree, 1, 1, 255 };
					RGBPixel rgb2 = csHSV2RGB(iFilter, hsv2);
					image->Img[y][x].c[image->r] = rgb2.r;
					image->Img[y][x].c[image->g] = rgb2.g;
					image->Img[y][x].c[image->b] = rgb2.b;
					image->Img[y][x].c[image->a] = 255;
				}
				//色環圈圈邊
				else if (tmp2 < pow((Gui.CW_RADIUS - (float)0.985 * Gui.CW_GAP), 2) && tmp2 > pow((Gui.CW_RADIUS - (float)0.995 * Gui.CW_GAP), 2)) {
					float degree = ((getDegreeHue(iFilter, Gui.CW_RADIUS, x, y) - 90) / 360);
					HSVPixel hsv2 = { degree, 1, 1, 255 };
					RGBPixel rgb2 = csHSV2RGB(iFilter, hsv2);
					image->Img[y][x].c[image->r] = floorf(rgb2.r / 2);
					image->Img[y][x].c[image->g] = floorf(rgb2.g / 2);
					image->Img[y][x].c[image->b] = floorf(rgb2.b / 2);
					image->Img[y][x].c[image->a] = 128;
				}
				else if (tmp2 <= pow((Gui.CW_RADIUS - (float)0.995* Gui.CW_GAP), 2) && tmp2 >= pow((Gui.CW_RADIUS - Gui.CW_GAP), 2)) {
					float degree = ((getDegreeHue(iFilter, Gui.CW_RADIUS, x, y) - 90) / 360);
					HSVPixel hsv2 = { degree, 1, 1, 255 };
					RGBPixel rgb2 = csHSV2RGB(iFilter, hsv2);
					image->Img[y][x].c[image->r] = floorf(rgb2.r / 4);
					image->Img[y][x].c[image->g] = floorf(rgb2.g / 4);
					image->Img[y][x].c[image->b] = floorf(rgb2.b / 4);
					image->Img[y][x].c[image->a] = 64;
				}


				//色環以外區域
				else {
					image->Img[y][x].c[image->r] = 0;
					image->Img[y][x].c[image->g] = 0;
					image->Img[y][x].c[image->b] = 0;
					image->Img[y][x].c[image->a] = 0;
				}

			}
		}
	}

}


//繪製色相環
PIBlock*
DrawHue(PIFilter *iFilter)
{
	PIBlock *image;
	HSVPixel hsv = { mHSV.h, 1, 1, 255 };
	RGBPixel rgb = csHSV2RGB(iFilter, hsv);

	/*分配色相環PIBlock*/
	image = TVAllocPIBlock(iFilter, Gui.CW_SIZE, Gui.CW_SIZE, 0);
	if (image == NULL)return NULL;

	csHueBtn(iFilter);

	HueBlock(iFilter, image, rgb);

	return image;
}

//被動刷新色相環
void
RefreshHue(PIFilter *iFilter, PIBlock *image)
{
	HSVPixel hsv = { mHSV.h, 1, 1, 255 };
	RGBPixel rgb = csHSV2RGB(iFilter, hsv);

	if (image == NULL)return NULL;

	csHueBtn(iFilter);

	HueBlock(iFilter, image, rgb);

}

//主動刷新色相環
void
SlideHue(PIFilter *iFilter, PIBlock *image) {
	mHSV.h = ((getDegreeHue(iFilter, Gui.CW_RADIUS, Data.mHueClickX, Data.mHueClickY) - 90) / (float)360);
	if (mHSV.h < 0)mHSV.h += 1;
	else if (mHSV.h > 1)mHSV.h -= 1;
	HSVPixel hsv = { mHSV.h , 1, 1, 255 };
	RGBPixel rgb = csHSV2RGB(iFilter, hsv);

	mRGB = csHSV2RGB(iFilter, mHSV);
	TVSetAPen(iFilter, mRGB);

	if (image == NULL)return NULL;

	csHueBtn(iFilter);

	HueBlock(iFilter, image, rgb);

}


/**************************************************************************************/
/* 色相方塊繪製與刷新 */

/* 色相方塊刷新程式碼 */
void
HueSquareBlock(PIFilter* iFilter, PIBlock* image, HSVPixel hsv) {
	RGBPixel rgb = { 0 };
	float gap = (Gui.CS_SIZE - Gui.CS_PXSIZE) / (float)2;
	float radius = Gui.CS_BTN_SIZE / (float)2;

	for (int x = 0; x < Gui.CS_SIZE; x++) {
		for (int y = 0; y < Gui.CS_SIZE; y++) {

			float Rx = (float)x - Data.mSquaBtnX;
			float Ry = (float)y - Data.mSquaBtnY;
			float tmp = (Rx * Rx) + (Ry * Ry);

			if (tmp < pow(radius, 2) && tmp > pow((radius - (float)0.22 * Gui.CS_BTN_BLOD), 2)) {
				image->Img[y][x].c[image->r] = (mRGB.r + ((255 - mRGB.r) * 7) / 10);
				image->Img[y][x].c[image->g] = (mRGB.g + ((255 - mRGB.g) * 7) / 10);
				image->Img[y][x].c[image->b] = (mRGB.b + ((255 - mRGB.b) * 7) / 10);
				image->Img[y][x].c[image->a] = 255;
			}
			else if (tmp <= pow((radius - (float)0.22 * Gui.CS_BTN_BLOD), 2) && tmp >= pow((radius - (float)0.88 * Gui.CS_BTN_BLOD), 2)) {
				image->Img[y][x].c[image->r] = 255;
				image->Img[y][x].c[image->g] = 255;
				image->Img[y][x].c[image->b] = 255;
				image->Img[y][x].c[image->a] = 255;
			}
			else if (tmp < pow((radius - (float)0.88 * Gui.CS_BTN_BLOD), 2) && tmp >= pow((radius - Gui.CS_BTN_BLOD), 2)) {
				image->Img[y][x].c[image->r] = (mRGB.r + ((255 - mRGB.r) * 7) / 10);
				image->Img[y][x].c[image->g] = (mRGB.g + ((255 - mRGB.g) * 7) / 10);
				image->Img[y][x].c[image->b] = (mRGB.b + ((255 - mRGB.b) * 7) / 10);
				image->Img[y][x].c[image->a] = 255;
			}
			else if (tmp < pow((radius - Gui.CS_BTN_BLOD), 2) && tmp >= 0) {
				image->Img[y][x].c[image->r] = mRGB.r;
				image->Img[y][x].c[image->g] = mRGB.g;
				image->Img[y][x].c[image->b] = mRGB.b;
				image->Img[y][x].c[image->a] = 255;
			}
			else if (x >= gap && y >= gap && x < Gui.CS_PXSIZE + gap && y < Gui.CS_PXSIZE + gap) {
				hsv.s = (float)(x) / (Gui.CS_PXSIZE + gap);
				hsv.v = 1 - ((float)(y) / (Gui.CS_PXSIZE + gap));
				rgb = csHSV2RGB(iFilter, hsv);
				image->Img[y][x].c[image->r] = rgb.r;
				image->Img[y][x].c[image->g] = rgb.g;
				image->Img[y][x].c[image->b] = rgb.b;
				image->Img[y][x].c[image->a] = 255;
			}
			else {
				image->Img[y][x].c[image->r] = 0;
				image->Img[y][x].c[image->g] = 0;
				image->Img[y][x].c[image->b] = 0;
				image->Img[y][x].c[image->a] = 0;
			}
		}
	}


}

//繪製色相方塊
PIBlock*
DrawHueSquare(PIFilter* iFilter) {
	PIBlock* image;
	HSVPixel hsv = { mHSV.h, 1, 1, 255 };

	image = TVAllocPIBlock(iFilter, Gui.CS_SIZE, Gui.CS_SIZE, 0);
	if (image == NULL)return NULL;

	csSquaBtn(iFilter);

	HueSquareBlock(iFilter, image, hsv);

	return image;
}

//被動刷新色相方塊
void
RefreshHueSquare(PIFilter* iFilter, PIBlock* image) {
	HSVPixel hsv = { mHSV.h, 1, 1, 255 };

	int gap = (Gui.CS_SIZE - Gui.CS_PXSIZE) / (float)2;

	if (image == NULL)return NULL;

	csSquaBtn(iFilter);

	HueSquareBlock(iFilter, image, hsv);

}

//主動刷新色相方塊
void
SlideSquare(PIFilter *iFilter, PIBlock *image) {
	if (image == NULL)return NULL;

	HSVPixel hsv = { mHSV.h, 1, 1, 255 };

	/*更改mRGB 並將APEN設置成mRGB*/
	mHSV.s = ((float)Data.mSquaClickX - (float)Gui.CS_PXGAP) / (float)Gui.CS_PXSIZE;
	mHSV.v = 1 - ((float)Data.mSquaClickY - (float)Gui.CS_PXGAP) / (float)Gui.CS_PXSIZE;
	if (mHSV.s > 1)	mHSV.s = 1;
	else if (mHSV.s < 0)	mHSV.s = 0;
	if (mHSV.v > 1)	mHSV.v = 1;
	else if (mHSV.v < 0)	mHSV.v = 0;

	mRGB = csHSV2RGB(iFilter, mHSV);
	TVSetAPen(iFilter, mRGB);


	//設定按鈕邊界
	Data.mSquaBtnX = Data.mSquaClickX;
	Data.mSquaBtnY = Data.mSquaClickY;
	if (Data.mSquaBtnX > Gui.CS_PXGAP + Gui.CS_PXSIZE)	Data.mSquaBtnX = Gui.CS_PXGAP + Gui.CS_PXSIZE;
	else if (Data.mSquaBtnX < Gui.CS_PXGAP)	Data.mSquaBtnX = Gui.CS_PXGAP;
	if (Data.mSquaBtnY > Gui.CS_PXGAP + Gui.CS_PXSIZE)	Data.mSquaBtnY = Gui.CS_PXGAP + Gui.CS_PXSIZE;
	else if (Data.mSquaBtnY < Gui.CS_PXGAP)	Data.mSquaBtnY = Gui.CS_PXGAP;

	HueSquareBlock(iFilter, image, hsv);

}


/**************************************************************************************/
/* 滑桿方塊繪製與刷新 */

/* 滑桿方塊刷新程式碼 */
void
ColorSliderBlock(PIFilter* iFilter, PIBlock* image, HSVPixel hsv, RGBPixel rgb, int w, int h, char color) {
	int g = 2;

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int P255 = (x * 256) / w;
			if (P255 > 255)P255 = 255;

			switch (color) {
			case 'r':
				image->Img[y][x].c[image->r] = P255;
				image->Img[y][x].c[image->g] = mRGB.g;
				image->Img[y][x].c[image->b] = mRGB.b;
				image->Img[y][x].c[image->a] = 255;
				break;		
			case 'g':
				image->Img[y][x].c[image->r] = mRGB.r;
				image->Img[y][x].c[image->g] = P255;
				image->Img[y][x].c[image->b] = mRGB.b;
				image->Img[y][x].c[image->a] = 255;
				break;
			case 'b':
				image->Img[y][x].c[image->r] = mRGB.r;
				image->Img[y][x].c[image->g] = mRGB.g;
				image->Img[y][x].c[image->b] = P255;
				image->Img[y][x].c[image->a] = 255;
				break;
			case 'h':
				hsv.h = x / (float)w;
				hsv.s = 1;
				hsv.v = 1;
				if (hsv.h > 1) hsv.h = 1;
				rgb = csHSV2RGB(iFilter, hsv);
				image->Img[y][x].c[image->r] = rgb.r;
				image->Img[y][x].c[image->g] = rgb.g;
				image->Img[y][x].c[image->b] = rgb.b;
				image->Img[y][x].c[image->a] = 255;
				break;
			case 's':
				hsv.s = x / (float)w;
				hsv.h = mHSV.h;
				hsv.v = 1;
				if (hsv.s > 1) hsv.s = 1;
				rgb = csHSV2RGB(iFilter, hsv);
				image->Img[y][x].c[image->r] = rgb.r;
				image->Img[y][x].c[image->g] = rgb.g;
				image->Img[y][x].c[image->b] = rgb.b;
				image->Img[y][x].c[image->a] = 255;
				break;
			case 'v':
				hsv.v = x / (float)w;
				hsv.h = mHSV.h;
				hsv.s = 1;
				if (hsv.v > 1) hsv.v = 1;
				rgb = csHSV2RGB(iFilter, hsv);
				image->Img[y][x].c[image->r] = rgb.r;
				image->Img[y][x].c[image->g] = rgb.g;
				image->Img[y][x].c[image->b] = rgb.b;
				image->Img[y][x].c[image->a] = 255;
				break;
			}
		}
	}
}

//繪製滑桿方塊
PIBlock*
DrawColorSlider(PIFilter* iFilter, char color) {
	PIBlock* image;
	HSVPixel hsv = { mHSV.h, 1, 1, 255 };
	RGBPixel rgb = { 0, 0, 0, 255 };

	int w = Gui.SLIDER_W;
	int h = Gui.SLIDER_H - Gui.SLIDER_OFFSET;

	image = TVAllocPIBlock(iFilter, w, h, 0);
	if (image == NULL)return NULL;

	ColorSliderBlock(iFilter, image, hsv, rgb, w, h, color);


	return image;
}

//被動刷新滑桿方塊
void
RefreshColorSlider(PIFilter* iFilter, PIBlock* image, char color) {
	HSVPixel hsv = { mHSV.h, 1, 1, 255 };
	RGBPixel rgb = { 0 };
	int w = Gui.SLIDER_W;
	int h = Gui.SLIDER_H - Gui.SLIDER_OFFSET;

	if (image == NULL)return NULL;

	ColorSliderBlock(iFilter, image, hsv, rgb, w, h, color);

}


/**************************************************************************************/
/* AB顏色區塊繪製與刷新*/

/* A顏色區塊刷新程式碼 */
void
ColorASelectedBlock(PIFilter* iFilter, PIBlock* image, int w, int h) {
	int rA = floorf(Gui.C_A_RADIUS * 4 / (float)5);
	int xA = floorf(Gui.C_A_SIZE * 2 / (float)5);
	int yA = xA;

	int rB = Gui.C_B_RADIUS;
	int xB = floorf(Gui.C_A_SIZE * 3 / (float)4);
	int yB = xB;

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			float tmp = pow((i - xA), 2) + pow((j - yA), 2);


			if (tmp < (rA*rA) && tmp >= pow(rA - (float)0.3, 2)) {
				float tmp2 = pow((i - xB), 2) + pow((j - yB), 2);

				if (tmp2 < (rB*rB) && tmp2 >= pow(rB - 3, 2)) {
					image->Img[i][j].c[image->r] = 0;
					image->Img[i][j].c[image->g] = 0;
					image->Img[i][j].c[image->b] = 0;
					image->Img[i][j].c[image->a] = 0;
				}
				else if (tmp2 < pow(rB - 3, 2) && tmp2 >= pow(rB - 3.2, 2)) {
					image->Img[i][j].c[image->r] = floorf(128 + (mRGB2.r - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->g] = floorf(128 + (mRGB2.g - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->b] = floorf(128 + (mRGB2.b - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->a] = 64;
				}
				else if (tmp2 < pow(rB - 3.2, 2) && tmp2 >= pow(rB - 3.5, 2)) {
					image->Img[i][j].c[image->r] = floorf(128 + (mRGB2.r - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->g] = floorf(128 + (mRGB2.g - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->b] = floorf(128 + (mRGB2.b - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->a] = 128;
				}
				else if (tmp2 < pow(rB - 3.5, 2)) {
					image->Img[i][j].c[image->r] = mRGB2.r;
					image->Img[i][j].c[image->g] = mRGB2.g;
					image->Img[i][j].c[image->b] = mRGB2.b;
					image->Img[i][j].c[image->a] = 255;
				}
				else {
					image->Img[i][j].c[image->r] = mRGBreq.r / 4;
					image->Img[i][j].c[image->g] = mRGBreq.g / 4;
					image->Img[i][j].c[image->b] = mRGBreq.b / 4;
					image->Img[i][j].c[image->a] = 64;
				}
			}
			else if (tmp < pow(rA - (float)0.3, 2) && tmp >= pow(rA - (float)0.7, 2)) {
				float tmp2 = pow((i - xB), 2) + pow((j - yB), 2);

				if (tmp2 < (rB*rB) && tmp2 >= pow(rB - 3, 2)) {
					image->Img[i][j].c[image->r] = 0;
					image->Img[i][j].c[image->g] = 0;
					image->Img[i][j].c[image->b] = 0;
					image->Img[i][j].c[image->a] = 0;
				}
				else if (tmp2 < pow(rB - 3, 2) && tmp2 >= pow(rB - 3.2, 2)) {
					image->Img[i][j].c[image->r] = floorf(128 + (mRGB2.r - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->g] = floorf(128 + (mRGB2.g - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->b] = floorf(128 + (mRGB2.b - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->a] = 64;
				}
				else if (tmp2 < pow(rB - 3.2, 2) && tmp2 >= pow(rB - 3.5, 2)) {
					image->Img[i][j].c[image->r] = floorf(128 + (mRGB2.r - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->g] = floorf(128 + (mRGB2.g - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->b] = floorf(128 + (mRGB2.b - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->a] = 128;
				}
				else if (tmp2 < pow(rB - 3.5, 2)) {
					image->Img[i][j].c[image->r] = mRGB2.r;
					image->Img[i][j].c[image->g] = mRGB2.g;
					image->Img[i][j].c[image->b] = mRGB2.b;
					image->Img[i][j].c[image->a] = 255;
				}
				else {
					image->Img[i][j].c[image->r] = mRGBreq.r / 2;
					image->Img[i][j].c[image->g] = mRGBreq.g / 2;
					image->Img[i][j].c[image->b] = mRGBreq.b / 2;
					image->Img[i][j].c[image->a] = 128;
				}
			}
			else if (tmp < pow(rA - (float)0.7, 2) && tmp >= pow(rA - 1.4, 2)) {
				image->Img[i][j].c[image->r] = mRGBreq.r;
				image->Img[i][j].c[image->g] = mRGBreq.g;
				image->Img[i][j].c[image->b] = mRGBreq.b;
				image->Img[i][j].c[image->a] = 255;
			}
			else if (tmp < pow(rA - 1.4, 2) && tmp >= pow(rA - 1.8, 2)) {
				image->Img[i][j].c[image->r] = mRGBreq.r / 2;
				image->Img[i][j].c[image->g] = mRGBreq.g / 2;
				image->Img[i][j].c[image->b] = mRGBreq.b / 2;
				image->Img[i][j].c[image->a] = 128;
			}
			else if (tmp < pow(rA - 1.8, 2) && tmp > pow(rA - 2.1, 2)) {
				image->Img[i][j].c[image->r] = mRGBreq.r / 4;
				image->Img[i][j].c[image->g] = mRGBreq.g / 4;
				image->Img[i][j].c[image->b] = mRGBreq.b / 4;
				image->Img[i][j].c[image->a] = 64;
			}

			else if (tmp < pow(rA - 3, 2) && tmp >= pow(rA - 3.4, 2)) {
				image->Img[i][j].c[image->r] = floorf(128 + (mRGB.r - 128) * 3 / 5) / 4;
				image->Img[i][j].c[image->g] = floorf(128 + (mRGB.g - 128) * 3 / 5) / 4;
				image->Img[i][j].c[image->b] = floorf(128 + (mRGB.b - 128) * 3 / 5) / 4;
				image->Img[i][j].c[image->a] = 64;
			}
			else if (tmp < pow(rA - 3.4, 2) && tmp >= pow(rA - 4, 2)) {
				image->Img[i][j].c[image->r] = floorf(128 + (mRGB.r - 128) * 4 / 5) / 2;
				image->Img[i][j].c[image->g] = floorf(128 + (mRGB.g - 128) * 4 / 5) / 2;
				image->Img[i][j].c[image->b] = floorf(128 + (mRGB.b - 128) * 4 / 5) / 2;
				image->Img[i][j].c[image->a] = 128;
			}
			else if (tmp < pow(rA - 4, 2)) {
				image->Img[i][j].c[image->r] = mRGB.r;
				image->Img[i][j].c[image->g] = mRGB.g;
				image->Img[i][j].c[image->b] = mRGB.b;
				image->Img[i][j].c[image->a] = 255;
			}
			else if (tmp >= pow(rA - (float)0.7, 2)) {
				float tmp2 = pow((i - xB), 2) + pow((j - yB), 2);

				if (tmp2 < (rB*rB) && tmp2 >= pow(rB - 3, 2)) {
					image->Img[i][j].c[image->r] = 0;
					image->Img[i][j].c[image->g] = 0;
					image->Img[i][j].c[image->b] = 0;
					image->Img[i][j].c[image->a] = 0;
				}
				else if (tmp2 < pow(rB - 3, 2) && tmp2 >= pow(rB - 3.2, 2)) {
					image->Img[i][j].c[image->r] = floorf(128 + (mRGB2.r - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->g] = floorf(128 + (mRGB2.g - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->b] = floorf(128 + (mRGB2.b - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->a] = 64;
				}
				else if (tmp2 < pow(rB - 3.2, 2) && tmp2 >= pow(rB - 3.5, 2)) {
					image->Img[i][j].c[image->r] = floorf(128 + (mRGB2.r - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->g] = floorf(128 + (mRGB2.g - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->b] = floorf(128 + (mRGB2.b - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->a] = 128;
				}
				else if (tmp2 < pow(rB - 3.5, 2)) {
					image->Img[i][j].c[image->r] = mRGB2.r;
					image->Img[i][j].c[image->g] = mRGB2.g;
					image->Img[i][j].c[image->b] = mRGB2.b;
					image->Img[i][j].c[image->a] = 255;
				}
			}
			else {
				image->Img[i][j].c[image->r] = 0;
				image->Img[i][j].c[image->g] = 0;
				image->Img[i][j].c[image->b] = 0;
				image->Img[i][j].c[image->a] = 0;
			}
		}
	}
}

/* B顏色區塊刷新程式碼 */
void
ColorBSelectedBlock(PIFilter* iFilter, PIBlock* image, int w, int h) {
	int rA = floorf(Gui.C_A_RADIUS * 4 / (float)5);
	int xA = floorf(Gui.C_A_SIZE * 2 / (float)5);
	int yA = xA;

	int rB = Gui.C_B_RADIUS;
	int xB = floorf(Gui.C_A_SIZE * 3 / (float)4);
	int yB = xB;

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			float tmp2 = pow((i - xB), 2) + pow((j - yB), 2);

			if (tmp2 < (rB*rB) && tmp2 >= pow(rB - (float)0.2, 2)) {
				float tmp = pow((i - xA), 2) + pow((j - yA), 2);

				if (tmp < (rA*rA) && tmp >= pow(rA - 3, 2)) {
					image->Img[i][j].c[image->r] = 0;
					image->Img[i][j].c[image->g] = 0;
					image->Img[i][j].c[image->b] = 0;
					image->Img[i][j].c[image->a] = 0;
				}
				else if (tmp <= pow(rA - 3, 2) && tmp >= pow(rA - 3.4, 2)) {
					image->Img[i][j].c[image->r] = floorf(128 + (mRGB2.r - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->g] = floorf(128 + (mRGB2.g - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->b] = floorf(128 + (mRGB2.b - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->a] = 64;
				}
				else if (tmp < pow(rA - 3.4, 2) && tmp >= pow(rA - 4, 2)) {
					image->Img[i][j].c[image->r] = floorf(128 + (mRGB2.r - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->g] = floorf(128 + (mRGB2.g - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->b] = floorf(128 + (mRGB2.b - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->a] = 128;
				}
				else if (tmp < pow(rA - 4, 2)) {
					image->Img[i][j].c[image->r] = mRGB2.r;
					image->Img[i][j].c[image->g] = mRGB2.g;
					image->Img[i][j].c[image->b] = mRGB2.b;
					image->Img[i][j].c[image->a] = 255;
				}
				else {
					image->Img[i][j].c[image->r] = mRGBreq.r / 4;
					image->Img[i][j].c[image->g] = mRGBreq.g / 4;
					image->Img[i][j].c[image->b] = mRGBreq.b / 4;
					image->Img[i][j].c[image->a] = 64;
				}
			}
			else if (tmp2 < pow(rB - (float)0.2, 2) && tmp2 >= pow(rB - (float)0.5, 2)) {
				float tmp = pow((i - xA), 2) + pow((j - yA), 2);

				if (tmp < (rA*rA) && tmp >= pow(rA - 3, 2)) {
					image->Img[i][j].c[image->r] = 0;
					image->Img[i][j].c[image->g] = 0;
					image->Img[i][j].c[image->b] = 0;
					image->Img[i][j].c[image->a] = 0;
				}
				else if (tmp <= pow(rA - 3, 2) && tmp >= pow(rA - 3.4, 2)) {
					image->Img[i][j].c[image->r] = floorf(128 + (mRGB2.r - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->g] = floorf(128 + (mRGB2.g - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->b] = floorf(128 + (mRGB2.b - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->a] = 64;
				}
				else if (tmp < pow(rA - 3.4, 2) && tmp >= pow(rA - 4, 2)) {
					image->Img[i][j].c[image->r] = floorf(128 + (mRGB2.r - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->g] = floorf(128 + (mRGB2.g - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->b] = floorf(128 + (mRGB2.b - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->a] = 128;
				}
				else if (tmp < pow(rA - 4, 2)) {
					image->Img[i][j].c[image->r] = mRGB2.r;
					image->Img[i][j].c[image->g] = mRGB2.g;
					image->Img[i][j].c[image->b] = mRGB2.b;
					image->Img[i][j].c[image->a] = 255;
				}
				else {
					image->Img[i][j].c[image->r] = mRGBreq.r / 2;
					image->Img[i][j].c[image->g] = mRGBreq.g / 2;
					image->Img[i][j].c[image->b] = mRGBreq.b / 2;
					image->Img[i][j].c[image->a] = 128;
				}
			}
			else if (tmp2 < pow(rB - (float)0.5, 2) && tmp2 >= pow(rB - (float)1, 2)) {
				image->Img[i][j].c[image->r] = mRGBreq.r;
				image->Img[i][j].c[image->g] = mRGBreq.g;
				image->Img[i][j].c[image->b] = mRGBreq.b;
				image->Img[i][j].c[image->a] = 255;
			}
			else if (tmp2 < pow(rB - (float)1, 2) && tmp2 >= pow(rB - (float)1, 2)) {
				image->Img[i][j].c[image->r] = mRGBreq.r / 2;
				image->Img[i][j].c[image->g] = mRGBreq.g / 2;
				image->Img[i][j].c[image->b] = mRGBreq.b / 2;
				image->Img[i][j].c[image->a] = 128;
			}
			else if (tmp2 < pow(rB - 1.3, 2) && tmp2 > pow(rB - (float)1, 2)) {
				image->Img[i][j].c[image->r] = mRGBreq.r / 4;
				image->Img[i][j].c[image->g] = mRGBreq.g / 4;
				image->Img[i][j].c[image->b] = mRGBreq.b / 4;
				image->Img[i][j].c[image->a] = 64;
			}

			else if (tmp2 < pow(rB - 1.5, 2) && tmp2 >= pow(rB - 1.7, 2)) {
				image->Img[i][j].c[image->r] = floorf(128 + (mRGB.r - 128) * 3 / 5) / 4;
				image->Img[i][j].c[image->g] = floorf(128 + (mRGB.g - 128) * 3 / 5) / 4;
				image->Img[i][j].c[image->b] = floorf(128 + (mRGB.b - 128) * 3 / 5) / 4;
				image->Img[i][j].c[image->a] = 64;
			}
			else if (tmp2 < pow(rB - 3, 2) && tmp2 >= pow(rB - 3.3, 2)) {
				image->Img[i][j].c[image->r] = floorf(128 + (mRGB.r - 128) * 4 / 5) / 2;
				image->Img[i][j].c[image->g] = floorf(128 + (mRGB.g - 128) * 4 / 5) / 2;
				image->Img[i][j].c[image->b] = floorf(128 + (mRGB.b - 128) * 4 / 5) / 2;
				image->Img[i][j].c[image->a] = 128;
			}
			else if (tmp2 < pow(rB - 3.3, 2)) {
				image->Img[i][j].c[image->r] = mRGB.r;
				image->Img[i][j].c[image->g] = mRGB.g;
				image->Img[i][j].c[image->b] = mRGB.b;
				image->Img[i][j].c[image->a] = 255;
			}
			else if (tmp2 >= pow(rB - (float)0.5, 2)) {
				float tmp = pow((i - xA), 2) + pow((j - yA), 2);


				if (tmp < (rA*rA) && tmp >= pow(rA - 3, 2)) {
					image->Img[i][j].c[image->r] = 0;
					image->Img[i][j].c[image->g] = 0;
					image->Img[i][j].c[image->b] = 0;
					image->Img[i][j].c[image->a] = 0;
				}
				else if (tmp <= pow(rA - 3, 2) && tmp >= pow(rA - 3.4, 2)) {
					image->Img[i][j].c[image->r] = floorf(128 + (mRGB2.r - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->g] = floorf(128 + (mRGB2.g - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->b] = floorf(128 + (mRGB2.b - 128) * 3 / 5) / 4;
					image->Img[i][j].c[image->a] = 64;
				}
				else if (tmp < pow(rA - 3.4, 2) && tmp >= pow(rA - 4, 2)) {
					image->Img[i][j].c[image->r] = floorf(128 + (mRGB2.r - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->g] = floorf(128 + (mRGB2.g - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->b] = floorf(128 + (mRGB2.b - 128) * 4 / 5) / 2;
					image->Img[i][j].c[image->a] = 128;
				}
				else if (tmp < pow(rA - 4, 2)) {
					image->Img[i][j].c[image->r] = mRGB2.r;
					image->Img[i][j].c[image->g] = mRGB2.g;
					image->Img[i][j].c[image->b] = mRGB2.b;
					image->Img[i][j].c[image->a] = 255;
				}
				else {
				}

			}
			else {
				image->Img[i][j].c[image->r] = 0;
				image->Img[i][j].c[image->g] = 0;
				image->Img[i][j].c[image->b] = 0;
				image->Img[i][j].c[image->a] = 0;
			}
		}
	}
}

/* AB顏色區塊刷新程式碼 */
void
ColorABBlock(PIFilter* iFilter, PIBlock* image, int w, int h, int iArgs) {
	switch (iArgs) {
	case 0:
		ColorASelectedBlock(iFilter, image, w, h);
		break;
	case 1:
		ColorBSelectedBlock(iFilter, image, w, h);
		break;
	}

}



//繪製AB顏色區塊
PIBlock*
DrawABColor(PIFilter* iFilter) {
	PIBlock* image;

	image = TVAllocPIBlock(iFilter, Gui.C_A_SIZE, Gui.C_A_SIZE, 0);
	if (image == NULL)return NULL;

	ColorABBlock(iFilter, image, Gui.C_A_SIZE, Gui.C_A_SIZE, Data.mABPenSelected);

	return image;
}

//被動刷新AB顏色區塊
void
RefreshABColor(PIFilter* iFilter, PIBlock* image) {
	if (image == NULL)return NULL;

	ColorABBlock(iFilter, image, Gui.C_A_SIZE, Gui.C_A_SIZE, Data.mABPenSelected);

	return image;

}



/**************************************************************************************/
/* 黑白顏色按鈕繪製與刷新*/

/* 黑白顏色按鈕刷新程式碼 */
void
BlackWhiteBlock(PIFilter* iFilter, PIBlock* image) {
	int r = floorf(Gui.BLACKWHITE_SIZE / (float)2);
	for (int x = 0; x < Gui.BLACKWHITE_SIZE; x++) {
		for (int y = 0; y < Gui.BLACKWHITE_SIZE; y++) {
			float tmp = pow(x - r, 2) + pow(y - r, 2);

			if (tmp < pow(r, 2) && tmp >= pow(r - (float)0.3, 2)) {
				if (4 * x + 3 * y <= 3.5*Gui.BLACKWHITE_SIZE) {
					image->Img[y][x].c[image->r] = 64;
					image->Img[y][x].c[image->g] = 64;
					image->Img[y][x].c[image->b] = 64;
					image->Img[y][x].c[image->a] = 255;
				}
				else {
					image->Img[y][x].c[image->r] = 190;
					image->Img[y][x].c[image->g] = 190;
					image->Img[y][x].c[image->b] = 190;
					image->Img[y][x].c[image->a] = 255;
				}
			}
			else if (tmp < pow(r - (float)0.3, 2) && tmp >= pow(r - (float)0.7, 2)) {
				if (4 * x + 3 * y <= 3.5*Gui.BLACKWHITE_SIZE) {
					image->Img[y][x].c[image->r] = 32;
					image->Img[y][x].c[image->g] = 32;
					image->Img[y][x].c[image->b] = 32;
					image->Img[y][x].c[image->a] = 255;
				}
				else {
					image->Img[y][x].c[image->r] = 220;
					image->Img[y][x].c[image->g] = 220;
					image->Img[y][x].c[image->b] = 220;
					image->Img[y][x].c[image->a] = 255;
				}
			}
			else if (tmp < pow(r - (float)0.7, 2)) {
				if (4 * x + 3 * y == 3.5*Gui.BLACKWHITE_SIZE - 1) {
					image->Img[y][x].c[image->r] = 128;
					image->Img[y][x].c[image->g] = 128;
					image->Img[y][x].c[image->b] = 128;
					image->Img[y][x].c[image->a] = 255;
				}
				else if (4 * x + 3 * y <= 3.5*Gui.BLACKWHITE_SIZE) {
					image->Img[y][x].c[image->r] = 0;
					image->Img[y][x].c[image->g] = 0;
					image->Img[y][x].c[image->b] = 0;
					image->Img[y][x].c[image->a] = 255;
				}
				else if (4 * x + 3 * y <= 3.5*Gui.BLACKWHITE_SIZE + 1) {
					image->Img[y][x].c[image->r] = 128;
					image->Img[y][x].c[image->g] = 128;
					image->Img[y][x].c[image->b] = 128;
					image->Img[y][x].c[image->a] = 255;
				}
				else {
					image->Img[y][x].c[image->r] = 255;
					image->Img[y][x].c[image->g] = 255;
					image->Img[y][x].c[image->b] = 255;
					image->Img[y][x].c[image->a] = 255;
				}
			}
			else {
				image->Img[y][x].c[image->r] = 0;
				image->Img[y][x].c[image->g] = 0;
				image->Img[y][x].c[image->b] = 0;
				image->Img[y][x].c[image->a] = 0;
			}

		}
	}




}

/* 黑白顏色按鈕繪製*/
PIBlock*
DrawBlackWhiteBtn(PIFilter* iFilter) {
	PIBlock* image;

	image = TVAllocPIBlock(iFilter, Gui.BLACKWHITE_SIZE, Gui.BLACKWHITE_SIZE, 0);
	if (image == NULL)return NULL;

	BlackWhiteBlock(iFilter, image);

	return image;

}


/* 黑白顏色按鈕刷新*/
void
RefreshBlackWhiteBtn(PIFilter* iFilter, PIBlock* image) {
	if (image == NULL)return NULL;

	BlackWhiteBlock(iFilter, image);

	return image;

}



/**************************************************************************************/
/* AB顏色互換按鈕繪製與刷新*/

/* AB顏色互換按鈕刷新程式碼 */
void
CSwitchBlock(PIFilter* iFilter, PIBlock* image) {
	int t = floorf(Gui.CSWITCH_SIZE / (float)4);
	for (int x = 0; x < Gui.CSWITCH_SIZE; x++) {
		for (int y = 0; y < Gui.CSWITCH_SIZE; y++) {
			float tmp = pow(x - t, 2) + pow(y - 3 * t, 2);

			//左箭頭上半部
			if (x <= t && y < t && x + y >= t + 1) {
				image->Img[y][x].c[image->r] = 70;
				image->Img[y][x].c[image->g] = 70;
				image->Img[y][x].c[image->b] = 70;
				image->Img[y][x].c[image->a] = 255;
			}
			//左箭頭下半部
			else if (x <= t && y >= t && y < 2 * t && y - x <= t - 1) {
				image->Img[y][x].c[image->r] = 70;
				image->Img[y][x].c[image->g] = 70;
				image->Img[y][x].c[image->b] = 70;
				image->Img[y][x].c[image->a] = 255;
			}
			//下箭頭左半部
			else if (x >= 2 * t && x < 3 * t && y > 3 * t && y - x <= t) {
				image->Img[y][x].c[image->r] = 70;
				image->Img[y][x].c[image->g] = 70;
				image->Img[y][x].c[image->b] = 70;
				image->Img[y][x].c[image->a] = 255;
			}
			//下箭頭右半部
			else if (x >= 3 * t && y > 3 * t && x + y <= 7 * t) {
				image->Img[y][x].c[image->r] = 70;
				image->Img[y][x].c[image->g] = 70;
				image->Img[y][x].c[image->b] = 70;
				image->Img[y][x].c[image->a] = 255;
			}

			//箭頭半圓
			else if (x > t && y <= 3 * t && tmp >= pow((2 * t) - t / 3, 2) && tmp <= pow((2 * t) + t / 3, 2)) {
				image->Img[y][x].c[image->r] = 70;
				image->Img[y][x].c[image->g] = 70;
				image->Img[y][x].c[image->b] = 70;
				image->Img[y][x].c[image->a] = 255;
			}
			else {
				image->Img[y][x].c[image->r] = 0;
				image->Img[y][x].c[image->g] = 0;
				image->Img[y][x].c[image->b] = 0;
				image->Img[y][x].c[image->a] = 0;
			}

		}
	}




}

/* AB顏色互換按鈕繪製*/
PIBlock*
DrawCSwitchBtn(PIFilter* iFilter) {
	PIBlock* image;

	image = TVAllocPIBlock(iFilter, Gui.CSWITCH_SIZE, Gui.CSWITCH_SIZE, 0);
	if (image == NULL)return NULL;

	CSwitchBlock(iFilter, image);

	return image;

}


/* AB顏色互換按鈕刷新*/
void
RefreshCSwitchBtn(PIFilter* iFilter, PIBlock* image) {
	if (image == NULL)return NULL;

	CSwitchBlock(iFilter, image);

	return image;

}

/**************************************************************************************/
/*吸色GUI繪製與刷新*/

/*吸色GUI刷新程式碼 */
void
PickBlock(PIFilter* iFilter, PIBlock* image) {
	for (int x = 0; x < Gui.PICK_W; x++) {
		for (int y = 0; y < Gui.PICK_H; y++) {
			if (y < Gui.PICK_H / 2) {
				image->Img[y][x].c[image->r] = mRGBtmp.r;
				image->Img[y][x].c[image->g] = mRGBtmp.g;
				image->Img[y][x].c[image->b] = mRGBtmp.b;
				image->Img[y][x].c[image->a] = 255;
			}
			else {
				image->Img[y][x].c[image->r] = mRGB.r;
				image->Img[y][x].c[image->g] = mRGB.g;
				image->Img[y][x].c[image->b] = mRGB.b;
				image->Img[y][x].c[image->a] = 255;
			}
		}
	}
}

/* 繪製吸色GUI*/
PIBlock*
DrawPickGUI(PIFilter* iFilter) {
	PIBlock* image;
	image = TVAllocPIBlock(iFilter, Gui.PICK_W, Gui.PICK_H, 0);
	if (image == NULL)return NULL;

	PickBlock(iFilter, image);

	return image;
}

/* 刷新吸色GUI*/
void
RefreshPickGUI(PIFilter* iFilter, PIBlock* image) {
	if (image == NULL)return NULL;

	PickBlock(iFilter, image);
}


/**************************************************************************************/
/* 轉換輸入的顏色數值*/

InputColor(PIFilter* iFilter) {
	char tmp[256];
	int i;

	switch (Data.mInputColorNum) {
	case 1: //R
		TVGetButtonString(iFilter, Data.mReq, ID_NUM_R, tmp, 256);
		i = atoi(tmp);
		if (i > 255) {
			i = 255;
			sprintf(tmp, "%d", i);
			TVPutButtonString(iFilter, Data.mReq, ID_NUM_R, tmp);
		}
		else if (i <0) {
			i = 0;
			sprintf(tmp, "%d", i);
			TVPutButtonString(iFilter, Data.mReq, ID_NUM_R, tmp);
		}
		mRGB.r = i;
		mHSV = csRGB2HSV(iFilter, mRGB);
		break;

	case 2: //G
		TVGetButtonString(iFilter, Data.mReq, ID_NUM_G, tmp, 256);
		i = atoi(tmp);
		if (i > 255) {
			i = 255;
			sprintf(tmp, "%d", i);
			TVPutButtonString(iFilter, Data.mReq, ID_NUM_G, tmp);
		}
		else if (i <0) {
			i = 0;
			sprintf(tmp, "%d", i);
			TVPutButtonString(iFilter, Data.mReq, ID_NUM_G, tmp);
		}
		mRGB.g = i;
		mHSV = csRGB2HSV(iFilter, mRGB);
		break;

	case 3: //B
		TVGetButtonString(iFilter, Data.mReq, ID_NUM_B, tmp, 256);
		i = atoi(tmp);
		if (i > 255) {
			i = 255;
			sprintf(tmp, "%d", i);
			TVPutButtonString(iFilter, Data.mReq, ID_NUM_B, tmp);
		}
		else if (i <0) {
			i = 0;
			sprintf(tmp, "%d", i);
			TVPutButtonString(iFilter, Data.mReq, ID_NUM_B, tmp);
		}
		mRGB.b = i;
		mHSV = csRGB2HSV(iFilter, mRGB);
		break;

	case 4: //H
		TVGetButtonString(iFilter, Data.mReq, ID_NUM_H, tmp, 256);
		i = atoi(tmp);
		if (i > 360) {
			i = 360;
			sprintf(tmp, "%d", i);
			TVPutButtonString(iFilter, Data.mReq, ID_NUM_H, tmp);
		}
		else if (i <0) {
			i = 0;
			sprintf(tmp, "%d", i);
			TVPutButtonString(iFilter, Data.mReq, ID_NUM_H, tmp);
		}
		mHSV.h = i / (float)360;
		mRGB = csHSV2RGB(iFilter, mHSV);
		break;

	case 5: //S
		TVGetButtonString(iFilter, Data.mReq, ID_NUM_S, tmp, 256);
		i = atoi(tmp);
		if (i > 100) {
			i = 100;
			sprintf(tmp, "%d", i);
			TVPutButtonString(iFilter, Data.mReq, ID_NUM_S, tmp);
		}
		else if (i <0) {
			i = 0;
			sprintf(tmp, "%d", i);
			TVPutButtonString(iFilter, Data.mReq, ID_NUM_S, tmp);
		}
		mHSV.s = i / (float)100;
		mRGB = csHSV2RGB(iFilter, mHSV);
		break;

	case 6: //V
		TVGetButtonString(iFilter, Data.mReq, ID_NUM_V, tmp, 256);
		i = atoi(tmp);
		if (i > 100) {
			i = 100;
			sprintf(tmp, "%d", i);
			TVPutButtonString(iFilter, Data.mReq, ID_NUM_V, tmp);
		}
		else if (i <0) {
			i = 0;
			sprintf(tmp, "%d", i);
			TVPutButtonString(iFilter, Data.mReq, ID_NUM_V, tmp);
		}
		mHSV.v = i / (float)100;
		mRGB = csHSV2RGB(iFilter, mHSV);
		break;
	}
}

/**************************************************************************************/
/* 齒輪按鈕繪製與刷新*/

/* 齒輪按鈕程式碼*/
void
GearCode(PIFilter *iFilter, PIBlock *image) {
	float r = Gui.OTHER_SIZE / (float)2;
	float tmp;
	for (int x = 0; x<Gui.OTHER_SIZE; x++) {
		for (int y = 0; y < Gui.OTHER_SIZE; y++) {
			tmp = pow(x - r, 2) + pow(y - r, 2);
			if (tmp < pow(r * 6 / 8, 2) && tmp >= pow(r * 5 / 8, 2)) {
				if (y > floor(0.85 * r) && floor(y < 1.15 * r)) {
					image->Img[x][y].c[image->r] = mRGBreq.r;
					image->Img[x][y].c[image->g] = mRGBreq.g;
					image->Img[x][y].c[image->b] = mRGBreq.b;
					image->Img[x][y].c[image->a] = 255;
				}
				else if (x >(float)0.85 * r && x < 1.15 * r) {
					image->Img[x][y].c[image->r] = mRGBreq.r;
					image->Img[x][y].c[image->g] = mRGBreq.g;
					image->Img[x][y].c[image->b] = mRGBreq.b;
					image->Img[x][y].c[image->a] = 255;
				}
				else if (x + y >= 2 * r - (float)0.2 * r && x + y <= 2 * r + (float)0.2 * r) {
					image->Img[x][y].c[image->r] = mRGBreq.r;
					image->Img[x][y].c[image->g] = mRGBreq.g;
					image->Img[x][y].c[image->b] = mRGBreq.b;
					image->Img[x][y].c[image->a] = 255;
				}
				else if (x >= y - (float)0.2 * r && x <= y + (float)0.2 * r) {
					image->Img[x][y].c[image->r] = mRGBreq.r;
					image->Img[x][y].c[image->g] = mRGBreq.g;
					image->Img[x][y].c[image->b] = mRGBreq.b;
					image->Img[x][y].c[image->a] = 255;
				}
			}
			else if (tmp < pow(r * 5 / 8, 2) && tmp >= pow(r * 3 / 8, 2)) {
				image->Img[x][y].c[image->r] = mRGBreq.r;
				image->Img[x][y].c[image->g] = mRGBreq.g;
				image->Img[x][y].c[image->b] = mRGBreq.b;
				image->Img[x][y].c[image->a] = 255;
			}
			else {
				image->Img[x][y].c[image->r] = 0;
				image->Img[x][y].c[image->g] = 0;
				image->Img[x][y].c[image->b] = 0;
				image->Img[x][y].c[image->a] = 0;
			}
		}
	}
}


/* 齒輪按鈕繪製*/
PIBlock *
DrawGear(PIFilter *iFilter)
{
	PIBlock *image;
	image = TVAllocPIBlock(iFilter, Gui.OTHER_SIZE, Gui.OTHER_SIZE, 0);
	if (image == NULL)return NULL;

	GearCode(iFilter, image);

	return image;
}

/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/* 刷新按鈕 */

/*更新按鈕圖片*/
void
TVHudRefresh(PIFilter *iFilter) {
	TVRefreshButtonImage(iFilter, Data.mReq, ID_COLORWHEEL, 0, 0, Gui.CW_SIZE, Gui.CW_SIZE);
	TVRefreshButtonImage(iFilter, Data.mReq, ID_COLORSQUARE, 0, 0, Gui.CS_SIZE, Gui.CS_SIZE);
	TVRefreshButtonImage(iFilter, Data.mReq, ID_COLORABBTN, 0, 0, Gui.C_A_SIZE, Gui.C_A_SIZE);
	TVRefreshButtonImage(iFilter, Data.mReq, ID_COLORSWITCHBTN, 0, 0, Gui.CSWITCH_SIZE, Gui.CSWITCH_SIZE);
	TVRefreshButtonImage(iFilter, Data.mReq, ID_BLACKWHITEBTN, 0, 0, Gui.BLACKWHITE_SIZE, Gui.BLACKWHITE_SIZE);
	TVRefreshButtonImage(iFilter, Data.mReq, ID_GEAR, 0, 0, Gui.OTHER_SIZE, Gui.OTHER_SIZE);
}
/**/
void
TVSliderRefresh(PIFilter *iFilter) {
	if (!Data.mIsSliderHide) {
		RefreshColorSlider(iFilter, Data.mBlockR, 'r');
		TVRefreshButtonImage(iFilter, Data.mReq, ID_SLIDER_R, 0, 0, Gui.SLIDER_W, Gui.SLIDER_H - 2 * Gui.SLIDER_OFFSET);
		TVChangeSliderReq(iFilter, Data.mReq, ID_SLIDER_R, 256, 1, floorf(mRGB.r), 1, 1, 1);

		RefreshColorSlider(iFilter, Data.mBlockG, 'g');
		TVRefreshButtonImage(iFilter, Data.mReq, ID_SLIDER_G, 0, 0, Gui.SLIDER_W, Gui.SLIDER_H - 2 * Gui.SLIDER_OFFSET);
		TVChangeSliderReq(iFilter, Data.mReq, ID_SLIDER_G, 256, 1, floorf(mRGB.g), 1, 1, 1);

		RefreshColorSlider(iFilter, Data.mBlockB, 'b');
		TVRefreshButtonImage(iFilter, Data.mReq, ID_SLIDER_B, 0, 0, Gui.SLIDER_W, Gui.SLIDER_H - 2 * Gui.SLIDER_OFFSET);
		TVChangeSliderReq(iFilter, Data.mReq, ID_SLIDER_B, 256, 1, floorf(mRGB.b), 1, 1, 1);

		RefreshColorSlider(iFilter, Data.mBlockH, 'h');
		TVRefreshButtonImage(iFilter, Data.mReq, ID_SLIDER_H, 0, 0, Gui.SLIDER_W, Gui.SLIDER_H - 2 * Gui.SLIDER_OFFSET);
		TVChangeSliderReq(iFilter, Data.mReq, ID_SLIDER_H, 361, 1, floorf(mHSV.h * 360), 1, 1, 1);

		RefreshColorSlider(iFilter, Data.mBlockS, 's');
		TVRefreshButtonImage(iFilter, Data.mReq, ID_SLIDER_S, 0, 0, Gui.SLIDER_W, Gui.SLIDER_H - 2 * Gui.SLIDER_OFFSET);
		TVChangeSliderReq(iFilter, Data.mReq, ID_SLIDER_S, 101, 1, floorf(mHSV.s * 100), 1, 1, 1);

		RefreshColorSlider(iFilter, Data.mBlockV, 'v');
		TVRefreshButtonImage(iFilter, Data.mReq, ID_SLIDER_V, 0, 0, Gui.SLIDER_W, Gui.SLIDER_H - 2 * Gui.SLIDER_OFFSET);
		TVChangeSliderReq(iFilter, Data.mReq, ID_SLIDER_V, 101, 1, floorf(mHSV.v * 100), 1, 1, 1);
	}

	RefreshABColor(iFilter, Data.mABColorBlock);

	RefreshCSwitchBtn(iFilter, Data.mCSwitchBlock);

	RefreshBlackWhiteBtn(iFilter, Data.mBlackWhiteBlock);
}

void FAR PASCAL
TVSliderNumRefresh(PIFilter *iFilter) {
	if (!Data.mIsSliderHide) {
		char tmp[256];
		//R 滑桿
		sprintf(tmp, "%d", (int)floorf(mRGB.r));
		TVChangeButtonReq(iFilter, Data.mReq, ID_NUM_R, (Data.mInputColorNum == 1) ? PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT | PIRBF_BUTTON_TEXT_ACTIVE : PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT, tmp);

		//G 滑桿
		sprintf(tmp, "%d", (int)floorf(mRGB.g));
		TVChangeButtonReq(iFilter, Data.mReq, ID_NUM_G, (Data.mInputColorNum == 2) ? PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT | PIRBF_BUTTON_TEXT_ACTIVE : PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT, tmp);

		//B 滑桿
		sprintf(tmp, "%d", (int)floorf(mRGB.b));
		TVChangeButtonReq(iFilter, Data.mReq, ID_NUM_B, (Data.mInputColorNum == 3) ? PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT | PIRBF_BUTTON_TEXT_ACTIVE : PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT, tmp);

		//H 滑桿
		sprintf(tmp, "%d", (int)floorf(mHSV.h * 360));
		TVChangeButtonReq(iFilter, Data.mReq, ID_NUM_H, (Data.mInputColorNum == 4) ? PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT | PIRBF_BUTTON_TEXT_ACTIVE : PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT, tmp);

		//S 滑桿
		sprintf(tmp, "%d", (int)floorf(mHSV.s * 100));
		TVChangeButtonReq(iFilter, Data.mReq, ID_NUM_S, (Data.mInputColorNum == 5) ? PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT | PIRBF_BUTTON_TEXT_ACTIVE : PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT, tmp);

		//V 滑桿
		sprintf(tmp, "%d", (int)floorf(mHSV.v * 100));
		TVChangeButtonReq(iFilter, Data.mReq, ID_NUM_V, (Data.mInputColorNum == 6) ? PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT | PIRBF_BUTTON_TEXT_ACTIVE : PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT, tmp);
	}

}


/* 建立視窗按鈕 */
void AddButtonReq(PIFilter *iFilter) {
	char tmp[256];

	//HUE色環
	TVAddButtonReq(iFilter, Data.mReq, Gui.CW_X, Gui.CW_Y, Gui.CW_SIZE, Gui.CW_SIZE, ID_COLORWHEEL, PIRBF_BUTTON_IMMEDIATE | PIRBF_BUTTON_FLAT | PIRBF_BUTTON_QUIET | PIRBF_BUTTON_RIGHT, NULL);
	Data.mHueBlock = DrawHue(iFilter);
	TVPutButtonImage(iFilter, Data.mReq, ID_COLORWHEEL, Data.mHueBlock, 0);
	TVSetButtonInfoText(iFilter, Data.mReq, ID_COLORWHEEL, TXT_COLORWHEEL_HELP);

	//顏色AB 區塊按鈕
	TVAddButtonReq(iFilter, Data.mReq, Gui.C_A_X, Gui.C_A_Y, Gui.C_A_SIZE, Gui.C_A_SIZE, ID_COLORABBTN, PIRBF_BUTTON_FLAT | PIRBF_BUTTON_QUIET | PIRBF_BUTTON_RIGHT, NULL);
	Data.mABColorBlock = DrawABColor(iFilter);
	TVPutButtonImage(iFilter, Data.mReq, ID_COLORABBTN, Data.mABColorBlock, 0);
	TVSetButtonInfoText(iFilter, Data.mReq, ID_COLORABBTN, TXT_COLORABBTN_HELP);

	//顏色黑白按鈕
	TVAddButtonReq(iFilter, Data.mReq, Gui.BLACKWHITE_X, Gui.BLACKWHITE_Y, Gui.BLACKWHITE_SIZE, Gui.BLACKWHITE_SIZE, ID_BLACKWHITEBTN, PIRBF_BUTTON_FLAT | PIRBF_BUTTON_QUIET | PIRBF_BUTTON_RIGHT, NULL);
	Data.mBlackWhiteBlock = DrawBlackWhiteBtn(iFilter);
	TVPutButtonImage(iFilter, Data.mReq, ID_BLACKWHITEBTN, Data.mBlackWhiteBlock, 0);
	TVSetButtonInfoText(iFilter, Data.mReq, ID_BLACKWHITEBTN, TXT_BLACKWHITEBTN_HELP);

	//顏色交換按鈕
	TVAddButtonReq(iFilter, Data.mReq, Gui.CSWITCH_X, Gui.CSWITCH_Y, Gui.CSWITCH_SIZE, Gui.CSWITCH_SIZE, ID_COLORSWITCHBTN, /*PIRBF_BUTTON_ACTION |*/ PIRBF_BUTTON_FLAT | PIRBF_BUTTON_QUIET | PIRBF_BUTTON_RIGHT, NULL);
	Data.mCSwitchBlock = DrawCSwitchBtn(iFilter);
	TVPutButtonImage(iFilter, Data.mReq, ID_COLORSWITCHBTN, Data.mCSwitchBlock, 0);
	TVSetButtonInfoText(iFilter, Data.mReq, ID_COLORSWITCHBTN, TXT_COLORSWITCH_HELP);
	
	//齒輪按鈕
	TVAddButtonReq(iFilter, Data.mReq, Gui.OTHER_X, Gui.OTHER_Y, Gui.OTHER_SIZE, Gui.OTHER_SIZE, ID_GEAR, (Data.mIsGearSelect) ? PIRBF_BUTTON_STAT | PIRBF_BUTTON_FLAT | PIRBF_BUTTON_SELECT : PIRBF_BUTTON_STAT | PIRBF_BUTTON_FLAT, NULL);
	Data.mGearBlock = DrawGear(iFilter);
	TVPutButtonImage(iFilter, Data.mReq, ID_GEAR, Data.mGearBlock, 0);
	TVSetButtonInfoText(iFilter, Data.mReq, ID_GEAR, TXT_GEAR_HELP);

	//鎖定比例
	TVAddButtonReq(iFilter, Data.mReq, Gui.LOCKRATIO_X, Gui.LOCKRATIO_Y, Gui.LOCKRATIO_SIZE, Gui.LOCKRATIO_SIZE, ID_COLORGET, (Data.mIsColorGet) ? PIRBF_BUTTON_CHECK | PIRBF_BUTTON_SELECT : PIRBF_BUTTON_CHECK, TXT_COLORGET);
	TVSetButtonInfoText(iFilter, Data.mReq, ID_COLORGET, TXT_COLORGET_HELP);

	//模式選擇按鈕
	TVAddButtonReq(iFilter, Data.mReq, Gui.POPUP_X, Gui.POPUP_Y, Gui.POPUP_W, Gui.POPUP_H, ID_POPUP, PIRBF_BUTTON_POPUP, (!Data.mIsSliderHide) ? TXT_POPUP_0 : TXT_POPUP_1);
	TVSetButtonInfoText(iFilter, Data.mReq, ID_POPUP, TXT_POPUP_HELP);

	//HSV顏色方塊
	TVAddButtonReq(iFilter, Data.mReq, Gui.CS_X, Gui.CS_Y, Gui.CS_SIZE, Gui.CS_SIZE, ID_COLORSQUARE, PIRBF_BUTTON_IMMEDIATE | PIRBF_BUTTON_FLAT | PIRBF_BUTTON_QUIET | PIRBF_BUTTON_RIGHT, NULL);
	Data.mSquareBlock = DrawHueSquare(iFilter);
	TVPutButtonImage(iFilter, Data.mReq, ID_COLORSQUARE, Data.mSquareBlock, 0);
	TVSetButtonInfoText(iFilter, Data.mReq, ID_COLORSQUARE, TXT_COLORSQUARE_HELP);



	if (!Data.mIsSliderHide) {

		//R 滑桿
		int pos = Gui.SLIDER_Y;
		sprintf(tmp, "%d", (int)floorf(mRGB.r));
		TVAddButtonReq(iFilter, Data.mReq, Gui.SLIDER_NAME_X, pos, Gui.SLIDER_NAME_W, Gui.SLIDER_H, ID_NAME_R, PIRBF_BUTTON_FLAT | PIRBF_BUTTON_QUIET, TXT_R);
		TVAddButtonReq(iFilter, Data.mReq, Gui.SLIDER_NUM_X, pos, Gui.SLIDER_NUM_W, Gui.SLIDER_H, ID_NUM_R, PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT, tmp);
		TVAddSliderReq(iFilter, Data.mReq, Gui.SLIDER_X, pos + Gui.SLIDER_OFFSET / 2, Gui.SLIDER_W, Gui.SLIDER_H - Gui.SLIDER_OFFSET, ID_SLIDER_R, PIRBF_BUTTON_DRAG, 256, 1, floorf(mRGB.r), 1, 1, 1);
		Data.mBlockR = DrawColorSlider(iFilter, 'r');
		TVPutButtonImage(iFilter, Data.mReq, ID_SLIDER_R, Data.mBlockR, 0);
		TVSetButtonInfoText(iFilter, Data.mReq, ID_NUM_R, TXT_NUM_R_HELP);
		TVSetButtonInfoText(iFilter, Data.mReq, ID_SLIDER_R, TXT_SLIDER_R_HELP);

		//G 滑桿
		pos += Gui.SLIDER_GAP_Y + Gui.SLIDER_H;
		sprintf(tmp, "%d", (int)floorf(mRGB.g));
		TVAddButtonReq(iFilter, Data.mReq, Gui.SLIDER_NAME_X, pos, Gui.SLIDER_NAME_W, Gui.SLIDER_H, ID_NAME_G, PIRBF_BUTTON_FLAT | PIRBF_BUTTON_QUIET, TXT_G);
		TVAddButtonReq(iFilter, Data.mReq, Gui.SLIDER_NUM_X, pos, Gui.SLIDER_NUM_W, Gui.SLIDER_H, ID_NUM_G, PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT, tmp);
		TVAddSliderReq(iFilter, Data.mReq, Gui.SLIDER_X, pos + Gui.SLIDER_OFFSET / 2, Gui.SLIDER_W, Gui.SLIDER_H - Gui.SLIDER_OFFSET, ID_SLIDER_G, PIRBF_BUTTON_DRAG, 256, 1, floorf(mRGB.g), 1, 1, 1);
		Data.mBlockG = DrawColorSlider(iFilter, 'g');
		TVPutButtonImage(iFilter, Data.mReq, ID_SLIDER_G, Data.mBlockG, 0);
		TVSetButtonInfoText(iFilter, Data.mReq, ID_NUM_G, TXT_NUM_G_HELP);
		TVSetButtonInfoText(iFilter, Data.mReq, ID_SLIDER_G, TXT_SLIDER_G_HELP);

		//B 滑桿
		pos += Gui.SLIDER_GAP_Y + Gui.SLIDER_H;
		sprintf(tmp, "%d", (int)floorf(mRGB.b));
		TVAddButtonReq(iFilter, Data.mReq, Gui.SLIDER_NAME_X, pos, Gui.SLIDER_NAME_W, Gui.SLIDER_H, ID_NAME_B, PIRBF_BUTTON_FLAT | PIRBF_BUTTON_QUIET, TXT_B);
		TVAddButtonReq(iFilter, Data.mReq, Gui.SLIDER_NUM_X, pos, Gui.SLIDER_NUM_W, Gui.SLIDER_H, ID_NUM_B, PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT, tmp);
		TVAddSliderReq(iFilter, Data.mReq, Gui.SLIDER_X, pos + Gui.SLIDER_OFFSET / 2, Gui.SLIDER_W, Gui.SLIDER_H - Gui.SLIDER_OFFSET, ID_SLIDER_B, PIRBF_BUTTON_DRAG, 256, 1, floorf(mRGB.b), 1, 1, 1);
		Data.mBlockB = DrawColorSlider(iFilter, 'b');
		TVPutButtonImage(iFilter, Data.mReq, ID_SLIDER_B, Data.mBlockB, 0);
		TVSetButtonInfoText(iFilter, Data.mReq, ID_NUM_B, TXT_NUM_B_HELP);
		TVSetButtonInfoText(iFilter, Data.mReq, ID_SLIDER_B, TXT_SLIDER_B_HELP);

		//H 滑桿
		pos = pos + floorf(2 * Gui.SLIDER_GAP_Y) + Gui.SLIDER_H;
		sprintf(tmp, "%d", (int)floorf(mHSV.h * 360));
		TVAddButtonReq(iFilter, Data.mReq, Gui.SLIDER_NAME_X, pos, Gui.SLIDER_NAME_W, Gui.SLIDER_H, ID_NAME_H, PIRBF_BUTTON_FLAT | PIRBF_BUTTON_QUIET, TXT_H);
		TVAddButtonReq(iFilter, Data.mReq, Gui.SLIDER_NUM_X, pos, Gui.SLIDER_NUM_W, Gui.SLIDER_H, ID_NUM_H, PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT, tmp);
		TVAddSliderReq(iFilter, Data.mReq, Gui.SLIDER_X, pos + Gui.SLIDER_OFFSET / 2, Gui.SLIDER_W, Gui.SLIDER_H - Gui.SLIDER_OFFSET, ID_SLIDER_H, PIRBF_BUTTON_DRAG, 361, 1, floorf(mHSV.h * 360), 1, 1, 1);
		Data.mBlockH = DrawColorSlider(iFilter, 'h');
		TVPutButtonImage(iFilter, Data.mReq, ID_SLIDER_H, Data.mBlockH, 0);
		TVSetButtonInfoText(iFilter, Data.mReq, ID_NUM_H, TXT_NUM_H_HELP);
		TVSetButtonInfoText(iFilter, Data.mReq, ID_SLIDER_H, TXT_SLIDER_H_HELP);

		//S 滑桿
		pos += Gui.SLIDER_GAP_Y + Gui.SLIDER_H;
		sprintf(tmp, "%d", (int)floorf(mHSV.s * 100));
		TVAddButtonReq(iFilter, Data.mReq, Gui.SLIDER_NAME_X, pos, Gui.SLIDER_NAME_W, Gui.SLIDER_H, ID_NAME_S, PIRBF_BUTTON_FLAT | PIRBF_BUTTON_QUIET, TXT_S);
		TVAddButtonReq(iFilter, Data.mReq, Gui.SLIDER_NUM_X, pos, Gui.SLIDER_NUM_W, Gui.SLIDER_H, ID_NUM_S, PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT, tmp);
		TVAddSliderReq(iFilter, Data.mReq, Gui.SLIDER_X, pos + Gui.SLIDER_OFFSET / 2, Gui.SLIDER_W, Gui.SLIDER_H - Gui.SLIDER_OFFSET, ID_SLIDER_S, PIRBF_BUTTON_DRAG, 101, 1, floorf(mHSV.s * 100), 1, 1, 1);
		Data.mBlockS = DrawColorSlider(iFilter, 's');
		TVPutButtonImage(iFilter, Data.mReq, ID_SLIDER_S, Data.mBlockS, 0);
		TVSetButtonInfoText(iFilter, Data.mReq, ID_NUM_S, TXT_NUM_S_HELP);
		TVSetButtonInfoText(iFilter, Data.mReq, ID_SLIDER_S, TXT_SLIDER_S_HELP);

		//V 滑桿
		pos += Gui.SLIDER_GAP_Y + Gui.SLIDER_H;
		sprintf(tmp, "%d", (int)floorf(mHSV.v * 100));
		TVAddButtonReq(iFilter, Data.mReq, Gui.SLIDER_NAME_X, pos, Gui.SLIDER_NAME_W, Gui.SLIDER_H, ID_NAME_V, PIRBF_BUTTON_FLAT | PIRBF_BUTTON_QUIET, TXT_V);
		TVAddButtonReq(iFilter, Data.mReq, Gui.SLIDER_NUM_X, pos, Gui.SLIDER_NUM_W, Gui.SLIDER_H, ID_NUM_V, PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_INT, tmp);
		TVAddSliderReq(iFilter, Data.mReq, Gui.SLIDER_X, pos + Gui.SLIDER_OFFSET / 2, Gui.SLIDER_W, Gui.SLIDER_H - Gui.SLIDER_OFFSET, ID_SLIDER_V, PIRBF_BUTTON_DRAG, 101, 1, floorf(mHSV.v * 100), 1, 1, 1);
		Data.mBlockV = DrawColorSlider(iFilter, 'v');
		TVPutButtonImage(iFilter, Data.mReq, ID_SLIDER_V, Data.mBlockV, 0);
		TVSetButtonInfoText(iFilter, Data.mReq, ID_NUM_V, TXT_NUM_V_HELP);
		TVSetButtonInfoText(iFilter, Data.mReq, ID_SLIDER_V, TXT_SLIDER_V_HELP);
	}
}

/*刪除所有按鈕-> 計算視窗大小-> 生成所有按鈕*/
void FAR PASCAL
TVRedrawAllButton(PIFilter* iFilter, int w, int h, int x, int y) {
	TVRemoveButtonReq(iFilter, Data.mReq, ID_COLORABBTN);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_BLACKWHITEBTN);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_COLORWHEEL);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_COLORSQUARE);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_COLORSWITCHBTN);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_COLORGET);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_POPUP);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_GEAR);

	TVRemoveButtonReq(iFilter, Data.mReq, ID_NAME_R);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_NUM_R);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_SLIDER_R);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_NAME_G);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_NUM_G);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_SLIDER_G);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_NAME_B);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_NUM_B);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_SLIDER_B);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_NAME_H);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_NUM_H);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_SLIDER_H);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_NAME_S);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_NUM_S);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_SLIDER_S);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_NAME_V);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_NUM_V);
	TVRemoveButtonReq(iFilter, Data.mReq, ID_SLIDER_V);


	TVResizeGUI(iFilter, w, h);
	TVResizeReq(iFilter, Data.mReq, x, y, Gui.REQUESTER_W, Gui.REQUESTER_H);
	AddButtonReq(iFilter);
}

void RefreshGUI(PIFilter* iFilter) {
	TVHudRefresh(iFilter);
	TVSendCmd(iFilter, " tv_UnLockMouse", NULL);
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

/**************************************************************************************/

void FAR PASCAL
PI_About(PIFilter* iFilter)
{
	char  text[256];

	sprintf(text, "%s %d,%d", iFilter->PIName, iFilter->PIVersion, iFilter->PIRevision);

	// Just open a warning popup with the filter name and version.
	// You can open a much nicer requester if you want.
	TVWarning(iFilter, text);
}

/**************************************************************************************/

int FAR PASCAL
PI_Open(PIFilter* iFilter)
{
	char	tmp[256];
	char	tmprgb[256];

	TVSendCmd(iFilter, "tv_getlanguage", tmp);
	if (!strcmp(tmp, "chinese") || !strcmp(tmp, "繁體中文") || !strcmp(tmp, "Yanun")) {
		Data.mLocalFile = 0;
		//Data.mLocalFile = TVOpenLocalFile(iFilter, "Color Picker.chinese.loc", 0);
	}
	else {
		Data.mLocalFile = 1;
		//Data.mLocalFile = TVOpenLocalFile(iFilter, "Color Picker.english.loc", 0);
	}
	// Load the .loc file.
	// We don't really cares if it fails here, since we do care in GetLocalString()

	strcpy(iFilter->PIName, TXT_NAME);
	iFilter->PIVersion = 1;
	iFilter->PIRevision = 1;

	// If this plugin was the one open at Aura shutdown, re-open it
	// 如果此插件是在TVPaint關閉時打開的插件，請重新打開它

	TVReadUserString(iFilter, iFilter->PIName, "Open", tmp, "0", 255);
	if (atoi(tmp))
	{
		PI_Parameters(iFilter, NULL); // NULL as iArg means "open the requester"
	}

	//TVCreateFilterAlias(iFilter, "手翻書", "f");

	/*
	TVReadUserString(iFilter, "Main", "apen", tmprgb, "0", 256);
	TVReadUserString(iFilter, "SysColors", "mreqbackgfloorf", tmprgb, "0", 256);
	sscanf(tmprgb, "%f %f %f", &mRGB.r, &mRGB.g, &mRGB.b);
	mRGB.a = 255;
	*/

	return  1; // OK
}

/**************************************************************************************/

void FAR PASCAL
PI_Close(PIFilter* iFilter)
{
	if (Data.mReq)
	{
		TVCloseReq(iFilter, Data.mReq);
		TVCloseReq(iFilter, Data.mReqPick);
		TVCloseReq(iFilter, Data.mReqOther);
		TVGrabCoords(iFilter, Data.mReq, 0);
		TVGrabTicks(iFilter, Data.mReq, 0);
	}
}


/**************************************************************************************/


int FAR PASCAL
PI_Parameters(PIFilter* iFilter, char* iArg)
{
	if (Data.mReq == 0)
	{
		TVWorkOn(iFilter, NULL);

		char tmp[256], tmprgb[256];

		TVReadUserString(iFilter, "SysColors", "mdocumentbackground", tmprgb, "255 255 255 255", 256);
		sscanf(tmprgb, "%f %f %f %d", &mRGBreq.r, &mRGBreq.g, &mRGBreq.b, &mRGBreq.a);

		TVReadUserString(iFilter, "Main", "apen", tmprgb, "0 0 0", 256);
		sscanf(tmprgb, "%f %f %f", &mRGB.r, &mRGB.g, &mRGB.b);
		mRGB.a = 255;

		TVReadUserString(iFilter, "Main", "bpen", tmprgb, "0 0 0", 256);
		sscanf(tmprgb, "%f %f %f", &mRGB2.r, &mRGB2.g, &mRGB2.b);
		mRGB2.a = 255;

		mHSV = csRGB2HSV2(iFilter, mRGB, mHSV);
		mHSV2 = csRGB2HSV2(iFilter, mRGB2, mHSV2);

		TVReadUserString(iFilter, iFilter->PIName, "mIsColorGet", tmp, "0", 256);
		Data.mIsColorGet = atoi(tmp);

		TVReadUserString(iFilter, iFilter->PIName, "mModeSelect", tmp, "0", 256);
		Data.mModeSelect = atoi(tmp);

		TVReadUserString(iFilter, iFilter->PIName, "mIsSliderHide", tmp, "0", 256);
		Data.mIsSliderHide = atoi(tmp);

		/* X/Y */
		TVReadUserString(iFilter, iFilter->PIName, "mRequestX", tmp, "150", 256);
		Data.mRequestX = atoi(tmp);

		TVReadUserString(iFilter, iFilter->PIName, "mRequestY", tmp, "50", 256);
		Data.mRequestY = atoi(tmp);

		/* W/H */
		TVReadUserString(iFilter, iFilter->PIName, "mRequestWidth", tmp, "0", 256);
		Data.mRequestWidth = atoi(tmp);
		TVReadUserString(iFilter, iFilter->PIName, "mRequestHeight", tmp, "0", 256);
		Data.mRequestHeight = atoi(tmp);

		TVResizeGUI(iFilter, Data.mRequestWidth, Data.mRequestHeight);
		//DWORD req = TVOpenFilterReqEx(iFilter, Data.mRequestWidth, Data.mRequestHeight, NULL, NULL, PIRF_STANDARD_REQ | PIRF_RESIZE_REQ | PIRF_COLLAPSABLE_REQ, FILTERREQ_NO_PATH | FILTERREQ_NO_FILE | FILTERREQ_NO_TBAR);
		DWORD req = TVOpenReq(iFilter, Data.mRequestWidth, Data.mRequestHeight, Data.mRequestX, Data.mRequestY, PIRF_STANDARD_REQ | PIRF_RESIZE_REQ | PIRF_COLLAPSABLE_REQ, NULL);

		if (req == 0)
		{
			TVWarning(iFilter, TXT_ERROR);
			return  0;
		}
		Data.mReq = req;


		TVSetReqTitle(iFilter, Data.mReq, TXT_REQUESTER);

		(!Data.mIsColorGet) ? TVGrabTicks(iFilter, Data.mReq, 0) : TVGrabTicks(iFilter, Data.mReq, 1);

		AddButtonReq(iFilter);
		TVSliderNumRefresh(iFilter);
		TVSliderRefresh(iFilter);

	}
	else
	{
		TVReqToFront(iFilter, Data.mReq);
	}
		
	

	return  1;
}

/**************************************************************************************/


int FAR PASCAL
PI_Msg(PIFilter* iFilter, DWORD iEvent, DWORD iReq, DWORD* iArgs)
{

	if (iEvent == PICBREQ_BUTTON_DOWN && iArgs[0] == ID_SLIDER_R) {
		iEvent = PICBREQ_SLIDER_MOVE;
		iArgs[1] = floorf((iArgs[1] - Gui.SLIDER_X) * 260 / Gui.SLIDER_W);
		iArgs[5] = 2;
	}
	else if (iEvent == PICBREQ_BUTTON_DOWN && iArgs[0] == ID_SLIDER_G) {
		iEvent = PICBREQ_SLIDER_MOVE;
		iArgs[1] = floorf((iArgs[1] - Gui.SLIDER_X) * 260 / Gui.SLIDER_W);
		iArgs[5] = 2;
	}
	else if (iEvent == PICBREQ_BUTTON_DOWN && iArgs[0] == ID_SLIDER_B) {
		iEvent = PICBREQ_SLIDER_MOVE;
		iArgs[1] = floorf((iArgs[1] - Gui.SLIDER_X) * 260 / Gui.SLIDER_W);
		iArgs[5] = 2;
	}
	else if (iEvent == PICBREQ_BUTTON_DOWN && iArgs[0] == ID_SLIDER_H) {
		iEvent = PICBREQ_SLIDER_MOVE;
		iArgs[1] = floorf((iArgs[1] - Gui.SLIDER_X) * 365 / Gui.SLIDER_W);
		iArgs[5] = 2;
	}
	else if (iEvent == PICBREQ_BUTTON_DOWN && iArgs[0] == ID_SLIDER_S) {
		iEvent = PICBREQ_SLIDER_MOVE;
		iArgs[1] = floorf((iArgs[1] - Gui.SLIDER_X) * 105 / Gui.SLIDER_W);
		iArgs[5] = 2;
	}
	else if (iEvent == PICBREQ_BUTTON_DOWN && iArgs[0] == ID_SLIDER_V) {
		iEvent = PICBREQ_SLIDER_MOVE;
		iArgs[1] = floorf((iArgs[1] - Gui.SLIDER_X) * 105 / Gui.SLIDER_W);
		iArgs[5] = 2;
	}

	switch (iEvent)
	{
		//按下按鈕
	case PICBREQ_BUTTON_DOWN:
	{
		switch (iArgs[0])   // iArgs[0] is the ID of the selected button
		{
		case ID_COLORWHEEL:
			Data.mHueClickX = iArgs[1];
			Data.mHueClickY = iArgs[2];
			if (pow((Data.mHueClickX - Gui.CW_RADIUS), 2) + pow((Data.mHueClickY - Gui.CW_RADIUS), 2) <= pow(Gui.CW_RADIUS, 2)) {
				if (pow((Data.mHueClickX - Gui.CW_RADIUS), 2) + pow((Data.mHueClickY - Gui.CW_RADIUS), 2) >= pow((Gui.CW_RADIUS - Gui.CW_GAP), 2)) {
					Data.mIsHueClick = 1;
					Data.mSlideHueBtn = 1;
					TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
					PI_Work(iFilter);
				}
			}
			break;
		case ID_COLORSQUARE:
			Data.mSquaClickX = iArgs[1];
			Data.mSquaClickY = iArgs[2];
			if (Data.mSquaClickX >= 0 && Data.mSquaClickY >= 0 && Data.mSquaClickX <= Gui.CS_SIZE && Data.mSquaClickY <= Gui.CS_SIZE) {
				Data.mIsSquaClick = 1;
				Data.mSlideSquaBtn = 1;
				TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
				PI_Work(iFilter);
			}
			break;
		}
	}
	break;

	//按著按鈕移動執行動作
	case PICBREQ_MOVE: // id, x, y, mousebutton, pressure, timer, CTRLKey, fx, fy
	{
		switch (iArgs[0])   // iArgs[0] is the ID of the selected button
		{
		case ID_COLORWHEEL:
			Data.mHueClickX = ((iArgs[7] / (float)65536 > 30000) ? (float)0 : iArgs[7] / (float)65536);
			Data.mHueClickY = ((iArgs[8] / (float)65536 > 30000) ? (float)0 : iArgs[8] / (float)65536);
			if (Data.mIsHueClick) {
				Data.mSlideHueBtn = 1;
				TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
				PI_Work(iFilter);
			}
			else
				return 0;


			break;

		case ID_COLORSQUARE:
			Data.mSquaClickX = ((iArgs[7] / (float)65536 > 30000) ? (float)0 : iArgs[7] / (float)65536);
			Data.mSquaClickY = ((iArgs[8] / (float)65536 > 30000) ? (float)0 : iArgs[8] / (float)65536);
			if (Data.mIsSquaClick) {
				Data.mSlideSquaBtn = 1;
				TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
				PI_Work(iFilter);
			}
			else
				return 0;

			break;



		}

	}
	break;

	//放開按鈕執行動作
	case PICBREQ_BUTTON_UP:
	{
		switch (iArgs[0])   // iArgs[0] is the ID of the selected button
		{
		case ID_COLORWHEEL: {
			Data.mIsHueClick = 0;
			}
			break;

		case ID_COLORSQUARE: {
			Data.mIsSquaClick = 0;
			}
			break;

		case ID_COLORSWITCHBTN: {
			char tmp[256];
			sprintf(tmp, "%d", iArgs[3]);
				if (!strcmp(tmp,"2"))/*右鍵點選時*/ {
					TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
					mRGB2 = mRGB;
					mHSV2 = csRGB2HSV(iFilter, mRGB2);
					TVSetBPen(iFilter, mRGB2);

					Data.mIsRefresh = 1;
					PI_Work(iFilter);
					TVSendCmd(iFilter, " tv_info AAAA", NULL);
				}
				else {
					TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
					Data.mIsColorSwitchOn = 1;
					PI_Work(iFilter);
				}
			}
			break;
		case ID_COLORABBTN: {
			if (!Data.mABPenSelected)/*選A時*/ {
				if (pow(iArgs[1] - floorf(Gui.C_A_SIZE * 2 / 5), 2) + pow(iArgs[2] - floorf(Gui.C_A_SIZE * 2 / 5), 2) <= pow(Gui.C_A_RADIUS * 4 / 5 - (float)0.8, 2)) {
					
					TVSendCmd(iFilter, " tv_LockMouse 21", NULL);
					TVPickColor(iFilter, &mPIPtmp);

					TVRemoveButtonReq(iFilter, Data.mReqPick, ID_PICKGUIBTN);
					TVCloseReq(iFilter, Data.mReqPick);
					Data.mReqPick = 0;
					PI_Work(iFilter);

				}
				else if (pow(iArgs[1] - floorf(Gui.C_A_SIZE * 3 / 4), 2) + pow(iArgs[2] - floorf(Gui.C_A_SIZE * 3 / 4), 2) <= pow(Gui.C_B_RADIUS, 2)) {
					TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
					Data.mABPenSelected = 1;
					Data.mIsRefresh = 1;
					SwitchABPen(iFilter);
					PI_Work(iFilter);
				}
			}
			else /*選B時*/ {
				if (pow(iArgs[1] - floorf(Gui.C_A_SIZE * 3 / 4), 2) + pow(iArgs[2] - floorf(Gui.C_A_SIZE * 3 / 4), 2) <= pow(Gui.C_B_RADIUS, 2)) {
					/*吸色*/
					TVSendCmd(iFilter, " tv_LockMouse 21", NULL);
					TVPickColor(iFilter, &mPIPtmp);

					TVRemoveButtonReq(iFilter, Data.mReqPick, ID_PICKGUIBTN);
					TVCloseReq(iFilter, Data.mReqPick);
					Data.mReqPick = 0;
					PI_Work(iFilter);
				}
				else if (pow(iArgs[1] - floorf(Gui.C_A_SIZE * 2 / 5), 2) + pow(iArgs[2] - floorf(Gui.C_A_SIZE * 2 / 5), 2) <= pow(Gui.C_A_RADIUS * 4 / 5 - (float)0.8, 2)) {
					TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
					Data.mABPenSelected = 0;
					Data.mIsRefresh = 1;
					SwitchABPen(iFilter);
					PI_Work(iFilter);
				}
			}
			}
			break;

		case ID_BLACKWHITEBTN: {
			TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
			Data.mBlackWhiteRefresh = 1;
			PI_Work(iFilter);
			}
			break;

		case ID_GEAR: {
				int di = TVDemand(iFilter, TXT_INFO, TXT_INFO_CLOSE, TXT_INFO_MORE);
				if (!di) {
					Data.mReqOther = TVOpenReq(iFilter, 350, 60, NULL, NULL, PIRF_CENTER_REQ | PIRF_NO_DEPTH_REQ, TXT_INFO_T1);
					TVAddButtonReq(iFilter, Data.mReqOther, 10, 10, 330, 20, ID_LSK_TXT, PIRBF_BUTTON_TEXT | PIRBF_BUTTON_TEXT_LOCK, TXT_INFO_T2);
					TVAddButtonReq(iFilter, Data.mReqOther, 150, 35, 50, 20, ID_INFO_OK, PIRBF_BUTTON_ACTION | PIRBF_BUTTON_FLAT, "OK");
				}
			}
			break;

		case ID_INFO_OK: {
			TVRemoveButtonReq(iFilter, Data.mReqOther, ID_LSK_TXT);
			TVRemoveButtonReq(iFilter, Data.mReqOther, ID_INFO_OK);
			TVCloseReq(iFilter, Data.mReqOther);
			}
			break;

		case ID_COLORGET: {

			if (!Data.mIsColorGet) {
				TVChangeButtonFlags(iFilter, Data.mReq, ID_COLORGET, PIRBF_BUTTON_CHECK | PIRBF_BUTTON_SELECT);
				TVGrabTicks(iFilter, Data.mReq, 1);
				Data.mIsColorGet = 1;
			}
			else {
				TVChangeButtonFlags(iFilter, Data.mReq, ID_COLORGET, PIRBF_BUTTON_CHECK);
				TVGrabTicks(iFilter, Data.mReq, 0);
				Data.mIsColorGet = 0;
			}

			char tmp[256];
			sprintf(tmp, "%d", Data.mIsColorGet);
			TVWriteUserString(iFilter, iFilter->PIName, "mIsColorGet", tmp);

			}
			break;
			
		case ID_POPUP:{
			PIPopup Popup[] =
			{
			{ TXT_POPUP_0, 61, PIPOPMODE_CHECK* !Data.mIsSliderHide },
			{ TXT_POPUP_1, 62, PIPOPMODE_CHECK* Data.mIsSliderHide },
			};
			char tmp[256];
			int choise, x, y, w, h;
			choise = TVPopup(iFilter, Popup, 2, 0);

			switch (choise) {
			case 61:
				if (Data.mIsSliderHide) {
					TVChangeButtonName(iFilter, Data.mReq, ID_POPUP, Popup[0].Name, 0);
					Data.mIsSliderHide = 2;

					TVInfoReq(iFilter, Data.mReq, &x, &y, &w, &h);
					TVRedrawAllButton(iFilter, w, h, x, y);
					Data.mRequestWidth = Gui.REQUESTER_W;
					Data.mRequestHeight = Gui.REQUESTER_H;
				}
				break;
			case 62:
				if (!Data.mIsSliderHide) {
					TVChangeButtonName(iFilter, Data.mReq, ID_POPUP, Popup[1].Name, 0);
					Data.mIsSliderHide = 1;

					TVInfoReq(iFilter, Data.mReq, &x, &y, &w, &h);
					TVRedrawAllButton(iFilter, w, h, x, y);
					Data.mRequestWidth = Gui.REQUESTER_W;
					Data.mRequestHeight = Gui.REQUESTER_H;
				}
				break;
			}

			sprintf(tmp, "%d", Data.mIsSliderHide);
			TVWriteUserString(iFilter, iFilter->PIName, "mIsSliderHide", tmp);

			}
			break;

		}
	}
	break;

	//滑桿移動
	case PICBREQ_SLIDER_MOVE:{
		switch (iArgs[0]) {

		case ID_SLIDER_R:
			TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
			mRGB.r = iArgs[1];
			mHSV = csRGB2HSV2(iFilter, mRGB, mHSV);
			Data.mIsRefresh = 1;
			break;

		case ID_SLIDER_G:
			TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
			mRGB.g = iArgs[1];
			mHSV = csRGB2HSV2(iFilter, mRGB, mHSV);
			Data.mIsRefresh = 1;
			break;

		case ID_SLIDER_B:
			TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
			mRGB.b = iArgs[1];
			mHSV = csRGB2HSV2(iFilter, mRGB, mHSV);
			Data.mIsRefresh = 1;
			break;

		case ID_SLIDER_H:
			TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
			mHSV.h = iArgs[1] / (float)360;
			mRGB = csHSV2RGB(iFilter, mHSV);
			Data.mIsRefresh = 1;
			break;

		case ID_SLIDER_S:
			TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
			mHSV.s = iArgs[1] / (float)100;
			mRGB = csHSV2RGB(iFilter, mHSV);
			Data.mIsRefresh = 1;
			break;

		case ID_SLIDER_V:
			TVSendCmd(iFilter, " tv_LockMouse 0", NULL);
			mHSV.v = iArgs[1] / (float)100;
			mRGB = csHSV2RGB(iFilter, mHSV);
			Data.mIsRefresh = 1;
			break;
		}
		PI_Work(iFilter);
	}
	break;

	//滑桿移動
	case PICBREQ_SLIDER_RELEASE:{
		switch (iArgs[0]) {

		case ID_SLIDER_R:
			Data.mIsRefresh = 0;
			break;

		case ID_SLIDER_G:
			Data.mIsRefresh = 0;
			break;

		case ID_SLIDER_B:
			Data.mIsRefresh = 0;
			break;

		case ID_SLIDER_H:
			Data.mIsRefresh = 0;
			break;

		case ID_SLIDER_S:
			Data.mIsRefresh = 0;
			break;

		case ID_SLIDER_V:
			Data.mIsRefresh = 0;
			break;
		}

	}
	break;

	case PICBREQ_BUTTON_TEXT:{
		char tmp[256];
		switch (iArgs[0]) {
		case ID_NUM_R:
			Data.mInputColorNum = 1;
			break;

		case ID_NUM_G:
			Data.mInputColorNum = 2;
			break;

		case ID_NUM_B:
			Data.mInputColorNum = 3;
			break;

		case ID_NUM_H:
			Data.mInputColorNum = 4;
			break;

		case ID_NUM_S:
			Data.mInputColorNum = 5;
			break;

		case ID_NUM_V:
			Data.mInputColorNum = 6;
			break;

		PI_Work(iFilter);
		}

	}
	break;

	case PICBREQ_DRAG:{
		char tmp[256];

		sprintf(tmp, "%d", iArgs[0]);
		Data.mRequestX = tmp;
		TVWriteUserString(iFilter, iFilter->PIName, "mRequestX", tmp);
		sprintf(tmp, "%d", iArgs[1]);
		Data.mRequestY = tmp;
		TVWriteUserString(iFilter, iFilter->PIName, "mRequestY", tmp);
	}
	break;

	case PICBREQ_RESIZE:{
		char tmp[256];
		TVRedrawAllButton(iFilter, iArgs[0], iArgs[1], iArgs[2], iArgs[3]);

		Data.mRequestWidth = Gui.REQUESTER_W;
		Data.mRequestHeight = Gui.REQUESTER_H;

		sprintf(tmp, "%d", Data.mRequestWidth);
		TVWriteUserString(iFilter, iFilter->PIName, "mRequestWidth", tmp);
		sprintf(tmp, "%d", Data.mRequestHeight);
		TVWriteUserString(iFilter, iFilter->PIName, "mRequestHeight", tmp);

		sprintf(tmp, "%d", iArgs[2]);
		Data.mRequestX = tmp;
		TVWriteUserString(iFilter, iFilter->PIName, "mRequestX", tmp);
		sprintf(tmp, "%d", iArgs[3]);
		Data.mRequestY = tmp;
		TVWriteUserString(iFilter, iFilter->PIName, "mRequestY", tmp);


	}
	break;

	case PICB_PICK_COLOR:{
		char tmp[256];
		if (Data.mReqPick == 0) {
			Data.mIsPickMode = 1;
			Data.mReqPick = TVOpenReq(iFilter, Gui.PICK_W + 1, Gui.PICK_H + 1, (iArgs[0] / 65536) + Gui.PICK_X, (iArgs[1] / 65536) + Gui.PICK_Y, PIRF_NO_SYSBUTTON_REQ, NULL);
			TVAddButtonReq(iFilter, Data.mReqPick, 0, 0, Gui.PICK_W, Gui.PICK_H, ID_PICKGUIBTN, PIRBF_BUTTON_QUIET, NULL);
			Data.mPickBlock = DrawPickGUI(iFilter);
			TVPutButtonImage(iFilter, Data.mReqPick, ID_PICKGUIBTN, Data.mPickBlock, 0);
		}
		else {
			Data.mIsPickMode = 1;
			TVReqToFront(iFilter, Data.mReqPick);
			TVMoveReq(iFilter, Data.mReqPick, (iArgs[0] / 65536) + Gui.PICK_X, (iArgs[1] / 65536) + Gui.PICK_Y);
			RefreshPickGUI(iFilter, Data.mPickBlock);
			TVRefreshButtonImage(iFilter, Data.mReqPick, ID_PICKGUIBTN, 0, 0, Gui.PICK_W, Gui.PICK_H);
		}
		mRGBtmp = csHEX2RGB(iFilter, iArgs[5]);

		switch (iArgs[2]) {
		case 1:
			mRGB = mRGBtmp;
			mHSV = csRGB2HSV(iFilter, mRGB);

			Data.mIsRefresh = 1;
			PI_Work(iFilter);
			break;
		case 2:
			TVSendCmd(iFilter, " tv_UnLockMouse", NULL);
			PI_Work(iFilter);
			break;
		}


	}
	break;

	case PICBREQ_FKEY_DOWN: {
		TVWarning(iFilter, "!!");
	}
	break;

	case PICBREQ_ROOM_CHANGE:{
		char tmp[256];
		//TVWarning(iFilter, iArgs[0]);
		//sscanf(iArgs[0], "%d", &Data.mRoom);
		sprintf(&Data.mRoom, "%s", iArgs[0]);
		//Data.mRoom =(char)iArgs[0];
		TVReadUserString(iFilter, iArgs[0], "colorssheme-mdocumentbackground", tmp, "255 255 255 255", 256);
		sscanf(tmp, "%f %f %f %d", &mRGBreq.r, &mRGBreq.g, &mRGBreq.b, &mRGBreq.a);

		Data.mIsRefresh = 1;
		PI_Work(iFilter);

	}
	break;

	case PICBREQ_TICKS:{
		PI_Work(iFilter);
	}
	break;


	// The requester was just closed.
	case PICBREQ_CLOSE:
	{
		if (Data.mReqPick != 0) {
			Data.mReqPick == 0;
		}
		else if (Data.mReqOther != 0) {
			Data.mReqOther = 0;
		}
		else {
			char  tmp[256];

			Data.mReq = 0;

			sprintf(tmp, "%d", (int)(iArgs[4]));

			TVWriteUserString(iFilter, iFilter->PIName, "Open", tmp);
		}
	}
	break;

	}
	return  1;
}

/**************************************************************************************/

int FAR PASCAL
PI_Work(PIFilter* iFilter)
{
	if (Data.mIsRefresh) {
		TVSetAPen(iFilter, mRGB);

		TVSliderNumRefresh(iFilter);
		TVSliderRefresh(iFilter);
		RefreshHue(iFilter, Data.mHueBlock);
		RefreshHueSquare(iFilter, Data.mSquareBlock);

		Data.mIsRefresh = 0;

		RefreshGUI(iFilter);
	}
	else if (Data.mIsColorSwitchOn) {
		SwitchABPen(iFilter);
		TVSliderNumRefresh(iFilter);
		TVSliderRefresh(iFilter);
		RefreshHue(iFilter, Data.mHueBlock);
		RefreshHueSquare(iFilter, Data.mSquareBlock);
		Data.mIsColorSwitchOn = 0;

		RefreshGUI(iFilter);
	}
	else if (Data.mBlackWhiteRefresh) {
		if (!Data.mABPenSelected) {
			mRGB.r = 0;
			mRGB.g = 0;
			mRGB.b = 0;
			mRGB2.r = 255;
			mRGB2.g = 255;
			mRGB2.b = 255;
			mHSV = csRGB2HSV2(iFilter, mRGB, mHSV);
			mHSV2 = csRGB2HSV2(iFilter, mRGB2, mHSV2);

			TVSendCmd(iFilter, "tv_SetAPen 0 0 0", NULL);
			TVSendCmd(iFilter, "tv_SetBPen 255 255 255", NULL);
		}
		else if (Data.mABPenSelected) {
			mRGB.r = 255;
			mRGB.g = 255;
			mRGB.b = 255;
			mRGB2.r = 0;
			mRGB2.g = 0;
			mRGB2.b = 0;
			mHSV = csRGB2HSV2(iFilter, mRGB, mHSV);
			mHSV2 = csRGB2HSV2(iFilter, mRGB2, mHSV2);

			TVSendCmd(iFilter, "tv_SetAPen 255 255 255", NULL);
			TVSendCmd(iFilter, "tv_SetBPen 0 0 0", NULL);
		}

		TVSliderNumRefresh(iFilter);
		TVSliderRefresh(iFilter);
		RefreshHue(iFilter, Data.mHueBlock);
		RefreshHueSquare(iFilter, Data.mSquareBlock);

		Data.mBlackWhiteRefresh = 0;

		RefreshGUI(iFilter);
	}

	else if (Data.mSlideHueBtn) {
		SlideHue(iFilter, Data.mHueBlock);
		TVSliderNumRefresh(iFilter);
		TVSliderRefresh(iFilter);
		RefreshHueSquare(iFilter, Data.mSquareBlock);
		Data.mSlideHueBtn = 0;

		RefreshGUI(iFilter);
	}
	else if (Data.mSlideSquaBtn) {
		SlideSquare(iFilter, Data.mSquareBlock);
		TVSliderNumRefresh(iFilter);
		TVSliderRefresh(iFilter);
		Data.mSlideSquaBtn = 0;

		RefreshGUI(iFilter);
	}
	else if (Data.mInputColorNum != 0) {
		InputColor(iFilter);
		TVSetAPen(iFilter, mRGB);
		TVSliderNumRefresh(iFilter);
		TVSliderRefresh(iFilter);
		RefreshHue(iFilter, Data.mHueBlock);
		RefreshHueSquare(iFilter, Data.mSquareBlock);
		Data.mInputColorNum = 0;

		RefreshGUI(iFilter);
	}
	else {
		mRGBtmp = GetAPenRGB(iFilter);

		if (mRGBtmp.r != mRGB.r || mRGBtmp.g != mRGB.g || mRGBtmp.b != mRGB.b) {
			mRGB = mRGBtmp;
			mHSV = csRGB2HSV(iFilter, mRGB);
			RefreshHue(iFilter, Data.mHueBlock);
			RefreshHueSquare(iFilter, Data.mSquareBlock);
			TVSliderNumRefresh(iFilter);
			TVSliderRefresh(iFilter);

			RefreshGUI(iFilter);
		}
	}
	return  1;
}