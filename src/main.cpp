
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

int textExtents( const char* text, int& w, int& h )
{
	int dx, dy;
	fl_text_extents(text, dx, dy, w, h);
}

const int NODE_TITLE_FONT_SIZE = 14;
const int NODE_TITLE_Y_OFFSET = -5;
const int INOUTS_FONT_SIZE = 12;
const int INOUTS_X_OFFSET = 5;
const int INOUTS_Y_OFFSET = 17;
const int INOUTS_Y_SPACING = 10;
const int INOUTS_SIZE = 5;

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
		r.h = std::max(ins.size() * 15 + 60, outs.size() * 15 + 60);
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
		ins.push_back(c);
	}

	void AddOutput( std::string name )
	{
		Connection c;
		c.type = Connection::Type::OUTPUT;
		c.name = name;
		c.offx = r.w;
		c.offy = INOUTS_Y_OFFSET - 7 + INOUTS_Y_SPACING * outs.size();
		outs.push_back(c);
	}


	void RenderNode(float offx, float offy)
	{
		float nx = r.x + offx;
		float ny = r.y + offy;
		//fl_rectf( nx+5, ny-5, n->r.w, n->r.h);
		//fl_rectf( nx, ny, n->r.w, n->r.h, FL_RED);
		fl_color(128, 0, 0);
		fl_draw_box( FL_RFLAT_BOX, nx+5, ny-5, r.w, r.h, FL_DARK_RED);
		fl_draw_box( FL_RFLAT_BOX, nx, ny, r.w, r.h, FL_RED);

		for( int i = 0; i < ins.size(); i++ )
		{
			RenderIn(i, offx, offy);
		}
		for( int i = 0; i < outs.size(); i++ )
		{
			RenderOut( i, offx, offy );
		}
	}

	void RenderOut( int index, float offx, float offy )
	{
		Connection output = outs[index];
		int wext, hext;
		textExtents( output.name.c_str(), wext, hext );
		Rect rc = GetOutRect( index );
		fl_rectf( rc.x + offx, rc.y + offy, rc.w, rc.h, FL_BLUE );
		fl_color(FL_YELLOW);
		fl_draw(output.name.c_str(), rc.x - wext + offx, rc.y + 6 + offy);
	}
	void RenderIn( int index, float offx, float offy )
	{
		Connection output = ins[index];
		Rect rc = GetInRect( index );
		fl_rectf( rc.x + offx, rc.y + offy, rc.w, rc.h, FL_BLUE );
		fl_color(FL_YELLOW);
		fl_draw(output.name.c_str(), offx + rc.x + 10, offy + rc.y + 6);
	}

	Rect GetOutRect( int index, int k = 0 )
	{
		// check index valid?
		Connection o = outs[index];
		float hk = float(k) / 2.f;
		return Rect(
			r.x + INOUTS_X_OFFSET + o.offx - 5 - hk,
			r.y + INOUTS_Y_OFFSET + INOUTS_Y_SPACING * index + o.offy - hk,
			INOUTS_SIZE + k, INOUTS_SIZE + k
		);
	}

	Rect GetInRect( int index, int k = 0 )
	{
		// check index valid?
		Connection o = ins[index];
		float hk = float(k) / 2.f;
		return Rect(
			r.x + INOUTS_X_OFFSET + o.offx - 5 - hk,
			r.y + INOUTS_Y_OFFSET + INOUTS_Y_SPACING * index + o.offy - hk,
			INOUTS_SIZE + k, INOUTS_SIZE + k
		);
	}

	const std::string& String()
	{
		return pretty;
	}

};

class Link
{
public:
	Link(Node* ni, Node* no, int ii, int io)
		: node_in(ni), node_out(no), index_in(ii), index_out(io) {}
	Node *node_in, *node_out;
	int index_in, index_out;
};


class NodeFactory
{
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
	std::vector<Node*>& NodeList()
	{
		return nodes;
	}
	Node* CreateDumbNode( int x, int y )
	{
		Node* n = new Node( x, y, 120, 100, "DumbNode" );
		nodes.push_back(n);
		n->AddInput("in1");
		/*
		n->AddInput("in2");
		n->AddInput("in3");
		n->AddInput("in4");
		n->AddInput("in5");
		n->AddInput("in6");
		n->AddOutput("out2");
		n->AddOutput("out3");
		n->AddOutput("out4");
		*/
		n->AddOutput("out1");
		n->ConfigHeight();
		return n;
	}
};



