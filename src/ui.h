#include <string.h>
#include <unistd.h>

// colored printing escape characters
#define r   "\x1B[31m"
#define g   "\x1B[32m"
#define b   "\x1B[34m"

#define c   "\x1B[36m"
#define y   "\x1B[33m"
#define m   "\x1B[35m"

#define w   "\x1B[37m"
#define q   "\x1B[0m"

// default values
#define defcol 80
#define defrow 24

void printBar(int len) { // prints bar with length len
  int i;

  for (i = 0; i < len; i++) printf("-");
  if (len == defcol) printf("\n");
}

void headerTitle(char header[20]) { // title header
  printBar((defcol - (strlen(header) + 4)) / 2); // computes for the half of the screen
  printf(" [" c "%s" q, header);
  if (strlen(header) % 2 != 0) printf(" ");
  printf("] ");
  printBar((defcol - (strlen(header) + 4)) / 2);

  printf("\n");
}

void margin(int len, int doEnter) { // sets for centering text
  int i;

  for (i = 0; i < ((defcol - len)) / 2; i++) printf(" ");
  if (doEnter) printf("\n");
}

void spacing(int height) { // sets new line characters
  int spaces;

  for (spaces = 0; spaces < height; spaces++) printf("\n");
}

void sysclr() { // clears screen
  system("clear");
}

void animateIn(char header[20], int lines) { // animates screen frame
  int space;
  lines = lines - 3; // computes for the lines needed

  for (space = (lines % 2 == 0 ? 0 : 1); space < lines; space += 2) {
    sysclr();
    spacing(((defrow - space) / 2) - 2);
    headerTitle(header);
    spacing(space + 1);
    printBar(defcol);
    spacing(((defrow - space) / 2) - 2);
    usleep(15000);
  }
  sysclr();
}

void animateOut(char header[20], int lines) { // animates screen frame out
  int space;
  lines = lines - 3;

  for (space = (lines % 2 == 0 ? lines : lines - 1); space != 0; space -= 2) {
    sysclr();
    spacing(((defrow - space) / 2) - 2);
    headerTitle(header);
    spacing(space - 1);
    printBar(defcol);
    spacing(((defrow - space) / 2) - 1);
    usleep(15000);
  }
  sysclr();
}

void formattingDef(int lines, int isHeader) { // formatting defaults
  int space;
  lines = (defrow - lines - 3) / 2;

  spacing(lines);
  if (!isHeader) printBar(defcol); // prints bar if for footer
}

void popmsg(int isError, char body[70]) { // error/notice messages
  if (isError) animateIn("ERROR", 6);
  else animateIn("NOTICE", 6);
  spacing(8);
  if (isError) headerTitle("ERROR");
  else headerTitle("NOTICE");
  spacing(1);
  margin(strlen(body), 0);
  if (isError) printf(r "%s\n" q, body);
  else printf(g "%s\n" q, body);
  spacing(1);
  printBar(defcol);
  spacing(8);
  sleep(2);
  if (isError) animateOut("ERROR", 6);
  else animateOut("NOTICE", 6);
}
