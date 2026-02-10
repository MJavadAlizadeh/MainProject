// Phase 4 Project
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
//متغییر ها
#define maxx 100
#define maxy 100
// const int maxx = 100 ;
// const int maxy = 100 ;
char Hunter_Ch = 'H' ;
char Runner_Ch = 'R' ;
char light = 'L' ;
char block = '.' ;
// char V = '|' ;
// char H = '_' ;

//استراکت ها
struct locate {
    int x,y;
};
struct Runner {
    struct locate L;
    int alive;
    int reached;
};
struct Hunter {
    struct locate L;
    int OnLight;
};
struct Wall {
    int h[maxx][maxy];
    int v[maxx][maxy];
};
struct TempWall {
    int h[maxx][maxy];
    int v[maxx][maxy];
};
struct Board {
    char map[maxx][maxy];
    struct locate light;
    int IsRunner[maxx][maxy];
    int IsHunter[maxx][maxy];
    struct Wall walls;
    struct TempWall tempWalls;
};
struct Data {
    int x,y;
    int rcount,hcount;
    int CurrentRunner;
    int CurrentHunter;
    int MaxTempWall;
    int UsedTempWall;
    int ReachedLamp;
    int RWin;
    char turn;
    int game;
};

//تابع DFS
void dfs(int i, int j, int wallh[][maxy], int wallv[][maxy],int visited[][maxy],int x ,int y) {
    visited[i][j] = 1;
    if (i > 0 && !visited[i-1][j] && wallh[i-1][j] == 0)
        dfs(i-1, j, wallh, wallv,visited,x,y);

    if (i < x-1 && !visited[i+1][j] && wallh[i][j] == 0)
        dfs(i+1, j, wallh, wallv,visited,x,y);

    if (j > 0 && !visited[i][j-1] && wallv[i][j-1] == 0)
        dfs(i, j-1, wallh, wallv,visited,x,y);

    if (j < y-1 && !visited[i][j+1] && wallv[i][j] == 0)
        dfs(i, j+1, wallh, wallv,visited,x,y);
}

