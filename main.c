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


int main () {
    int wallh[maxx][maxy];
    int wallv[maxx][maxy];
    int visited[maxx][maxy];


     srand(time(NULL));
     int x , y ;
     printf("Please enter size of map :\n ");
     scanf("%d %d" , &x,&y );
     while (x<2 || y<2) {
          printf("!EROR!\n(The map size must be greater than 2*2.)\nPlease enter again:\n");
          scanf("%d %d" , &x,&y );
     }
     char map[maxx][maxy] ;
     for (int i=0 ; i<x ; i++ ){
          for(int j=0 ; j<y ; j++){
               map[i][j] = block ;
          }
     }
     int lightx = (rand() % x);
     int lighty = (rand() % y);
     map[lightx][lighty] = light ;

     int rcount ;
     int Runner[maxx][maxy] ;
     for (int i=0 ; i<x ; i++ ){
          for(int j=0 ; j<y ; j++){
               Runner[i][j] = 0 ;
          }
     }
     printf("Please enter the number of runners:\n");
     scanf ("%d",&rcount ) ;
     while (rcount<1 || rcount>(x*y)-2) {
          if (rcount<1)
               printf("!EROR!\n(Invalid number of runners)\nPlease enter again:\n");
          else
               printf("!EROR!\n(The number of runners entered is greater than the number of empty spaces on the map)\nPlease enter again:\n");
          scanf ("%d",&rcount ) ;
     }
     for(int i=0 ; i<rcount ; i++) {
          int a = (rand() % x);
          int b = (rand() % y);
          while ((a==lightx && b==lighty) || Runner[a][b]==1){
               a = (rand() % x);
               b = (rand() % y);
          }
          map[a][b] = runner ;
          Runner[a][b] = 1;
     }

     int hcount ;
     int Hunter[maxx][maxy] ;
     for (int i=0 ; i<x ; i++ ){
          for(int j=0 ; j<y ; j++){
               Hunter[i][j] = 0 ;
          }
     }
     printf("Please enter the number of hunters:\n");
     scanf ("%d",&hcount) ;
     while (hcount<1 || hcount>((x*y)-rcount-1)) {
          if (hcount<1)
               printf("!EROR!\n(Invalid number of hunters)\nPlease enter again:\n");
          else
               printf("!EROR!\n(The number of runners entered is greater than the number of empty spaces on the map)\nPlease enter again:\n");
          scanf ("%d",&hcount) ;
     }
     for(int i=0 ; i<hcount ; i++) {
          int a = (rand() % x);
          int b = (rand() % y);
          while ((a==lightx && b==lighty) || Runner[a][b]==1 || Hunter[a][b]==1){
               a = (rand() % x);
               b = (rand() % y);
          }
          map[a][b] = hunter ;
          Hunter[a][b] = 1;
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
    int counter = 0;
    int tries = 0;
    int MAX_TRIES = wcount * 30;

    while (counter < wcount && tries < MAX_TRIES) {
        tries++;

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
                counter++;
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
                counter++;
            }
        }
    }
    if (counter < wcount) {
        printf("Warning: Only %d walls could be placed safely.\n", counter);
    }

     //چاپ نقشه با ریلیب
    const int CELL = 50;
    const int WALL_THICK = 4;

    int screenW = y * CELL;
    int screenH = x * CELL;
    InitWindow(screenW, screenH, "board");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);


        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                Color bg = LIGHTGRAY;
                char ch = map[i][j];
                if (ch == runner || ch == hunter || ch == light) {
                    bg = GRAY;
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

        EndDrawing();
    }



     }