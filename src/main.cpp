
#include "node.h"
#include "editor.h"

int main() {
    Fl_Double_Window win(200,200,"Node editor");
    NodeEditor node_editor(0, 0, win.w(), win.h());
    win.resizable(node_editor);
    win.show();
    return(Fl::run());
}

