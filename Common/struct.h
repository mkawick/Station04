// Struct.h

//******************************************
// UNCLASSIFIED
// Copyright © 1994-Pres	Mickey Kawick
// This file may be used anywhere for free, but this
// comment info must be left here.

// If you use this code in any shape or form or even
// as a reference for profit send me a dollar. 
// Not a dollar per copy, just a single US dollar. 

// If you are not in the US, send me
// a few different coins or the equivalent of a dollar.
// I love foreign currency, and I have a little collection.
// MKawick@sprintmail.com
//******************************************

//---------------------------------------------------------------------------

#pragma once		// include only once per build
//#pragma pack(2)		// pack all structures to 2 bytes

//#pragma TODO("What about energy damage?-SCS")

#ifndef NULL
#define NULL 0L
#define DEFINENULL
#endif

#ifndef  __M_macro
#define  __M_macro
    
//---------------------------------------------------------------------------
#if !(defined (PI))
#define  PI					3.14159265358979  // 180∞ degrees
#endif

#define  PI_LONG			3.141592653589793238462643383279502884197169399375105820974944	// 180∞ degrees
#define  PI2				6.28318530717958  // 360∞ degrees
#define  PI2_LONG			6.283185307179586476925286766559005768394338798750211641949888	// 360∞ degrees
#define  PIx2				PI2               // 360∞ degrees
#define  PItimes2			PI2               // 360∞ degrees
#define  PIdiv2				1.570796326794896619231321691639751442038584699687552910487472	// 90∞ degrees
#define  PIover2			PIdiv2            // 90∞ degrees
#define  PIdiv4				0.78539816339744830961566084581988// 45∞ degrees
#define  PIover4			PIdiv4
#define  DEG45				PIdiv4			  // 45∞ degrees
#define  DEG90				PIdiv2            // same
#define  DEG180				PI				  // same
#define  DEG270				-PIdiv2           // same
#define  DEG1				0.017453292519943295769236907684886   // 1∞ degrees
#define  DEG2				0.034906585039886591538473815369772   // 2∞ degrees
#define  DEG3				0.052359877559829887307710723054658   // 3∞ degrees
#define  DEG4				0.069813170079773183076947630739545   // 4∞ degrees
#define  DEG5				0.087266462599716478846184538424431   // 5∞ degrees
#define  DEG10				0.17453292519943295769236907684886    // 10∞ degrees
#define  DEG20				0.34906585039886591538473815369772    // 20∞ degrees 
#define  DEG30				0.52359877559829887307710723054658    // 30∞ degrees
#define  DEG60				1.0471975511965977461542144610932     // 60∞ degrees
#define  DEG120				2.0943951023931954923084289221863     // 120∞ degrees
#define  DEG40				0.69813170079773183076947630739545    // 40∞ degrees
#define  DEG80				1.3962634015954636615389526147909     // 80∞ degrees
#define  DEG160				2.7925268031909273230779052295818     // 160∞ degrees
#define  DEG140				2.4434609527920614076931670758841     // 140∞ degrees

#define  SQRT2				1.4142135623730950488016887242097	  // 45* hypoteneuse
#define  SQRT_2				0.70710678118654752440084436210485    // 1/SQRT2
#define  SQRT3				1.7320508075688772935274463415059	  // 30∞,60∞,90∞ triangle - 60∞ side
#define  logLN				0.69314718055994530941723212145818	  // doubling rate
#define  logNE				2.7182818284590452353602874713527     // natural log
#define  logN2				logLN								  // doubling rate
#define  logN10				2.3025850929940456840179914546844	  // 10x rate

#define	 GRAVITATIONAL_CONSTANT	        6.672e-11	// G - universal gravitational constant: 6.672 * 10^-11 N*m^2/kg^2
#define	 SPEED_OF_LIGHT			        299792458	// m/s   c - speed of light in a vacuum: 2.997 924 58 * 10^8 m/s
#define	 EARTH_GRAVITY_ACCELERATION	    9.8		// m/s^2 g - acceleration due to the Earth's gravity: 9.8 m/s^2
#define  FEIGENBAUM			            4.6692016091029906718532038204662016172581 // ratio between consecutive period doubling intervals.
// related to the logistic equation.

/*
k - Boltzmann's constant: 1.380 622 * 10^-23 J/K
h - Plank's constant: 6.626 176 * 10^-34 J*s
µ0 - permeability of free space: 4*PI*10^-7 N/A^2
epsilon0 - permittivity of free space: 8.8542*10^-12 C^2/N*m^2

R - gas constant: 8.314 41 J/K*mol
N - Avagadro's number: 6.022 045 * 10^23 molecules/mol

e - charge on electron: 1.602 189 * 10^-19 C
*/
/*
// metric to standard english 
http://www.iol.ie/~derfitz/Conversion.html
 */

