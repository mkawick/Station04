/*
 *  AssetLoadManager.cpp
 *  Station05
 */

#include "StdAfx.h"
#include <windows.h>
#include "../tools/GL/include/glut.h"
#include <stdlib.h>
#include "../tools/SDL/include/SDL.h"
#include <iostream>
#include <fstream>
using namespace std;

#include <boost/scoped_array.hpp>
#include <boost/functional/hash.hpp>
#include "AssetLoadManager.h"
#include "GlobalDefinitions.h"
#include "GameFramework.h"

//--------------------------------------------------------------------
//--------------------------------------------------------------------

AssetLoadManager :: AssetLoadManager()
{
	loggingStream = aiGetPredefinedLogStream( aiDefaultLogStream_STDOUT,NULL );
	aiAttachLogStream( &loggingStream );

	loggingStream2 = aiGetPredefinedLogStream( aiDefaultLogStream_FILE,"assimp_log.txt" );
	aiAttachLogStream( &loggingStream2 );
}

AssetLoadManager :: ~AssetLoadManager()
{
	for( AssetPairIter it = assets.begin(); it != assets.end(); it++ )
	{
		delete it->second;
	}
	aiDetachAllLogStreams();
}

//--------------------------------------------------------------------

bool	AssetLoadManager :: LoadModel( json_t* item )
{
	if( json_object_get( item, "ignore") )
	{
		return true;
	}

	const char* keyLookup = NULL;
	const char* fileName = NULL;
	json_t *		fileObj = json_object_get( item, "file");
	json_t *		keyObj = json_object_get( item, "key");
	if( json_is_string( keyObj ) )
		keyLookup = json_string_value( keyObj );
	if( json_is_string( fileObj ) )
		fileName = json_string_value( fileObj );

	if( stricmp( fileName, "end" ) == 0 )
		return true;

	if( keyLookup && fileName )
	{
		U32 hashKey = Load( keyLookup, fileName );
		if( hashKey && hashKey != UINT_MAX)
		{
			json_t *scaleObj = json_object_get( item, "scale");
			if( json_is_real( scaleObj ) )
			{
				float newScale = static_cast< float >( json_real_value( scaleObj ) );
				AssetPairIter it = assets.find( hashKey );
				if( it != assets.end() )
				{
					AssetObject* obj = it->second;
					if( obj )
						obj->SetScale( newScale );
				}
			}
		}
		
		return true;
	}

	return false;
}



bool	AssetLoadManager :: LoadTexture( json_t* item )
{
	if( json_object_get( item, "ignore") )
	{
		return true;
	}

	const char* keyLookup = NULL;
	const char* fileName = NULL;
	json_t *		fileObj = json_object_get( item, "file");
	json_t *		keyObj = json_object_get( item, "key");
	if( json_is_string( keyObj ) )
		keyLookup = json_string_value( keyObj );
	if( json_is_string( fileObj ) )
		fileName = json_string_value( fileObj );

	if( stricmp( fileName, "end" ) == 0 )
		return true;

	if( keyLookup && fileName )
	{
		const aiScene*	scene = aiImportFile( fileName, aiProcessPreset_TargetRealtime_MaxQuality );

		if (scene) 
		{
			/*GetBoundingBox( &scene_min, &scene_max );
			scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
			scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
			scene_center.z = (scene_min.z + scene_max.z) / 2.0f;*/
			return true;
		}
	}

	return false;
}
//--------------------------------------------------------------------

