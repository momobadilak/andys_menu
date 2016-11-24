/*
  file: andys_menu/example.ino

  Author: Andy Idsinga ( https://twitter.com/andyidsinga )

  The library has been tested with Arduino UNO and Micro boards

  Before I pushed to github I tested built and ran this code
  with Arduino 1.6.12 and an Arduino Micro board https://www.arduino.cc/en/Main/ArduinoBoardMicro

*/

/* include the library -- which should have been previously copied to 
   your Arduino libraries folder */
#include <andys_menu.h>


/* create an instance of the menu class */
AndysMenu theMenu;


/* define a top level menu

   format is
   menu character, menu description string, C function handler */

void topmenu_HelloWorld(void* context);
void topmenu_SubMenu(void* context);
void topmenu_Ok(void* context);
void topmenu_YesNo(void* context);

AndysMenuItem topMenu[] = {
  {'h', "say hello",      topmenu_HelloWorld },
  {'s', "go to sub-menu", topmenu_SubMenu    },
  {'o', "OK prompt",      topmenu_Ok         },
  {'y', "Yes/No prompt",  topmenu_YesNo      },
};

/* define a a sub-menu */
void submenu_Yay(void* context);
AndysMenuItem subMenu[] = {
  {'y', "say yay", submenu_Yay }
};

/* 
   PROMPT MENUS
   These can reused throughout the code where a prompt is required
*/
AndysPromptItem okPrompt[] = {
  {'o', "ok"}
};

AndysPromptItem yesNoPrompt[] = {
  {'y', "Yes"},
  {'n', "No"}
};

/* define an error handler that will be called from the menu code 
   ..wait for it ...if an error occurs :) */
void menu_ErrorHandler(const uint8_t code, const char* msg);

/*
  should look familiar - the arduino setup function - called once at device boot
 */
void setup(){
  /* setup menu object */
  theMenu.setup(115200);
}

/*
  should look familiar - the arduino loop function - called over and over
 */
void loop(){
  /* run the top level menu.
     Note: I'm not passing anything for the context arg, but
     that can be your goodies, and will not be touched by the menu code. */
  theMenu.run(
    "=== Example Top Menu ===",
    topMenu, 
    sizeof(topMenu)/sizeof(AndysMenuItem), 
    (void*)0,
    menu_ErrorHandler);
}


void topmenu_HelloWorld(void* context){
  //Serial.print("+,"); Serial.println(__FUNCTION__);

  Serial.println("Hello menu world!");
  
  //Serial.print("-,"); Serial.println(__FUNCTION__);
}

void topmenu_Ok(void* context){
  //Serial.print("+,"); Serial.println(__FUNCTION__);

  /* run the OK prompt and report input*/
  char promptCh = 0;

  promptCh = theMenu.prompt("Hit ok when ready to do debug the thing...",
                         okPrompt,
                         sizeof(okPrompt)/sizeof(AndysPromptItem));
  if(promptCh == 'o') {
    Serial.println("you hit ok, the example code thanks you");
  }
  
  //Serial.print("-,"); Serial.println(__FUNCTION__);
}

void topmenu_YesNo(void* context){
  //Serial.print("+,"); Serial.println(__FUNCTION__);

  /* run the YesNo prompt and report input*/
  char promptCh = 0;

  promptCh = theMenu.prompt("Is everything totally cool?",
                         yesNoPrompt,
                         sizeof(yesNoPrompt)/sizeof(AndysPromptItem));
  if(promptCh == 'y') {
    Serial.println("cool, everything is ok");
  }
  else {
    Serial.println("bummer, things aren't right");
  }
  
  //Serial.print("-,"); Serial.println(__FUNCTION__);
}

void topmenu_SubMenu(void* context){
  //Serial.print("+,"); Serial.println(__FUNCTION__);

  /* run our predefined submenu */
  theMenu.run(
    "=== Example Sub Menu ===",
    subMenu, 
    sizeof(subMenu)/sizeof(AndysMenuItem), 
    (void*)0,
    menu_ErrorHandler);
  
  //Serial.print("-,"); Serial.println(__FUNCTION__);
}

void submenu_Yay(void* context){

  //Serial.print("+,"); Serial.println(__FUNCTION__);

  Serial.println("YAY - the submenu works!");
  
  //Serial.print("-,"); Serial.println(__FUNCTION__);
}

/*
  generic menu error handler - error code and message dumper
*/
void menu_ErrorHandler(const uint8_t code, const char* msg)
{
  Serial.print("+menu_ErrorHandler,");
  Serial.print("code,"); 
  Serial.print(code,HEX);
  Serial.print(",msg,"); 
  Serial.println(msg);
}
