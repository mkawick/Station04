/*
 *  NetworkManager.h
 *  Station05
 */

#include "GameData.h"

class NetworkManager
{
public:
	NetworkManager ();
	~NetworkManager ();
	void	Update (GameData& restrict); // data
    void    Shutdown() {}
};