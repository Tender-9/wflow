#ifndef WINCONTROLLER
#define WINCONTROLLER

#include "activewindow.hpp"
#include <string>
#include <map>

class WinController {
public:
  WinController(ActiveWindow window);
  
  std::string get_motion(std::string direction); 
  void look(std::string direction);
  void move(std::string direction);
  
  ActiveWindow window;
  int boarder;
  std::map<std::string, std::string> look_commands;
  std::map<std::string, std::string> move_commands;
  
};

#endif //WINCONTROLLER