//µ = mu
//********************************************
// measure
template <typename type>					// Convert inches to millimeters
type  INCHtoMILLI (type val) {return static_cast <type> (val * 25.4);}

template <typename type>					// Convert inches to centimeters
type  INCHtoCENTI (type val) {return static_cast <type> (val * 2.54);}

template <typename type>					// Convert feet to centimeters
type  FEETtoCENTI (type val) {return static_cast <type> (val * 30.48);}

template <typename type>					// Convert yards to meters
type  YARDtoMETER (type val) {return static_cast <type> (val * 0.9144);}

template <typename type>					// Convert miles to kilometers
type  MILEtoKILO (type val) {return static_cast <type> (val * 1.609344);}

template <typename type>					// Convert millimeters to inches
type  MILLItoINCH (type val) {return static_cast <type> (val * 0.03937007874015748031496062992126);}

template <typename type>					// Convert millimeters to inches
type  CENTItoINCH (type val) {return static_cast <type> (val * 0.3937007874015748031496062992126);}

template <typename type>					// Convert centimeters to feet
type  CENTItoFEET (type val) {return static_cast <type> (val * 0.032808398950131233595800524934383);}

template <typename type>					// Convert meters to yards
type  METERtoYARD (type val) {return static_cast <type> (val * 1.0936132983377077865266841644794);}

template <typename type>					// Convert kilometers to miles
type  KILOtoMILE (type val) {return static_cast <type> (val * 0.62137119223733396961743418436332);}

		///////////////////////

// area
template <typename type>					// Convert square inches to square centimeters
type  INCH2toCENTI2 (type val) {return static_cast <type> (val * 6.4516);}

template <typename type>					// Convert square feet to square meters
type  FEET2toMETER2 (type val) {return static_cast <type> (val * 0.092903);}

template <typename type>					// Convert square yards to square meters
type  YARD2toMETER2 (type val) {return static_cast <type> (val * 0.836127);}

template <typename type>					// Convert square miles to square kilometers
type  MILE2toKILO2 (type val) {return static_cast <type> (val *  2.5899);}

template <typename type>					// Convert square acre to square hectares
type  ACRE2toHECT2 (type val) {return static_cast <type> (val * 0.40486);}


template <typename type>					// Convert square centimeters to square inches
type  CENTI2toINCH2 (type val) {return static_cast <type> (val * 0.155);}

template <typename type>					// Convert square meters to square feet
type  METER2toFEET2 (type val) {return static_cast <type> (val * 10.7639);}

template <typename type>					// Convert square meters to square yards
type  METER2toYARD2 (type val) {return static_cast <type> (val * 1.19599);}

template <typename type>					// Convert square kilometers to square miles
type  KILO2toMILE2 (type val) {return static_cast <type> (val * 0.38611);}

template <typename type>					// Convert square hectares to square acre
type  HECT2toACRE2 (type val) {return static_cast <type> (val * 2.471);}
		///////////////////////

// volume
template <typename type>					// Convert fluid ounces to millilitres
type  OUNCEtoMILLI (type val) {return static_cast <type> (val * 28.4131);}

template <typename type>					// Convert pints to litres
type  PINTtoLITRE (type val) {return static_cast <type> (val * 0.568);}

template <typename type>					// Convert quarts to litres
type  QUARTtoLITRE (type val) {return static_cast <type> (val * 1.137);}

template <typename type>					// Convert gallons to litres
type  GALLONtoLITRE (type val) {return static_cast <type> (val * 4.546);}


template <typename type>					// Convert millilitres to fluid ounces
type  MILLItoOUNCE (type val) {return static_cast <type> (val * 0.03519);}

template <typename type>					// Convert litres to pints
type  LITREtoPINT (type val) {return static_cast <type> (val * 1.76);}

template <typename type>					// Convert litres to quarts
type  LITREtoQUART (type val) {return static_cast <type> (val * 0.8795);}

template <typename type>					// Convert litres to gallons
type  LITREtoGALLON (type val) {return static_cast <type> (val * 0.21997);}

		///////////////////////
// mass and weight
template <typename type>					// Convert ounces to grams
type  OUNCEtoGRAM (type val) {return static_cast <type> (val * 28.349);}

template <typename type>					// Convert pounds to kilos
type  POUNDtoKILO (type val) {return static_cast <type> (val * 0.45359);}

template <typename type>					// Convert tons to tonnes
type  TONtoTONNE (type val) {return static_cast <type> (val * 1.01605);}


template <typename type>					// Convert grams to ounces
type  GRAMtoOUNCE (type val) {return static_cast <type> (val * 0.3527);}

