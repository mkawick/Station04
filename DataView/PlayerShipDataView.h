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

	int		GetNumColumns() const { return Column_Count; }

	int		GetNumRows() const;
	bool	GetColumnName( int column, std::string& name ) const;
	bool	GetData( int row, int column, std::string& data ) const;
private:
	static const char*		DataViewName;
};
