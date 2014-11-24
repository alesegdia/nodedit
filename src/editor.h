
#pragma once

#include "node.h"

// SIMPLE WIDGET THAT DRAWS AN 'X'
class NodeEditor : public Fl_Widget {

private:
	NodeFactory nf;
	enum SelectionState { NODE, LINK } selection_state;
	std::vector<Link::Logic> links;
	bool clicked;
	float offx, offy, prevx, prevy;
	Node* selected;
	Link::Logic* selectedLink;
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
					Rect r = n->GetInRect(j,40);
					if( r.contains_point( x - offx, y - offy ) )
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
					Rect r = n->GetOutRect(j,40);
					if( r.contains_point( x - offx, y - offy ) )
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
    NodeEditor(int X, int Y, int W, int H, const char*L=0) : Fl_Widget(X,Y,W,H,L)
    {
    	selected = NULL;
    	offx = offy = 0;
    	Node* n1 = nf.CreateDumbNode(0,100);
    	Node* n2 = nf.CreateDumbNode(400,100);
    	links.push_back( Link::Logic( n1, n2, 1, 1 ) );
		//links.push_back(Link::Logic(n1,n2,2,2));
		//links.push_back(Link::Logic(n1,n2,4,6));
		//links.push_back(Link::Logic(n1,n2,3,4));
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
