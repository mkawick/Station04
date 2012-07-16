// ConfigFileLoader.h

#include <boost/scoped_array.hpp>
#include "../tools/json/jansson.h"

namespace ConfigFileLoader
{

	/*
		Notes:
		the Loader class will need to support the following interface:
	*/
	class FileLoader
	{
	public:
		virtual bool	LoadOtherFiles( json_t* jsonObj, const char* filePath ) {return false;}
		virtual bool	LoadTextureFiles( json_t* jsonObj, const char* filePath ) {return false;}
		virtual bool	LoadModelFiles( json_t* jsonObj, const char* filePath ) {return false;}
		virtual bool	LoadAudioFiles( json_t* jsonObj, const char* filePath ) {return false;}
		virtual bool	LoadIniFiles( json_t* jsonObj, const char* filePath ) {return false;}
		virtual bool	LoadModel( json_t* item ) {return false;}
		virtual bool	LoadTexture( json_t* item ) {return false;}
	};

template <typename Loader>
bool LoadConfigFile( Loader* loader, const char* filePath )
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

}