#pragma once
#include <list>
#include "DataView.h"


class DataViewManager
{
public:
	DataViewManager();
	~DataViewManager();

	void	AddView( DataView* view );

	DataView* GetDataView( const std::string& name );

	void	OnUpdate();
protected:

	DataViewList ListOfViews;
	DataViewLookup ViewLookup;
};
