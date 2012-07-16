#pragma once
/*
 *  AssetLoadManager.h
 *  SDL04_Station
 */

// assimp include files. These three are usually needed.
#include <assimp.h>
#include <aiscene.h>
#include <aipostprocess.h>

#include "struct.h"

#include "../tools/json/jansson.h"
#include "../common/ConfigFileLoader.h"
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

	void	SetScale( float _scale ) { scale = _scale; }

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
	float			scale;

};

 typedef pair <U32, AssetObject*> AssetPair;
 typedef stdext::hash_map < U32, AssetObject* >::iterator  AssetPairIter;
//--------------------------------------------------------------------------

class AssetLoadManager : public ConfigFileLoader::FileLoader
{
public:
	AssetLoadManager();
	~AssetLoadManager();

	int		LoadFromManifest( const char* manifestFile );
	AssetObject* FindObject( const char* key );

	void	Draw();
	void	Draw( int id );

protected:
	U32		Load( const char* keyLoopup, const char* filePath );

	stdext::hash_map< U32, AssetObject* >	assets;
	aiLogStream								loggingStream, loggingStream2;

public:
	bool	LoadOtherFiles( json_t* jsonObj, const char* filePath );
	bool	LoadTextureFiles( json_t* jsonObj, const char* filePath );
	bool	LoadModelFiles( json_t* jsonObj, const char* filePath );
	bool	LoadAudioFiles( json_t* jsonObj, const char* filePath );
	bool	LoadIniFiles( json_t* jsonObj, const char* filePath );
	bool	LoadModel( json_t* item );
	bool	LoadTexture( json_t* item );
};

//--------------------------------------------------------------------------
