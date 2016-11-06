//#include <stdio.h>
//int main() {
int n = 6;printf("%d ", __LINE__);
    int result;
        int previous_result;
	    int previous_pre_result;
	        result=1;  printf("%d ", __LINE__);
		    previous_result=1;  printf("%d ", __LINE__);
		        while(n>2)
	    {
		            n--;  printf("%d ", __LINE__);
			            previous_pre_result=previous_result;  printf("%d ", __LINE__);
				            previous_result=result;  printf("%d ", __LINE__);
					            result=previous_result+previous_pre_result; printf("%d ", __LINE__);
						        }
    printf("", result);  printf("%d ", __LINE__);
//}
