// Quadtree.cpp

#include "stdafx.h"

#include "Quadtree.h"
#include "PartitionObject.h"

using namespace std;
#include <math.h>

//-------------------------------------------------------------------------------

Quadtree::Quadtree() : 
	x1		( 0 ),
	y1		( 0 ),
	x2		( 0 ),
	y2		( 0 ),
	level	( UninitializedLevel ),
	maxLevel( UninitializedLevel ),
	objects(),
	NW		( NULL ),
	NE		( NULL ),
	SW		( NULL ),
	SE		( NULL )
{
}

//-------------------------------------------------------------------------------

void Quadtree::Init( float _x1, float _y1, float _x2, float _y2, int _level, int _maxLevel )
{
	if( _x2< _x1 )
		SWAP( _x2, _x1 );
	if( _y2 < _y1 )
		SWAP( _y2, _y1 );
	x1		= _x1,
	y1		= _y1,
	x2		= _x2,
	y2		= _y2,
	level	= _level,
	maxLevel=_maxLevel;

	objects.clear();

	if ( level == maxLevel ) 
	{
		return;
	}

	float halfX = ( x2+x1 ) * 0.5f;
	float halfY = ( y2+y1 ) * 0.5f;
	NW = new Quadtree();
	NE = new Quadtree();
	SW = new Quadtree();
	SE = new Quadtree();
	NW->Init( x1, y1, halfX, halfY, level+1, maxLevel );
	NE->Init( halfX, y1, x2, halfY, level+1, maxLevel );
	SW->Init( x1, halfY, halfX, y2, level+1, maxLevel );
	SE->Init( halfX, halfY, x2, y2, level+1, maxLevel );
}

//-------------------------------------------------------------------------------

float	 Quadtree::GetMinimumPartitionX() const 
{ 
	return ( x1 + x2 ) / pow( 2.0f, maxLevel ) ; 
}

//-------------------------------------------------------------------------------

float	 Quadtree::GetMinimumPartitionY() const
{ 
	return ( y1 + y2 ) / pow( 2.0f, maxLevel ) ; 
}

//-------------------------------------------------------------------------------

void Quadtree::AddPartitionObject( PartitionObject *object ) 
{
	if( object == NULL )
		return;

	if ( level == maxLevel ) 
	{
		objects.push_back( object );
		return;
	}
	if ( Contains( NW, object ) ) 
	{
		NW->AddPartitionObject( object ); 
		return;
	} 
	else if ( Contains( NE, object ) ) 
	{
		NE->AddPartitionObject( object ); 
		return;
	} 
	else if ( Contains( SW, object ) ) 
	{
		SW->AddPartitionObject( object ); 
		return;
	} 
	else if ( Contains( SE, object ) ) 
	{
		SE->AddPartitionObject( object ); 
		return;
	}
	if ( Contains( this, object ) ) 
	{
		objects.push_back( object );
	}
}

//-------------------------------------------------------------------------------

list<PartitionObject*>  FilterList( list<PartitionObject*>& listOfObjects, U32 collisionFlags )
{
	if( collisionFlags == 0 )
		return listOfObjects;

	list<PartitionObject*> temp;
	list<PartitionObject*>::iterator it = listOfObjects.begin();
	while( it != listOfObjects.end() )
	{
		if( ( (*it)->collisionFlag & collisionFlags ) != CollisionFlags_None )
		{
			temp.push_back( *it );
		}

		++it;
	}

	return temp;
}

//-------------------------------------------------------------------------------

void CopyFilterList( list<PartitionObject*>& sourceObjects, list<PartitionObject*>& destList, U32 collisionFlags )
{
	if( collisionFlags == 0 )
	{
		destList.insert( destList.end(), sourceObjects.begin(), sourceObjects.end() );
		return;
	}

	list<PartitionObject*>::iterator it = sourceObjects.begin();
	while( it != sourceObjects.end() )
	{
		if( ( (*it)->collisionFlag & collisionFlags ) != CollisionFlags_None )
		{
			destList.push_back( *it );
		}

		++it;
	}
}

//-------------------------------------------------------------------------------

