#pragma once


namespace Marbles
{
	class Service;
	class Application
	{
	public:
		
		// static Settings& Settings();
		// static int Run();

		static void Exit(int code = 0);
		static void OnExit(Callback& callback);
		// static Rendering* Renderer();

		template<typename T> static Service Start(T fn);

	private:
		static Application* sApplication;
	};
}