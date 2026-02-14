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
char box_char = '?';

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
struct Board {
    char map[maxx][maxy];
    struct locate light;
    int IsRunner[maxx][maxy];
    int IsHunter[maxx][maxy];
    struct Wall walls;
    struct Wall tempWalls;
    int LuckyBoxes[maxx][maxy];
};
struct Data {
    int x,y;
    int rcount,hcount;
    int CurrentRunner;
    int CurrentHunter;
    int MaxTempWall;
    int UsedTempWall;
    int ReachedLamp;
    int DeadRunners;
    int RWin;
    char turn;
    int game;
    int boxcount ;
};
struct MoveWall {
    int TempWallLife;
    int Active;
    int selx,sely;
    int Orientation;
    int WallValid;
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
void RHWReset(int map[][maxy],struct Data data) {
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
//تابع تشخیص پایان بازی با گرفتن مختصات 3 کاراکتر اصلی
// int GameState(int Lx,int Ly,int Rx,int Ry,int Hx,int Hy) {
//     if (Hx==Rx && Hy==Ry) return 2;
//     else if (Lx==Rx && Ly==Ry) return 1;
//     else return 0;
// }

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

void DecreaseTempWalls(struct Board *board, struct Data data) {
    for (int i = 0; i < data.x; i++) {
        for (int j = 0; j < data.y; j++) {
            if (board->tempWalls.h[i][j] > 0)
                board->tempWalls.h[i][j]--;
            if (board->tempWalls.v[i][j] > 0)
                board->tempWalls.v[i][j]--;
        }
    }
}

int AddRandomWall(struct Board *board, struct Data data, int visited[][maxy]) {
    int added = 0;
    int tries = 0;

    while (!added && tries < 50) {
        tries++;
        int choice = rand() % 2;

        if (choice) {
            int i = rand() % (data.x - 1);
            int j = rand() % data.y;

            if (board->walls.h[i][j] == 0 && board->tempWalls.h[i][j] == 0) {
                board->walls.h[i][j] = 1;
                if (Connected(board->walls.h, board->walls.v, visited, data.x, data.y)) {
                    added = 1;
                } else {
                    board->walls.h[i][j] = 0;
                }
            }
        } else {
            int i = rand() % data.x;
            int j = rand() % (data.y - 1);

            if (board->walls.v[i][j] == 0 && board->tempWalls.v[i][j] == 0) {
                board->walls.v[i][j] = 1;
                if (Connected(board->walls.h, board->walls.v, visited, data.x, data.y)) {
                    added = 1;
                } else {
                    board->walls.v[i][j] = 0;
                }
            }
        }
    }
    return added;
}


void MoveRunnerRandom(struct Runner runner[], int runner_index,
                      struct Board *board, struct Data data) {
    if (runner_index < 0 || runner_index >= data.rcount) return;
    if (runner[runner_index].alive == 0) return;

    int old_x = runner[runner_index].L.x;
    int old_y = runner[runner_index].L.y;

    for (int attempt = 0; attempt < 50; attempt++) {
        int new_x = old_x + (rand() % 3) - 1;
        int new_y = old_y + (rand() % 3) - 1;

        if (new_x >= 0 && new_x < data.x && new_y >= 0 && new_y < data.y &&
            board->map[new_x][new_y] == block) {

            board->map[old_x][old_y] = block;
            board->IsRunner[old_x][old_y] = 0;

            runner[runner_index].L.x = new_x;
            runner[runner_index].L.y = new_y;

            board->map[new_x][new_y] = Runner_Ch;
            board->IsRunner[new_x][new_y] = 1;

            printf("   Runner %d moved to (%d,%d)\n", runner_index, new_x, new_y);
            break;
        }
    }
}

void MoveHunterRandom(struct Hunter hunter[], int hunter_index,
                      struct Board *board, struct Data data) {
    if (hunter_index < 0 || hunter_index >= data.hcount) return;

    int old_x = hunter[hunter_index].L.x;
    int old_y = hunter[hunter_index].L.y;

    for (int attempt = 0; attempt < 50; attempt++) {
        int new_x = old_x + (rand() % 3) - 1;
        int new_y = old_y + (rand() % 3) - 1;

        if (new_x >= 0 && new_x < data.x && new_y >= 0 && new_y < data.y &&
            board->map[new_x][new_y] == block) {

            if (hunter[hunter_index].OnLight) {
                board->map[old_x][old_y] = light;
            } else {
                board->map[old_x][old_y] = block;
            }
            board->IsHunter[old_x][old_y] = 0;

            hunter[hunter_index].L.x = new_x;
            hunter[hunter_index].L.y = new_y;

            board->map[new_x][new_y] = Hunter_Ch;
            board->IsHunter[new_x][new_y] = 1;

            if (board->map[new_x][new_y] == light) {
                hunter[hunter_index].OnLight = 1;
            }

            printf("   Hunter %d moved to (%d,%d)\n", hunter_index, new_x, new_y);
            break;
        }
    }
}

int MoveHunterManually(struct Hunter hunter[], int hunter_index,
                       struct Board *board, struct Data data,
                       int screenW, int screenH, int CELL, int WALL_THICK) {

    if (hunter_index < 0 || hunter_index >= data.hcount) return 0;

    int old_x = hunter[hunter_index].L.x;
    int old_y = hunter[hunter_index].L.y;
    int moved = 0;
    int timer = 300; // 5 ثانیه فرصت

    printf("\n🎯 MOVE HUNTER %d!\n", hunter_index);
    printf("Use arrow keys to move hunter to an adjacent empty cell\n");
    printf("NO WALL between cells!\n");
    printf("Press SPACE to skip\n");

    while (timer > 0 && !moved) {
        if (IsKeyPressed(KEY_UP)) {
            if (old_x > 0 && !board->walls.h[old_x - 1][old_y] &&
                !board->tempWalls.h[old_x - 1][old_y] &&
                board->map[old_x - 1][old_y] == block) {

                if (hunter[hunter_index].OnLight) {
                    board->map[old_x][old_y] = light;
                } else {
                    board->map[old_x][old_y] = block;
                }
                board->IsHunter[old_x][old_y] = 0;

                hunter[hunter_index].L.x = old_x - 1;
                hunter[hunter_index].L.y = old_y;
                board->map[old_x - 1][old_y] = Hunter_Ch;
                board->IsHunter[old_x - 1][old_y] = 1;

                if (board->map[old_x - 1][old_y] == light) {
                    hunter[hunter_index].OnLight = 1;
                }

                moved = 1;
                printf("   Hunter %d moved UP to (%d,%d)\n",
                       hunter_index, old_x - 1, old_y);
            }
        } else if (IsKeyPressed(KEY_DOWN)) {
            if (old_x < data.x - 1 && !board->walls.h[old_x][old_y] &&
                !board->tempWalls.h[old_x][old_y] &&
                board->map[old_x + 1][old_y] == block) {

                if (hunter[hunter_index].OnLight) {
                    board->map[old_x][old_y] = light;
                } else {
                    board->map[old_x][old_y] = block;
                }
                board->IsHunter[old_x][old_y] = 0;

                hunter[hunter_index].L.x = old_x + 1;
                hunter[hunter_index].L.y = old_y;
                board->map[old_x + 1][old_y] = Hunter_Ch;
                board->IsHunter[old_x + 1][old_y] = 1;

                if (board->map[old_x + 1][old_y] == light) {
                    hunter[hunter_index].OnLight = 1;
                }

                moved = 1;
                printf("   Hunter %d moved DOWN to (%d,%d)\n",
                       hunter_index, old_x + 1, old_y);
            }
        } else if (IsKeyPressed(KEY_LEFT)) {
            if (old_y > 0 && !board->walls.v[old_x][old_y - 1] &&
                !board->tempWalls.v[old_x][old_y - 1] &&
                board->map[old_x][old_y - 1] == block) {

                if (hunter[hunter_index].OnLight) {
                    board->map[old_x][old_y] = light;
                } else {
                    board->map[old_x][old_y] = block;
                }
                board->IsHunter[old_x][old_y] = 0;

                hunter[hunter_index].L.x = old_x;
                hunter[hunter_index].L.y = old_y - 1;
                board->map[old_x][old_y - 1] = Hunter_Ch;
                board->IsHunter[old_x][old_y - 1] = 1;

                if (board->map[old_x][old_y - 1] == light) {
                    hunter[hunter_index].OnLight = 1;
                }

                moved = 1;
                printf("   Hunter %d moved LEFT to (%d,%d)\n",
                       hunter_index, old_x, old_y - 1);
            }
        } else if (IsKeyPressed(KEY_RIGHT)) {
            if (old_y < data.y - 1 && !board->walls.v[old_x][old_y] &&
                !board->tempWalls.v[old_x][old_y] &&
                board->map[old_x][old_y + 1] == block) {

                if (hunter[hunter_index].OnLight) {
                    board->map[old_x][old_y] = light;
                } else {
                    board->map[old_x][old_y] = block;
                }
                board->IsHunter[old_x][old_y] = 0;

                hunter[hunter_index].L.x = old_x;
                hunter[hunter_index].L.y = old_y + 1;
                board->map[old_x][old_y + 1] = Hunter_Ch;
                board->IsHunter[old_x][old_y + 1] = 1;

                if (board->map[old_x][old_y + 1] == light) {
                    hunter[hunter_index].OnLight = 1;
                }

                moved = 1;
                printf("   Hunter %d moved RIGHT to (%d,%d)\n",
                       hunter_index, old_x, old_y + 1);
            }
        } else if (IsKeyPressed(KEY_SPACE)) {
            printf("   Move skipped!\n");
            moved = 1;
        }

        timer--;

        BeginDrawing();
        ClearBackground(WHITE);

        DrawText("MOVE HUNTER!",
                 screenW / 2 - MeasureText("MOVE HUNTER!", 40) / 2,
                 screenH / 2 - 80, 40, PURPLE);

        char msg[100];
        sprintf(msg, "Hunter %d - Use arrow keys to move", hunter_index);
        DrawText(msg,
                 screenW / 2 - MeasureText(msg, 20) / 2,
                 screenH / 2 - 20, 20, DARKGRAY);

        DrawText("Cell must be EMPTY and NO WALL between",
                 screenW / 2 - MeasureText("Cell must be EMPTY and NO WALL between", 20) / 2,
                 screenH / 2 + 10, 20, DARKGRAY);

        DrawText("Valid moves:",
                 screenW / 2 - MeasureText("Valid moves:", 20) / 2,
                 screenH / 2 + 50, 20, GREEN);

        int y_offset = 80;
        if (old_x > 0 && !board->walls.h[old_x - 1][old_y] &&
            !board->tempWalls.h[old_x - 1][old_y] &&
            board->map[old_x - 1][old_y] == block) {
            DrawText("↑ UP", screenW / 2 - 30, screenH / 2 + y_offset, 20, GREEN);
            y_offset += 25;
        }
        if (old_x < data.x - 1 && !board->walls.h[old_x][old_y] &&
            !board->tempWalls.h[old_x][old_y] &&
            board->map[old_x + 1][old_y] == block) {
            DrawText("↓ DOWN", screenW / 2 - 40, screenH / 2 + y_offset, 20, GREEN);
            y_offset += 25;
        }
        if (old_y > 0 && !board->walls.v[old_x][old_y - 1] &&
            !board->tempWalls.v[old_x][old_y - 1] &&
            board->map[old_x][old_y - 1] == block) {
            DrawText("← LEFT", screenW / 2 - 40, screenH / 2 + y_offset, 20, GREEN);
            y_offset += 25;
        }
        if (old_y < data.y - 1 && !board->walls.v[old_x][old_y] &&
            !board->tempWalls.v[old_x][old_y] &&
            board->map[old_x][old_y + 1] == block) {
            DrawText("→ RIGHT", screenW / 2 - 40, screenH / 2 + y_offset, 20, GREEN);
        }

        DrawText("Press SPACE to skip",
                 screenW / 2 - MeasureText("Press SPACE to skip", 20) / 2,
                 screenH / 2 + 180, 20, RED);

        EndDrawing();
    }

    return moved;
}

int SelectHunterManually(struct Hunter hunter[], struct Data data,
                         struct Board *board, int screenW, int screenH,
                         int CELL, int WALL_THICK) {

    int selected = -1;
    int timer = 300;
    int selector_x = 0, selector_y = 0;


    for (int i = 0; i < data.x; i++) {
        for (int j = 0; j < data.y; j++) {
            if (board->IsHunter[i][j]) {
                selector_x = i;
                selector_y = j;
                break;
            }
        }
    }

    printf("\n🎯 SELECT HUNTER TO MOVE!\n");
    printf("Use arrow keys to move cursor\n");
    printf("Press ENTER to select hunter under cursor\n");
    printf("Press SPACE to cancel\n");

    while (timer > 0 && selected == -1) {
        if (IsKeyPressed(KEY_UP) && selector_x > 0) selector_x--;
        if (IsKeyPressed(KEY_DOWN) && selector_x < data.x - 1) selector_x++;
        if (IsKeyPressed(KEY_LEFT) && selector_y > 0) selector_y--;
        if (IsKeyPressed(KEY_RIGHT) && selector_y < data.y - 1) selector_y++;

        if (IsKeyPressed(KEY_ENTER)) {
            if (board->IsHunter[selector_x][selector_y]) {
                for (int i = 0; i < data.hcount; i++) {
                    if (hunter[i].L.x == selector_x && hunter[i].L.y == selector_y) {
                        selected = i;
                        printf("   ✅ Hunter %d selected at (%d,%d)\n", i, selector_x, selector_y);
                        break;
                    }
                }
                if (selected == -1) {
                    printf("   ⚠️ No hunter at this position!\n");
                }
            } else {
                printf("   ⚠️ No hunter at this position!\n");
            }
        }

        if (IsKeyPressed(KEY_SPACE)) {
            printf("   ❌ Selection cancelled!\n");
            return -1;
        }

        timer--;

        BeginDrawing();
        ClearBackground(WHITE);

        for (int i = 0; i < data.x; i++) {
            for (int j = 0; j < data.y; j++) {
                DrawRectangle(j * CELL, i * CELL, CELL, CELL, WHITE);
                DrawRectangleLines(j * CELL, i * CELL, CELL, CELL, LIGHTGRAY);

                if (board->IsHunter[i][j]) {
                    DrawCircle(j * CELL + CELL / 2, i * CELL + CELL / 2, CELL / 3, RED);
                }
            }
        }

        DrawRectangleLines(selector_y * CELL, selector_x * CELL, CELL, CELL, GREEN);

        DrawText("SELECT HUNTER - Use arrows to move, ENTER to select, SPACE to cancel",
                 10, screenH - 30, 20, DARKGRAY);

        EndDrawing();
    }

    return selected;
}

int ActivateLuckyBox(struct Runner runner[], struct Hunter hunter[],
                     struct Board *board, struct Data *data,
                     int visited[][maxy], int *extra_turn,
                     int runner_index, int screenW, int screenH,
                     int CELL, int WALL_THICK, char *boxMessage, int *boxMessageTimer) {

    int chance = rand() % 4;

    switch (chance) {
        case 0: // نوبت مجدد
            printf("\n🎁 LUCKY BOX: EXTRA TURN!\n");
            printf("   Runner %d can move again!\n", runner_index);
            *extra_turn = 1;
            sprintf(boxMessage, "EXTRA TURN!");
            *boxMessageTimer = 120;
            return 1;

        case 1: // +۲ دیوار
            printf("\n🎁 LUCKY BOX: +2 WALLS ADDED!\n");
            int added = 0;
            for (int w = 0; w < 2; w++) {
                if (AddRandomWall(board, *data, visited)) {
                    added++;
                    printf("   Wall %d added!\n", w + 1);
                }
            }
            printf("   %d walls placed successfully!\n", added);
            *extra_turn = 0;
            sprintf(boxMessage, "+2 WALLS ADDED!");
            *boxMessageTimer = 120;
            return 2;

        case 2: // زلزله
            printf("\n🎁 LUCKY BOX: EARTHQUAKE!\n");
            printf("   Everyone moves randomly!\n");

            for (int i = 0; i < data->rcount; i++) {
                if (runner[i].alive) {
                    MoveRunnerRandom(runner, i, board, *data);
                }
            }

            for (int i = 0; i < data->hcount; i++) {
                MoveHunterRandom(hunter, i, board, *data);
            }

            *extra_turn = 0;
            sprintf(boxMessage, "EARTHQUAKE!");
            *boxMessageTimer = 120;
            return 3;

        case 3: // جابجایی هانتر
            printf("\n🎁 LUCKY BOX: MOVE HUNTER!\n");
            printf("   Select a hunter to move!\n");

            int hunter_index = SelectHunterManually(hunter, *data, board,
                                                    screenW, screenH, CELL, WALL_THICK);

            if (hunter_index >= 0) {
                int moved = MoveHunterManually(hunter, hunter_index, board, *data,
                                               screenW, screenH, CELL, WALL_THICK);

                if (moved) {
                    printf("   ✅ Hunter %d moved successfully!\n", hunter_index);
                    sprintf(boxMessage, "HUNTER MOVED!");
                } else {
                    printf("   ⚠️ Hunter %d stayed in place.\n", hunter_index);
                    sprintf(boxMessage, "HUNTER SKIPPED");
                }
            } else {
                printf("   ❌ No hunter selected!\n");
                sprintf(boxMessage, "NO HUNTER SELECTED");
            }

            *boxMessageTimer = 120;
            *extra_turn = 0;
            return 4;

        default:
            *extra_turn = 0;
            return 0;
    }
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
     data.rcount = RHGetter('R',data,0);
    //گرفتن تعداد هانتر
     // int Hunter[maxx][maxy] ;
   data.hcount = RHGetter('H',data,data.rcount);


    printf("\n=== LUCKY BOXES ===\n");
    printf("Please enter the number of lucky boxes:\n");
    scanf("%d", &data.boxcount);
    int maxBoxes = data.x * data.y - data.rcount - data.hcount - 1;
    while (data.boxcount < 0 || data.boxcount > maxBoxes) {
        if (data.boxcount < 0)
            printf("!ERROR!\n(Invalid number of lucky boxes)\nPlease enter again:\n");
        else
            printf("!ERROR!\n(Too many lucky boxes! Max %d)\nPlease enter again:\n", maxBoxes);
        scanf("%d", &data.boxcount);
    }


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
            RHWReset(board.IsHunter,data) ;
            RHWReset(board.IsRunner,data) ;

            RHWReset(board.LuckyBoxes,data) ;

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
            /*if (CharactersCount == CharactersSum) {
                ok = 1;
                break;
            }
        }
        if (!ok) {
            printf("!EROR!\n(Your Runner and Hunter input values are not optimal for the map dimensions)\nPlease re-enter the values carefully\n");
            data.rcount = RHGetter('R',data,0);
            data.hcount = RHGetter('H',data,data.rcount);
        }
    }*/

    int boxPlaced = 0;
    int boxTries = 0;
    while (boxPlaced < data.boxcount && boxTries < TryLimit * 2) {
        boxTries++;
        int bx = rand() % data.x;
        int by = rand() % data.y;

        if (board.map[bx][by] == block && board.LuckyBoxes[bx][by] == 0) {
            board.LuckyBoxes[bx][by] = 1;
            board.map[bx][by] = box_char;
            boxPlaced++;
            CharactersCount++;
        }
    }

            if (CharactersCount == CharactersSum) {
                ok = 1;
                break;
            }
        }

        if (!ok) {
            printf("!ERROR!\n(Values not optimal)\nPlease re-enter\n");
            data.rcount = RHGetter('R', data, 0);
            data.hcount = RHGetter('H', data, data.rcount);
            printf("\n=== LUCKY BOXES ===\n");
            printf("Please enter the number of lucky boxes:\n");
            scanf("%d", &data.boxcount);
        }
    }