void CopyFilterList( list<PartitionObject*>& sourceObjects, list<PartitionObject*>& destList, float x, float y, float dist, U32 collisionFlags )
{
	if( collisionFlags == 0 )
	{
		dist = dist * dist;
		list<PartitionObject*>::iterator it = sourceObjects.begin();
		while( it != sourceObjects.end() )
		{
			PartitionObject* po = *it;
			if( DIST_SQUARED( po->cx(), po->cy(), x, y ) <= dist )// this is slightly poblematic since it only considers the center.
			{
				destList.push_back( *it );
			}
			++it;
		}
		return;
	}

	list<PartitionObject*>::iterator it = sourceObjects.begin();
	while( it != sourceObjects.end() )
	{
		PartitionObject* po = *it;
		if( ( po->collisionFlag & collisionFlags ) != CollisionFlags_None )
		{
			if( DIST_SQUARED( po->cx(), po->cy(), x, y ) <= dist )// this is slightly poblematic since it only considers the center.
			{
				destList.push_back( *it );
			}
		}

		++it;
	}
}

//-------------------------------------------------------------------------------

list<PartitionObject*>  Quadtree::GetObjectsAt( float x1, float y1, float x2, float y2, U32 collisionFlags )
{
	list<PartitionObject*> temp;

	GetObjectsAt( temp, x1, y1, collisionFlags );
	GetObjectsAt( temp, x2, y1, collisionFlags );
	GetObjectsAt( temp, x1, y2, collisionFlags );
	GetObjectsAt( temp, x2, y2, collisionFlags );

	return temp;
}

//-------------------------------------------------------------------------------

bool Quadtree::GetObjectsAt( list<PartitionObject*>& listOfStuff, float _x, float _y, U32 collisionFlags )
{
	if ( level == maxLevel ) 
	{
		CopyFilterList( objects, listOfStuff, collisionFlags );
		return true;
	}

	list<PartitionObject*> returnObjects;
	if ( !objects.empty() ) 
	{
		CopyFilterList( objects, returnObjects, collisionFlags );
	}
	float halfX = ( x2+x1 ) * 0.5f;
	float halfY = ( y2+y1 ) * 0.5f;

	if ( _x > halfX && _x < x2 ) 
	{
		if ( _y > halfY && _y < y2 ) 
		{
			CopyFilterList( SE->GetObjectsAt( _x, _y, collisionFlags ), returnObjects, collisionFlags );
			return true;
		} 
		else if ( _y > y1 && _y <= halfY ) 
		{
			CopyFilterList( NE->GetObjectsAt( _x, _y, collisionFlags ), returnObjects, collisionFlags );
			return true;
		}
	} 
	else if ( _x > x1 && _x <= halfX ) 
	{
		if ( _y > halfY && _y < y2 ) 
		{
			CopyFilterList( SW->GetObjectsAt( _x, _y, collisionFlags ), returnObjects, collisionFlags );
			return true;
		} 
		else if ( _y > y1 && _y <= halfY ) 
		{
			CopyFilterList( NW->GetObjectsAt( _x, _y, collisionFlags ), returnObjects, collisionFlags );
			return true;
		}
	}
	return false;
}

//-------------------------------------------------------------------------------


bool Quadtree::GetObjectsAtMin( list<PartitionObject*>& listOfStuff, float _x, float _y, U32 collisionFlags )
{
	if ( level == maxLevel ) 
	{
		CopyFilterList( objects, listOfStuff, collisionFlags );
		return true;
	}

	float halfX = ( x2+x1 ) * 0.5f;
	float halfY = ( y2+y1 ) * 0.5f;

	Quadtree* childToCopy = NULL;
	if ( _x > halfX && _x < x2 ) 
	{
		if ( _y > halfY && _y < y2 ) 
		{
			childToCopy = SE;
		} 
		else if ( _y > y1 && _y <= halfY ) 
		{
			childToCopy = NE;
		}
	} 
	else if ( _x > x1 && _x <= halfX ) 
	{
		if ( _y > halfY && _y < y2 ) 
		{
			childToCopy = SW;
		} 
		else if ( _y > y1 && _y <= halfY ) 
		{
			childToCopy = NW;
		}
	}

	if( childToCopy )
	{
		return childToCopy->GetObjectsAtMin( listOfStuff, _x, _y, collisionFlags );
	}
	return false;
}

