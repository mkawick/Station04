#pragma once
#include "DataView.h"

class PlayerShipDataView : public DataView
{
public:
	enum Columns
	{
		Column_ShipName,
		Column_Type,
		Column_ShipShield1,
		Column_ShipShield2,
		Column_ShipShield3,
		Column_Speed,
		Column_Direction,
		Column_Health,
		Column_Count
	};
	PlayerShipDataView();
	~PlayerShipDataView();
	const char* GetName() const { return DataViewName; }

	int		GetNumRows() const { return 0; }
	int		GetNumColumns() const { return Column_Count; }
	bool	GetColumnName( std::string& name ) const { return false; }
	bool	GetData( int row, int column, std::string& data ) const {return false;}
private:
	static const char*		DataViewName;
};
