/*
 *  LinkedListContainer.h
 *  SDL04_Station
 */

//---------------------------------------------------------
//---------------------------------------------------------

template <typename type>
class LinkContainer
{
public:
	//------------------------------------
	LinkContainer () {Head = NULL;}
	~LinkContainer ()
	{
		while (Head)
		{
			Link* next = Head->next;
			delete Head;
			Head = next;
		}
	}
	//------------------------------------
	void	AddValue (type val)
	{
		Link* newLink = new Link;
		newLink->value = val;
		newLink->next = NULL;
		
		if (Head == NULL)
		{
			Head = newLink;
			return;
		}
		
		if (Head->value > val)// special case where we need a new head
		{
			newLink->next = Head;
			Head = newLink;
			return;
		}
		
		Link* ptr = Head;// track previous and current
		Link* prev = Head;
		while (prev && ptr)
		{
			if (ptr->value > val)
			{
				newLink->next = ptr;
				prev->next = newLink;
				return;
			}
			prev = ptr;
			ptr = ptr->next;// advance the pointer
		}
		
		Head->next = newLink;
	}
	//------------------------------------
	void	RemoveValue (type val)
	{
		if (Head->value == val)// special case, remove head
		{
			Link* temp = Head;
			Head = Head->next;
			delete temp;		
		}
		
		Link* ptr = Head;// track previous and current
		Link* prev = Head;
		while (prev && ptr)
		{
			if (ptr->value == val)
			{
				Link* temp = ptr;
				prev->next = ptr->next;
				delete temp;
				return;
			}
			prev = ptr;
			ptr = ptr->next;// advance the pointer
		}
	}
	//------------------------------------
	void	Output ()
	{
		Link* walker = Head;
		while (walker)
		{
			cout << walker->value << ", ";
			walker = walker->next;
		}
		cout << endl;
	}
protected:
	Link*	Head;
protected:
	struct Link 
	{
		type	value;
		Link*	next;
	};
};

//---------------------------------------------------------
//---------------------------------------------------------