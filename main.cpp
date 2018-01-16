#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <time.h>
#include <iostream>
void Drawsnake(HDC hdc,HWND hwnd,int* x,int* y,int n);//画蛇（连续的小方块）
void Initsnake(int* x,int* y,int* P_Node);       //初始化蛇的长度和位置
void Drawfood(HDC hdc,HWND hwnd,int* x,int* y);                 //刷新食物
int Orient=4,                                    //方向变量，1 2 3 4 对应上下左右
    x[500]={0},y[500]={0},                       //蛇身体的坐标数组
    Foodx,Foody,                                 //食物坐标
    Node;                                        //蛇身体长度（方块数）
bool Pause=FALSE;                                //判断是否暂停
LPCSTR youFail="You Fuck The Snake?!";

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("HHHHHHHH");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = "HHHHH";                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("HHHHHHHHHHH"),       /* Title Text */
           WS_SYSMENU|WS_MINIMIZEBOX, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           800,                 /* The programs width */
           450,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    Initsnake(x,y,&Node);                      //初始化蛇的长度和位置
    Foodx=(rand()%79+1);
    Foody=(rand()%42+1);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    RECT rect;
    HBRUSH hbrush;
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        MoveWindow(hwnd,0,0,806,459,TRUE);
        SetTimer(hwnd,1,100,NULL);
    break;
    case WM_TIMER:
    {
        int i;
        if((x[0]==0)|(y[0]==0)|(x[0]==80)|(y[0]==43))
        {
            KillTimer(hwnd,1);
            MessageBoxA(hwnd,youFail,NULL,MB_OK);
            Initsnake(x,y,&Node);
            SetTimer(hwnd,1,100,NULL);
        }
        if((x[0]==Foodx)&&(y[0]==Foody))
        {
            Foodx=(rand()%79+1);
            Foody=(rand()%42+1);
            Node=Node+5;
        }
        for(i=Node-1;i>0;i--)
        {
            x[i]=x[i-1];
            y[i]=y[i-1];
            if((x[i]==x[0])&&(y[i]==y[0])&&(i>2))
            {
                KillTimer(hwnd,1);
                MessageBoxA(hwnd,youFail,NULL,MB_OK);
                Initsnake(x,y,&Node);
                SetTimer(hwnd,1,100,NULL);
            }
        }
        switch(Orient)
        {
        case 1:
            y[0]--;
            break;
        case 2:
            y[0]++;
            break;
        case 3:
            x[0]--;
            break;
        case 4:
            x[0]++;
            break;
        default:
            x[0]++;
        }
    InvalidateRect(hwnd,NULL,TRUE);
    }
    break;
    case WM_KEYDOWN:
        switch(wParam)
        {
        case VK_UP:
            if((Orient!=1)&&(Orient!=2)&&(Pause==FALSE))
            Orient=1;
            break;
        case VK_DOWN:
            if((Orient!=1)&&(Orient!=2)&&(Pause==FALSE))
            Orient=2;
            break;
        case VK_LEFT:
            if((Orient!=3)&&(Orient!=4)&&(Pause==FALSE))
            Orient=3;
            break;
        case VK_RIGHT:
            if((Orient!=3)&&(Orient!=4)&&(Pause==FALSE))
            Orient=4;
            break;
        case VK_SPACE:
            if(Pause==FALSE)
            {
                KillTimer(hwnd,1);
                Pause=TRUE;
            }
            else
            {
                SetTimer(hwnd,1,100,NULL);
                Pause=FALSE;
            }
        default:
            Orient=4;
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            hdc = BeginPaint(hwnd,&ps);

            Drawsnake(hdc,hwnd,x,y,Node);              //Draw the snake and boundary continually
            Drawfood(hdc,hwnd,&Foodx,&Foody);

            //std::cout<<Foodx<<' '<<Foody<<std::endl;

            EndPaint(hwnd,&ps);
        }
    break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
void Drawsnake(HDC hdc,HWND hwnd,int *x,int *y,int n)
{
    int i;
    RECT rect;

    GetClientRect(hwnd,&rect);
    Rectangle(hdc,5,5,
              (rect.right-rect.left-5),(rect.bottom-rect.top-5));    //Draw the boundary

    //std::cout<<(rect.right-rect.left-5)<<' '<<(rect.bottom-rect.top-5)<<std::endl;
    for(i=0;i<n;i++)
    Rectangle(hdc,10*(x[i])-5,10*(y[i])-5,10*(x[i])+5,10*(y[i])+5);      //Draw the body of snake
}

void Initsnake(int* x,int* y,int* P_Node)
{
    Orient=4;
    srand((UINT)time(NULL));
    x[0]=rand()%40+20;
    srand((UINT)time(NULL));
    y[0]=rand()%20+10;
    *P_Node=5;
    x[1]=x[0]-1;
    y[1]=y[0]-1;
}

void Drawfood(HDC hdc,HWND hwnd,int* x,int* y)              //1~79   1~42
{
    Rectangle(hdc,(*x)*10-5,(*y)*10-5,(*x)*10+5,(*y)*10+5);
}