//تابع برسی اتصال نقشه
int Connected(int wallh[][maxy], int wallv[][maxy],int visited[][maxy],int x ,int y) {
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
            visited[i][j] = 0;
    dfs(0, 0, wallh, wallv,visited,x,y);
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (!visited[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

//تابع فاصله دو کاراکتر
int dist(int x1, int y1, int x2, int y2) {
    if (x1 > x2) x1 -= x2;
    else x1 = x2 - x1;

    if (y1 > y2) y1 -= y2;
    else y1 = y2 - y1;
    return (x1+y1);
}

//تابع ریست ارایه های مربوط به مپ
void MapReset(char map[][maxy],char input ,int x ,int y) {
    for (int i=0 ; i<x ; i++ ){
        for(int j=0 ; j<y ; j++){
            map[i][j] = input ;
        }
    }
}
//تابع ریست ارایه های مربوط به رانر و هانتر و دیوار ها
void RHWReset(int map[][maxy],int x ,int y) {
    for (int i=0 ; i<x ; i++ ){
        for(int j=0 ; j<y ; j++){
            map[i][j] = 0 ;
        }
    }
}
//تابع گیرنده تعداد هانتر و رانر
int RHGetter(char input,int x ,int y,int MainRcount) {
    int rcount,hcount;
    if (input == 'R') {
        printf("Please enter the number of runners:\n");
        scanf ("%d",&rcount ) ;
        while (rcount<1 || rcount>(x*y)-2) {
            if (rcount<1)
                printf("!ERROR!\n(Invalid number of runners)\nPlease enter again:\n");
            else
                printf("!ERROR!\n(The number of runners entered is greater than the number of empty spaces on the map)\nPlease enter again:\n");
            scanf ("%d",&rcount ) ;
        }
        return rcount ;
    }
    else if (input == 'H') {
        printf("Please enter the number of hunters:\n");
        scanf ("%d",&hcount) ;
        while (hcount<1 || hcount>((x*y)-MainRcount-1)) {
            if (hcount<1)
                printf("!ERROR!\n(Invalid number of hunters)\nPlease enter again:\n");
            else
                printf("!ERROR!\n(The number of runners entered is greater than the number of empty spaces on the map)\nPlease enter again:\n");
            scanf ("%d",&hcount) ;
        }
        return hcount ;
    }
}
//تابع تشخیص پایان بازی با گرفتن مختصات 3 کاراکتر اصلی
// int GameState(int Lx,int Ly,int Rx,int Ry,int Hx,int Hy) {
//     if (Hx==Rx && Hy==Ry) return 2;
//     else if (Lx==Rx && Ly==Ry) return 1;
//     else return 0;
// }

int GameState(int Lx,int Ly,struct Runner runner[],int *rcount,struct Hunter hunter[],int hcount,int *Reachedlight,int Runner[][maxy],char map[][maxy],int RWin) {
    int R = *rcount ;
    int dead[maxx];
    for (int i=0 ; i<R ; i++) {
        dead[i] = 0 ;
    }
    for (int i=0 ; i<R ; i++) {
        for (int j=0 ; j<hcount ; j++) {
            if (runner[i].L.x==hunter[j].L.x && runner[i].L.y==hunter[j].L.y) {
                Runner[runner[i].L.x][runner[i].L.y] = 0;
                dead[i]=1;
            }
        }
    }
    for (int i=0 ; i<R ; i++) {
        if (runner[i].L.x == Lx && runner[i].L.y == Ly && dead[i]!=1) {
            Runner[runner[i].L.x][runner[i].L.y] = 0;
            map[Lx][Ly] = light;
            dead[i]=2;
            *Reachedlight += 1;
        }
    }
    for (int i=0 ; i<*rcount ; i++) {
        if (dead[i]) {
            for (int k=i ; k<*rcount-1 ; k++) {
                runner[k].L.x = runner[k+1].L.x ;
                runner[k].L.y = runner[k+1].L.y ;
            }
            *rcount -= 1;
            i--;
        }
    }
    if (*Reachedlight >= RWin) return 1;
    else if (*rcount == 0 && *Reachedlight<RWin) return 2;
    else return 0;
}
//تابع پیدا کردن نزدیک ترین رانر به هانتر
int ClosestRunner(struct Runner runner[],int rcount,int Hx,int Hy){
    int closest =999;
    int distance;
    int RNum;
    for (int i =0 ; i<rcount ; i++) {
        distance = dist(runner[i].L.x,runner[i].L.y,Hx,Hy);
        if (distance < closest) {
            closest = distance;
            RNum = i;
        }
    }
    return RNum;
}

int main () {
    srand(time(NULL));
    struct Board board;
    struct Data data ;
    struct Runner runner[maxx*maxy];
    struct Hunter hunter[maxx*maxy];
    // int wallh[maxx][maxy];
    // int wallv[maxx][maxy];
    int visited[maxx][maxy];
    // char map[maxx][maxy] ;
    int runnerx,runnery,hunterx,huntery ;
    //گرفتن ابعاد مپ
     printf("Please enter size of map :\n ");
     scanf("%d %d" ,&data.x,&data.y );
     while ((data.x<2 || data.y<2) || (data.x==2 && data.y==2)) {
          printf("!ERROR!\n(The map size must be greater than 2*2.)\nPlease enter again:\n");
          scanf("%d %d" ,&data.x,&data.y );
     }
    //گرفتن تعداد رانر
     // int Runner[maxx][maxy] ;
     data.rcount = RHGetter('R',data.x,data.y,0);
    //گرفتن تعداد هانتر
     // int Hunter[maxx][maxy] ;
   data.hcount = RHGetter('H',data.x,data.y,data.rcount);

    //مشخص کردن خونه های هر کاراکتر در مپ
    int ok = 0;
    int TryLimit = 500;
    while (!ok) {
        int CharactersTries = 0;
        int CharactersSum = data.rcount + data.hcount + 1 ;
        int LDistance,RDistance;
        while (CharactersTries<TryLimit ) {
            int CharactersCount = 0;
            CharactersTries++;
            //پاکسازی ارایه برای مشخص کردن مشخصات کاراکتر ها
            MapReset(board.map,block,data.x,data.y) ;
            RHWReset(board.IsHunter,data.x,data.y) ;
            RHWReset(board.IsRunner,data.x,data.y) ;
            //مشخص کردن مشخصات چراغ
            board.light.x = (rand() % data.x);
            board.light.y = (rand() % data.y);
            board.map[board.light.x][board.light.y] = light ;
            CharactersCount++;
            //مشخص کردن مشخصات رانر ها
            for(int i=0 ; i<data.rcount; i++) {
                int a,b,try=0;
                 do{
                    a = (rand() % data.x);
                    b = (rand() % data.y);
                    LDistance = dist(a,b,board.light.x,board.light.y);
                    try++;
                }while (((a==board.light.x && b==board.light.y) || board.IsRunner[a][b]==1 || LDistance <2 ) && try < TryLimit );
                board.map[a][b] = Runner_Ch ;
                board.IsRunner[a][b] = 1;
                CharactersCount++;
            }
            //مشخص کردن مشخصات هانتر ها
            for(int k=0 ; k<data.hcount ; k++) {
                int a = (rand() % data.x);
                int b = (rand() % data.y);
                int sw=1;
                int try=0;
                while (sw && try<TryLimit) {
                    try++;
                    sw = 0;
                    LDistance = dist(a,b,board.light.x,board.light.y);
                    if ((a==board.light.x && b==board.light.y) || board.IsRunner[a][b]==1 || board.IsHunter[a][b]==1)
                        sw=1;
                    if (LDistance <2)
                        sw=1;
                    for (int i = 0; i < data.x; i++) {
                        for (int j = 0; j < data.y; j++) {
                            if (board.IsRunner[i][j] == 1) {
                                RDistance=dist(a, b, i, j);
                                if (RDistance < 2)
                                    sw=1;
                            }
                        }
                    }
                    if (sw) {
                        a = (rand() % data.x);
                        b = (rand() % data.y);
                    }
                }
                if (!sw) {
                    board.map[a][b] = Hunter_Ch ;
                    board.IsHunter[a][b] = 1;
                    CharactersCount++;
                }
            }
            if (CharactersCount == CharactersSum) {
                ok = 1;
                break;
            }
        }
        if (!ok) {
            printf("!EROR!\n(Your Runner and Hunter input values are not optimal for the map dimensions)\nPlease re-enter the values carefully\n");
            data.rcount = RHGetter('R',data.x,data.y,0);
            data.hcount = RHGetter('H',data.x,data.y,data.rcount);
        }
    }

     int wcount ;
    printf("Please enter the number of walls:\n");
    scanf("%d",&wcount) ;
     while(wcount<0||wcount>((data.x-1)*(data.y-1))) {
         printf("!ERROR!\n(Invalid number of walls)\nPlease enter again:\n");
         scanf ("%d",&wcount) ;
     }

     RHWReset(board.walls.h,data.x,data.y);
     RHWReset(board.walls.v,data.x,data.y);

     // اجرای رندوم دیوار ها با توابع dfs  و Connected
    int Wallcounter = 0;
    int WallTries = 0;

    while (Wallcounter < wcount && WallTries < TryLimit) {
        WallTries++;

        int choice = rand() % 2;
        int i, j;

        if (choice) {
            i = rand() % (data.x - 1);
            j = rand() % data.y;

            if (board.walls.h[i][j] == 1)
                continue;

            board.walls.h[i][j] = 1;

            if (!Connected(board.walls.h, board.walls.v,visited,data.x,data.y)) {
                board.walls.h[i][j] = 0;
            } else {
                Wallcounter++;
            }

        } else {

            i = rand() % data.x;
            j = rand() % (data.y - 1);

            if (board.walls.v[i][j] == 1)
                continue;

            board.walls.v[i][j] = 1;

            if (!Connected(board.walls.h, board.walls.v,visited,data.x,data.y)) {
                board.walls.v[i][j] = 0;
            } else {
                Wallcounter++;
            }
        }
    }
    if (Wallcounter < wcount) {
        printf("Warning: Only %d walls could be placed safely.\n", Wallcounter);
    }
    // int Rx[maxx];
    // int Ry[maxy];
    // int Hx[maxx];
    // int Hy[maxy];
    int num=0;
    for (int i = 0; i <data.x; i++)
        for (int j = 0; j <data.y; j++)
            if(board.IsRunner[i][j] == 1){
                runner[num].L.x=i ;
                runner[num].L.y=j ;
                runner[num].alive=1;
                runner[num].reached=0;
                num++;
            }
    num=0;
    for (int i = 0; i <data.x; i++)
        for (int j = 0; j <data.y; j++)
            if(board.IsHunter[i][j] == 1){
                hunter[num].L.x=i ;
                hunter[num].L.y=j ;
                num++;
            }
     //چاپ نقشه با ریلیب
    const int CELL = 50;
    const int WALL_THICK = 4;
    int ErrorTimer = 0 ;
    int EndTimer = 0;
    float HunterDelay=0.0f;
    int screenW = data.y * CELL;
    int screenH = data.x * CELL;
    InitWindow(screenW, screenH, "board");
    SetTargetFPS(60);

    int Game = 0;
    int End = 1;
    data.CurrentRunner = 0;
    data.CurrentHunter = 0;
    int HunterOnLight[data.hcount];
    for (int i = 0; i < data.hcount; i++) {
        HunterOnLight[i] = 0;
    }
    int RWin = data.rcount/3;
    if (data.rcount%3 != 0) {
        RWin += 1;
    }
    data.ReachedLamp = 0;
    data.turn ='R';
    while (End) {
        if (WindowShouldClose()) {
            End = 0;
            break;
        }

        int RMove = 0 ;
        int NoMovement = 0;
        if (data.turn == 'R' && !Game) {
            runnerx = runner[data.CurrentRunner].L.x ;
            runnery = runner[data.CurrentRunner].L.y ;
            int newrx = runnerx ;
            int newry = runnery ;
            if (IsKeyPressed(KEY_UP))    { newrx--; RMove = 1; }
            if (IsKeyPressed(KEY_DOWN))  { newrx++; RMove = 1; }
            if (IsKeyPressed(KEY_LEFT))  { newry--; RMove = 1; }
            if (IsKeyPressed(KEY_RIGHT)) { newry++; RMove = 1; }
            if (IsKeyPressed(KEY_SPACE)) {NoMovement = 1; RMove = 1;}
            if (RMove) {
                int valid = 1;
                if(newrx<0||newrx>=data.x||newry<0||newry>=data.y)
                    valid =0 ;
                if(valid && !NoMovement){
                    if(newrx==(runnerx-1) && board.walls.h[runnerx-1][runnery] ) valid=0 ;
                    if(newrx==(runnerx+1) && board.walls.h[runnerx][runnery]) valid=0 ;
                    if(newry==(runnery-1) && board.walls.v[runnerx][runnery-1]) valid=0 ;
                    if(newry==(runnery+1) && board.walls.v[runnerx][runnery]) valid=0 ;

                }
                if (valid && !NoMovement) {
                    if (board.map[newrx][newry]== Runner_Ch) valid = 0;
                }
                if (valid || NoMovement) {
                    board.map[runnerx][runnery] = block;
                    board.IsRunner[runnerx][runnery] = 0;
                    runnerx = newrx;
                    runnery = newry;
                    board.map[runnerx][runnery] = Runner_Ch;
                    board.IsRunner[runnerx][runnery] = 1;
                    runner[data.CurrentRunner].L.x = runnerx;
                    runner[data.CurrentRunner].L.y = runnery;
                    data.CurrentRunner++;
                    if (data.CurrentRunner==data.rcount) {
                        data.CurrentRunner = 0;
                        data.turn='H';
                        HunterDelay=0.5f;
                    }
                }
                else {
                    ErrorTimer=60 ;
                }
            }
        }
            Game = GameState(board.light.x,board.light.y,runner,&data.rcount,hunter,data.hcount,&data.ReachedLamp,board.IsRunner,board.map,RWin);
            if (HunterDelay > 0) {
                HunterDelay -= GetFrameTime();
            }

            if (data.turn=='H' && !Game && HunterDelay<=0) {
                hunterx = hunter[data.CurrentHunter].L.x;
                huntery = hunter[data.CurrentHunter].L.y;
                int newhx = hunterx ;
                int newhy = huntery ;
                int Rnum = ClosestRunner(runner,data.rcount,hunterx,huntery);
                if (runner[Rnum].L.y>huntery && !(board.walls.v[hunterx][huntery]) && huntery+1<data.y && board.map[newhx][newhy+1]!=Hunter_Ch) {newhy++;}
                else if (runner[Rnum].L.y<huntery && !(board.walls.v[hunterx][huntery-1]) && huntery-1>=0 && board.map[newhx][newhy-1]!=Hunter_Ch) {newhy--;}
                else if (runner[Rnum].L.x>hunterx && !(board.walls.h[hunterx][huntery]) && hunterx+1<data.x && board.map[newhx+1][newhy]!=Hunter_Ch) {newhx++;}
                else if (runner[Rnum].L.x<hunterx && !(board.walls.h[hunterx-1][huntery]) && hunterx-1>=0 && board.map[newhx-1][newhy]!=Hunter_Ch) {newhx--;}
                if (HunterOnLight[data.CurrentHunter]) {
                    board.map[hunterx][huntery] = light;
                    HunterOnLight[data.CurrentHunter] = 0;
                }
                else board.map[hunterx][huntery] = block;
                if (board.map[newhx][newhy]==light){HunterOnLight[data.CurrentHunter] = 1;}
                board.IsHunter[hunterx][huntery]=0;
                hunterx = newhx;
                huntery = newhy;
                board.map[hunterx][huntery] = Hunter_Ch;
                board.IsHunter[hunterx][huntery] = 1;
                hunter[data.CurrentHunter].L.x = hunterx;
                hunter[data.CurrentHunter].L.y = huntery;
                data.CurrentHunter++;
                if (data.CurrentHunter==data.hcount) {
                    data.CurrentHunter = 0;
                    data.turn='R';
                }
            }
            Game = GameState(board.light.x,board.light.y,runner,&data.rcount,hunter,data.hcount,&data.ReachedLamp,board.IsRunner,board.map,RWin);
        BeginDrawing();
        ClearBackground(WHITE);


        for (int i = 0; i < data.x; i++) {
            for (int j = 0; j < data.y; j++) {
                Color bg = WHITE;
                char ch = board.map[i][j];
                if (ch == Runner_Ch || ch == Hunter_Ch || ch == light) {
                    bg = WHITE;
                }
                DrawRectangle(j*CELL, i*CELL, CELL, CELL, bg);
            }
        }


        for (int i = 0; i < data.x-1; i++) {
            for (int j = 0; j < data.y; j++) {
                Color lineColor = board.walls.h[i][j] ? BLACK : GRAY;
                DrawRectangle(j*CELL, i*CELL + CELL - WALL_THICK/2,
                              CELL, WALL_THICK, lineColor);
            }
        }


        for (int i = 0; i < data.x; i++) {
            for (int j = 0; j < data.y-1; j++) {
                Color lineColor = board.walls.v[i][j] ? BLACK : GRAY;
                DrawRectangle(j*CELL + CELL - WALL_THICK/2, i*CELL,
                              WALL_THICK, CELL, lineColor);
            }
        }


        for (int i = 0; i < data.x; i++) {
            for (int j = 0; j < data.y; j++) {
                char ch = board.map[i][j];
                if (ch == Runner_Ch) {
                    DrawCircle(j*CELL + CELL/2, i*CELL + CELL/2, CELL/3, BLUE);
                } else if (ch == Hunter_Ch) {
                    DrawCircle(j*CELL + CELL/2, i*CELL + CELL/2, CELL/3, RED);
                } else if (ch == light) {
                    int pad = WALL_THICK / 2;
                    DrawRectangle(j*CELL + pad , i*CELL + pad , CELL - WALL_THICK , CELL - WALL_THICK, YELLOW);
                }
            }
        }

        if (ErrorTimer > 0) {
            DrawText("INVALID MOVE!", 10, 10, 20, RED);
            ErrorTimer--;
        }
        if (Game && EndTimer==0) {
            EndTimer=180;
        }
        if (Game) {
            ClearBackground(WHITE);
            DrawRectangle(0, 0, CELL*data.y, CELL*data.x, WHITE);
            if (Game == 1) {
                DrawText("YOU WIN",
                         screenW/2 - MeasureText("YOU WIN", 40)/2,
                         screenH/2 - 20,
                         40,
                         GREEN);
            }
            else if (Game == 2) {
                DrawText("YOU LOSE",
                         screenW/2 - MeasureText("YOU LOSE", 40)/2,
                         screenH/2 - 20,
                         40,
                         RED);
            }
            EndTimer--;
            if (EndTimer <= 0) {
                End = 0;
            }
        }
        EndDrawing();
    }
}