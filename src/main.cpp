
#include "node.h"
#include "editor.h"

int main() {
    Fl_Double_Window win(200,200,"Draw X");
    DrawX draw_x(0, 0, win.w(), win.h());
    win.resizable(draw_x);
    win.show();
    return(Fl::run());
}

