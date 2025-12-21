// Phase 2 Project
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
const int maxx = 100 ;
const int maxy = 100 ;
char hunter = 'H' ;
char runner = 'R' ;
char light = 'L' ;
char block = '.' ;
char V = '|' ;
char H = '_' ;
int main () {
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
     int wallh[maxx][maxy];
     int wallv[maxx][maxy];
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

     for(int i=0 ; i<wcount ; i++) {
          int a , b ;
          char c ;
          printf("Please enter the coordinates of the wall(%d) and (H/V):\n",i+1);

          while (1) {
               scanf ("%d  %d %c",&a , &b ,&c) ;

               if (!(c=='H' || c=='h' || c=='V' || c=='v')) {
                    printf("!EROR!\n(Character must be H or V) Try again.\nPlese enter again the coordinates of the wall(%d):\n",i+1);
                    continue;
               }

               if ((c=='H' || c=='h') && (a < 0 || a >= x-1 || b < 0 || b >= y)) {
                    printf("!ERROR!\n(Out of map range)\nPlese enter again the coordinates of the wall(%d):\n",i+1);
                    continue;
               }

               if ((c=='V' || c=='v') && (a < 0 || a >= x || b < 0 || b >= y-1)) {
                    printf("!ERROR!\n(Out of map range)\nPlese enter again the coordinates of the wall(%d):\n",i+1);
                    continue;
               }

               break;
          }
          if (c=='H' || c=='h') wallh[a][b] = 1;
          else wallv[a][b] = 1;
     }
     for(int i =0 ; i<x ; i++) {
          for(int j=0 ; j<y ; j++){
               printf("%c",map[i][j]) ;
               if (wallv[i][j]==1){
                    printf("%c",V) ;
               }
               else printf(" ") ;

          }
          printf("\n") ;
          for(int j=0;j<x;j++) {
               if(wallh[i][j] == 1) printf("%c ",H);
               else printf("  ");
          }
          printf("\n");
     }
     }