#include <FastLED.h>
#define random8or16	random8

FASTLED_USING_NAMESPACE

// used demo reel by -Mark Kriegsman,  and some other rewriten

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

void text_arr_RU_greetings();
	#define MATRIXfonth
							//#define textMATRIX_overlay
	#define WIDTH	16
	#define HEIGHT	15
	//#define WIDTH	32
	//#define HEIGHT	8
	#define bMatrixZigZag 1
	#define CONNECTION_ANGLE 0//1    // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
	#define STRIP_DIRECTION  1//3//(0 for USB view. TODO use STRIP_DIRECTION in USB app )    // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
	/* //for USB view w matrix_HORIZONTAL=true;
	#define CONNECTION_ANGLE 1 
	#define STRIP_DIRECTION  0
	*/
	
	//#define startLoadEffN  222
	//#define use_ESP8266
	//	#define use_ESP_WiFioff

	#define LEDs_RENDER
	#define gNUM_LEDS 		WIDTH*HEIGHT
	#define NUM_LEDS_type 		uint16_t

	// #define gDelayMore	5	

	// #define demo_enable
	// byte random_demo_sw_speed_td_m=3;
	// byte random_demo_sw_speed_td_M=15;

	#define FASTLED_ESP8266_RAW_PIN_ORDER
	
#define LEDp    4
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    WIDTH*HEIGHT//150
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

#define standalone
#include "eff__variables.h"



void rainbow2();
void rainbow2anim();
void rainbowWithGlitter2();
void confetti3();
void sinelon3();
void juggle2();
void juggle2();
void bpm2();

void eff_fade_d();

void move_p1();
void rgb_firework_tick();
void rgb_rainbow_tick();
void rgb_stars_tick();

void wavesOppositePos();
void wavesOppositePosM();

void cloudTwinkles();
void rainbowTwinkles();
void snowTwinkles();
void incandescentTwinkles();
void pride();

void addGlitter2(fract8 N);
void nextPattern();
void selectPattern(byte N);

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {
	#if defined(MATRIXfonth)
	text_arr_RU_greetings,
	#endif
	
	//rainbow2,	eff_fade_d,
	cloudTwinkles,
	pride,
	rainbowTwinkles,
	
	rainbowWithGlitter2, eff_fade_d, confetti3, sinelon3,  bpm2,
	//juggle2,
	eff_fade_d, 
	snowTwinkles, 
	incandescentTwinkles, 
	
	move_p1,
	move_p1,
	move_p1, eff_fade_d, 
	rgb_firework_tick, eff_fade_d, 
	rgb_rainbow_tick, eff_fade_d,
	rgb_stars_tick, eff_fade_d,
	wavesOppositePosM,
	wavesOppositePos,
	eff_fade_d
 };


uint8_t gCurrentPatternNumber = 0; 
uint8_t rgb_effects_stage=0;
uint16_t effects_i; 
byte fade=20;
 

void moveOutAllRemainFirst()
{
	for(byte i = NUM_LEDS-1; i >0 ; i--) 
	{
		leds[i]=leds[i-1];
	}
}


#include "eff_set_matrix_RU_font.h"
#include "pride.h"
#include "Twinkles.h"

void setup() {
  delay(1000);
  
  FastLED.addLeds<LED_TYPE,LEDp,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  FastLED.setBrightness(BRIGHTNESS);
}



void loop()
{
	DemoReel101();

	FastLED.show();  

	FastLED.delay(1000/FRAMES_PER_SECOND); 
}

byte gHue2 = 0; // rotating "base color" used by many of the patterns
  
long pattern_il = 0; 

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


int gHue2incr=1;

long nextPattern_t=10000;
byte eff_stage2=0;
void DemoReel101()
{
	//EVERY_N_MILLISECONDS( 10 ) 
	{
		gPatterns[gCurrentPatternNumber]();
		effects_i++;
	}
																				
	EVERY_N_MILLISECONDS( 20 ) { gHue2+=gHue2incr; } // slowly cycle the "base color" through the rainbow

	if(millis()>nextPattern_t) nextPattern();
}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  pattern_il=0;
  effects_i=0;
  
  eff_stage2++;
  
  if(gPatterns[gCurrentPatternNumber]== eff_fade_d) nextPattern_t=millis()+2000;
  else
  if(gPatterns[gCurrentPatternNumber]== rgb_stars_tick) nextPattern_t=millis()+18000;
  else
  if(gPatterns[gCurrentPatternNumber]== rgb_rainbow_tick) nextPattern_t=millis()+18000;
  else nextPattern_t=millis()+10000;
}
void selectPattern(byte N)
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (N) % ARRAY_SIZE( gPatterns);
}


