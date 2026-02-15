// Phase 5 Project
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
//متغییر ها
#define maxx 100
#define maxy 100
char Hunter_Ch = 'H' ;
char Runner_Ch = 'R' ;
char Lucky_Ch = '?' ;
char light = 'L' ;
char block = '.' ;

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
    int OnLuckyBox;
};
struct LuckyBox {
    struct locate L;
    int active;
    int state;
};
struct Wall {
    int h[maxx][maxy];
    int v[maxx][maxy];
};
struct Board {
    char map[maxx][maxy];
    struct locate light;
    int IsRunner[maxx][maxy];
    int IsHunter[maxx][maxy];
    int IsLuckyBox[maxx][maxy];
    struct Wall walls;
    struct Wall tempWalls;
};
struct Data {
    int x,y;
    int rcount,hcount;
    int CurrentRunner;
    int CurrentHunter;
    int MaxTempWall;
    int UsedTempWall;
    int LuckyBoxCount;
    int UsedLuckyBox;
    int ReachedLamp;
    int DeadRunners;
    int RWin;
    char turn;
    int game;
};
struct MoveWall {
    int TempWallLife;
    int Active;
    int selx,sely;
    int Orientation;
    int WallValid;
};
struct SwapMode {
    int active;
    int stage;
    int HunterNum;
    int DestX,DestY;
    int valid;
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
void MapReset(struct Board *board,struct Data data,char input) {
    for (int i=0 ; i<data.x ; i++ ){
        for(int j=0 ; j<data.y ; j++){
            board->map[i][j] = input ;
        }
    }
}
//تابع ریست ارایه های مربوط به رانر و هانتر و دیوار ها
void CharReset(int map[][maxy],struct Data data) {
    for (int i=0 ; i<data.x ; i++ ){
        for(int j=0 ; j<data.y ; j++){
            map[i][j] = 0 ;
        }
    }
}
//تابع گیرنده تعداد هانتر و رانر
int RHGetter(char input,struct Data data,int MainRcount) {
    int rcount,hcount;
    if (input == 'R') {
        printf("Please enter the number of runners:\n");
        scanf ("%d",&rcount ) ;
        while (rcount<1 || rcount>(data.x*data.y)-2) {
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
        while (hcount<1 || hcount>((data.x*data.y)-MainRcount-1)) {
            if (hcount<1)
                printf("!ERROR!\n(Invalid number of hunters)\nPlease enter again:\n");
            else
                printf("!ERROR!\n(The number of runners entered is greater than the number of empty spaces on the map)\nPlease enter again:\n");
            scanf ("%d",&hcount) ;
        }
        return hcount ;
    }
}
//تشخیص برد و باخت بازی
int GameState(struct Board *board,struct Data *data,struct Runner runner[],struct Hunter hunter[]) {

    for (int i=0 ; i<data->rcount ; i++) {
        for (int j=0 ; j<data->hcount ; j++) {
            if (runner[i].L.x==hunter[j].L.x && runner[i].L.y==hunter[j].L.y && runner[i].alive==1) {
                board->IsRunner[runner[i].L.x][runner[i].L.y] = 0;
                runner[i].alive=0;
                data->DeadRunners++;
                if (hunter[j].OnLight) {
                    board->map[runner[i].L.x][runner[i].L.y] = light;
                }
                else if (hunter[j].OnLuckyBox) {
                    board->map[runner[i].L.x][runner[i].L.y] = Lucky_Ch;
                }
                else {
                    board->map[runner[i].L.x][runner[i].L.y] = Hunter_Ch;
                }
            }
        }
    }
    for (int i=0 ; i<data->rcount ; i++) {
        if (runner[i].L.x == board->light.x && runner[i].L.y == board->light.y && runner[i].alive==1) {
            board->IsRunner[runner[i].L.x][runner[i].L.y] = 0;
            board->map[runner[i].L.x][runner[i].L.y] = light;
            runner[i].reached=1;
            runner[i].alive=0;
            data->ReachedLamp++;
            data->DeadRunners++;
        }
    }
    if (data->ReachedLamp>=data->RWin) return 1;
    else if (data->DeadRunners==data->rcount) return 2;
    else return 0;
}
//تابع پیدا کردن نزدیک ترین رانر به هانتر
int ClosestRunner(struct Runner runner[],int rcount,int Hx,int Hy){
    int closest =999;
    int distance;
    int RNum= -1;
    for (int i =0 ; i<rcount ; i++) {
        if (runner[i].alive==0) continue;
        distance = dist(runner[i].L.x,runner[i].L.y,Hx,Hy);
        if (distance < closest) {
            closest = distance;
            RNum = i;
        }
    }
    return RNum;
}
//برسی امکان گذاشتن دیوار موقت
int TempWallValid(struct Data data,struct Board board,struct MoveWall MWall) {
    int valid =1;
    if (MWall.Orientation == 1) {
        if (MWall.sely>=data.y-1)valid=0;
        else if (board.walls.v[MWall.selx][MWall.sely] || board.tempWalls.v[MWall.selx][MWall.sely])valid=0;
    }
    else if (MWall.Orientation == 0) {
        if (MWall.selx>=data.x-1)valid=0;
        else if (board.walls.h[MWall.selx][MWall.sely] || board.tempWalls.h[MWall.selx][MWall.sely])valid=0;
    }
    else valid=0;
    return valid;
}
//تابع زلزله لاکی باکس
void Earthquake(struct Data data,struct Board *board,struct Runner runner[],struct Hunter hunter[]) {
    //حرکت رانر ها
    for (int i =0 ; i<data.rcount ; i++) {
        if (runner[i].alive==0)continue;
        int Ori = rand() % 4;
        int try=0;
        int moved=0;
        int x = runner[i].L.x;
        int y = runner[i].L.y;
        int NewX = x;
        int NewY = y;
        while (try<4 && !moved){
            if (Ori==0) {
            if (x+1<data.x && board->map[x+1][y]==block && !board->walls.h[x][y] && !board->tempWalls.h[x][y]) {NewX++;moved++;}

            }
            else if (Ori==1) {
                if (x-1>=0 && board->map[x-1][y]==block && !board->walls.h[x-1][y] && !board->tempWalls.h[x-1][y]) {NewX--;moved++;}
            }
            else if (Ori==2) {
                if (y+1<data.y && board->map[x][y+1]==block && !board->walls.v[x][y] && !board->tempWalls.v[x][y]) {NewY++;moved++;}
            }
            else {
                if (y-1>=0 && board->map[x][y-1]==block && !board->walls.v[x][y-1] && !board->tempWalls.v[x][y-1]) {NewY--;moved++;}
            }
            if (!moved) {
                Ori = (Ori+1) % 4;
                try++;
            }
        }
        if (moved) {
            board->map[x][y] = block;
            board->IsRunner[x][y] = 0;
            runner[i].L.x = NewX;
            runner[i].L.y = NewY;
            board->map[NewX][NewY] = Runner_Ch;
            board->IsRunner[NewX][NewY] = 1;
        }
    }
    //حرکت هانتر ها
    for (int i =0 ; i<data.hcount ; i++) {
        int Ori = rand() % 4;
        int try=0;
        int moved=0;
        int x = hunter[i].L.x;
        int y = hunter[i].L.y;
        int NewX = x;
        int NewY = y;
        while (try<4 && !moved){
            if (Ori==0) {
                if (x+1<data.x && (board->map[x+1][y]!=Runner_Ch && board->map[x+1][y]!=Hunter_Ch) && !board->walls.h[x][y] && !board->tempWalls.h[x][y]) {NewX++;moved++;}

            }
            else if (Ori==1) {
                if (x-1>=0 && (board->map[x-1][y]!=Runner_Ch && board->map[x-1][y]!=Hunter_Ch) && !board->walls.h[x-1][y] && !board->tempWalls.h[x-1][y]) {NewX--;moved++;}
            }
            else if (Ori==2) {
                if (y+1<data.y && (board->map[x][y+1]!=Runner_Ch && board->map[x][y+1]!=Hunter_Ch) && !board->walls.v[x][y] && !board->tempWalls.v[x][y]) {NewY++;moved++;}
            }
            else {
                if (y-1>=0 && (board->map[x][y-1]!=Runner_Ch && board->map[x][y-1]!=Hunter_Ch) && !board->walls.v[x][y-1] && !board->tempWalls.v[x][y-1]) {NewY--;moved++;}
            }
            if (!moved) {
                Ori = (Ori+1) % 4;
                try++;
            }
        }
        if (moved) {
            if (hunter[i].OnLight) {
                board->map[x][y] = light;
                hunter[i].OnLight=0;
            }
            else if (hunter[i].OnLuckyBox) {
                board->map[x][y] = Lucky_Ch;
                hunter[i].OnLuckyBox=0;
            }
            else {board->map[x][y] = block;}
            board->IsHunter[x][y] = 0;
            hunter[i].L.x = NewX;
            hunter[i].L.y = NewY;
            board->map[NewX][NewY] = Hunter_Ch;
            if (board->map[NewX][NewY]==light) {
                hunter[i].OnLight=1;
                board->map[NewX][NewY] = light;
            }
            if (board->map[NewX][NewY]==Lucky_Ch) {
                hunter[i].OnLuckyBox=1;
                board->map[NewX][NewY] = Lucky_Ch;
            }
            else {
                hunter[i].OnLight=0;
                board->IsHunter[NewX][NewY] = 1;
            }
        }
    }
}




int main () {
    srand(time(NULL));
    struct Board board;
    struct Data data ;
    struct Runner runner[maxx*maxy];
    struct Hunter hunter[maxx*maxy];
    struct LuckyBox luckyBox[maxx*maxy];
    int visited[maxx][maxy];
    int runnerx,runnery,hunterx,huntery ;
    //گرفتن ابعاد مپ
     printf("Please enter size of map :\n ");
     scanf("%d %d" ,&data.x,&data.y );
     while ((data.x<2 || data.y<2) || (data.x==2 && data.y==2)) {
          printf("!ERROR!\n(The map size must be greater than 2*2.)\nPlease enter again:\n");
          scanf("%d %d" ,&data.x,&data.y );
     }
    //گرفتن تعداد رانر
     data.rcount = RHGetter('R',data,0);
    //گرفتن تعداد هانتر
   data.hcount = RHGetter('H',data,data.rcount);

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
            MapReset(&board,data,block) ;
            CharReset(board.IsHunter,data) ;
            CharReset(board.IsRunner,data) ;
            CharReset(board.IsLuckyBox,data) ;
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
            data.rcount = RHGetter('R',data,0);
            data.hcount = RHGetter('H',data,data.rcount);
        }
    }
    //مشخص کردن خونه های لاکی باکس
    data.LuckyBoxCount=data.rcount;
    int placed=0;
    int try=0;
    while (placed<data.LuckyBoxCount && try<TryLimit) {
        int LBx = rand() % data.x ;
        int LBy = rand() % data.y ;
        int ok =1;
        if (LBx<0 || LBx>=data.x || LBy<0 || LBy>=data.y) ok=0;
        if (board.IsRunner[LBx][LBy]==1 || board.IsHunter[LBx][LBy]==1) ok=0;
        if (LBx == board.light.x && LBy == board.light.y) ok=0;
        if (board.IsLuckyBox[LBx][LBy]==1) ok=0;
        if (ok) {
            board.IsLuckyBox[LBx][LBy] = 1;
            board.map[LBx][LBy]=Lucky_Ch;
            luckyBox[placed].L.x=LBx;
            luckyBox[placed].L.y=LBy;
            luckyBox[placed].active=1;
            luckyBox[placed].state=-1;
            placed++;
        }
        try++;
    }
    data.LuckyBoxCount=placed;
     int wcount ;
    printf("Please enter the number of walls:\n");
    scanf("%d",&wcount) ;
     while(wcount<0||wcount>((data.x-1)*(data.y-1))) {
         printf("!ERROR!\n(Invalid number of walls)\nPlease enter again:\n");
         scanf ("%d",&wcount) ;
     }
    CharReset(board.walls.h,data);
    CharReset(board.walls.v,data);
    CharReset(board.tempWalls.h,data);
    CharReset(board.tempWalls.v,data);
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
    struct MoveWall MWall;
    struct SwapMode SwapHunter;
    const int CELL = 50;
    const int WALL_THICK = 4;
    int MoveErrorTimer = 0 ;
    int TempWallErrorTimer = 0;
    int TempWallLimitTimer = 0;
    int EndTimer = 0;
    float HunterDelay=0.0f;
    int screenW = data.y * CELL;
    int screenH = data.x * CELL;
    InitWindow(screenW, screenH, "board");
    SetTargetFPS(60);

