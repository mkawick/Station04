/*
 *  AssetLoadManager.cpp
 *  SDL04_Station
 */
#include "StdAfx.h"
#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <iostream>
#include <fstream>
using namespace std;

#include <boost/scoped_array.hpp>
#include <boost/functional/hash.hpp>
#include "AssetLoadManager.h"

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
		if( hashKey )
		{
			json_t *scaleObj = json_object_get( item, "scale");
			if( json_is_real( scaleObj ) )
			{
				float newScale = static_cast< float >( json_real_value( scaleObj ) );
				AssetPairIter it = assets.find( hashKey );
				AssetObject* obj = it->second;
				if( obj )
					obj->SetScale( newScale );
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

template <typename Loader>
bool LoadStuff( Loader* loader, const char* filePath )
{
	//TCHAR dirName[MAX_PATH];
	
	//GetCurrentDirectory( MAX_PATH, dirName );
	ifstream fileStream( filePath );
	if( fileStream.is_open() == false )//open();
	{
		cout << "ERROR: file will not open: " << filePath << endl;
		return false;
	}
	
	int fileSize = fileStream.tellg();// determine how much memory we need
    fileStream.seekg( 0, std::ios::end );
    fileSize = (int)fileStream.tellg() - fileSize + 1;
	fileStream.seekg( 0, std::ios::beg );// move back to the beginning
	if( fileSize < 5 )// absurdly small
	{
		cout << "ERROR: file is absurdly small: " << filePath << endl;
		return false;
	}

	boost::scoped_array<char> buffer(new char[fileSize]);

	fileStream.get( buffer.get(), fileSize, 0 );
	json_error_t	errors;
	json_t *		jsonObj = json_loads( buffer.get(), JSON_DECODE_ANY, &errors );
	if( jsonObj )
	{
		void *iter = json_object_iter( jsonObj );
		while( iter )
		{			
			const char *name = json_object_iter_key( iter );
			json_t * objDetails = json_object_iter_value( iter );

			if( stricmp( name, "files" ) == 0 )
			{
				loader->LoadOtherFiles( objDetails, filePath );
			}
			else if( stricmp( name, "models" ) == 0 )
			{
				loader->LoadModelFiles( objDetails, filePath );
			}
			else if( stricmp( name, "textures" ) == 0 )
			{
				loader->LoadTextureFiles( objDetails, filePath );
			}
			else if( stricmp( name, "ini" ) == 0 )
			{
				loader->LoadIniFiles( objDetails, filePath );
			}
			else if( stricmp( name, "audio" ) == 0 )
			{
				loader->LoadAudioFiles( objDetails, filePath );
			}
			
			iter = json_object_iter_next( jsonObj, iter);
		}
		return true;
	}
	else
	{
		cout << "ERROR: file is formatted improperly: " << filePath << endl;
		cout << "source: " << errors.source << endl;
		cout << "code: " << errors.text << endl;
		return false;
	}
	return false;
}


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
					if( LoadStuff( this, fileName ) == false )
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

bool AssetLoadManager :: LoadIniFiles( json_t* filesObj, const char* filePath )
{
	return false;
}

bool AssetLoadManager :: LoadAudioFiles( json_t* filesObj, const char* filePath )
{
	return false;
}


//--------------------------------------------------------------------

int AssetLoadManager :: LoadFromManifest( const char* filePath )
{
	if( LoadStuff( this, filePath ) == false )
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

