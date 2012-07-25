#pragma once
#include <list>

// This class exists to allow a observer on data, often for the UI.
// It provides a nice abstraction allowing a UI element to observe
// data in the game and when things change, to update internal values.

class DataViewObserver
{
public:
	virtual void	OnUpdate(){}
	virtual void	OnRowChange( int row ){}
	virtual void	OnDatumChanged( int row, int column, const char* strValue ){}
	virtual void	OnDatumChanged( int row, int column, const std::string& strValue ){}
};
//-------------------------------------------------------------------

typedef std::list< DataViewObserver* >		DataViewObserverList;
typedef DataViewObserverList::iterator		DataViewObserverListIter;

//-------------------------------------------------------------------
// The data view is an abstraction that the UI can use to 
class DataView
{
public:
	DataView();
	~DataView();

	virtual const char* GetName() const { return NULL; }
	virtual void	OnUpdate();
	virtual void	OnRowChange( int row );
	virtual void	OnDatumChanged( int row, int column, const char* strValue );
	virtual void	OnDatumChanged( int row, int column, const std::string& strValue );

	virtual int		GetNumRows() const { return 0; }
	virtual int		GetNumColumns() const { return 0; }
	virtual bool	GetColumnName( int column, std::string& name ) const { return false; }
	virtual bool	GetData( int row, int column, std::string& data ) const {return false;}

public:
	
protected:
	DataViewObserverList	Registrants;
};
//-------------------------------------------------------------------

typedef std::list< DataView* >  DataViewList;
typedef DataViewList::iterator  DataViewListIter;