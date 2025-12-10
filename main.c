// Phase 1 Project
#include <stdio.h>

const int maxx = 100 ;
const int maxy = 100 ;
char hunter = 'H' ;
char runner = 'R' ;
char block = '.' ;
char V = '|' ;
char H = '_' ;
char light = 'L' ;
int main () {
     int x , y ;
     printf("Please enter size of map :\n ");
     scanf("%d %d" , &x,&y );
     while (x<1 || y<1) {
          printf("!EROR!\n(invalid coordinates of map!)\nPlease enter again:\n");
          scanf("%d %d" , &x,&y );
     }
     char map[maxx][maxy] ;
     for (int i=0 ; i<x ; i++ ){
          for(int j=0 ; j<y ; j++){
               map[i][j] = block ;
          }
     }
     int lightx , lighty ;
     printf("Please enter the coordinates of the lamp:\n");
     scanf ("%d %d",&lightx,&lighty) ;
     while (lightx<0 || lighty<0 || lightx>=x || lighty>=y) {
          printf("ERROR!\nOut of map range\nPlese enter again the coordinates of the lamp:\n");
          scanf ("%d %d",&lightx,&lighty) ;
     }
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
     while (rcount<1) {
          printf("!EROR!\n(invalid number of runners)\nPlease enter again:\n");
          scanf ("%d",&rcount ) ;
     }
     for(int i=0 ; i<rcount ; i++) {
          int a , b ;
          printf("Please enter the coordinates of the runner(%d):\n",i+1);
          scanf("%d %d",&a,&b) ;
          while (a<0 || b<0 || a>=x || b>=y) {
               printf("ERROR!\nOut of map range\nPlese enter again the coordinates of the runner(%d):\n",i+1);
               scanf ("%d %d",&a,&b) ;
          }
          while (a==lightx && b==lighty || Runner[a][b]==1) {
               if (a==lightx && b==lighty)
                    printf("same coordinates of the runner(%d) and lamp!\n Please enter again coordinates of the runner(%d):\n",i+1,i+1);
               else if (Runner[a][b]==1)
               printf("!EROR!\n(same coordinates of the runner(%d) and another runner!\n Please enter again coordinates of the runner(%d):\n",i+1,i+1);
               scanf("%d %d",&a,&b) ;
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
     while (hcount<1) {
          printf("!EROR!\n(invalid number of hunters)\nPlease enter again:\n");
          scanf ("%d",&hcount) ;
     }
     for(int i=0 ; i<hcount ; i++) {
          int a , b ;
          printf("Please enter the coordinates of the hunter(%d):\n",i+1);
          scanf("%d %d",&a,&b) ;
          while (a<0 || b<0 || a>=x || b>=y) {
               printf("ERROR!\nOut of map range\nPlese enter again the coordinates of the hunter(%d):\n",i+1);
               scanf ("%d %d",&a,&b) ;
          }
          while (a==lightx && b==lighty || Runner[a][b]==1 || Hunter[a][b]==1) {
               if (a==lightx && b==lighty)
               printf("!EROR!\n(same coordinates of the hunter(%d) and lamp!\n Please enter again coordinates of the hunter(%d):\n",i+1,i+1);
               else if (Runner[a][b]==1)
               printf("!EROR!\n(same coordinates of the hunter(%d) and runner!\n Please enter again coordinates of the hunter(%d):\n",i+1,i+1);
               else if (Hunter[a][b]==1)
               printf("!EROR!\n(same coordinates of the hunter(%d) and another hunter!\n Please enter again coordinates of the hunter(%d):\n",i+1,i+1);
               scanf("%d %d",&a,&b) ;
          }
          Hunter[a][b] = 1 ;
          map[a][b] = hunter ;
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
          while (1) {
               printf("Please enter the coordinates of the wall(%d) and (H/V):\n",i+1);
               scanf ("%d  %d %c",&a , &b ,&c) ;
               if (c=='H'||c == 'h'||c=='V'||c=='v') break;
               printf("!EROR!\n(Character must be H or V. Try again.)\n");
          }
          if (c=='H'||c == 'h') wallh[a][b] = 1 ;
          else wallv[a][b] = 1 ;
     }
     for(int i =0 ; i<x ; i++) {
          for(int j=0 ; j<y ; j++){
               printf("%c",map[i][j]) ;
               if (wallv[i][j]==1){
                    printf("%c",V) ;
               }
               else printf(" ") ;

          } printf("\n") ;
          for(int j=0;j<x;j++) {
               if(wallh[i][j] == 1) printf("%c ",H);
               else printf("  ");
          }
          printf("\n");
     }
     }