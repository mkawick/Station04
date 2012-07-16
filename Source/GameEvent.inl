/*
 *  GameEvent.inl
 *  Station05
 */

#include <assert.h>

//----------------------------------------------

template <typename t>
GameEventWrapper :: GameEventWrapper (const t& event)
		{
			SetMessage (event);
		}
		
template <typename t>
void	GameEventWrapper :: SetMessage (const t& event)// it's copied anyway
		{
			assert (sizeof (event) <= MAX_DATA_SIZE);
			int size = sizeof (event);
			memcpy (BufferToHoldGenericData, &event, size);
		}

//----------------------------------------------
		
template <typename gameEvent>
bool	GameEventQueue :: Enqueue (const gameEvent& event)
		{
			assert (size () < MAX_ITEMS);
			
			if (size () >= MAX_ITEMS) 
				return false;
			
			GameEventWrapper wrapper (event);
			push (wrapper);
			
			return true;
		}

//----------------------------------------------
		
template <typename gameEvent>
void	MessageSenderReceiver :: SendMessages (const gameEvent& event)
		{
			SendQueue.Enqueue (event);
		}

//----------------------------------------------
