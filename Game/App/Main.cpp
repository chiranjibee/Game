// App headers
#include "App.h"
#include "Sound.h"
#include "Controller.h"

// Defaults
int WINDOW_WIDTH = APP_INIT_WINDOW_WIDTH;
int WINDOW_HEIGHT = APP_INIT_WINDOW_HEIGHT;
HWND MAIN_WINDOW_HANDLE = nullptr;

static constexpr double UPDATE_MAX = ((1.0 / APP_MAX_FRAME_RATE)*1000.0);

// Internal globals for timing
double GPCFreq = 0.0;
__int64 GCounterStart = 0;
double GLastTime;

// Extern Game functions
extern void Init();
extern void Update(float DeltaTime);
extern void Render();
extern void Shutdown();

// Extern Editor functions
extern void EditorInit();
extern void EditorUpdate(float DeltaTime);
extern void EditorRender();
extern void EditorShutdown();

void StartCounter()
{
	LARGE_INTEGER Li;
	if (!QueryPerformanceFrequency(&Li))
	{
		printf("Failed to init performance counters.");
	}
	GPCFreq = static_cast<double>(Li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&Li);
	GCounterStart = Li.QuadPart;
}

double GetCounter()
{
	LARGE_INTEGER Li;
	QueryPerformanceCounter(&Li);
	return (static_cast<double>(Li.QuadPart - GCounterStart) / GPCFreq);
}


class CProfiler
{
public:
	CProfiler() : StartTime(0), ElapsedTime(0)
	{		
	}
	void Start()
	{
		StartTime = GetCounter();
	}
	double Stop()
	{ 
		ElapsedTime = GetCounter() - StartTime;
		return ElapsedTime;
	}
	void Print(float InX, float InY, const char* InText)
	{
		char TextBuffer[64];
		sprintf(TextBuffer, "%s: %0.4f ms", InText,ElapsedTime);
		App::RenderText(InX, InY, TextBuffer, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_10);
	}
private:	
	double StartTime;
	double ElapsedTime;
};

CProfiler GUserRenderProfiler;
CProfiler GUserUpdateProfiler;
CProfiler GUpdateDeltaTime;
bool bRenderUpdateTimes = APP_RENDER_UPDATE_TIMES;
bool bEditorMode = false;

/**
 * InitGL: Initialize OpenGL graphics
 */
void InitGL()
{
	StartCounter();
	GLastTime = GetCounter();

	// Black and opaque
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

/**
 * DisplayGL: Callback for window repaint GL function
 */
void DisplayGL()
{
	// Clear the color buffer with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);

	// Call user defined render
	GUserRenderProfiler.Start();
	if (bEditorMode)
	{
		EditorRender();
	}
	else
	{
		Render();
	}
	GUserRenderProfiler.Stop();

	// Render profiler stats
	if (bRenderUpdateTimes)
	{
		GUpdateDeltaTime.Print(10, 750, "Update");
		GUserRenderProfiler.Print(110, 750, "User Render");
		GUserUpdateProfiler.Print(210, 750, "User Update");
	}

	// Render now
	glFlush();
}

/**
 * IdleGL: Main game loop to use with GL idle
 */
void IdleGL()
{	
	static double PrevTime = GetCounter();
	double Tick = GetCounter() - PrevTime;
	const double CurrentTime = GetCounter();
	const double DeltaTime = CurrentTime - GLastTime;
	
	// Update
	if (DeltaTime > (UPDATE_MAX))
	{	
		GUpdateDeltaTime.Stop();
		glutPostRedisplay();
		CControllerManager::GetInstance().Update();

		// Call user defined update
		GUserUpdateProfiler.Start();
		if (bEditorMode)
		{
			EditorUpdate(static_cast<float>(DeltaTime));
		}
		else
		{
			Update(static_cast<float>(DeltaTime));
		}
		GUserUpdateProfiler.Stop();
		
		GLastTime = CurrentTime;		
		RECT tileClientArea;
		if (GetClientRect(MAIN_WINDOW_HANDLE, &tileClientArea))
		{
			WINDOW_WIDTH = tileClientArea.right - tileClientArea.left;
			WINDOW_HEIGHT = tileClientArea.bottom - tileClientArea.top;
		}

		if (App::GetController().CheckButton(APP_ENABLE_DEBUG_INFO_BUTTON) )
		{
			bRenderUpdateTimes = !bRenderUpdateTimes;
		}

		if (App::IsKeyPressed(APP_QUIT_KEY))
		{		
			glutLeaveMainLoop();
		}
		
		GUpdateDeltaTime.Start();
	}
	
}

void CheckMemCallback()
{
}

/**
 * WinMain: Entry Point
 */
int APIENTRY wWinMain(	_In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPWSTR    lpCmdLine,
						_In_ int       nCmdShow)
{
	int Argc = 0;
	char* Argv;

	// Exit handler to check memory on exit
	const int ResHandler = std::atexit(CheckMemCallback);

	// TODO: @CS, Uncomment below line to run editor mode without cmd line
	std::wstring CmdLine(lpCmdLine);
	bEditorMode = CmdLine.find(L"-Editor") != std::string::npos;
	//bEditorMode = true;

	// Setup glut
	glutInit(&Argc, &Argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	int GlutWnd = glutCreateWindow(APP_WINDOW_TITLE);	
	HDC Hdc = wglGetCurrentDC();
	MAIN_WINDOW_HANDLE = WindowFromDC(Hdc);
	glutIdleFunc(IdleGL);
	glutDisplayFunc(DisplayGL);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	InitGL();


	// Init sounds system
	CSound::GetInstance().Initialize(MAIN_WINDOW_HANDLE);
	
	// Call user defined init
	if (bEditorMode)
	{
		EditorInit();
	}
	else
	{
		Init();
	}

	// Enter glut the event-processing loop				
	glutMainLoop();
	
	// Call user shutdown
	if (bEditorMode)
	{
		EditorShutdown();
	}
	else
	{
		Shutdown();
	}

	// Shutdown sound system
	CSound::GetInstance().Shutdown();

	// And we are done
	return 0;
}


