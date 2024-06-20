#ifndef WINCONTROLLER
#define WINCONTROLLER

#include "activewindow.hpp"
#include <string>
#include <map>

class WinController {
public:
  WinController(ActiveWindow window);
  
  std::string get_motion(char direction); 
  void look(char direction);
  void move(char direction);
  
  ActiveWindow window;
  int boarder;
  std::map<std::string, std::string> look_commands;
  std::map<std::string, std::string> move_commands;
  
};

#endif //WINCONTROLLER