static uint16_t var1, var2, var3;
static uint16_t DotPos;
static uint16_t DotPos2;
static CRGB color1, color2;


byte blend_amount=0;
uint16_t eff_stage;
int eff_ticks=0;
  
byte FADER_iM=122;
int eff_ticksM=11;


void rainbow2() 
{
  
  fill_rainbow( leds, NUM_LEDS, gHue2, 7);
}

void rainbowWithGlitter2() 
{
  
  rainbow2();
  addGlitter2(80);
}

void addGlitter2( fract8 chanceOfGlitter) 
{
  //if( random8() < chanceOfGlitter)
  {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti3() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue2 + random8(64), 200, 255);
}

void sinelon3()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue2, 255, 192);
}

void bpm2()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue2+(i*2), beat-gHue2+(i*10));
  }
}

void juggle2() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void fillBetween(uint16_t x1, uint16_t x2, byte h)
{
	uint16_t m1=x1<x2?x1:x2;
	uint16_t M1=x1>x2?x1:x2;
	
	for(uint16_t i=m1;i<M1;i++)
	{
		leds[i%NUM_LEDS]+=CHSV(h, 255,222);
	}
}
void fillBetween_mM(uint16_t m, uint16_t M, byte h)
{
	for(uint16_t i=m;i<M;i++)
	{
		leds[i%NUM_LEDS]+=CHSV(h, 255,222);
	}
}
void fillBetween2(uint16_t var1, uint16_t var1_last, byte h)
{
	int xd=var1-var1_last;
	if(xd>1)
		fillBetween_mM(var1_last, var1, h);					
	else 
	if(xd<-1)
		fillBetween_mM(var1+1, var1_last+1, h);
}
uint16_t var1_last=0;
uint16_t var2_last=0;
void wavesOppositePos()
{
	fadeToBlackBy(leds, NUM_LEDS, 3);
	
	var1_last=var1;
	var2_last=var2;
	var1=beatsin8(15,0,NUM_LEDS-1,NUM_LEDS/4,NUM_LEDS/4);
	var2=beatsin8(15,0,NUM_LEDS-1,NUM_LEDS/4,NUM_LEDS/4);
	
	
	leds[var1]+=CHSV(millis()/100, 255,255);
	leds[NUM_LEDS-1-var2]+=CHSV(millis()/100+180, 255,255);
	
	fillBetween2(var1,var1_last,(millis()/100)%256);
	fillBetween2(var2_last,var2,(millis()/100)%256);
													//Serial.println();Serial.println(var1);Serial.println(var1_last);
	//fillBetween(var2_last, var2, (millis()/100+180)%256);
}
void wavesOppositePosM()
{
	fadeToBlackBy(leds, NUM_LEDS, 4);
	
	var1++; if(var1>NUM_LEDS-1) var1=0;
	//var2--; if(var2>NUM_LEDS-1) var2=NUM_LEDS-1;
	
	leds[var1]+=CHSV(millis()/100, 255,255);
	leds[NUM_LEDS-1-var1]+=CHSV(millis()/100+180, 255,255);
}



#define move_p1_FADEOUT   5

