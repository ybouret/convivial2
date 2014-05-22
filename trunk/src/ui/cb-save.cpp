#include "convivial.h"
#include "FL/fl_message.H"

#include "yocto/fs/local-fs.hpp"
#include <FL/Fl_File_Chooser.H>

void SaveFile()
{
    try
    {
        vfs &fs = local_fs::instance();
        
        const char *current_file_name = FileName->value();
        
        const char *file_name = fl_file_chooser("Choose File To Save", 0, current_file_name,1);
        if(!file_name)
            return;
        
        const string filename = file_name;
        vfs::entry   ep(filename,fs);
        if(ep.attr==ep.is_reg)
        {
            PrepareMessage("Confirmation needed");
            if( !Confirm("File aready exits: overwrite?") )
                return;
        }
        App->save(filename);
        FileName->value(filename.c_str());
        SetClean();
        
    }
    catch(const exception &e )
    {
        PrepareMessage("Save File Error");
        DisplayException(e);
    }
    catch(...)
    {
        PrepareMessage("Save File Unhandled Error");
        fl_message("Something bad happened...");
    }
}