// SIMPLE WIDGET THAT DRAWS AN 'X'
class DrawX : public Fl_Widget {

private:
	NodeFactory nf;
	enum SelectionState { NODE, LINK } selection_state;
	std::vector<Link> links;
	bool clicked;
	float offx, offy, prevx, prevy;
	Node* selected;
	Link* selectedLink;
	int selected_in, dropped_out;


	int onDrag()
	{
		int x = Fl::event_x();
		int y = Fl::event_y();

		if( !clicked )
		{
			bool got_one = false;
			for( int i = 0; i < nf.NodeList().size(); i++ )
			{
				Node* n = nf.NodeList()[i];
				float nx = n->r.x + offx;
				float ny = n->r.y + offy;
				const int s = 40;
				for( int j = 0; j < n->ins.size(); j++ )
				{
					Rect r = n->GetInRect(j,10);
					r.x += offx;
					r.y += offy;
					printf("in %d: %f, %f, %f, %f\n", j, r.x, r.y, r.w, r.h);
					fflush(0);
					if( r.contains_point( x + offx, y + offy ) )
					{
						got_one = true;
						printf("EUREKA!");
						selected = nf.NodeList()[i];
						prevx = x - selected->r.x;
						prevy = y - selected->r.y;
					}
				}
				for( int j = 0; j < n->outs.size(); j++ )
				{
					Rect r = n->GetOutRect(j,10);
					r.x += offx;
					r.y += offy;
					printf("out %d: %f, %f, %f, %f\n", j, r.x, r.y, r.w, r.h);
					fflush(0);
					if( r.contains_point( x + offx, y + offy ) )
					{
						got_one = true;
						selected = nf.NodeList()[i];
						prevx = x - selected->r.x;
						prevy = y - selected->r.y;
						printf("EUREKA!");
					}
				}
				if(!got_one && nf.NodeList()[i]->r.contains_point( x - offx, y - offy ))
				{
					got_one = true;
					selected = nf.NodeList()[i];
					prevx = x - selected->r.x;
					prevy = y - selected->r.y;
				}
			}
			if( !got_one )
			{
				selected = NULL;
				prevx = x - offx;
				prevy = y - offy;
			}
			clicked = true;
		}

		if( selected == NULL )
		{
			offx = x - prevx;
			offy = y - prevy;
		}
		else
		{
			selected->r.x = x - prevx;
			selected->r.y = y - prevy;
		}

		redraw();
		return 0;
	}

	int onRelease()
	{
		clicked = false;
		return 0;
	}


public:
    DrawX(int X, int Y, int W, int H, const char*L=0) : Fl_Widget(X,Y,W,H,L)
    {
    	selected = NULL;
    	offx = offy = 0;
    	Node* n1 = nf.CreateDumbNode(0,100);
    	Node* n2 = nf.CreateDumbNode(400,100);
		links.push_back(Link(n1,n2,1,1));
		//links.push_back(Link(n1,n2,2,2));
		//links.push_back(Link(n1,n2,4,6));
		//links.push_back(Link(n1,n2,3,4));
    }


    int handle(int event)
	{
		switch(event)
		{
		case FL_DRAG: return onDrag(); break;
		case FL_RELEASE: return onRelease(); break;
		}
	}
    void draw() {
        // DRAW BLACK 'X'
        fl_color(FL_BLACK);
        fl_rectf(0,0,w(),h());

		for( int i = 0; i < links.size(); i++ )
		{
			Node* src = links[i].node_in;
			Node* dst = links[i].node_out;
			float x0, y0, x1, y1;
			x0 = src->r.x + offx + src->r.w + 2.5; y0 = INOUTS_Y_OFFSET + src->r.y + offy + (links[i].index_in*2 -1 +0.2) * INOUTS_Y_SPACING;
			x1 = dst->r.x + offx - 2.5;            y1 = INOUTS_Y_OFFSET + dst->r.y + offy + (links[i].index_out*2 -1 +0.2) * INOUTS_Y_SPACING;
			fl_color(255,0,255);
			fl_line( x0, y0, x1, y1 );
		}

		std::vector<Node*>& nodes = nf.NodeList();
        for( int i = 0; i < nodes.size(); i++ )
		{
			Node* n = nodes[i];
			n->RenderNode(offx,offy);
		}

    }

};
int main() {
    Fl_Double_Window win(200,200,"Draw X");
    DrawX draw_x(0, 0, win.w(), win.h());
    win.resizable(draw_x);
    win.show();
    return(Fl::run());
}