void move_p1()
{
	byte h=0;
  switch(eff_stage2%3)
  {
	  case 0:
	  {
		fadeToBlackBy(leds, NUM_LEDS,move_p1_FADEOUT);
		h=(millis()<1000)?effects_i/100:millis()/2000*33;
		leds[DotPos]= CHSV(h,255,255);
		DotPos++;
		DotPos=DotPos%NUM_LEDS;

	  }
	  break;
	  case 1:
	  {
		fadeToBlackBy(leds, NUM_LEDS,move_p1_FADEOUT/2);
		leds[DotPos]= CHSV(gHue2,255,255);
		DotPos++;
		DotPos=DotPos%NUM_LEDS;
	  }
	  break;
	  case 2:
	  {
		fadeToBlackBy(leds, NUM_LEDS,move_p1_FADEOUT);
		h=(millis()<1000)?effects_i/512:millis()/2000*33;
		leds[DotPos]= CHSV(effects_i/512,sin8(effects_i*5*(1+millis()/4000%2)),qadd8(20,sin8(effects_i*(1+millis()/4000%2))));
		DotPos++;
		DotPos=DotPos%NUM_LEDS;
	  }
	  break;
  }
}


void eff_fade_d()
{
	fadeToBlackBy(leds, NUM_LEDS, 1);
}

#define RGB_FW_STEP_NUM                3


#define RGB_FW_DOT_BRIGHTNESS          50

#define RGB_FW_DOT_SLOWDOWN            2 

#define RGB_FW_DOTSNUM_FADING          (NUM_LEDS / 8)

#define RGB_FW_FLASH_RADIUS            20

#define RGB_FW_FLASH_PERIOD            5

#define RGB_FW_FLASH_BRIGHTNESS        255

#define RGB_FW_NOISE_STEP              10

#define RGB_FW_FADEOUT_STEP            1

#define RGB_FW_FADEOUT_PERIOD          6

long fly_end_t;
long fly_0_t;

void rgb_firework_tick()
{
  switch(eff_stage)
  {
	 case 0:
	  {
	   
		DotPos = 0;
		
		
		fly_0_t=millis();
		fly_end_t=millis()+800+random8()*4;
		//var2 = (NUM_LEDS / 4) + (random8or16() % (NUM_LEDS - RGB_FW_FLASH_RADIUS - (NUM_LEDS / 4)	-2));
		
		
		color1.r = 0; color1.g = 0; color1.b = 0; 
		switch (random8() % 6)
		{
		  case 0: 
			color1.r = RGB_FW_DOT_BRIGHTNESS; 
			break;
		  case 1: 
			color1.g = RGB_FW_DOT_BRIGHTNESS; 
			break;
		  case 2: 
			color1.b = RGB_FW_DOT_BRIGHTNESS; 
			break;
		  case 3: 
			color1.r = color1.g = RGB_FW_DOT_BRIGHTNESS; 
			break;
		  case 4: 
			color1.r = color1.b = RGB_FW_DOT_BRIGHTNESS; 
			break;
		  case 5: 
			color1.g = color1.b = RGB_FW_DOT_BRIGHTNESS; 
			break;
		}
						
		eff_stage++;
	  }
	  break;
	  
	   
	  case 1:
	  {
		fadeToBlackBy(leds, NUM_LEDS,2);
		  
		if(millis()/1024%2)
		{
		  for(byte i=0;i<3;i++)
			if(random8()> map(DotPos, 0, NUM_LEDS, 0, 255)) DotPos++; 
		
		DotPos=constrain(  DotPos,0,NUM_LEDS-1); 
		}
		else
		{
			DotPos=constrain(  map(millis(),fly_0_t,fly_end_t,0,NUM_LEDS-10),0,NUM_LEDS-1); 
		}
		  
		leds[DotPos]=color1;
		
							
		
		if(10+random8()> map(DotPos,0,NUM_LEDS-10, 400, 5))
		{
			eff_stage++;
			var1=0;
		}
	  }
	  break;
	  
	  case 2:
	  {

		for(byte i=0;i<random8(4);i++)
		{
		
		CRGB c= 
		(millis()/1024%2)?
		CHSV(random8(),RGB_FW_FLASH_BRIGHTNESS,RGB_FW_FLASH_BRIGHTNESS):
		CHSV(
		(millis()/1224%2)?random8():gHue2,
		(millis()/1224%2)?random8()/8:(128+random8(133)),
		random8()>100?RGB_FW_FLASH_BRIGHTNESS:random8()
		);
		
		leds[(DotPos+var1)%NUM_LEDS]+=c;
		
		leds[(DotPos-var1)%NUM_LEDS]+=c;

		var1++;
		if(var1>RGB_FW_FLASH_RADIUS)
		{
			eff_stage++;
			eff_ticks=0;
		}
		}
	  }
	  break;
	  case 3:  
	  {
		
		int Val = DotPos - RGB_FW_FLASH_RADIUS + 1 + (rand() % ((RGB_FW_FLASH_RADIUS * 2) - 1));
		if (rand() & 1)
			leds[Val%NUM_LEDS]+=CRGB(RGB_FW_NOISE_STEP,RGB_FW_NOISE_STEP,RGB_FW_NOISE_STEP);
		else
			leds[Val%NUM_LEDS]-=CRGB(RGB_FW_NOISE_STEP,RGB_FW_NOISE_STEP,RGB_FW_NOISE_STEP);

		
		fadeToBlackBy(leds, NUM_LEDS,RGB_FW_FADEOUT_STEP);

		eff_ticks++;
		if(eff_ticks>220)  
		{
			if(random8()>250)	eff_stage++;
		}
	  }
	  break;
	  
	  default:
	  eff_stage=0;
	   break;
	  
  }
  
  gHue2++;
}