bool AssetLoadManager :: LoadOtherFiles( json_t* filesObj, const char* filePath )
{
	if( json_is_array( filesObj ) )
	{
		int numItems = json_array_size( filesObj );
		for( int i=0; i< numItems; i++ )
		{
			json_t * arrayItem = json_array_get( filesObj, i );
			if( json_is_object( arrayItem ) )
			{
				json_t *		fileObj = json_object_get( arrayItem, "manifest");
				if( fileObj )
				{
					const char*		fileName = json_string_value( fileObj );
					if( ConfigFileLoader::LoadConfigFile( this, fileName ) == false )
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}

bool AssetLoadManager :: LoadModelFiles( json_t* filesObj, const char* filePath )
{
	if( json_is_array( filesObj ) )
	{
		int numItems = json_array_size( filesObj );
		for( int i=0; i< numItems; i++ )
		{
			json_t * arrayItem = json_array_get( filesObj, i );
			if( json_is_object( arrayItem ) )
			{
				if( LoadModel( arrayItem ) == false )
				{
					cout << "ERROR: file entries are bad: " << filePath << endl;
					return false;
				}
			}
		}
	}
	else
	{
		cout << "ERROR: file is missing main key: " << filePath << endl;
		return false;
	}
	return true;
}

bool AssetLoadManager :: LoadTextureFiles( json_t* filesObj, const char* filePath )
{
	if( json_is_array( filesObj ) )
	{
		int numItems = json_array_size( filesObj );
		for( int i=0; i< numItems; i++ )
		{
			json_t * arrayItem = json_array_get( filesObj, i );
			if( json_is_object( arrayItem ) )
			{
				if( LoadTexture( arrayItem ) == false )
				{
					cout << "ERROR: file entries are bad: " << filePath << endl;
					return false;
				}
			}
		}
	}
	else
	{
		cout << "ERROR: file is missing main key: " << filePath << endl;
		return false;
	}
	return true;
}

bool AssetLoadManager :: LoadIniFiles( json_t* root, const char* filePath )
{
	json_t *		iniType = json_object_get( root, "type");
	if( iniType )
	{
		const char* keyLookup = json_string_value( iniType );

		if( stricmp( keyLookup, "keyboard" ) == 0 )
		{
			json_t * key_set = json_object_get( root, "key_set");
			bool success = LoadKeyboardFile( key_set, filePath );
			if( success == false )
			{
				cout << "ERROR: keyboard ini file bad: " << filePath << endl;
				return false;
			}
		}
		else if( stricmp( keyLookup, "player_config" ) == 0 )
		{
		}
		else if( stricmp( keyLookup, "network" ) == 0 )
		{
		}
	}

	return false;
}

bool AssetLoadManager :: LoadAudioFiles( json_t* filesObj, const char* filePath )
{
	return false;
}


bool AssetLoadManager :: LoadKeyboardFile( json_t* keySetObj, const char* filePath )
{
	if( json_is_array( keySetObj ) )
	{
		int numItems = json_array_size( keySetObj );
		for( int i=0; i< numItems; i++ )
		{
			json_t * keysetForMode = json_array_get( keySetObj, i );
			if( json_is_object( keysetForMode ) )
			{
				const char* modeLookup = NULL;
				json_t * modeObj = json_object_get( keysetForMode, "mode");
				if( json_is_string( modeObj ) )
					modeLookup = json_string_value( modeObj );
				json_t * setObj = json_object_get( keysetForMode, "set");

				GameMode mode = LookupGameMode( modeLookup );
				if( mode == GameMode_none )
				{
					cout << "ERROR: keyboard ini file has bad mode: " << filePath <<", mode=" << modeLookup << endl;
					return false;
				}
				if( json_is_array( setObj ) )
				{
					int numKeys = json_array_size( setObj );
					for( int i=0; i< numKeys; i++ )
					{
						json_t * keyDefnObj = json_array_get( setObj, i );
						if( json_is_object( keyDefnObj ) )
						{
							U32 modifiers = KeyModifier_none;
							json_t * modifierObj = json_object_get( keyDefnObj, "modifier");
							if( modifierObj )
							{
								const char* modifierString = json_string_value( modifierObj );
								modifiers = LookupKeyModifier( modifierString );
							}
							json_t * keyboardObj = json_object_get( keyDefnObj, "key");
							if( keyboardObj == NULL )
							{
								cout << "ERROR: keyboard ini file has bad key: " << filePath <<", mode=" << modeLookup << endl;
								return false;
							}
							const char* keyString = json_string_value( keyboardObj );
							if( keyString == NULL )
							{
								cout << "ERROR: keyboard ini file has bad key: " << filePath <<", mode=" << modeLookup << endl;
								return false;
							}
							json_t * eventObj = json_object_get( keyDefnObj, "event");
							const char* eventString = json_string_value( eventObj );
							if( eventString == NULL )
							{
								cout << "ERROR: keyboard ini file has bad event: " << filePath <<", key=" << keyString << endl;
								return false;
							}
							json_t * typeObj = json_object_get( keyDefnObj, "type");
							const char* typeString = json_string_value( typeObj );

							json_t * selectionObj = json_object_get( keyDefnObj, "selection");
							int selection = static_cast<int> ( json_integer_value( selectionObj ) );

							json_t * repeatObj = json_object_get( keyDefnObj, "repeat");
							int repeat = static_cast<int> ( json_integer_value( repeatObj ) );

							json_t * holdObj = json_object_get( keyDefnObj, "hold");
							bool allowHold = json_integer_value( holdObj ) ? true : false;

							GlobalGameFramework->GetInput().AddKeyMapping( mode, keyString, eventString, typeString, allowHold, repeat, modifiers, selection );
						}
						else
						{
							cout << "ERROR: keyboard ini file has bad key: " << filePath <<", mode=" << modeLookup << endl;
							return false;
						}
					}
				}
				else
				{
					cout << "ERROR: keyboard ini file has bad set: " << filePath <<", mode=" << modeLookup << endl;
					return false;
				}
			}
		}
	}
	return true;
}
//--------------------------------------------------------------------

int AssetLoadManager :: LoadFromManifest( const char* filePath )
{
	if( ConfigFileLoader::LoadConfigFile( this, filePath ) == false )
	{
		return false;
	}
	return true;
}

AssetObject* AssetLoadManager :: FindObject( const char* key )
{
	boost::hash<std::string> string_hash;

    std::size_t hash = string_hash( key );

	AssetPairIter iter;
	iter = assets.find( hash );
	if( iter == assets.end() )
		return NULL;

	AssetObject* asset = (iter)->second;

	return asset;
}

//--------------------------------------------------------------------

U32	AssetLoadManager :: Load( const char* key, const char* filePath )
{
	boost::hash<std::string> string_hash;

    std::size_t hash = string_hash( key );

	//int id = assets.size();
	AssetObject* asset = new AssetObject();
	if( asset->Load( filePath ) == true )
	{
		assets.insert( AssetPair( hash, asset ) );
		return hash;
	}
	return UINT_MAX;
}


void AssetLoadManager :: Draw( int id )
{
	AssetPairIter it = assets.find( id );
	if( it != assets.end() )
	{
		it->second->Display();
	}
}

void AssetLoadManager :: Draw()
{
	for( AssetPairIter it = assets.begin(); it != assets.end(); it++ )
	{
		it->second->Display();
	}
}

// ----------------------------------------------------------------------------
/*void reshape(int width, int height)
{
	const double aspectRatio = (float) width / height, fieldOfView = 45.0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, aspectRatio,
		1.0, 1000.0);  // Znear and Zfar 
	glViewport(0, 0, width, height);
}*/

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void color4_to_float4(const struct aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

// ----------------------------------------------------------------------------
void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

// ----------------------------------------------------------------------------
AssetObject :: AssetObject() : 
	scene( NULL ), 
	DrawList( 0 ), 
	angle( 0 ),
	scale( 1.0f )
{
}

AssetObject :: ~AssetObject()
{
	if( scene != 0 )
	{
		aiReleaseImport( scene );
	}
}


// ----------------------------------------------------------------------------

bool AssetObject :: Load( const char* path )
{
	// we are taking one of the postprocessing presets to avoid
	// spelling out 20+ single postprocessing flags here.
	scene = aiImportFile( path, aiProcessPreset_TargetRealtime_MaxQuality );

	if (scene) 
	{
		GetBoundingBox( &scene_min, &scene_max );
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
void AssetObject::ApplyMaterial( const struct aiMaterial *mtl )
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	struct aiColor4D diffuse;
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if(ret1 == AI_SUCCESS) {
    	max = 1;
    	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if(ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
        else
        	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else 
		glEnable(GL_CULL_FACE);
}

// ----------------------------------------------------------------------------

void AssetObject::RecursiveRender (const struct aiScene *sc, const struct aiNode* nd)
{
	unsigned int i;
	unsigned int n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) 
	{
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		if( mesh == NULL )
			continue;

		ApplyMaterial(sc->mMaterials[mesh->mMaterialIndex]);

		if(mesh->mNormals == NULL) 
		{
			glDisable(GL_LIGHTING);
		} 
		else 
		{
			glEnable(GL_LIGHTING);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) 
		{
			const struct aiFace* face = &mesh->mFaces[t];
			if( face == NULL )
				continue;
			GLenum face_mode;

			switch(face->mNumIndices) 
			{
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++) 
			{
				int index = face->mIndices[i];
				if(mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if(mesh->mNormals != NULL) 
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		RecursiveRender(sc, nd->mChildren[n]);
	}

	glPopMatrix();
}

// ----------------------------------------------------------------------------
void AssetObject::DoMotion ()
{
	static GLint prev_time = 0;

	int time = glutGet(GLUT_ELAPSED_TIME);
	angle += (time-prev_time)*0.01f;
	prev_time = time;

	glutPostRedisplay ();
}

// ----------------------------------------------------------------------------
void AssetObject::Display()
{
	/*float tmp;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.f,0.f,3.f,0.f,0.f,-5.f,0.f,1.f,0.f);

	// rotate it around the y axis
	glRotatef(angle,0.f,1.f,0.f);

	// scale the whole asset to fit into our view frustum 
	tmp = scene_max.x-scene_min.x;
	tmp = aisgl_max(scene_max.y - scene_min.y,tmp);
	tmp = aisgl_max(scene_max.z - scene_min.z,tmp);
	tmp = 1.f / tmp;
	glScalef(tmp, tmp, tmp);

        // center the model
	glTranslatef( -scene_center.x, -scene_center.y, -scene_center.z );*/

        // if the display list has not been made yet, create a new one and
        // fill it with scene contents
	
	InitDrawList();
	glCallList( DrawList );

/*	glutSwapBuffers();

	do_motion();*/
}

void	AssetObject::InitDrawList()
{
	if(DrawList == 0) 
	{
	    DrawList = glGenLists(1);
	    glNewList( DrawList, GL_COMPILE);
            // now begin at the root node of the imported data and traverse
            // the scenegraph by multiplying subsequent local transforms
            // together on GL's matrix stack.
	    RecursiveRender( scene, scene->mRootNode );
	    glEndList();
	}
}

void	AssetObject::Render( float x, float y, float z, float rx, float ry, float rz, float _scale )
{
	float finalScale = scale * _scale;
	glPushMatrix();		
	
		glTranslatef ( x, y, z);
		
		//float Scale = 0.12F;//1; //0.2
		glScalef ( finalScale, finalScale, finalScale );// order is critical here, we must scale after rotating
		glRotatef (rx+180, 1, 0, 0);// flip front to back
		glRotatef (ry, 0, 1, 0);
		glRotatef (rz, 0, 0, 1);
		
		InitDrawList();

		glCallList (DrawList);

	glPopMatrix();
}
// ----------------------------------------------------------------------------
void AssetObject::GetBoundingBoxForNode (const struct aiNode* nd, 
	struct aiVector3D* min, 
	struct aiVector3D* max, 
	struct aiMatrix4x4* trafo)
{
	struct aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			struct aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);

			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);

			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		GetBoundingBoxForNode(nd->mChildren[n],min,max,trafo);
	}
	*trafo = prev;
}

// ----------------------------------------------------------------------------
void AssetObject::GetBoundingBox (struct aiVector3D* min, struct aiVector3D* max)
{
	struct aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
	GetBoundingBoxForNode(scene->mRootNode,min,max,&trafo);
}

