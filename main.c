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
int X,Y;
void dfs(int i, int j, int wallh[][maxy], int wallv[][maxy],int visited[maxx][maxy]) {
    visited[i][j] = 1;
    if (i > 0 && !visited[i-1][j] && wallh[i-1][j] == 0)
        dfs(i-1, j, wallh, wallv,visited);

    if (i < X-1 && !visited[i+1][j] && wallh[i][j] == 0)
        dfs(i+1, j, wallh, wallv,visited);

    if (j > 0 && !visited[i][j-1] && wallv[i][j-1] == 0)
        dfs(i, j-1, wallh, wallv,visited);

    if (j < Y-1 && !visited[i][j+1] && wallv[i][j] == 0)
        dfs(i, j+1, wallh, wallv,visited);
}

//تابع برسی اتصال نقشه
int Connected(int wallh[][maxy], int wallv[][maxy],int visited[maxx][maxy]) {
    for (int i = 0; i < X; i++)
        for (int j = 0; j < Y; j++)
            visited[i][j] = 0;
    dfs(0, 0, wallh, wallv,visited);
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
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
//تابع ریست ارایه های مربوط به رانر و هانتر
void RHReset(int map[][maxy],int x ,int y) {
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
                printf("!EROR!\n(Invalid number of runners)\nPlease enter again:\n");
            else
                printf("!EROR!\n(The number of runners entered is greater than the number of empty spaces on the map)\nPlease enter again:\n");
            scanf ("%d",&rcount ) ;
        }
        return rcount ;
    }
    else if (input == 'H') {
        printf("Please enter the number of hunters:\n");
        scanf ("%d",&hcount) ;
        while (hcount<1 || hcount>((x*y)-MainRcount-1)) {
            if (hcount<1)
                printf("!EROR!\n(Invalid number of hunters)\nPlease enter again:\n");
            else
                printf("!EROR!\n(The number of runners entered is greater than the number of empty spaces on the map)\nPlease enter again:\n");
            scanf ("%d",&hcount) ;
        }
        return hcount ;
    }
}

int main () {
    srand(time(NULL));
    int wallh[maxx][maxy];
    int wallv[maxx][maxy];
    int visited[maxx][maxy];
    char map[maxx][maxy] ;
    //گرفتن ابعاد مپ
     int x , y ;
     printf("Please enter size of map :\n ");
     scanf("%d %d" , &x,&y );
     while ((x<2 || y<2) || (x==2 && y==2)) {
          printf("!EROR!\n(The map size must be greater than 2*2.)\nPlease enter again:\n");
          scanf("%d %d" , &x,&y );
     }
    //گرفتن تعداد رانر
     int rcount = 1;
     int Runner[maxx][maxy] ;
     /*rcount = RHGetter('R',x,y,0);*/
    //گرفتن تعداد هانتر
     int hcount = 1 ;
     int Hunter[maxx][maxy] ;
   /* hcount = RHGetter('H',x,y,rcount);*/

    //مشخص کردن خونه های هر کاراکتر در مپ
    int ok = 0;
    while (!ok) {
        int TryLimit = 500;
        int CharactersTries = 0;
        int CharactersSum = rcount + hcount + 1 ;
        int LDistance,RDistance;
        while (CharactersTries<TryLimit ) {
            int CharactersCount = 0;
            CharactersTries++;
            //پاکسازی ارایه برای مشخص کردن مشخصات کاراکتر ها
            MapReset(map,block,x,y) ;
            RHReset(Hunter,x,y) ;
            RHReset(Runner,x,y) ;
            //مشخص کردن مشخصات چراغ
            int lightx = (rand() % x);
            int lighty = (rand() % y);
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
        if (!ok) {
            printf("!EROR!\n(Your Runner and Hunter input values are not optimal for the map dimensions)\nPlease re-enter the values carefully\n");
            //rcount = RHGetter('R',x,y,0);
           // hcount = RHGetter('H',x,y,rcount);
        }
    }
    int  runnerx,runnery ;
    for (int i = 0; i <x; i++)
        for (int j = 0; j <y; j++)
            if(Runner[i][j] == 1){
                runnerx=i ;
                runnery=j ;
            }
     int wcount ;
    printf("Please enter the number of walls:\n");
    scanf("%d",&wcount) ;
     while(wcount<0||wcount>((x-1)*(y-1))) {
         printf("!EROR!\n(Invalid number of walls)\nPlease enter again:\n");
         scanf ("%d",&wcount) ;
     }

     for (int i=0 ; i<x ; i++ ) {
          for(int j=0 ; j<y ; j++) {
               wallh[i][j] = 0 ;
          }
     }
     for (int i=0 ; i<x ; i++ ) {
          for(int j=0 ; j<y ; j++) {
               wallv[i][j] = 0 ;
          }
     }

     // اجرای رندوم دیوار ها با توابع dfs  و Connected
    int Wallcounter = 0;
    int WallTries = 0;
    int WallTry = wcount * 30;
    X = x;
    Y = y;
    while (Wallcounter < wcount && WallTries < WallTry) {
        WallTries++;

        int choice = rand() % 2;
        int i, j;

        if (choice) {
            i = rand() % (x - 1);
            j = rand() % y;

            if (wallh[i][j] == 1)
                continue;

            wallh[i][j] = 1;

            if (!Connected(wallh, wallv,visited)) {
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

            if (!Connected(wallh, wallv,visited)) {
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
    int erortime = 0 ;

    int screenW = y * CELL;
    int screenH = x * CELL;
    InitWindow(screenW, screenH, "board");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        int newx=runnerx ;
        int newy=runnery;
        int move = 0 ;
        if (IsKeyPressed(KEY_UP))    { newx--; move = 1; }
        if (IsKeyPressed(KEY_DOWN))  { newx++; move = 1; }
        if (IsKeyPressed(KEY_LEFT))  { newy--; move = 1; }
        if (IsKeyPressed(KEY_RIGHT)) { newy++; move = 1; }
        if (move) {
            int valid = 1;
            if(newx<0||newx>=x||newy<0||newy>=y)
                valid =0 ;
            if(valid){
                if(newx==(runnerx-1) && wallh[newx][newy]) valid=0 ;
                if(newx==(runnerx+1) && wallh[runnerx][runnery]) valid=0 ;
                if(newy==(runnery-1) && wallv[newx][newy]) valid=0 ;
                if(newy==(runnery+1) && wallv[runnerx][runnery]) valid=0 ;
            }
            if (valid) {
                map[runnerx][runnery] = block;
                runnerx = newx;
                runnery = newy;
                map[runnerx][runnery] = runner;
            }
            else {
                erortime=60 ;
            }

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
                    DrawCircle(j*CELL + CELL/2, i*CELL + CELL/2, CELL/3, YELLOW);
                }
            }
        }

        if (erortime > 0) {
            DrawText("INVALID MOVE!", 10, 10, 20, RED);
            erortime--;
        }

        EndDrawing();
    }



     }