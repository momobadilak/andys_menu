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

void AndysMenu::setup(uint32_t serialSpeed, HardwareSerial* altSerial) {

  if(altSerial != NULL){
    mpSerial = altSerial;
  }
  else {
    mpSerial = (HardwareSerial*)&Serial;
  }
  
  mpSerial->begin(serialSpeed);
}

void AndysMenu::run (
    const char* menuTitle,
    const AndysMenuItem *items,
    uint8_t numItems,
    void* userContext,
    andyMenuErrorFn errorFn )
{
  //mpSerial->println("+AndysMenu::run");

  uint8_t menuState = 0;
  while(menuState != 3) {
    
    switch(menuState) {
      /* display menu */
      case 0:
	mpSerial->println(menuTitle);
	for (uint8_t x = 0; x < numItems; x++){
	  mpSerial->print("[");
	  mpSerial->print((items + x)->itemKeyChar);
	  mpSerial->print("]:");
	  mpSerial->println((items + x)->itemDescString);
	}

	/* wait for a menu key and call the handler if it exists */
	mpSerial->println("[x]:exit");
	mpSerial->print("$ ");
	menuState = 1;
      break;

    /* read input state*/
    case 1:
      {
	int userChar = mpSerial->read();
	if(userChar != -1 && userChar != 0x0D && userChar != 0x0A){
	  /* echo back to user */
	  mpSerial->print((char)userChar);
	  //mpSerial->print(userChar,HEX);
	  mpSerial->println("");
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
	      mpSerial->println("unknown item, please try again.");
	    }
	    /* switch back to displaying the menu */
	    menuState = 0;
	  }
	}
      }
      break;

    case 2:
      mpSerial->println("exiting...");
      menuState = 3;
      break;

    default:
      /* call error handler ! */
      errorFn(1,"weirdness, unknown menu state!");
      break;
    }
  }

  //mpSerial->println("-AndysMenu::run");
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
	mpSerial->println(promptMsg);
	for (uint8_t x = 0; x < numItems; x++){
	  mpSerial->print("[");
	  mpSerial->print((items + x)->itemKeyChar);
	  mpSerial->print("]:");
	  mpSerial->println((items + x)->itemDescString);
	}

	/* wait for a menu key and call the handler if it exists */
	mpSerial->print("$ ");
	menuState = 1;
      break;

    /* read input state*/
    case 1:
      {
	int userChar = mpSerial->read();
	if(userChar != -1 && userChar != 0x0D && userChar != 0x0A){

	  /* echo back to user */
	  mpSerial->print((char)userChar);
	  //mpSerial->print(userChar,HEX);

	  mpSerial->println("");

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
	    mpSerial->println("unknown item, please try again.");
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
      mpSerial->println("weirdness, unknown prompt menu state!");
      break;
    }
  }

  return retVal;
}
