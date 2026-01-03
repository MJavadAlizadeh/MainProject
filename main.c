// Phase 2 Project
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>

const int maxx = 100 ;
const int maxy = 100 ;
char hunter = 'H' ;
char runner = 'R' ;
char light = 'L' ;
char block = '.' ;
char V = '|' ;
char H = '_' ;

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
    if (x1+y1 >= 2)
        return 1;
    else
        return 0;
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
int GameState(int Lx,int Ly,int Rx,int Ry,int Hx,int Hy) {
    if (Lx==Rx && Ly==Ry) return 1;
    else if (Hx==Rx && Hy==Ry) return 2;
    else return 0;
}

int main () {
    srand(time(NULL));
    int wallh[maxx][maxy];
    int wallv[maxx][maxy];
    int visited[maxx][maxy];
    char map[maxx][maxy] ;
    int lightx,lighty,runnerx,runnery,hunterx,huntery ;
    //گرفتن ابعاد مپ
     int x , y ;
     printf("Please enter size of map :\n ");
     scanf("%d %d" , &x,&y );
     while ((x<2 || y<2) || (x==2 && y==2)) {
          printf("!ERROR!\n(The map size must be greater than 2*2.)\nPlease enter again:\n");
          scanf("%d %d" , &x,&y );
     }
    //گرفتن تعداد رانر
     int rcount = 1;
     int Runner[maxx][maxy] ;
     //rcount = RHGetter('R',x,y,0);
    //گرفتن تعداد هانتر
     int hcount = 1 ;
     int Hunter[maxx][maxy] ;
   // hcount = RHGetter('H',x,y,rcount);

    //مشخص کردن خونه های هر کاراکتر در مپ
    int ok = 0;
    int TryLimit = 500;
    while (!ok) {
        int CharactersTries = 0;
        int CharactersSum = rcount + hcount + 1 ;
        int LDistance,RDistance;
        while (CharactersTries<TryLimit ) {
            int CharactersCount = 0;
            CharactersTries++;
            //پاکسازی ارایه برای مشخص کردن مشخصات کاراکتر ها
            MapReset(map,block,x,y) ;
            RHWReset(Hunter,x,y) ;
            RHWReset(Runner,x,y) ;
            //مشخص کردن مشخصات چراغ
            lightx = (rand() % x);
            lighty = (rand() % y);
            map[lightx][lighty] = light ;
            CharactersCount++;
            //مشخص کردن مشخصات رانر ها
            for(int i=0 ; i<rcount; i++) {
                int a,b,try=0;
                 do{
                    a = (rand() % x);
                    b = (rand() % y);
                    LDistance = dist(a,b,lightx,lighty);
                    try++;
                }while (((a==lightx && b==lighty) || Runner[a][b]==1 || LDistance == 0 ) && try < TryLimit );
                map[a][b] = runner ;
                Runner[a][b] = 1;
                CharactersCount++;
            }
            //مشخص کردن مشخصات هانتر ها
            for(int k=0 ; k<hcount ; k++) {
                int a = (rand() % x);
                int b = (rand() % y);
                int sw=1;
                int try=0;
                while (sw && try<TryLimit) {
                    try++;
                    sw = 0;
                    LDistance = dist(a,b,lightx,lighty);
                    if ((a==lightx && b==lighty) || Runner[a][b]==1 || Hunter[a][b]==1)
                        sw=1;
                    if (!LDistance)
                        sw=1;
                    for (int i = 0; i < x; i++) {
                        for (int j = 0; j < y; j++) {
                            if (Runner[i][j] == 1) {
                                RDistance=dist(a, b, i, j);
                                if (!RDistance)
                                    sw=1;
                            }
                        }
                    }
                    if (sw) {
                        a = (rand() % x);
                        b = (rand() % y);
                    }
                }
                if (!sw) {
                    map[a][b] = hunter ;
                    Hunter[a][b] = 1;
                    CharactersCount++;
                }
            }
            if (CharactersCount == CharactersSum) {
                ok = 1;
                break;
            }
        }
        // if (!ok) {
        //     printf("!EROR!\n(Your Runner and Hunter input values are not optimal for the map dimensions)\nPlease re-enter the values carefully\n");
        //     rcount = RHGetter('R',x,y,0);
        //     hcount = RHGetter('H',x,y,rcount);
        // }
    }
    for (int i = 0; i <x; i++)
        for (int j = 0; j <y; j++)
            if(Runner[i][j] == 1){
                runnerx=i ;
                runnery=j ;
            }
    for (int i = 0; i <x; i++)
        for (int j = 0; j <y; j++)
            if(Hunter[i][j] == 1){
                hunterx=i ;
                huntery=j ;
            }
     int wcount ;
    printf("Please enter the number of walls:\n");
    scanf("%d",&wcount) ;
     while(wcount<0||wcount>((x-1)*(y-1))) {
         printf("!ERROR!\n(Invalid number of walls)\nPlease enter again:\n");
         scanf ("%d",&wcount) ;
     }

     RHWReset(wallh,x,y);
     RHWReset(wallv,x,y);

     // اجرای رندوم دیوار ها با توابع dfs  و Connected
    int Wallcounter = 0;
    int WallTries = 0;

    while (Wallcounter < wcount && WallTries < TryLimit) {
        WallTries++;

        int choice = rand() % 2;
        int i, j;

        if (choice) {
            i = rand() % (x - 1);
            j = rand() % y;

            if (wallh[i][j] == 1)
                continue;

            wallh[i][j] = 1;

            if (!Connected(wallh, wallv,visited,x,y)) {
                wallh[i][j] = 0;
            } else {
                Wallcounter++;
            }

        } else {

            i = rand() % x;
            j = rand() % (y - 1);

            if (wallv[i][j] == 1)
                continue;

            wallv[i][j] = 1;

            if (!Connected(wallh, wallv,visited,x,y)) {
                wallv[i][j] = 0;
            } else {
                Wallcounter++;
            }
        }
    }
    if (Wallcounter < wcount) {
        printf("Warning: Only %d walls could be placed safely.\n", Wallcounter);
    }

     //چاپ نقشه با ریلیب
    const int CELL = 50;
    const int WALL_THICK = 4;
    int ErrorTimer = 0 ;
    int EndTimer = 0;

    int screenW = y * CELL;
    int screenH = x * CELL;
    InitWindow(screenW, screenH, "board");
    SetTargetFPS(60);

    int Game = 0;
    int End = 1;
    char turn ='R';
    while (End) {
        if (WindowShouldClose()) {
            End = 0;
            break;
        }

        int newrx = runnerx ;
        int newry = runnery ;
        int newhx = hunterx ;
        int newhy = huntery ;
        int RMove = 0 ;

        if (!Game){
            if (IsKeyPressed(KEY_UP))    { newrx--; RMove = 1; }
            if (IsKeyPressed(KEY_DOWN))  { newrx++; RMove = 1; }
            if (IsKeyPressed(KEY_LEFT))  { newry--; RMove = 1; }
            if (IsKeyPressed(KEY_RIGHT)) { newry++; RMove = 1; }
            if (RMove) {
                while (turn=='R') {
                    int valid = 1;
                    if(newrx<0||newrx>=x||newry<0||newry>=y)
                        valid =0 ;
                    if(valid){
                        if(newrx==(runnerx-1) && wallh[runnerx-1][runnery]) valid=0 ;
                        if(newrx==(runnerx+1) && wallh[runnerx][runnery]) valid=0 ;
                        if(newry==(runnery-1) && wallv[runnerx][runnery-1]) valid=0 ;
                        if(newry==(runnery+1) && wallv[runnerx][runnery]) valid=0 ;

                    }
                    if (valid) {
                        map[runnerx][runnery] = block;
                        runnerx = newrx;
                        runnery = newry;
                        map[runnerx][runnery] = runner;
                        turn='H';
                    }
                    else {
                        ErrorTimer=60 ;
                        break;
                    }
                }
            }
            Game = GameState(lightx,lighty,runnerx,runnery,hunterx,huntery);
            if (turn=='H' && !Game) {
                if (runnery>huntery && !(wallv[hunterx][huntery]) && huntery+1<y) {newhy++;}
                else if (runnery<huntery && !(wallv[hunterx][huntery-1]) && huntery-1>=0) {newhy--;}
                else if (runnerx>hunterx && !(wallh[hunterx][huntery]) && hunterx+1<x) {newhx++;}
                else if (runnerx<hunterx && !(wallh[hunterx-1][huntery]) && hunterx-1>=0) {newhx--;}
                map[hunterx][huntery] = block;
                hunterx = newhx;
                huntery = newhy;
                map[hunterx][huntery] = hunter;
                turn='R';
            }
            Game = GameState(lightx,lighty,runnerx,runnery,hunterx,huntery);
        }
        BeginDrawing();
        ClearBackground(WHITE);


        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                Color bg = WHITE;
                char ch = map[i][j];
                if (ch == runner || ch == hunter || ch == light) {
                    bg = WHITE;
                }
                DrawRectangle(j*CELL, i*CELL, CELL, CELL, bg);
            }
        }


        for (int i = 0; i < x-1; i++) {
            for (int j = 0; j < y; j++) {
                Color lineColor = wallh[i][j] ? BLACK : GRAY;
                DrawRectangle(j*CELL, i*CELL + CELL - WALL_THICK/2,
                              CELL, WALL_THICK, lineColor);
            }
        }


        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y-1; j++) {
                Color lineColor = wallv[i][j] ? BLACK : GRAY;
                DrawRectangle(j*CELL + CELL - WALL_THICK/2, i*CELL,
                              WALL_THICK, CELL, lineColor);
            }
        }


        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                char ch = map[i][j];
                if (ch == runner) {
                    DrawCircle(j*CELL + CELL/2, i*CELL + CELL/2, CELL/3, BLUE);
                } else if (ch == hunter) {
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
            DrawRectangle(0, 0, CELL*y, CELL*x, WHITE);
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