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
     printf("Please enter map size:\n");
     scanf ("%d %d",&x,&y) ;
     if (x<1 || y<1) {
          printf("!EROR!\n(invalid size)\n");
          return 0 ;
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
     map[lightx][lighty] = light ;


     int rcount ;
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
          map[a][b] = runner ;
     }

     int hcount ;
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