template <typename type>					// Convert kilos to pounds
type  KILOtoPOUND (type val) {return static_cast <type> (val * 2.2046);}

template <typename type>					// Convert tonnes to tons
type  TONNEtoTON (type val) {return static_cast <type> (val * 0.9842);}
		///////////////////////

template <typename type>					// Convert farenheit to celsius/centigrade
type  TOCELSIUS (type val) {return static_cast <type> (((val)-32)/5*9);}

template <typename type>					// Convert celsius to farenheit
type  TOFAREN (type val) {return static_cast <type> (((val)*5/9)+32);}


////////////********************************************

//formula for VerticalPosition
//S = -16t^2 + Sπ + Pπ.
template <typename type>					// VerticalPositionAtTime
type  VerticalPositionAtTime (type time, type InitialSpeedUpwards, type InitialPosition) 
{return static_cast <type> (-16 * (time * time) + (InitialSpeedUpwards * time) + InitialPosition);}

template <typename type>					// VerticalSpeedAtTime
type  VerticalSpeedAtTime (type time, type InitialSpeedUpwards, type InitialPosition) 
{return static_cast <type> (-32 * time);}

//Acceleration due to gravity is fixed at -32 feet per second per second

template <typename type>					// AngularSpeed returns RadiansPerSecond
type  AngularSpeed (type AngleTravelledThrough, type time)
{return static_cast <type> (AngleTravelledThrough / time);}

template <typename type>					// AngularVelocity
type  AngularVelocity (type AngularSpeed, type Radius)// AngularSpeed must be in RadiansPerSecond
{return static_cast <type> (AngularSpeed*Radius);}


template <typename type>					// KINETIC_ENERGY  // 1/2 mass times velocity squared
type  KINETIC_ENERGY (type mass, type velocuty)
{return static_cast <type> ((.5)*(mass)*(velocuty)*(velocuty));}

template <typename type>					// MOMENTUM // mass times velocity
type  MOMENTUM (type mass, type velocity) {return static_cast <type> (mass * velocity);}

template <typename type>					// ACCELERATION
type  ACCELERATION (type force, type mass) {return static_cast <type> (force * mass);}

template <typename type>					// VELOCITY
type  VELOCITY (type acceleration, type time) {return static_cast <type> ((acceleration)/(time));}

template <typename type>					// WORK in Kilogram meters
type  WORK (type mass, type velocity) {return static_cast <type> (mass * velocity * velocity);}

template <typename type>					// NEWTON
type  NEWTON (type kg, type meters) {return static_cast <type> (kg * meters);}

////////////********************************************

template <typename type>					// RADtoDEG
type  RADtoDEG (type val) {return static_cast <type> (val * 57.2957795132);}
template <typename type>					// DEGtoRAD
type  DEGtoRAD (type val) {return static_cast <type> (val * 0.0174532925199);}
			
	//****************
#ifndef ODD // This is often defined.
template <typename type>					// ODD integers only
type  ODD (type a) {return (a&1);}
#endif

#ifndef ABS // This is often defined.
template <typename type>					// ABS
type  ABS (type a) {return ((a)<0) ? -(a):(a);}
/*
cwd
xor	ax, dx
sub	ax, dx
*/
#endif

template <typename type>					// EVEN integers only
type  EVEN (type a) {return (!ODD(a));}
template <typename type>					// SGN
type  SGN (type a) {return ((a<0) ? -1:(a>0) ? 1:0);}
template <typename type>					// SQR
type  SQR (type a) {return a*a;}
template <typename type>					// CUBE
type  CUBE (type a) {return a*a*a;}
template <typename type>					// ROUND
type  ROUND (type a) {return (a>0 ? static_cast <type>((int)(a+0.5F)):-static_cast <type>((int)(0.5F-a)));}
template <typename type>					// TRUNC
type  TRUNC (type a) {return (int)(a);}

	//****************

template <typename type>					// MIN
type  MIN (type a, type b) {return ((a<b) ? a:b);}   
template <typename type>					// MIN smallest in array
type  MIN (type* array, int num) 
{
	type val = array[0];
	for (int i=1; i<num; i++)
	if (val>array[i]) val = array[i];
	return val;
} 
template <typename type>					// MIN3
type  MIN3 (type a, type b, type c) {return (MIN(MIN(a,b),c));}  
  
      
template <typename type>					// MAX
type  MAX (type a, type b) {return ((a>b) ? a:b);}    
template <typename type>					// MAX largest in array
type  MAX (type* array, int num) 
{
	type val = array[0];
	for (int i=1; i<num; i++)
	if (val<array[i]) val = array[i];
	return val;
} 
template <typename type>					// MAX3
type  MAX3 (type a, type b, type c) {return (MAX(MAX(a,b),c));}  


