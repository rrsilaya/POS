/*
   oooo           oooo  oooo              oooooooooo.
   `888           `888  `888              `888'   `Y8b
    888  .ooooo.   888   888  oooo    ooo  888      888  .ooooo.
    888 d88' `88b  888   888   `88.  .8'   888      888 d88' `88b
    888 888   888  888   888    `88..8'    888      888 888   888
    888 888   888  888   888     `888'     888     d88' 888   888
.o. 88P `Y8bod8P' o888o o888o     .8'     o888bood8P'   `Y8bod8P'
`Y888P                        .o..P'
                              `Y8P'

Developed by Ralph Lawrence Silaya, CMSC 21 ST-5L

References:
http://patorjk.com/software/taag/#p=display&f=Roman&t=JollyDo
http://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
http://stackoverflow.com/questions/27002438/strcat-int-variable-c
http://stackoverflow.com/questions/5141960/get-the-current-time-in-c
*/

#include <stdio.h>
#include <stdlib.h>

#include "struct.h"
#include "menu.h"
#include "search.h"
#include "itemsList.h"
#include "priceList.h"
#include "encryption.h"
#include "fileHandler.h"
#include "inventoryCore.h"
#include "salesCore.h"

int main() {
  int menuChoice, itemCount = 0, orderCount = 0;
  ITEM *itemRoster = NULL, *orderRoster = NULL;
  char defFilename[50] = "inventory.txt", password[30];

  if (loadSys(password)){ // loads system file, pops error message if file not existing
    splashMenu();
    initItemNodes(&itemRoster); // initializes dummy nodes
    chkDir(); // checks if Receipts/ directory exists, otherwise executes mkdir

    while (1) { // infinite loop until exit
      choiceMenu(4, "Please choose system to open.", "Inventory\nSales\nAbout\nExit\n", &menuChoice); // choose which to use
      if (menuChoice == 1) {
        while (1) {
          inventoryMenu(&menuChoice); // shows inventory menu
          switch(menuChoice) {
            case 1:
              addItem(&itemRoster, &itemCount); // adds item to the inventory roster
              break;
            case 2:
              editItem(&itemRoster); // edits item
              break;
            case 3:
              delete(&itemRoster, &itemCount); // deletes item
              break;
            case 4:
              displayItems(itemRoster); // display items accd to options
              break;
            case 5:
              displayReceipts(); // displays receipt number provided by user
              break;
            case 6:
              computeSales(); // computes total sales using existing receipts
              break;
            case 7:
              getInventoryLoad(&itemRoster, &itemCount, defFilename); // loads inventory
              break;
            case 8:
              getInventoryWrite(itemRoster, itemCount, defFilename); // saves inventory
              break;
          }

          if (menuChoice == 9) break; // exit to main menu
        }
      } else if (menuChoice == 2 && !isEmptyList(itemRoster)) { // does not open sales system if roster is empty
        initItemNodes(&orderRoster); // initializes dummy nodes on order roster
        while (1) {
          salesMenu(&menuChoice); // prints sales menu

          switch(menuChoice) {
            case 1:
              order(&itemRoster, &orderRoster, &orderCount); // order
              break;
            case 2:
              voidItems(itemRoster, &orderRoster, password); // void items in the order list
              break;
            case 3:
              checkOut(&itemRoster, &orderRoster, orderCount, itemCount, defFilename); // check out
              break;
            case 4:
              if (orderRoster -> next -> next != NULL) { // if there are existing orders in the order roster
                choiceMenu(2, "There are existing item orders.", "Exit Anyway\nCancel\n", &menuChoice);
                if (menuChoice == 1) { // if exit anyway
                  menuChoice = 4; // reset all
                  popmsg(0, "All ordered items deleted.");
                  orderCount = 0;
                  itemCount = 0;
                }
              }
              break;
          }

          if (menuChoice == 4) { // on exit, resets all rosters
            clean(&orderRoster); // cleans order roster (deletes nodes)
            initItemNodes(&orderRoster);

            clean(&itemRoster);
            initItemNodes(&itemRoster);
            loadInventory(&itemRoster, &itemCount, defFilename);
            break;
          }
        }
      } else if (menuChoice == 3) { // displays developer credits
        credits();
      } else if (menuChoice == 4) { // exit
        clean(&itemRoster);
        popmsg(0, "Exit.");
        break;
      }
    }
  }
}
