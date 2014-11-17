
// DEMONSTRATE HOW TO DRAW AN 'X' IN FLTK
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>

#include <vector>
#include <string>
#include <algorithm>

struct Rect {
	float x, y, w, h;
	Rect( float x_, float y_, float w_, float h_ ) { x=x_;y=y_;w=w_;h=h_; }
	bool contains_point( float x, float y )
	{
		return x > this->x && x < this->x+w &&
			   y > this->y && y < this->y+h;
	}
};

const int NODE_TITLE_FONT_SIZE = 14;
const int NODE_TITLE_Y_OFFSET = -5;
const int INOUTS_FONT_SIZE = 12;
const int INOUTS_X_OFFSET = 5;
const int INOUTS_Y_OFFSET = 17;
const int INOUTS_Y_SPACING = 20;

struct Connection
{
	enum Type { INPUT, OUTPUT } type;
	std::string name;
	float offx, offy;
};

class Node {
public:

	Rect r;
	std::string pretty;
	std::vector<Connection> ins;
	std::vector<Connection> outs;

	void ConfigHeight()
	{
		r.h = std::max(ins.size(), outs.size() * 15 + 50);
	}

	Node( float x, float y, float w, float h, const std::string& pretty_ )
		: r(x,y,w,h), pretty(pretty_) {}

	void AddInput( std::string name )
	{
		Connection c;
		c.type = Connection::Type::INPUT;
		c.name = name;
		c.offx = -5;
		c.offy = INOUTS_Y_OFFSET - 7 + INOUTS_Y_SPACING * ins.size();
	}

	void AddOutput( std::string name )
	{
		Connection c;
		c.type = Connection::Type::OUTPUT;
		c.name = name;
		c.offx = r.w;
		c.offy = INOUTS_Y_OFFSET - 7 + INOUTS_Y_SPACING * outs.size();
	}

	const std::string& String()
	{
		return pretty;
	}

};

class Link
{
public:
	Link(Node ni, Node no, int ii, int io)
		: node_in(ni), node_out(no), index_in(ii), index_out(io) {}
	Node node_in, node_out;
	int index_in, index_out;
};


class NodeFactory {
private:
	std::vector<Node*> nodes;

public:
	~NodeFactory()
	{
		for( int i = 0; i < nodes.size(); i++ )
		{
			delete nodes[i];
		}
	}
	Node* CreateDumbNode( int x, int y )
	{
		Node* n = NULL;
		nodes.push_back(n);
		return n;
	}
};



// SIMPLE WIDGET THAT DRAWS AN 'X'
class DrawX : public Fl_Widget {

private:
	enum SelectionState { NODE, LINK } selection_state;
	std::vector<Node> nodes;
	std::vector<Link> links;
	bool clicked;
	float offx, offy, prevx, prevy;
	Node* selected;
	int selected_in, dropped_out;


public:
    DrawX(int X, int Y, int W, int H, const char*L=0) : Fl_Widget(X,Y,W,H,L) {
    }
    void draw() {
        // DRAW BLACK 'X'
        fl_color(FL_BLACK);
        int x1 = x(),       y1 = y();
        int x2 = x()+w()-1, y2 = y()+h()-1;
        fl_line(x1, y1, x2, y2);
        fl_line(x1, y2, x2, y1);
    }
};
int main() {
    Fl_Double_Window win(200,200,"Draw X");
    DrawX draw_x(0, 0, win.w(), win.h());
    win.resizable(draw_x);
    win.show();
    return(Fl::run());
}