template <typename type>					// AVG
type  AVG (type a, type b) {return ((a+b)/2);} 
template <typename type>					// AVG of an array
type  AVG (type* array, int num) 
{
	type sum = array[0];
	for (int i=1; i<num; i++)
	sum += array[i];
	return sum/num;
} 

template <typename type>					// SUM add two numbers
type  SUM (type a, type b) {return (a+b);} 
template <typename type>					// SUM add array of numbers
type  SUM (type* array, int num) 
{
	type sum = array[0];
	for (int i=1; i<num; i++)
	sum += array[i];
	return sum;
} 
template <typename type>					// SUM add two numbers
type  FACTORIAL	 (type a) 
{
	type total = 1;
	for (int i=2; i<=a; i++)
		total *= i;
	return total;
} 

template <typename type>					// MEDIAN find the middle value in a list
type  MEDIAN (type* array, int num) 
{
	int Odd = (ODD(num));
	for (int i=0; i<num; i++)
	{
		int CountBelow = 0;
		int CountAbove = 0;
		for (int j=0; j<num; j++)
		{
			if (j == i) continue;
			if (array[i] <= array[j]) CountAbove ++;
			if (array[i] >= array[j]) CountBelow ++;
		}
		if (Odd)
		{
			if (CountBelow == CountAbove) return array[i];
		}
		else
		{
			if (CountBelow+1 >= CountAbove &&
				CountBelow-1 <= CountAbove) return array[i];
		}
	}
	return array[0];// should never happen, but just in case
}   

	//****************   
          
template <typename type>					// SWAP // a xor b; b xor a; a xor b;
void  SWAP (type& a, type& b) 
{
	//a^=b; b^=a; a^=b;
	type temp = a; a = b; b = temp;
} 
#define  SWAPPT(a,b) (SWAP(a.y,b.y), SWAP(a.x,b.x))// SWAP swap points   
template <typename type>					// RECIP // a^-1
type  RECIP (type a) {return (1/a);}  
template <typename type>					// NOT  // exclusive or unary
type  NOT (type a) {return (~(a));} 
template <typename type>					// XOR exclusive or binary
type  XOR (type a, type b) {return (a^b);}  
template <typename type>					// BSL bit shift left
type  BSL (type a) {return (a<<1);} 
template <typename type>					// BSL2 bit shift left by b positions
type  BSL2 (type a, type b) {return (a<<b);}  
template <typename type>					// BSR  bit shift right
type  BSR (type a) {return (a>>1);} 
template <typename type>					// BSR2 bit shift right by b positions
type  BSR2 (type a, type b) {return (a>>b);}  
template <typename type>					// BIT turns a bit on
type  BIT (type x) {return static_cast <type>(1<<x);}  

	//****************

template <typename type>					// LO_BYTE
type  LO_BYTE (type a) {return (a&=0xff);}  
template <typename type>					// HI_BYTE 
type  HI_BYTE (type a) {return (a&=0xff00);} 
template <typename type>					// LO_WORD
type  LO_WORD (type a) {return (a&=0xffff);}  
template <typename type>					// HI_WORD 
type  HI_WORD (type a) {return (a&=0xffff0000);} 
template <typename type>					// HI_LOSWAPshort
type  HI_LOSWAPshort (type a) {return (LO_BYTE(a)<<8) | (HI_BYTE(a)>>8);}  
template <typename type>					// HI_LOSWAPlong
type  HI_LOSWAPlong (type a) {return (LO_WORD(a)<<16) | (HIWORD(a)>>16);}  

	//****************

template <typename type, typename type2>		// LIMIT limit the range of a number
type  LIMIT (type val, type2 l, type2 h) 
{
	if (val<l) return static_cast <type>(l);
	if (val>h) return static_cast <type>(h);
	return val;
} 

template <typename type, typename type2>		// WRAP_IT limits the range of a number
type  WRAP_IT (type val, type2 low, type2 high) // and wraps it around on top
{	
	type2 range = high - low;
	type2 t = static_cast <type2> ((int)(val/range));
	if (val>high) 
	{
		type2 t = static_cast <type2> ((int)(val/range));
		val -= static_cast <type> (range * t); //+/- range
		if (val>high)
		val = static_cast <type> (low) + val - static_cast <type>(high);
		return val;
	}
	if (val<low)
	{
		type2 t = static_cast <type2> ((int)(val/range));
		val -= static_cast <type> (range * t);
		if (val<low)
		val = static_cast <type> (high) + val-static_cast <type>(low);
		return val;
	}
	return val;
} 

