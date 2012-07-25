#include "StdAfx.h"
#include "DataViewManager.h"
#include <boost/functional/hash.hpp>

DataViewManager :: DataViewManager()
{
}

DataViewManager :: ~DataViewManager()
{
}

void	DataViewManager :: AddView( DataView* view )
{
	boost::hash< const char* > hasher;
	U32 lookup = hasher( view->GetName() );
	DataViewLookup::iterator it = ViewLookup.find( lookup );
	assert ( it == ViewLookup.end() );
	U32DataViewMappingPair pair( lookup, view );
	ViewLookup.insert( pair );
}

DataView* DataViewManager :: GetDataView( const std::string& name )
{
	boost::hash< std::string > hasher;
	U32 lookup = hasher( name );
	DataViewLookup::iterator it = ViewLookup.find( lookup );
	assert ( it != ViewLookup.end() );
	return (it)->second;
}