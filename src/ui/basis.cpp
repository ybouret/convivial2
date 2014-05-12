#include "./basis.hpp"
#include "FL/Fl_Box.H"
#include "FL/Fl_Input.H"

uiBasis:: ~uiBasis() throw()
{
}


uiBasis:: uiBasis( Fl_Group &tabs, const BasisInfo  &bi ) :
name( bi.name ),
group( new Fl_Group( tabs.x(), tabs.y()+24, tabs.w(), tabs.h()-24, bi.name.c_str() ) ),
input()
{
    //--------------------------------------------------------------------------
    // record the new tab
    //--------------------------------------------------------------------------
    tabs.add(group);
    group->labelfont( FL_COURIER );
    group->labelsize( 14 );
  
    
    
    static const int bh = 23;
    
    //--------------------------------------------------------------------------
    // create the parameters
    //--------------------------------------------------------------------------
    const int    h     = group->h();
    const size_t nb    = bi.param.size();
    const int    hfree = h - nb * bh;
    const int    hskip = hfree/(nb+1);
    
    int       y = group->y();
    const int x = group->x() + 8;

    for( size_t i=1; i <= nb; ++i )
    {
        y += hskip;
        const string &param = bi.param[i];
        Fl_Input *box = new Fl_Input(x,y,100,bh,param.c_str());
        box->labeltype( FL_NORMAL_LABEL );
        box->align(FL_ALIGN_RIGHT);
        box->labelfont(FL_HELVETICA);
        box->labelsize(12);
        box->textsize(12);
        box->textfont(FL_HELVETICA);
        group->add(box);
        box->value("0.0");
        y += bh;
    }
    
    
    
}

const string & uiBasis:: key() const throw()
{
    return name;
}

