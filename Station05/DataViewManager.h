#pragma once
#include <list>
#include "DataView.h"


class DataViewManager
{
public:
	DataViewManager();
	~DataViewManager();

	void	AddView( DataView* view );

	void	OnUpdate();
protected:

	DataViewList ListOfViews;
};