template <typename type> inline type AlignUp (type n, int align) throw()
{
	return (type ((n+(align-1))&~(type (align)-1)));
}

template <typename type> inline type AlignDown (type n, int align) throw()
{
	return (type (n&~ (type(align)-1)));
}

template <typename type> inline type* AlignMem (type* n, int align) throw()
{
	return (type*) AlignUp ((int)n, align);
}

/*
#if !defined(bool)
typedef int bool;
#define false 0
#define true 1
#endif*/

template <typename Type>// Steve from Westwood gave me this one
bool	POWER_OF_2 (Type x)
{
	return ((x-1)&x) ? true:false;
}
template <typename Type> 
bool	IMPLIES (Type a, Type b)
{
	return (a | (~b)) ? true:false;
}

template <typename type>					// TORAD return the radian measure of the degrees passed
type  TORAD (type a) {return DEGtoRAD(a);}  
template <typename type>					// TODEG convert to degrees from radians
type  TODEG (type a) {return RADtoDEG(a);} 
template <typename type>					// CSC
type  CSC (type a) {return ((a==0 || a==DEG180) ? static_cast <type>(0.0):static_cast <type>(1/sin(a)));}  
template <typename type>					// SEC
type  SEC (type a) {return ((a==DEG90 || a==DEG270) ? static_cast <type>(1.0e18):static_cast <type>(1/cos(a)));} 
template <typename type>					// COT
type  COT (type a) {return ((a==DEG90 || a==DEG270) ? static_cast <type>(1.0e18):static_cast <type>(1/tan(a)));}         
template <typename type>					// ARCSIN
type  ARCSIN (type a) {return (atan((a)/sqrt(-(a)*(a)+1)));}  
template <typename type>					// ARCCOS
type  ARCCOS (type a) {return (-atan((a)/sqrt(-(a)*(a)+1))+PIdiv2);} 


	//****************

template <typename type>					// perppos
type  perppos (type a) {return (a+PIdiv2);}         
template <typename type>					// perpneg
type  perpneg (type a) {return (a-PIdiv2);}  
template <typename type>					// DIST // convert to polar distance
type  DIST (type a, type b) {return static_cast <type>(sqrt(a*a + b*b));}         
template <typename type>					// ANGLE //pass either the xdist or ydist and return angle
type  ANGLE (type x, type y) {return (x!=0) ? static_cast <type>(cos(x)) : static_cast <type>(sin(y));} 
template <typename type>					// ANGLE //pass either the xdist or ydist and return angle
type  DIST_SQUARED (type x1, type y1, type x2, type y2 ) 
{
	type x = x2-x1; type y = y2-y1;
	return (x*x + y*y);
} 

	//****************  the rest are better left as macros, note the heavy use of parenths in the macro itself.

#define  derive(a, x, b, c)  (((a)*(b))*pow((x),(b)-1))// take the derivitive, 
														// in the form ax^b+c
#define  VOLCYLINDER(r,h)    ((h)*(PI*((r)*(r))))// cylinder volume
#define  VOLCUBE(s)          ((s)*(s)*(s))// cube
#define  VOLSPHERE(r)        (4/3*PI*((r)*(r)*(r)))// sphere volume
#define  SURCYLINDER(r,h)    ((h)*2*(r)*PI + (PI)*(r)*(r)*2)// surface area + 2 bases
#define  SURCUBE(s)          ((s)*(s)*(s))
#define  SURSPHERE(r)        (4*PI*(r)*(r))
#define  SURCONE(r,h)        (((PI*(r)*(h))/2) + (PI)*(r)*(r))  //side plus base
#define  AREARECT(x,y)       ((x)*(y))
#define  AREACONE(r,h)       (((PI*(r)*(r))*h/3)
#define  AREACIRCLE(r)       ((PI*(r)*(r))
#define  AREATRIANGLE(b,h)   ((b)*(h)*0.5)
#define  GALTOLITRE(a)       ((a)*3.78541)
#define  LITRETOGAL(a)       ((a)*.264172)
#define  LB_toKG(a)          ((a)*.453592)
#define  KG_toLB(a)          ((a)*2.20462)
#define  GRAVITY(m1,m2,dist) ((((m1)+(m2))*0.00000000006672)/(dist*dist))   // attraction of two masses
#define  GOLDEN_MEAN(a)		 ((a)*(0.61803398874989484820458683436564))// note that the difference between these 2 is 1.0 ...
#define  GOLDEN_RATIO(a)     ((a)*(1.6180339887498948482045868343656))// and also that each is the reciprical of the other.