    data.game = 0;
    int End = 1;
    data.CurrentRunner = 0;
    data.CurrentHunter = 0;
    for (int i = 0; i < data.hcount; i++) {
        hunter[i].OnLight = 0;
        hunter[i].OnLuckyBox = 0;
    }
    data.RWin = (data.rcount+2)/3;
    data.ReachedLamp = 0;
    data.DeadRunners =0;
    MWall.TempWallLife=2;
    MWall.Active=0;
    MWall.WallValid=0;
    MWall.Orientation=0;
    SwapHunter.active = 0;
    SwapHunter.stage = 0;
    SwapHunter.HunterNum = 0;
    SwapHunter.DestX = 0;
    SwapHunter.DestY = 0;
    SwapHunter.valid = 0;
    data.MaxTempWall =1;
    data.UsedTempWall=0;
    if (data.x<data.y) {
        if (data.x/3 > data.MaxTempWall) {
            data.MaxTempWall = data.x/3;
        }
    }
    else {
        if (data.y/3 > data.MaxTempWall) {
            data.MaxTempWall = data.y/3;
        }
    }
    data.turn ='R';
    while (End) {
        if (WindowShouldClose()) {
            End = 0;
            break;
        }

        int RMove = 0 ;
        int NoMovement = 0;
        int ReachedLuckyBox = 0;
        int HandleSwap = 0;
        if (SwapHunter.active) {
            HandleSwap = 1;
    // مرحله‌ی انتخاب هانتر
    if (SwapHunter.stage == 0) {
        // حرکت در لیست هانترها
        if (IsKeyPressed(KEY_D)) {
            int next = SwapHunter.HunterNum;
            do {
                next = (next + 1) % data.hcount;
            } while (!board.IsHunter[hunter[next].L.x][hunter[next].L.y]);
            SwapHunter.HunterNum = next;
            SwapHunter.DestX = hunter[next].L.x;
            SwapHunter.DestY = hunter[next].L.y;
        }
        if (IsKeyPressed(KEY_A)) {
            int prev = SwapHunter.HunterNum;
            do {
                prev = (prev - 1 + data.hcount) % data.hcount;
            } while (!board.IsHunter[hunter[prev].L.x][hunter[prev].L.y]);
            SwapHunter.HunterNum = prev;
            SwapHunter.DestX = hunter[prev].L.x;
            SwapHunter.DestY = hunter[prev].L.y;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            SwapHunter.stage = 1;
        }
    }
    // انتخاب خانه ی منتقل شونده
    else {
        if (IsKeyPressed(KEY_W)   && SwapHunter.DestX > 0){SwapHunter.DestX--;}
        if (IsKeyPressed(KEY_S) && SwapHunter.DestX < data.x - 1){SwapHunter.DestX++;}
        if (IsKeyPressed(KEY_A) && SwapHunter.DestY > 0){SwapHunter.DestY--;}
        if (IsKeyPressed(KEY_D)&& SwapHunter.DestY < data.y - 1){SwapHunter.DestY++;}

        char target = board.map[SwapHunter.DestX][SwapHunter.DestY];
        if (target == block || target == Lucky_Ch || target == light) {
            SwapHunter.valid = 1;
        }
        else{SwapHunter.valid = 0;}
        
        if (IsKeyPressed(KEY_ENTER) && SwapHunter.valid) {
            int hx = hunter[SwapHunter.HunterNum].L.x;
            int hy = hunter[SwapHunter.HunterNum].L.y;

            if (hunter[SwapHunter.HunterNum].OnLight) {
                board.map[hx][hy] = light;
            } else if (hunter[SwapHunter.HunterNum].OnLuckyBox) {
                board.map[hx][hy] = Lucky_Ch;
            } else {
                board.map[hx][hy] = block;
            }
            board.IsHunter[hx][hy] = 0;
            hunter[SwapHunter.HunterNum].OnLight = 0;
            hunter[SwapHunter.HunterNum].OnLuckyBox = 0;

            char targetCell = board.map[SwapHunter.DestX][SwapHunter.DestY];

            hunter[SwapHunter.HunterNum].L.x = SwapHunter.DestX;
            hunter[SwapHunter.HunterNum].L.y = SwapHunter.DestY;

            if (targetCell == light) {
                hunter[SwapHunter.HunterNum].OnLight = 1;
            } else if (targetCell == Lucky_Ch) {
                hunter[SwapHunter.HunterNum].OnLuckyBox = 1;
            }

            board.map[SwapHunter.DestX][SwapHunter.DestY] = Hunter_Ch;
            board.IsHunter[SwapHunter.DestX][SwapHunter.DestY] = 1;

            SwapHunter.active = 0;
            SwapHunter.stage = 0;

            data.CurrentRunner++;
            if (data.CurrentRunner >= data.rcount) {
                data.CurrentRunner = 0;
                data.turn = 'H';
                HunterDelay = 0.5f;
            }
        }
    }
}
        if (data.turn == 'R' && !data.game) {
            while (data.CurrentRunner<data.rcount && runner[data.CurrentRunner].alive==0) {
                data.CurrentRunner++;
                if (data.CurrentRunner >= data.rcount) {
                    data.CurrentRunner = 0;
                    data.turn='H';
                    HunterDelay=0.5f;
                    break;
                }
            }


            if (data.turn == 'R' && runner[data.CurrentRunner].alive==1 && !HandleSwap) {
                runnerx = runner[data.CurrentRunner].L.x ;
                runnery = runner[data.CurrentRunner].L.y ;
                int newrx = runnerx ;
                int newry = runnery ;
                if (IsKeyPressed(KEY_T)) {
                    if (!MWall.Active) {
                        if (data.UsedTempWall < data.MaxTempWall) {
                            MWall.Active = 1;
                            MWall.selx = runnerx;
                            MWall.sely = runnery;
                        }
                        else {
                            TempWallLimitTimer=60;
                        }
                    }
                    else {
                        MWall.Active = 0;
                    }
                }
                if (IsKeyPressed(KEY_UP) && !MWall.Active)    { newrx--; RMove = 1; }
                if (IsKeyPressed(KEY_DOWN) && !MWall.Active)  { newrx++; RMove = 1; }
                if (IsKeyPressed(KEY_LEFT) && !MWall.Active)  { newry--; RMove = 1; }
                if (IsKeyPressed(KEY_RIGHT) && !MWall.Active) { newry++; RMove = 1; }
                if (IsKeyPressed(KEY_SPACE) && !MWall.Active) {NoMovement = 1; RMove = 1;}
                if (MWall.Active) {
                    if (IsKeyPressed(KEY_W) && MWall.selx > 0) MWall.selx--;
                    if (IsKeyPressed(KEY_S) && MWall.selx < data.x - 1) MWall.selx++;
                    if (IsKeyPressed(KEY_A) && MWall.sely > 0) MWall.sely--;
                    if (IsKeyPressed(KEY_D) && MWall.sely < data.y - 1) MWall.sely++;
                    if (IsKeyPressed(KEY_H))MWall.Orientation=0;
                    if (IsKeyPressed(KEY_V))MWall.Orientation=1;
                    MWall.WallValid=TempWallValid(data,board,MWall);
                    if (IsKeyPressed(KEY_ENTER)) {
                        if (MWall.WallValid) {
                            if (MWall.Orientation==0)
                                board.tempWalls.h[MWall.selx][MWall.sely] = MWall.TempWallLife;
                            else
                                board.tempWalls.v[MWall.selx][MWall.sely] = MWall.TempWallLife;
                            data.UsedTempWall++;
                            MWall.Active=0;
                            data.CurrentRunner++;
                            if (data.CurrentRunner==data.rcount) {
                                data.CurrentRunner = 0;
                                data.turn='H';
                                HunterDelay=0.5f;
                            }
                        }
                        else TempWallErrorTimer = 60;
                    }
                }
                if (RMove) {
                    int valid = 1;
                    if(newrx<0||newrx>=data.x||newry<0||newry>=data.y)
                        valid =0 ;
                    if(valid && !NoMovement){
                        if(newrx==(runnerx-1) && (board.walls.h[runnerx-1][runnery] || board.tempWalls.h[runnerx-1][runnery]) ) valid=0 ;
                        if(newrx==(runnerx+1) && (board.walls.h[runnerx][runnery] || board.tempWalls.h[runnerx][runnery])) valid=0 ;
                        if(newry==(runnery-1) && (board.walls.v[runnerx][runnery-1] || board.tempWalls.v[runnerx][runnery-1])) valid=0 ;
                        if(newry==(runnery+1) && (board.walls.v[runnerx][runnery] || board.tempWalls.v[runnerx][runnery])) valid=0 ;

                    }
                    if (valid && !NoMovement) {
                        if (board.map[newrx][newry]== Runner_Ch) valid = 0;
                    }
                    if (valid || NoMovement) {
                        board.map[runnerx][runnery] = block;
                        board.IsRunner[runnerx][runnery] = 0;
                        runnerx = newrx;
                        runnery = newry;
                        if (board.map[runnerx][runnery] == Lucky_Ch) {
                            ReachedLuckyBox =1;
                            board.IsLuckyBox[runnerx][runnery] = 0;
                            data.UsedLuckyBox++;
                        }
                        board.map[runnerx][runnery] = Runner_Ch;
                        board.IsRunner[runnerx][runnery] = 1;
                        runner[data.CurrentRunner].L.x = runnerx;
                        runner[data.CurrentRunner].L.y = runnery;
                        data.CurrentRunner++;
                        //اعمال لاکی باکس
                        if (ReachedLuckyBox) {
                            for (int i=0;i<data.LuckyBoxCount;i++) {
                                if (luckyBox[i].active && luckyBox[i].L.x == runnerx && luckyBox[i].L.y == runnery) {
                                    luckyBox[i].active = 0;
                                    luckyBox[i].state = rand()%4;
                                }
                                if (luckyBox[i].state!=-1) {
                                    if (luckyBox[i].state == 0) {
                                        data.CurrentRunner--;
                                        if (data.CurrentRunner<0) data.CurrentRunner = data.rcount-1;
                                    }
                                    if (luckyBox[i].state == 1) {
                                        data.MaxTempWall += 2;
                                    }
                                    if (luckyBox[i].state == 2) {
                                        Earthquake(data,&board,runner,hunter);
                                    }
                                    if (luckyBox[i].state == 3) {
                                        data.CurrentRunner--;
                                        if (data.CurrentRunner<0) data.CurrentRunner = data.rcount-1;
                                        SwapHunter.active =1;
                                        SwapHunter.stage =0;
                                        SwapHunter.HunterNum=0;
                                        SwapHunter.valid=0;
                                        SwapHunter.DestX = hunter[0].L.x;
                                        SwapHunter.DestY = hunter[0].L.y;
                                    }
                                    luckyBox[i].state = -1;
                                    break;
                                }
                            }
                        }
                        if (data.CurrentRunner==data.rcount) {
                            data.CurrentRunner = 0;
                            data.turn='H';
                            HunterDelay=0.5f;
                        }
                    }
                    else {
                        MoveErrorTimer=60 ;
                    }
                }
            }
        }
            data.game = GameState(&board,&data,runner,hunter);
            if (HunterDelay > 0) {
                HunterDelay -= GetFrameTime();
            }

            if (data.turn=='H' && !data.game && HunterDelay<=0) {
                hunterx = hunter[data.CurrentHunter].L.x;
                huntery = hunter[data.CurrentHunter].L.y;
                int newhx = hunterx ;
                int newhy = huntery ;
                int Rnum = ClosestRunner(runner,data.rcount,hunterx,huntery);
                if (Rnum==-1){newhx = hunterx; newhy = huntery;}
                else if (runner[Rnum].L.y>huntery && huntery+1<data.y && !(board.walls.v[hunterx][huntery]) && !(board.tempWalls.v[hunterx][huntery]) && board.map[newhx][newhy+1]!=Hunter_Ch) {newhy++;}
                else if (runner[Rnum].L.y<huntery && huntery-1>=0 && !(board.walls.v[hunterx][huntery-1]) && !(board.tempWalls.v[hunterx][huntery-1]) && board.map[newhx][newhy-1]!=Hunter_Ch) {newhy--;}
                else if (runner[Rnum].L.x>hunterx && hunterx+1<data.x && !(board.walls.h[hunterx][huntery]) && !(board.tempWalls.h[hunterx][huntery]) && board.map[newhx+1][newhy]!=Hunter_Ch) {newhx++;}
                else if (runner[Rnum].L.x<hunterx && hunterx-1>=0 && !(board.walls.h[hunterx-1][huntery]) && !(board.tempWalls.h[hunterx-1][huntery]) && board.map[newhx-1][newhy]!=Hunter_Ch) {newhx--;}
                if (hunter[data.CurrentHunter].OnLight) {
                    board.map[hunterx][huntery] = light;
                    hunter[data.CurrentHunter].OnLight = 0;
                }
                else if (hunter[data.CurrentHunter].OnLuckyBox) {
                    board.map[hunterx][huntery] = Lucky_Ch;
                    hunter[data.CurrentHunter].OnLuckyBox = 0;
                }
                else {board.map[hunterx][huntery] = block;}
                if (board.map[newhx][newhy]==light){hunter[data.CurrentHunter].OnLight = 1;}
                if (board.map[newhx][newhy]==Lucky_Ch){hunter[data.CurrentHunter].OnLuckyBox = 1;}
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
                    //کم کردن یک دور از عمر دیوار موقت
                    for (int i = 0; i < data.x; i++) {
                        for (int j = 0; j < data.y; j++) {
                            if (board.tempWalls.h[i][j]>0)
                                board.tempWalls.h[i][j]--;
                            if (board.tempWalls.v[i][j]>0)
                                board.tempWalls.v[i][j]--;
                        }
                    }
                }
            }
            data.game = GameState(&board,&data,runner,hunter);
        BeginDrawing();
        ClearBackground(WHITE);


