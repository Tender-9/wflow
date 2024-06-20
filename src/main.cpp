#include "wincontroller.hpp"
#include "activewindow.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
  
  if (argc != 3) {
    std::cout << "Usage: wflow <mode> <direction>" << std::endl;
    return 1;
  }
  
  std::string mode = argv[1];
  char dir = argv[2][0];
  

  if (mode != "move" && mode != "look"){
    std::cout << "Error: \"" << mode << "\" is not a valid mode" << std::endl;
    return 1;
  }
  if (dir != 'u' && dir != 'd' && dir != 'l' && dir != 'r') {
    std::cout << "Error: \"" << dir << "\" is not a valid direction" << std::endl;
    return 1;
  }

  ActiveWindow win = ActiveWindow();
  WinController controller = WinController(win);

  if (mode == "look") {
    try {
      controller.look(dir);
    }
    catch (std::runtime_error& e){
      std::cout << '\a' << e.what() << std::endl;
    }
  }
  else if (mode == "move") {
    try {
      controller.move(dir);
    }
    catch (std::runtime_error& e) {
      std::cout << '\a' << e.what() << std::endl;
    }
  }
  return 0; 
}
