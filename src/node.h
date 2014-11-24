
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

class Node;


class Link
{

public:

	class Logic
	{
	public:
		Logic(Node* ni, Node* no, int ii, int io)
			: node_in(ni), node_out(no), index_in(ii), index_out(io) {}
		Node *node_in, *node_out;
		int index_in, index_out;
	};

	Logic* logic;

	Link()
	{
		logic = NULL;
	}

	enum Type { INPUT, OUTPUT } type;
	std::string name;
	float offx, offy;
};

class Node {
public:

	Rect r;
	std::string pretty;
	std::vector<Link> ins;
	std::vector<Link> outs;

	void Disconnect( int in_index )
	{
		// if is connected
		Link::Logic *logic = ins[in_index].logic;
		if( logic != NULL )
		{
			logic->node_out->outs[logic->index_out].logic = NULL;
			delete logic;
			ins[in_index].logic = NULL;
		}
	}

	void Connect(int in_index, Node* onode, int out_index)
	{
		if( ins[in_index].logic == NULL )
		{
			Link::Logic *logic = new Link::Logic( this, onode, in_index, out_index );
			ins[in_index].logic = logic;
			onode->outs[out_index].logic = logic;
		}
	}

	void ConfigHeight()
	{
		r.h = std::max(ins.size() * 15 + 60, outs.size() * 15 + 60);
	}

	Node( float x, float y, float w, float h, const std::string& pretty_ )
		: r(x,y,w,h), pretty(pretty_) {}

	void AddInput( std::string name )
	{
		Link c;
		c.type = Link::Type::INPUT;
		c.name = name;
		c.offx = -5;
		c.offy = INOUTS_Y_OFFSET - 7 + INOUTS_Y_SPACING * ins.size();
		ins.push_back(c);
	}

	void AddOutput( std::string name )
	{
		Link c;
		c.type = Link::Type::OUTPUT;
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
		Link output = outs[index];
		int wext, hext;
		textExtents( output.name.c_str(), wext, hext );
		Rect rc = GetOutRect( index );
		fl_rectf( rc.x + offx, rc.y + offy, rc.w, rc.h, FL_BLUE );
		fl_color(FL_YELLOW);
		fl_draw(output.name.c_str(), rc.x - wext + offx, rc.y + 6 + offy);
	}
	void RenderIn( int index, float offx, float offy )
	{
		Link output = ins[index];
		Rect rc = GetInRect( index );
		fl_rectf( rc.x + offx, rc.y + offy, rc.w, rc.h, FL_BLUE );
		fl_color(FL_YELLOW);
		fl_draw(output.name.c_str(), offx + rc.x + 10, offy + rc.y + 6);
	}

	Rect GetOutRect( int index, int k = 0 )
	{
		// check index valid?
		Link o = outs[index];
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
		Link o = ins[index];
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


