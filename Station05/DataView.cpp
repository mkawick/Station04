#include "StdAfx.h"
#include "DataView.h"

const char*		DataView::DataViewName = "None";
DataView::DataView()
{
}

DataView::~DataView()
{
}

void	DataView::OnUpdate()
{
	DataViewObserverListIter it = Registrants.begin();
	while( it != Registrants.end() )
	{
		DataViewObserver* listener = *it++;
		listener->OnUpdate();
	}
}

void	DataView::OnRowChange( int row )
{
	DataViewObserverListIter it = Registrants.begin();
	while( it != Registrants.end() )
	{
		DataViewObserver* listener = *it++;
		listener->OnRowChange( row );
	}
}