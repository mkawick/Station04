//  Input.h
//  Station05

namespace	CORE_AI
{
	class	GameObject;		// forward decl
	//--------------------------------------------

	class Input
	{
	public:
		Input ();
		
		void	Update (GameObject& game);
		bool	HasExited () const {return ExitKeyHit;}
		
	protected:
		bool	ExitKeyHit;
		unsigned char	LastKeyPressed;
	};
	//--------------------------------------------
};