template <typename Type>
Type ToInt (float x) // this assembly is significantly faster than standard conversions
// about 2.85 times as fast, 
// Numbers 7,530,000 per second on p180
// 251,000 per frame at 30 fps or 125,500 at 60fps
{	
	int res = 0;
#ifdef VCL_FULL
	_asm
	{
		fld		x
		fistp	res
	}
#endif
	return static_cast <Type> (res);
}

template <typename Type>
float ToFloat (Type x) // assumed to be some integer type
// this assembly is significantly faster than standard conversions
// about 18% faster
// Numbers 7,112,000 per second on p180
// 237,000 per frame at 30 fps or 118,500 at 60fps
{
	float res = 0;
	int	val = static_cast <int> (x);
#ifdef VCL_FULL
//#if defined()
  	_asm
	{
		fild	val
		fstp	res
	}
   #endif
	return res;
}

#endif


// this library is intended for 16bit graphics. All bitmaps will be converted from 4, 8,
// or 24 bit to a 16bit compressed format called lle. Ultimately, tiffs and
// some other formats will be supported.

#ifndef __STANDARD_TYPEDEFS__
#define __STANDARD_TYPEDEFS__

#if !(defined (U8))
typedef unsigned char   U8;
#endif

#if !(defined (U16))
typedef unsigned short U16;
#endif

#if !(defined (U32))
typedef unsigned int   U32;
#endif

#if !(defined (UL))
typedef unsigned long   UL;
#endif


typedef unsigned char  Uint8,  * U8ptr,  * U8Ptr;
typedef unsigned short Uint16, * U16ptr, * U16Ptr;
typedef unsigned int   Uint32, * U32ptr, * U32Ptr;
typedef unsigned long  Ulong,  * ULptr,  * ULPtr;

#if !(defined (U8))
typedef			 char   S8;
#endif
#if !(defined (S16))
typedef			 short S16;
#endif
#if !(defined (S32))
typedef			 int   S32;
#endif
#if !(defined (SL))
typedef			 int   SL;
#endif

typedef          signed __int8  Sint8,  * S8ptr,  * S8Ptr;
typedef          signed __int16 Sint16, * S16ptr, * S16Ptr;
typedef          signed __int32   Sint32, * S32ptr, * S32Ptr;
typedef          signed long  Slong,  * SLptr,  * SLPtr;

typedef			 float FL, F32, *Fptr;
typedef			 double DB, D80, *Dptr;

typedef			 void* VoidPtr;

typedef unsigned char  const * U8constptr;
typedef unsigned short const * U16constptr;
typedef unsigned int   const * U32constptr;
typedef unsigned long  const * ULconstptr;

typedef          char  const * S8constptr;
typedef          short const * S16constptr;
typedef          int   const * S32constptr;
typedef          long  const * SLconstptr;

typedef			 float const *Fconstptr;
typedef			 double const*Dconstptr;
// special arrays for handling large numbers of bits

typedef unsigned int  BIT64[2], BIT128[4], BIT192[6], BIT256[8];

			//*********************************
			//*********************************

struct BIT64Mgr
{
	BIT64	Bits;

	BIT64Mgr () {Clear ();}
	BIT64Mgr (const BIT64Mgr& bit) {Bits[0] = bit.Bits[0], Bits[1] = bit.Bits[1];}

	void Clear () {Bits[1] = 0L, Bits[0] = 0L;}

	//******************

	BIT64Mgr& operator = (const BIT64Mgr& temp) 
	{ 
		Bits[0] = temp.Bits[0]; 
		Bits[1] = temp.Bits[1];
		return *this;
	}
	BIT64Mgr operator ~ () 
	{ 
		BIT64Mgr temp (*this); 
		temp.Bits[0] = ~temp.Bits[0]; 
		temp.Bits[1] = ~temp.Bits[1];
		return BIT64Mgr (temp);
	}

	//******************
	BIT64Mgr& operator |= (const BIT64Mgr& b)
	{
		Bits[0] |= b.Bits[0]; 
		Bits[1] |= b.Bits[1];
		return *this;
	}
	BIT64Mgr& operator &= (const BIT64Mgr& b)
	{
		Bits[0] &= b.Bits[0]; 
		Bits[1] &= b.Bits[1];
		return *this;
	}
	BIT64Mgr& operator ^= (const BIT64Mgr& b)
	{
		Bits[0] ^= b.Bits[0]; 
		Bits[1] ^= b.Bits[1];
		return *this;
	}
	//******************
	BIT64Mgr operator | (const BIT64Mgr& temp) const 
	{
		return (BIT64Mgr (temp) |= *this);
	}
	BIT64Mgr operator & (const BIT64Mgr& temp) const 
	{ 
		return (BIT64Mgr (temp) &= *this);
	}
	BIT64Mgr operator ^ (const BIT64Mgr& temp) const 
	{ 
		return (BIT64Mgr (temp) ^= *this);
	}
	bool	operator []	(const int which) const
	{
		return GetBit (which);
	}
	//******************