//-------------------------------------------------------------------------------
/*
bool Quadtree::GetObjectsAtMin( list<PartitionObject*>& listOfStuff, float _x, float _y, U32 collisionFlags )
{
	if ( level == maxLevel ) 
	{
		CopyFilterList( objects, listOfStuff, collisionFlags );
		return true;
	}

	float halfX = ( x2+x1 ) * 0.5f;
	float halfY = ( y2+y1 ) * 0.5f;

	Quadtree* childToCopy = NULL;
	if ( _x > halfX && _x < x2 ) 
	{
		if ( _y > halfY && _y < y2 ) 
		{
			childToCopy = SE;
		} 
		else if ( _y > y1 && _y <= halfY ) 
		{
			childToCopy = NE;
		}
	} 
	else if ( _x > x1 && _x <= halfX ) 
	{
		if ( _y > halfY && _y < y2 ) 
		{
			childToCopy = SW;
		} 
		else if ( _y > y1 && _y <= halfY ) 
		{
			childToCopy = NW;
		}
	}

	if( childToCopy )
	{
		list<PartitionObject*>& listOfChldObjects = childToCopy->GetObjectsAtMin( _x, _y, collisionFlags );
		if( listOfChldObjects.size() == 0 )
		{
			CopyFilterList( objects, listOfChldObjects, collisionFlags );
		}
		CopyFilterList( listOfChldObjects, listOfStuff, collisionFlags );
		return true;
	}
	else// no too sure about this else case.
	{
		CopyFilterList( objects, listOfStuff, collisionFlags );
	}
	return false;
}*/

//-------------------------------------------------------------------------------

bool  Quadtree::GetObjectsAt( list<PartitionObject*>& listOfChldObjects, float _x, float _y, float dist, U32 collisionFlags )
{
	if ( level == maxLevel ) 
	{
		CopyFilterList( objects, listOfChldObjects, collisionFlags );
		return true;
	}

	float halfX = ( x2+x1 ) * 0.5f;
	float halfY = ( y2+y1 ) * 0.5f;

	Quadtree* childToCopy = NULL;
	if ( _x > halfX && _x < x2 ) 
	{
		if ( _y > halfY && _y < y2 ) 
		{
			childToCopy = SE;
		} 
		else if ( _y > y1 && _y <= halfY ) 
		{
			childToCopy = NE;
		}
	} 
	else if ( _x > x1 && _x <= halfX ) 
	{
		if ( _y > halfY && _y < y2 ) 
		{
			childToCopy = SW;
		} 
		else if ( _y > y1 && _y <= halfY ) 
		{
			childToCopy = NW;
		}
	}

	if( childToCopy )
	{
		childToCopy->GetObjectsAt( listOfChldObjects, _x, _y, dist, collisionFlags );
		if( listOfChldObjects.size() == 0 )
		CopyFilterList( objects, listOfChldObjects, _x, _y, dist, collisionFlags );
		return true;
	}
	else// no too sure about this else case.
	{
		CopyFilterList( objects, listOfChldObjects,  _x, _y, dist, collisionFlags );
	}
	return false;
}

