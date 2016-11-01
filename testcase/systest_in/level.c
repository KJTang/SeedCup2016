//#include <stdio.h>
//int main() {
  int i = 1;
  for (int i = 2; i; i--) {
    if ( i == 1) {
      break;
    }
    if (i == 2) {
      i = 0;
    }
    i = 3;
    if (i == 3) {
      i++;break;
    }
  }

  if (i == 1) {
	  i = -1;
}
  if (i == 0) {
	  i = -0;
}
  if (i == 3) {
	  i = -3;
}
//}
