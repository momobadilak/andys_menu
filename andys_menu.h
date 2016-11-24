/*!
  @file     andys_menu.h
  @author   Andy Idsinga
  @license  MIT (see LICENSE.txt)
  
  How to create and execute a menu:
  1) declare an array of menu items in your code. ( Note that the 'x' key is
  reserved for an 'exit' menu item that is provided by the library.
  
  ...
  
  2) call AndysMenu::run(...);
  
  You can create several menu arrays, and create sub-menus by 
  calling AndyMenu::run() from one of your menu handlers
  
  How to create an execute a prompt:
  1) declare an array of prompt items in your code
  2) call AndysMenu::prompt(...);
  3) the selected prompt character is returned to you

*/
#ifndef ANDYS_MENU_H
#define ANDYS_MENU_H

#include "Arduino.h"

typedef void (*andyMenuErrorFn)(const uint8_t code, const char* msg);
typedef void (*andyMenuHandler)(void* userContext);

struct AndysMenuItem
{
  const char            itemKeyChar;
  const char*           itemDescString;
  andyMenuHandler       itemHandlerFn;
};

struct AndysPromptItem
{
  const char            itemKeyChar;
  const char*           itemDescString;
};

class AndysMenu
{
 public:
  AndysMenu();
  void setup(uint32_t serialSpeed = 9600);
  void run( const char* menuTitle,
	    const AndysMenuItem  *items,
	    uint8_t numItems,
	    void* userContext,
	    andyMenuErrorFn errorFn);
  
  char prompt( const char* promptMsg,
	       const AndysPromptItem  *items,
	       uint8_t numItems);
};

#endif //ANDYS_MENU_H