	void	SetBit (const int i)
	{
		if (i>63) return;
		if (i>31)
			Bits[1] |= (1<<(i-32));
		else
			Bits[0] |= (1<<i);
	}

	bool	GetBit (const int i) const 
	{
		if (i>63) return false;
		if (i>31)
			return  (Bits[1] & (1<<(i-32))) ? true:false;
		else
			return  (Bits[0] & (1<<i)) ? true:false;
	}

	void	ClearBit (const int i)
	{
		if (i>63) return;
		if (i>31)
			Bits[1] &= 0xffffffff - (1<<(i-32));
		else
			Bits[0] &= 0xffffffff - (1<<i);
	}

	bool	FlipBit (const int i)
	{
		bool bit = GetBit (i);
		if (bit)
			ClearBit (i);
		else SetBit (i);
		return !bit;
	}
	//******************
};

			//*********************************
			//*********************************

#endif

			//*********************************
			//*********************************

typedef
struct SPOINT
{
	short x, y;
	SPOINT () {Clear();}
	SPOINT (SPOINT& pt) {x = pt.x, y = pt.y;}
	void Clear() {x = 0, y = 0;}
}SCREENPOINT, *SCREENPOINTptr;

			//*********************************
			//*********************************
typedef
struct WPOINT
{
	short x, y, z;
	WPOINT() {Clear();}
	void Clear() {x = 0, y = 0, z = 0;}
}WORLDPOINT, *WORLDPOINTptr;

			//*********************************
			//*********************************
typedef
struct  DRAWSTRUCT
{
	int			x, y, w, h;
	U16ptr		Image;
	DRAWSTRUCT () {x = 0, y = 0, w = 0, h = 0, Image = 0L;}
}*DRAWSTRUCTptr;

			//*********************************
			//*********************************

typedef 
struct RECTA
{
    int l, t, r, b;
	RECTA () {l = 0, t = 0, r = 0, b = 0;}
}RECTA, *RECTAptr, **RECTAlist;

			//*********************************
			//*********************************

typedef 
struct RECTANGLE
{
    U16		left, top, right, bottom;
	RECTANGLE () {left = 0, top = 0, right = 0, bottom = 0;}
	RECTANGLE (U16 l, U16 t, U16 r, U16 b) {left = l, top = t, right = r, bottom = b;}
	RECTANGLE (RECTANGLE& r) {left = r.left, top = r.top, right = r.right, bottom = r.bottom;}
	RECTANGLE& operator = (RECTANGLE& r) 
	{
		left = r.left, top = r.top, right = r.right, bottom = r.bottom;
		return *this;
	}
	void	Set (U16 l, U16 t, U16 r, U16 b) {left = l, top = t, right = r, bottom = b;}
}RECTANGLE, *RECTANGLEptr, **RECTANGLElist;

			//*********************************
			//*********************************
			//*********************************

typedef 
class SCREEN_STRUCT
{
public:
	U16ptr			Screen;
	int				Width, Height, RealWidth;
public:
	SCREEN_STRUCT () {Screen = 0L, Width = 0, Height = 0, RealWidth = 0;}
}SCREEN_STRUCT, *SCREEN_STRUCTptr, **SCREEN_STRUCTlist;

			//*********************************
			//*********************************
 
			//*********************************
			//*********************************

//#include <string.h>
typedef
struct FLAGS_FOR_BITMAP
{
	U32  ACTIVE:1; 			// 0, 0 -no response to any
							// selection, movement, and 
							// will not display
	U32  LOADED:1; 			// 1, 1 -file has been loaded
	U32  TRANSLUCENT:2;		// 2-3, 1 -drawing will be with translucent routines where avail
							// 1 for 50/50, 2 for 25/75 (light) and 2 for 75/25 (dark)
	U32  COMPRESSED:3;		// 4-6, 0 -no compression, 1 is lle,
							// 2 is rle, 3 is lzw
    U32  IMAGE_TYPE:2;		// 7-8, 0-none, 1-lle, 2-BMP, 3-TARGA

    //***8
	U32  BIT_DEPTH:3;		// 9-11, 0 :2, 1:4, 2:8, 3:16 (5,5,5),
							// 4:16(5,6,5), 5:24,
							// 6:32 regular, 7:32(24 regular, 8 alpha)
	U32  DRAW_ENABLED:1;	// 12, 0 -will not draw but all
							// other activity is normal
	U32  WRITE_ENABLED:1;	// 13, 1 -writting is possible, certain
							// sharing methods require
							// locking and unlocking of write ability
	U32  READ_ENABLED:1;	// 14, 1 -reading is possible, see
							// write above
	U32  SELECTED:1;		// 15, 1 -this object is selected
	U32  LINKED:1;			// 16, 1 -this object belongs to a list

