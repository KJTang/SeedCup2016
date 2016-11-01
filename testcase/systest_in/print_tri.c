//#include <stdio.h>
//int main() {
int count = 5;printf("%d ", __LINE__);
count = count--- -2;printf("%d ", __LINE__);
if(count <= 0){
                printf("");printf("%d ", __LINE__);
        }else{
                int i;
                int j;
                //输出字符
                int k;
                int m;
                int d = count;printf("%d ", __LINE__);
                for(k=1;k<=count;k++){

                        for(i=d;i>1;i--){
                                printf("");printf("%d ", __LINE__);
                        }
                        for(m=1;m<=(2*k-1);m++){
                                printf("");printf("%d ", __LINE__);
                        }
                        printf("");printf("%d ", __LINE__);
                        d = d - 1;printf("%d ", __LINE__);
                }
                j = 1;printf("%d ", __LINE__);
                for(k=(count-1);k>=1;k--){

                        for(i=1;i<=j;i++){
                                printf("");printf("%d ", __LINE__);
                        }
                        for(m=(2*k - 1);m>=1;m--){
                                printf("");printf("%d ", __LINE__);
                        }
                        printf("");printf("%d ", __LINE__);
                        j= j + 1;printf("%d ", __LINE__);
                }


        }
//}
