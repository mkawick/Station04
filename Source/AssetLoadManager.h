#pragma once
/*
 *  AssetLoadManager.h
 *  SDL04_Station
 */

// assimp include files. These three are usually needed.
#include <assimp.h>
#include <aiscene.h>
#include <aipostprocess.h>
/*
 *  AssetLoadManager.h
 *  SDL04_Station
 */
#include "../tools/json/jansson.h"
#include <hash_map>
#include <iostream>
using namespace std;

// forward decl

//--------------------------------------------------------------------------

struct AssetObject
{
	AssetObject();
	~AssetObject();

	bool	Load( const char* path );
	void	Display();
	void	Render( float x, float y, float z, float rx, float ry, float rz, float scale );
	void	DoMotion();

protected:
	void	ApplyMaterial( const struct aiMaterial *mtl );
	void	RecursiveRender( const struct aiScene *sc, const struct aiNode* nd );

	void	GetBoundingBox (struct aiVector3D* min, struct aiVector3D* max);
	void	GetBoundingBoxForNode( const struct aiNode* nd, 
			struct aiVector3D* min, 
			struct aiVector3D* max, 
			struct aiMatrix4x4* trafo );

	void	InitDrawList();

	const aiScene*	scene;
	Uint32		    DrawList;
	aiVector3D		scene_min, scene_max, scene_center;

	float			angle;


};

 typedef pair <int, AssetObject*> AssetPair;
 typedef stdext::hash_map < int, AssetObject* >::iterator  AssetPairIter;
//--------------------------------------------------------------------------

class AssetLoadManager
{
public:
	AssetLoadManager();
	~AssetLoadManager();

	int		LoadFromManifest( const char* manifestFile );
	AssetObject* FindObject( const char* key );

	void	Draw();
	void	Draw( int id );

protected:
	int		Load( const char* keyLoopup, const char* filePath );

	stdext::hash_map< int, AssetObject* >	assets;
	aiLogStream								loggingStream, loggingStream2;

public:
	bool	LoadItem( json_t* item );
};

//--------------------------------------------------------------------------
