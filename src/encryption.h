#include <ctype.h>

int enterPassword(char password[30]) { // password screen for void items
  char attempt[30];

  animateIn("AUTHORIZATION", 6);
  spacing(8);
  headerTitle("AUTHORIZATION");
  spacing(1);
  margin(43, 0);
  printf(b "Please authorize your login to void items.\n" q);
  spacing(1);
  printBar(defcol);
  spacing(8);

  printf("Enter Password: ");
  scanf("%[^\n]s", attempt);
  getchar();

  if (!strcmp(password, attempt)) { // if saved password and entered password is the same
    popmsg(0, "Password authorization successfull.");
    return 1;
  } else {
    popmsg(1, "Incorrect password.");
    return 0;
  }
}

void getDigit(int key, int *keyArray) { // transforms int digit to array
  int multiplier, i;

  for (multiplier = 10000, i = 0; i < 5; key %= multiplier, multiplier /= 10, i++) {
    (keyArray)[i] = key / multiplier;
  }
}

void decrypt(int key, char *word) { // descrypts encrypted password from sys.conf
  int keyArray[5], i, digit; getDigit(key, keyArray);

  for (i = 0, digit = 0; i < strlen(word); i++, digit = (digit == 4 ? 0 : digit + 1)) { // iterates to the whole password
    if (isupper(word[i]) && ((((int) (word)[i]) - keyArray[digit]) < 65)) { // uppercase, if rolls
      word[i] = (char) 91 - ((int) word[i] - 65) - keyArray[digit];
    } else if (islower(word[i]) && (((int) word[i]) - keyArray[digit]) < 97) { // lowercase, if rolls
      word[i] = (char) 123 - ((int) word[i] - 97) - keyArray[digit];
    } else {
      word[i] = (char) (word[i] - keyArray[digit]);
    }
  }
}
