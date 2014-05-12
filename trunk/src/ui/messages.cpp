#include "messages.hpp"

#include <FL/fl_ask.H>

void PrepareMessage( const char *title )
{
    fl_message_title(title);
    fl_message_font(FL_HELVETICA_BOLD, 12);
}

bool Confirm( const char *text)
{
    PrepareMessage("Confirmation requested");
    return 1 == fl_ask("%s",text);
}


void DisplayException( const exception &e )
{
    PrepareMessage("Error");
    fl_alert("%s:\n%s", e.what(),e.when());
}