     int wcount ;
    printf("Please enter the number of walls:\n");
    scanf("%d",&wcount) ;
     while(wcount<0||wcount>((data.x-1)*(data.y-1))) {
         printf("!ERROR!\n(Invalid number of walls)\nPlease enter again:\n");
         scanf ("%d",&wcount) ;
     }
    RHWReset(board.walls.h,data);
    RHWReset(board.walls.v,data);
    RHWReset(board.tempWalls.h,data);
    RHWReset(board.tempWalls.v,data);
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
    struct MoveWall MWall;
    const int CELL = 50;
    const int WALL_THICK = 4;
    int MoveErrorTimer = 0 ;
    int TempWallErrorTimer = 0;
    int TempWallLimitTimer = 0;
    int EndTimer = 0;

    int BoxMessageTimer = 0;
    char BoxMessage[100] = "";


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
    }
    data.RWin = (data.rcount+2)/3;
    data.ReachedLamp = 0;
    data.DeadRunners =0;
    MWall.TempWallLife=2;
    MWall.Active=0;
    MWall.WallValid=0;
    MWall.Orientation=0;
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

    int extraTurn = 0;


    while (End) {
        if (WindowShouldClose()) {
            End = 0;
            break;
        }

        int RMove = 0 ;
        int NoMovement = 0;
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
            if (data.turn == 'R' && runner[data.CurrentRunner].alive==1) {
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

                        int old_x = runnerx;
                        int old_y = runnery;

                        board.map[runnerx][runnery] = block;
                        board.IsRunner[runnerx][runnery] = 0;
                        runnerx = newrx;
                        runnery = newry;


                        /*board.map[runnerx][runnery] = Runner_Ch;
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
                        MoveErrorTimer=60 ;
                    }
                }
            }
        }
            data.game = GameState(&board,&data,runner,hunter);
            if (HunterDelay > 0) {
                HunterDelay -= GetFrameTime();
            }*/
                        if (board.LuckyBoxes[runnerx][runnery] == 1 && !NoMovement) {
                            board.LuckyBoxes[runnerx][runnery] = 0;
                            board.map[runnerx][runnery] = Runner_Ch;
                            board.IsRunner[runnerx][runnery] = 1;
                            runner[data.CurrentRunner].L.x = runnerx;
                            runner[data.CurrentRunner].L.y = runnery;

                            int boxResult = ActivateLuckyBox(runner, hunter, &board, &data,
                                                             visited, &extraTurn,
                                                             data.CurrentRunner,
                                                             screenW, screenH, CELL, WALL_THICK,
                                                             BoxMessage, &BoxMessageTimer);

                            if (boxResult == 1) {
                                // نوبت مجدد - CurrentRunner تغییر نمیکنه
                            } else {
                                data.CurrentRunner++;
                            }
                        } else {
                            board.map[runnerx][runnery] = Runner_Ch;
                            board.IsRunner[runnerx][runnery] = 1;
                            runner[data.CurrentRunner].L.x = runnerx;
                            runner[data.CurrentRunner].L.y = runnery;
                            data.CurrentRunner++;
                        }
                        // ======================================

                        if (data.CurrentRunner == data.rcount) {
                            if (!extraTurn) {
                                data.CurrentRunner = 0;
                                data.turn = 'H';
                                HunterDelay = 0.5f;
                            } else {
                                data.CurrentRunner = 0;
                                extraTurn = 0;  // نوبت مجدد تموم شد
                            }
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
                else board.map[hunterx][huntery] = block;
                if (board.map[newhx][newhy]==light){hunter[data.CurrentHunter].OnLight = 1;}
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
                   /* for (int i = 0; i < data.x; i++) {
                        for (int j = 0; j < data.y; j++) {
                            if (board.tempWalls.h[i][j]>0)
                                board.tempWalls.h[i][j]--;
                            if (board.tempWalls.v[i][j]>0)
                                board.tempWalls.v[i][j]--;
                        }
                    }*/

                    DecreaseTempWalls(&board, data);

                }
            }
            data.game = GameState(&board,&data,runner,hunter);
        BeginDrawing();
        ClearBackground(WHITE);


        for (int i = 0; i < data.x; i++) {
            for (int j = 0; j < data.y; j++) {
                Color bg = WHITE;
                char ch = board.map[i][j];
                // if (ch == Runner_Ch || ch == Hunter_Ch || ch == light) {
                //     bg = WHITE;
                // }
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
        //az in
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


        for (int i = 0; i < data.x; i++) {
            for (int j = 0; j < data.y; j++) {
                if (board.LuckyBoxes[i][j] == 1) {
                    int pad = WALL_THICK / 2;
                    DrawRectangle(j*CELL + pad, i*CELL + pad,
                                  CELL - WALL_THICK, CELL - WALL_THICK, PURPLE);
                    DrawText("?", j*CELL + CELL/2 - 10, i*CELL + CELL/2 - 10, 20, WHITE);
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



        for (int i = 0; i < data.x; i++) {
            for (int j = 0; j < data.y; j++) {
                // char ch = board.map[i][j];
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

        if (BoxMessageTimer > 0) {
            DrawText(BoxMessage, screenW/2 - MeasureText(BoxMessage, 30)/2,
                     screenH/2 - 50, 30, PURPLE);
            BoxMessageTimer--;
        }


        char status[100];
        if (data.turn == 'R') {
            sprintf(status, "Runner %d's turn %s", data.CurrentRunner + 1,
                    extraTurn ? "(EXTRA!)" : "");
            DrawText(status, 10, screenH - 30, 20, BLUE);
        } else {
            sprintf(status, "Hunter's Turn");
            DrawText(status, 10, screenH - 30, 20, RED);
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

    CloseWindow();
    return 0;

}