#define RGB_RB_FADE_STEP       3


CRGB RGB_RainbowColors[] =
{
  CRGB(0,   255, 0),    
  CRGB(165, 255, 0),    
  CRGB(255, 255, 0),    
  CRGB(255, 0,   0),    
  CRGB(255, 0,   255),  
  CRGB(0,   0,   255),  
  CRGB(0,   255, 255)   
};
    

void rgb_rainbow_tick()
{
  
  EVERY_N_MILLISECONDS(11100)
  {
	  	blend_amount=0;
		if(random8()<128) 
		{
			for(byte i=0;i<  ARRAY_SIZE(RGB_RainbowColors);i++)
			{
				RGB_RainbowColors[i]=
				CHSV(
				random8(15)*16,
				(random8()<50)? random8(2)*255:255,
				(random8()<50)? random8(2)*255:255
				); //CRGB(random8(2)*255,random8(2)*255,random8(2)*255);
			}
		}
  }
  
   blend_amount=qadd8(blend_amount,RGB_RB_FADE_STEP);   
  if (blend_amount==255)
  {
	  blend_amount=0;
    
    if (++var2 >= ARRAY_SIZE(RGB_RainbowColors)) 
      var2 = 0;
	leds[0]=leds[1];
  }
  
  moveOutAllRemainFirst();



  if(millis()/1024%10<2)
  {
	leds[0]=CHSV(gHue2,255,255);
  }
  else
  {
	leds[1]=blend(leds[0], RGB_RainbowColors[var2], blend_amount);


	 if(millis()/1000%16<6)
	 {
		//leds[beatsin8(beatsin8(2,6,18),NUM_LEDS/4,NUM_LEDS-1)]=blend(RGB_RainbowColors[var2],leds[0], blend_amount);
		leds[beatsin8(8+millis()/1000%6,NUM_LEDS/4,NUM_LEDS-1)]=blend(RGB_RainbowColors[var2],leds[0], blend_amount);
	 }
  }


}

#define RGB_ST_ADD_STEP        6
#define RGB_ST_DEC_STEP        1

void rgb_stars_tick()
{
	if(pattern_il==0)
	{
		
		DotPos = random8or16(NUM_LEDS); 

		color1=CHSV(random8(),random8(111),50); //start color
		color2=CHSV(random8(),random8(40),255); //end color

		blend_amount=0;

	}
	pattern_il++;
	
	fadeToBlackBy(leds, NUM_LEDS,RGB_ST_DEC_STEP);
	
	 blend_amount=qadd8 (blend_amount,RGB_ST_ADD_STEP);
	
	leds[DotPos]=blend(color1, color2, blend_amount);  //StepChangeColor((uint8_t *)&color2, (uint8_t *)&color1, RGB_ST_ADD_STEP);
	
	leds[DotPos2]+=CHSV(DotPos*11,200,30);
	if(random8()>254) DotPos2= random8or16(NUM_LEDS);
	
	leds[(DotPos2*DotPos)%NUM_LEDS]+=CHSV(DotPos*11,(DotPos2*DotPos)%4*64,50);

}
