// faze 1 progect
#include <stdio.h>

const int maxx = 100 ;
const int maxy = 100 ;
char hunter = 'H' ;
char runner = 'R' ;
char block = '.' ;
char wallv = '|' ;
char wallh = '_' ;
char light = 'L' ;

int main () {

     int x , y ;
     printf("Please enter map size:\n");
     scanf ("%d %d",&x,&y) ;

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
     for(int i=0 ; i<rcount ; i++) {
          int a , b ;
          printf("Please enter the coordinates of the runner:\n");
          scanf("%d %d",&a,&b) ;
          map[a][b] = runner ;
     }

     int hcount ;
     printf("Please enter the number of hunters:\n");
     scanf ("%d",&hcount) ;
     for(int i=0 ; i<hcount ; i++) {
          int a , b ;
          printf("Please enter the coordinates of the hunter:\n");
          scanf("%d %d",&a,&b) ;
          map[a][b] = hunter ;
     }

     int wcount ;
     printf("Please enter the number of walls:\n");
     scanf("%d",&wcount) ;
          int walh[maxx][maxy];
     for (int i=0 ; i<x ; i++ ){
          for(int j=0 ; j<y ; j++) {
               walh[i][j] = 0 ;
          }
            int walv[maxx][maxy] ;
            for (int i=0 ; i<x ; i++ ) {
                 for(int j=0 ; j<y ; j++) {
                      walv[i][j] = 0 ;
                 }
            }
     for(int i=0 ; i<wcount ; i++) {
          int a , b ;
          char c ;
          printf("Please enter the coordinates of the wall and (H/V):\n");
          scanf ("%d  %d %c",&a , &b ,&c) ;
          if (c=='H'||c == 'h') walh[a][b] = 1 ;
          else walv[a][b] = 1 ;
     }

for(int i =0 ; i<x ; i++) {
     for(int j=0 ; j<y ; j++){
          printf("%c",map[i][j]) ;
          if (walv[i][j]==1){
               printf("%c",wallv) ;
          }
          else printf(" ") ;

     } printf("\n") ;
          for(int j=0;j<x;j++) {
               if(walh[i][j] == 1) printf("%c ",wallh);
               else printf("  ");
          }
     printf("\n");
}
}
}