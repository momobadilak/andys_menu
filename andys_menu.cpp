/**************************************************************************/
/*!
  @file	    andys_menu.cpp
  @author   Andy Idsinga
  @license  MIT (see LICENSE)
*/

#include "andys_menu.h"

AndysMenu::AndysMenu() {
  //nothing to do here yet
}

void AndysMenu::setup(uint32_t serialSpeed) {
  Serial.begin(serialSpeed);
}

void AndysMenu::run (
    const char* menuTitle,
    const AndysMenuItem *items,
    uint8_t numItems,
    void* userContext,
    andyMenuErrorFn errorFn )
{
  //Serial.println("+AndysMenu::run");

  uint8_t menuState = 0;
  while(menuState != 3) {
    
    switch(menuState) {
      /* display menu */
      case 0:
	Serial.println(menuTitle);
	for (uint8_t x = 0; x < numItems; x++){
	  Serial.print("[");
	  Serial.print((items + x)->itemKeyChar);
	  Serial.print("]:");
	  Serial.println((items + x)->itemDescString);
	}

	/* wait for a menu key and call the handler if it exists */
	Serial.println("[x]:exit");
	Serial.print("$ ");
	menuState = 1;
      break;

    /* read input state*/
    case 1:
      {
	int userChar = Serial.read();
	if(userChar != -1 && userChar != 0x0D && userChar != 0x0A){
	  /* echo back to user */
	  Serial.print((char)userChar);
	  //Serial.print(userChar,HEX);
	  Serial.println("");
	  if(userChar == 'x') {
	    menuState = 2;
	  }
	  else {
	    /* process check to see if valid menu character and exec
	       handler function */
	    bool handled = false;
	    for (uint8_t x = 0; x < numItems; x++) {
	      const AndysMenuItem* currItem = (items + x);
	      if(currItem->itemKeyChar == (char)userChar) {
		currItem->itemHandlerFn(userContext);
		handled = true;
		break;
	      }
	    }
	    if(handled == false) {
	      Serial.println("unknown item, please try again.");
	    }
	    /* switch back to displaying the menu */
	    menuState = 0;
	  }
	}
      }
      break;

    case 2:
      Serial.println("exiting...");
      menuState = 3;
      break;

    default:
      /* call error handler ! */
      errorFn(1,"weirdness, unknown menu state!");
      break;
    }
  }

  //Serial.println("-AndysMenu::run");
}

char AndysMenu::prompt(
    const char* promptMsg,
    const AndysPromptItem  *items,
    uint8_t numItems) {
  char retVal = 0x0;

  uint8_t menuState = 0;
  while(menuState != 2){
    switch(menuState){
      /* display menu */
      case 0:
	Serial.println(promptMsg);
	for (uint8_t x = 0; x < numItems; x++){
	  Serial.print("[");
	  Serial.print((items + x)->itemKeyChar);
	  Serial.print("]:");
	  Serial.println((items + x)->itemDescString);
	}

	/* wait for a menu key and call the handler if it exists */
	Serial.print("$ ");
	menuState = 1;
      break;

    /* read input state*/
    case 1:
      {
	int userChar = Serial.read();
	if(userChar != -1 && userChar != 0x0D && userChar != 0x0A){

	  /* echo back to user */
	  Serial.print((char)userChar);
	  //Serial.print(userChar,HEX);

	  Serial.println("");

	  /* process check to see if valid menu character and exec
	     handler function */
	  bool handled = false;
	  for (uint8_t x = 0; x < numItems; x++){
	    const AndysPromptItem* currItem = (items + x);
	    if(currItem->itemKeyChar == (char)userChar){
	      retVal = currItem->itemKeyChar;
	      handled = true;
	      break;
	    }
	  }
	  if(handled == false) {
	    Serial.println("unknown item, please try again.");
	    /* displaying the menu again */
	    menuState = 0;
	  }
	  else {
	    //user selected one of the options ...exit
	    menuState = 2;
	  }
	}
      }
      break;

    default:
      Serial.println("weirdness, unknown prompt menu state!");
      break;
    }
  }

  return retVal;
}