    //***16
	U32  CONTROLLED:1;		// 17, 1 -this object is managed by
							// a managing class or tool
    U32  CLIP:1;			// 18, use clipping rect-1, or not 0
	U32  ATTACHED_SHADOW:1;	// 19, beyond the end of the lle
							// compressed image is the shadow 
							// for the image, which should be 
							// drawn (bottom up) starting at the
							// height of the image(down)
    U32  ATTACHED_BITMAP:2;	// 20-21, this tells us how many attached bitmaps there are
	U32  SPECIAL_BACKGROUND_COLOR:1;// 21 is there a special
							// color like magenta for the 
							// background
    U32  NO_SCRATCH_PAD:1;	// 23 0 means a scratch pad is already loaded, 1 means that
							// one will need to be created.

    U32  CLIPPED: 1;		// 24 when clipping a graphic, clip only once
	//U16  SPECIAL_DRAWING:2;// 24-25, any special drawing rules
    //***24
	//U16  COLOR_TRANSFORMATION:5;// 26-29, 0 is no transform, otherwise 
//color number

	U32  BIT_PATTERN:1;		//_5x5x5_ or _5x6x5_
	U32	 PAD:7;				//25-32

}* FLAGS_FOR_BITMAPptr;

			//*********************************
			//*********************************

typedef union  BITMAP_FLAGS
{
	U32 	 		 Value;
	FLAGS_FOR_BITMAP Flag; 
		//---------------------------
	BITMAP_FLAGS(){Clear();}
	void Clear(){Value = 0;}
}* BITMAP_FLAGSptr;

			//*********************************
			//*********************************
typedef struct FLAGS_FOR_CREATURES
{
	U32 ARTIFICIAL_INT:6;	// 0-5, which intelligence type 0 is none

	U32 ALIVE:1;			// 6, 1 creature is alive and can function
	U32 DYING:3;			// 7-9, 0 creature is healthy and 7 stages of 
								// dying, if a creature is already dead, this 
								// is the level of decay starting with
								// 7 and decaying to 0
	U32 PRIORITY_SETTING:5;// 10-14, how much processing time does this 
								// object need
	U32 WRITE_ENABLED:1;	// 15, 1 -writting is possible, 
								// certain sharing methods require 
								// locking and unlocking of write ability
	U32 READ_ENABLED:1;		// 16, 1 -reading is possible, see write above
	U32 ADAPTABLE:4;			// 17-20, varying states of adaptability, 0 is 
								// unadaptive
	U32 AGGRESSION:4;		// 21-24, aggression where 0 is docile and 15 
								// is hostile
	U32 FULL_PATH_FOUND:1;	// 25, during the last pathing, was the full 
								// path found
	U32 CURRENT_STATE:6;  	// 26-31, 32 different states
								// may be replaced

	U32 BLOCKING:2;			// can walk over or through, 0-no, 1-slowly, 
								// 2-moderate, 3-full speed
	U32 SELECTED:1;			// draw selection box
	U32 GROUP_ID:4;			// support for up to 15 group
	U32 TIME_SINCE_BIRTH:16;// how long have I been alive, over 30 minutes
								// at 30 fps, every time this rolls over, add
								// one experience point

	U32	EXPERIENCE:9;		// experience points. adds to chance for hitting,
								// damage, or intelligence
}* FLAGS_FOR_CREATURESptr;	// 64 bits


			//*********************************
			//*********************************

typedef union  CREATURE_FLAGS
{
		//---------------------------
	CREATURE_FLAGS() {Clear();}
	void Clear() const
	{
		Value[0] = 0, Value[1] = 0;
	}
	CREATURE_FLAGS& operator = (const CREATURE_FLAGS& c)
	{
		Value[0] = c.Value[0], Value[1] = c.Value[1];
		return *this;
	}
public:
	mutable	BIT64				Value;
	mutable	FLAGS_FOR_CREATURES	Flag;
}* CREATURE_FLAGSptr;

			//*********************************
			//*********************************


typedef
 void (* FUNC_DRAW) (U16ptr, int, int, int, int, BITMAP_FLAGSptr);
typedef void (* FUNC_VOID ) (void);
typedef void (* FUNC_CHARptr ) (char*);
typedef void (* FUNC_WORDptr ) (short*);
typedef void (* FUNC_INTptr ) (int*);

			//*********************************
			//*********************************

#ifdef	DEFINENULL // corrects for null definition
#undef  NULL 
#endif


