// PartitionObject.cpp

#include "stdafx.h"
#include "PartitionObject.h"


PartitionObject :: PartitionObject( float _x1, float _y1, float _x2, float _y2 ) : 
	x1		( _x1 ),
	y1		( _y1 ),
	x2		( _x2 ),
	y2		( _y2 ),
	obj		( NULL )
{
}

PartitionObject :: PartitionObject() :
	x1		( 0 ),
	y1		( 0 ),
	x2		( 0 ),
	y2		( 0 ),
	obj		( NULL )
{
}