        for (int i = 0; i < data.x; i++) {
            for (int j = 0; j < data.y; j++) {
                Color bg = WHITE;
                char ch = board.map[i][j];
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
        //چاپ دیوار موقت
        for (int i = 0; i < data.x - 1; i++) {
            for (int j = 0; j < data.y; j++) {
                if (board.tempWalls.h[i][j] > 0) {
                    DrawRectangle(j*CELL, i*CELL + CELL - WALL_THICK/2,
                                  CELL, WALL_THICK, SKYBLUE);
                }
            }
        }
        for (int i = 0; i < data.x; i++) {
            for (int j = 0; j < data.y - 1; j++) {
                if (board.tempWalls.v[i][j] > 0) {
                    DrawRectangle(j*CELL + CELL - WALL_THICK/2, i*CELL,
                                  WALL_THICK, CELL, SKYBLUE);
                }
            }
        }

        if (MWall.Active) {
            DrawRectangleLines(MWall.sely*CELL, MWall.selx*CELL, CELL, CELL, GREEN);
            if (!MWall.WallValid) {
                // جابجایی دیوار موقت
                if (MWall.Orientation == 1 && MWall.sely < data.y - 1) {
                    DrawRectangle(MWall.sely*CELL + CELL - WALL_THICK/2, MWall.selx*CELL,
                                  WALL_THICK, CELL, RED);
                } else if (MWall.Orientation == 0 && MWall.selx < data.x - 1) {
                    DrawRectangle(MWall.sely*CELL, MWall.selx*CELL + CELL - WALL_THICK/2,
                                  CELL, WALL_THICK, RED);
                }
            } else {
                if (MWall.Orientation == 1) {
                    DrawRectangle(MWall.sely*CELL + CELL - WALL_THICK/2, MWall.selx*CELL,
                                  WALL_THICK, CELL, SKYBLUE);
                } else {
                    DrawRectangle(MWall.sely*CELL, MWall.selx*CELL + CELL - WALL_THICK/2,
                                  CELL, WALL_THICK, SKYBLUE);
                }
            }
        }
        if (SwapHunter.active) {
            if (SwapHunter.stage == 0) {
                int hx = hunter[SwapHunter.HunterNum].L.y;
                int hy = hunter[SwapHunter.HunterNum].L.x;
                DrawRectangleLines(hx*CELL, hy*CELL, CELL, CELL, YELLOW);
            } else {
                Color c = SwapHunter.valid ? GREEN : RED;
                DrawRectangleLines(SwapHunter.DestY*CELL, SwapHunter.DestX*CELL, CELL, CELL, c);
            }
        }


        for (int i = 0; i < data.x; i++) {
            for (int j = 0; j < data.y; j++) {
                if (board.map[i][j] == Runner_Ch) {
                    if (runner[data.CurrentRunner].L.x == i && runner[data.CurrentRunner].L.y == j) {
                        DrawCircle(j*CELL + CELL/2, i*CELL + CELL/2, CELL/3, GREEN);
                    }
                    else
                    DrawCircle(j*CELL + CELL/2, i*CELL + CELL/2, CELL/3, BLUE);
                } else if (board.map[i][j] == Hunter_Ch) {
                    DrawCircle(j*CELL + CELL/2, i*CELL + CELL/2, CELL/3, RED);
                } else if (board.map[i][j] == light) {
                    int pad = WALL_THICK / 2;
                    DrawRectangle(j*CELL + pad , i*CELL + pad , CELL - WALL_THICK , CELL - WALL_THICK, YELLOW);
                }else if (board.map[i][j] == Lucky_Ch) {
                    DrawRectangleRounded((Rectangle){j*CELL + WALL_THICK/2.0f, i*CELL + WALL_THICK/2.0f, CELL - WALL_THICK, CELL - WALL_THICK}, 0.18f, 8, GOLD);
                    DrawText("?", j*CELL + CELL/2 - MeasureText("?", CELL/2)/2, i*CELL + CELL/2 - (CELL/2)/2, CELL/2, WHITE);
                }

            }
        }

        if (MoveErrorTimer > 0) {
            DrawText("INVALID MOVE!", 10, 10, 20, RED);
            MoveErrorTimer--;
        }
        if (TempWallLimitTimer > 0) {
            DrawText("You have run out of temporary walls!", 10, 10, 20, RED);
            TempWallLimitTimer--;
        }
        if (TempWallErrorTimer > 0) {
            DrawText("You can't put a wall here!", 10, 10, 20, RED);
            TempWallErrorTimer--;
        }
        if (data.game && EndTimer==0) {
            EndTimer=180;
        }
        if (data.game) {
            ClearBackground(WHITE);
            DrawRectangle(0, 0, CELL*data.y, CELL*data.x, WHITE);
            if (data.game == 1) {
                DrawText("YOU WIN",
                         screenW/2 - MeasureText("YOU WIN", 40)/2,
                         screenH/2 - 20,
                         40,
                         GREEN);
            }
            else if (data.game == 2) {
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