//-------------------------------------------------------------------------------
/*
list<PartitionObject*> Quadtree::GetObjectsAtMin( float _x, float _y, U32 collisionFlags ) 
{
	if ( level == maxLevel ) 
	{
		return FilterList( objects, collisionFlags );
	}
	
	list<PartitionObject*> returnObjects, childReturnObjects;

	float halfX = ( x2+x1 ) * 0.5f;
	float halfY = ( y2+y1 ) * 0.5f;

	Quadtree* childToCopy = NULL;
	if ( _x > halfX && _x < x2 ) 
	{
		if ( _y > halfY && _y < y2 ) 
		{
			childToCopy = SE;
		} 
		else if ( _y > y1 && _y <= halfY ) 
		{
			childToCopy = NE;
		}
	} 
	else if ( _x > x1 && _x <= halfX ) 
	{
		if ( _y > halfY && _y < y2 ) 
		{
			childToCopy = SW;
		} 
		else if ( _y > y1 && _y <= halfY ) 
		{
			childToCopy = NW;
		}
	}

	if( childToCopy )
	{
		list<PartitionObject*>& listOfChldObjects = childToCopy->GetObjectsAtMin( _x, _y, collisionFlags );
		if( listOfChldObjects.size() == 0 )
		{
			CopyFilterList( objects, listOfChldObjects, collisionFlags );
		}
		CopyFilterList( listOfChldObjects, returnObjects, collisionFlags );
	}
	return returnObjects;
}
*/
//-------------------------------------------------------------------------------

list<PartitionObject*> Quadtree::GetObjectsAt( float _x, float _y, U32 collisionFlags ) 
{
	if ( level == maxLevel ) 
	{
		return FilterList( objects, collisionFlags );
	}
	
	list<PartitionObject*> returnObjects, childReturnObjects;
	if ( !objects.empty() ) 
	{
		returnObjects = FilterList( objects, collisionFlags );
	}
	float halfX = ( x2+x1 ) * 0.5f;
	float halfY = ( y2+y1 ) * 0.5f;

	if ( _x > halfX && _x < x2 ) 
	{
		if ( _y > halfY && _y < y2 ) 
		{
			childReturnObjects = SE->GetObjectsAt( _x, _y, collisionFlags );
			returnObjects.insert( returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end() );
			return returnObjects;
		} 
		else if ( _y > y1 && _y <= halfY ) 
		{
			childReturnObjects = NE->GetObjectsAt( _x, _y, collisionFlags );
			returnObjects.insert( returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end() );
			return returnObjects;
		}
	} 
	else if ( _x > x1 && _x <= halfX ) 
	{
		if ( _y > halfY && _y < y2 ) 
		{
			childReturnObjects = SW->GetObjectsAt( _x, _y, collisionFlags );
			returnObjects.insert( returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end() );
			return returnObjects;
		} 
		else if ( _y > y1 && _y <= halfY ) 
		{
			childReturnObjects = NW->GetObjectsAt( _x, _y, collisionFlags );
			returnObjects.insert( returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end() );
			return returnObjects;
		}
	}
	return returnObjects;
}

//-------------------------------------------------------------------------------

void Quadtree::Clear() 
{
	if ( level == maxLevel ) 
	{
		objects.clear();
		return;
	} 
	else 
	{
		NW->Clear();
		NE->Clear();
		SW->Clear();
		SE->Clear();
	}
	if ( !objects.empty() ) 
	{
		objects.clear();
	}
}

//-------------------------------------------------------------------------------

bool Quadtree::Contains( Quadtree *child, PartitionObject *object ) 
{
	return	 !( object->x1 < child->x1 ||
				object->y1 < child->y1 ||
				object->x1 > child->x2 ||
				object->y1 > child->y2 ||
				object->x2 < child->x1 ||
				object->y2 < child->y1 ||
				object->x2 > child->x2 ||
				object->y2 > child->y2 );
}

//-------------------------------------------------------------------------------

void Quadtree::Remove( PartitionObject *object )
{
	if( object == NULL )
		return;

	if ( level == maxLevel ) 
	{
		objects.remove( object );
		return;
	}
	if ( Contains( NW, object ) ) 
	{
		NW->Remove( object ); 
		return;
	} 
	else if ( Contains( NE, object ) ) 
	{
		NE->Remove( object ); 
		return;
	} 
	else if ( Contains( SW, object ) ) 
	{
		SW->Remove( object ); 
		return;
	} 
	else if ( Contains( SE, object ) ) 
	{
		SE->Remove( object ); 
		return;
	}
	if ( Contains( this, object ) ) 
	{
		objects.remove( object );
	}
}

//-------------------------------------------------------------------------------

void Quadtree::UpdateObject( PartitionObject *object )
{
	Remove( object );
	AddPartitionObject( object );
}

//-------------------------------------------------------------------------------
