/*
 *  Drawing.h
 *  SDL04_Station
 *
 */
namespace	CORE_AI
{
class	SimpleDrawing
{
public:
	SimpleDrawing (int width = 640, int height = 480);
	~SimpleDrawing ();
	
	void	SetBackgroundColor (int red, int green, int blue);
	
	void	Render ();
	void	SetColor (int red = 0, int green = 0, int blue = 0);
	void	Line (int x1, int y1, int x2, int y2);
	void	Rect (int left, int top, int right, int bottom);
	void	Dot (int x, int y);
	
	void	Circle (int x, int y, int radius);
	void	Text (int x, int y, const char* text); 
protected:
	float	ConvertToColor (int);
	int		Width, Height;
};
}