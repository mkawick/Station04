/*
 *  AssetLoadManager.cpp
 *  Station05
 */

#include "StdAfx.h"
#include <windows.h>
#include <Shlwapi.h>
#include "../tools/GL/include/glut.h"
#include "../tools/SDL/include/SDL.h"
#include <iostream>
#include <fstream>
using namespace std;
#include <stdlib.h>

#include <boost/scoped_array.hpp>
#include <boost/functional/hash.hpp>
//#include <boost/filesystem.hpp>

#include "AssetLoadManager.h"

#include "GlobalDefinitions.h"
#include "GameFramework.h"

#include "../Common/TGA_Loader.h"

//--------------------------------------------------------------------
//--------------------------------------------------------------------

AssetLoadManager :: AssetLoadManager()
{
	loggingStream = aiGetPredefinedLogStream( aiDefaultLogStream_STDOUT,NULL );
	aiAttachLogStream( &loggingStream );

	loggingStream2 = aiGetPredefinedLogStream( aiDefaultLogStream_FILE, "assimp_log.txt" );
	aiAttachLogStream( &loggingStream2 );
}

AssetLoadManager :: ~AssetLoadManager()
{
	Shutdown();
}

void    AssetLoadManager :: Shutdown()
{
    for( AssetPairIter it = assets.begin(); it != assets.end(); it++ )
	{
		delete it->second;
	}
    assets.clear();
    for( TexturePairIter it = textures.begin(); it != textures.end(); it++ )
	{
		glDeleteTextures( 1, &it->second );
	}
    textures.clear();
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

GLuint LoadTGATexture( const char *TexName )
{
	TGA_Loader Img;        // Image loader
	GLuint Texture;

	// Load our Texture
	if( Img.Load( TexName ) != IMG_OK )
		return -1;

	glGenTextures(1,&Texture);            // Allocate space for texture
	glBindTexture(GL_TEXTURE_2D,Texture); // Set our Tex handle as current

	// Create the texture
	if( Img.GetBPP() == 24 )
	{
		glTexImage2D( GL_TEXTURE_2D, 0, 3, Img.GetWidth(), Img.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, Img.GetImg() );
	}
	else if( Img.GetBPP() == 32 )
	{
		glTexImage2D( GL_TEXTURE_2D, 0, 4, Img.GetWidth(), Img.GetHeight(),0, GL_RGBA, GL_UNSIGNED_BYTE, Img.GetImg() );
	}
	else
	 return -1;

	// Specify filtering and edge actions
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

	return Texture;
}

GLuint LoadBMPTexture( const char* fileName )
{
	SDL_Surface *bmp;
	bmp = SDL_LoadBMP( fileName );

	if(bmp == NULL)
	{
		return -1;
	}

	SDL_LockSurface( bmp );
	int bpp = bmp->format->BitsPerPixel;
	int height = bmp->h;
	int width = bmp->w;
	void* ptrToPixels = bmp->pixels;
	SDL_UnlockSurface( bmp );

	GLuint Texture;
	GLenum errorCode;
	if( height != 0 && width != 0 && ptrToPixels != NULL )
	{
		glGenTextures( 1, &Texture );
		glBindTexture(GL_TEXTURE_2D, Texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptrToPixels );

	}
	SDL_FreeSurface( bmp );

	errorCode = glGetError();

	return Texture;
}

GLuint LoadTexture( const char *TexName )
{
	//boost::filesystem::path const boostName( TexName );
	const char* extension = PathFindExtensionA( TexName );
	//boostName.stem().c_str();
	if( stricmp( extension, ".tga" ) == 0 )
	{
		return LoadTGATexture( TexName );
	}

	if( stricmp( extension, ".bmp" ) == 0 )
	{
		return LoadBMPTexture( TexName );
	}
	return -1;
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

	//TCHAR dirName[MAX_PATH];	
	//GetCurrentDirectory( MAX_PATH, dirName );
	if( keyLookup && fileName )
	{
		GLuint texture = ::LoadTexture( fileName );

		if ( texture != -1 ) 
		{
			boost::hash<std::string> string_hash;
			std::size_t hash = string_hash( keyLookup );

			//AssetObject* asset = new AssetObject();
			//if( asset->Load( filePath ) == true )
			{
				textures.insert( TexturePair( hash, texture ) );
				return true;
			}
			//return UINT_MAX;
			//return true;
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
			bool success = GlobalGameFramework->GetInput().LoadIniFile( root, filePath );
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
		else if( stricmp( keyLookup, "ui_status" ) == 0 )
		{
			bool success = GlobalGameFramework->GetUI().LoadIniFile( root, filePath );
			if( success == false )
			{
				cout << "ERROR: player status ui ini file bad: " << filePath << endl;
				return false;
			}
		}
	}

	return false;
}

bool AssetLoadManager :: LoadAudioFiles( json_t* filesObj, const char* filePath )
{
	return false;
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

GLuint	AssetLoadManager :: FindTexture( const char* key )
{
	boost::hash<std::string> string_hash;

    std::size_t hash = string_hash( key );

	TexturePairIter iter;
	iter = textures.find( hash );
	if( iter == textures.end() )
		return -1;

	GLuint texture = (iter)->second;

	return texture;
}

//--------------------------------------------------------------------

U32	AssetLoadManager :: Load( const char* key, const char* filePath )
{
	boost::hash<std::string> string_hash;

    std::size_t hash = string_hash( key );

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

void	AssetLoadManager :: RenderTexture( GLuint texture, const Vector2D& ul, const Vector2D& br )
{
	glBindTexture( GL_TEXTURE_2D, texture );

    glEnable( GL_TEXTURE_2D );

	glBegin( GL_QUADS );

	glTexCoord2f (0.0, 0.0);	glVertex3f ( ul.x, ul.y, 0.0 );
	glTexCoord2f (1.0, 0.0);	glVertex3f ( br.x, ul.y, 0.0 );
	glTexCoord2f (1.0, 1.0);	glVertex3f ( br.x, br.y, 0.0 );
	glTexCoord2f (0.0, 1.0);	glVertex3f ( ul.x, br.y, 0.0 );
	
	glEnd();

	glDisable( GL_TEXTURE_2D );
}

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

	if( DrawList )
	{
		glDeleteLists( DrawList, 1 );
	}
	DrawList = 0;
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
		const struct aiMesh* mesh = scene->mMeshes[ nd->mMeshes[n] ];
		if( mesh == NULL )
			continue;

		ApplyMaterial( sc->mMaterials[mesh->mMaterialIndex] );

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

