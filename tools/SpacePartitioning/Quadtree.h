#pragma once
//Quadtree.h

#include "../../Source/struct.h"
#include <vector>
#include <list>
using namespace std;

class PartitionObject;

class Quadtree 
{
	enum {UninitializedLevel = -1};
public:
	Quadtree();
	void					Init( float x1, float y1, float x2, float y2, int level, int maxLevel );
	int						GetMaxDepth() const { return maxLevel; }
	float					GetMinimumPartitionX() const;
	float					GetMinimumPartitionY() const;

	void					AddPartitionObject( PartitionObject *object );
	list<PartitionObject*>  GetObjectsAt( float x, float y, U32 collisionFlags = 0 );
	bool					GetObjectsAtMin( list<PartitionObject*>& listOfStuff, float x, float y, U32 collisionFlags = 0 );
	list<PartitionObject*>  GetObjectsAtMin( float x, float y, U32 collisionFlags = 0 );
	list<PartitionObject*>  GetObjectsAt( float x1, float y1, float x2, float y2, U32 collisionFlags = 0 );// this is an approximation only
	void					Clear();

	void					Remove( PartitionObject *object );
	void					UpdateObject( PartitionObject *object );
private:
	float					x1, y1, x2, y2;
	int						level;
	int						maxLevel;
	list<PartitionObject*>  objects;

	Quadtree *				parent;
	Quadtree *				NW;
	Quadtree *				NE;
	Quadtree *				SW;
	Quadtree *				SE;

	bool					GetObjectsAt( list<PartitionObject*>& listOfStuff, float x, float y, U32 collisionFlags = 0 );
	bool					Contains( Quadtree *child, PartitionObject *object );

	//list<PartitionObject*>  FilterList( list<PartitionObject*>& listOfObjects, U32 collisionFlags );
	
};
