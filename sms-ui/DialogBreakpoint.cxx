// generated by Fast Light User Interface Designer (fluid) version 1.0303

#include "DialogBreakpoint.h"
#if 1
#endif

/**
   Dialog constructor
*/
DialogBreakpoints::DialogBreakpoints() {
  { windowDialog = new Fl_Double_Window(361, 214, "Breakpoints");
    windowDialog->user_data((void*)(this));
    { Fl_Group* o = new Fl_Group(10, 100, 340, 95, "New breakpoint");
      o->deactivate();
      { textAddress = new Fl_Input(65, 115, 80, 25, "Address");
        textAddress->textfont(13);
      } // Fl_Input* textAddress
      { checkPc = new Fl_Check_Button(175, 115, 25, 25, "PC");
        checkPc->down_box(FL_DOWN_BOX);
        checkPc->align(Fl_Align(FL_ALIGN_LEFT));
      } // Fl_Check_Button* checkPc
      { checkRead = new Fl_Check_Button(235, 115, 25, 25, "Read");
        checkRead->down_box(FL_DOWN_BOX);
        checkRead->align(Fl_Align(FL_ALIGN_LEFT));
      } // Fl_Check_Button* checkRead
      { checkWrite = new Fl_Check_Button(295, 115, 25, 25, "Write");
        checkWrite->down_box(FL_DOWN_BOX);
        checkWrite->align(Fl_Align(FL_ALIGN_LEFT));
      } // Fl_Check_Button* checkWrite
      { textNote = new Fl_Input(65, 140, 260, 25, "Note");
        textNote->textfont(13);
      } // Fl_Input* textNote
      { buttonCreate = new Fl_Button(10, 170, 80, 25, "Create");
      } // Fl_Button* buttonCreate
      { buttonRemove = new Fl_Button(90, 170, 80, 25, "Remove");
      } // Fl_Button* buttonRemove
      o->end();
    } // Fl_Group* o
    { Fl_Group* o = new Fl_Group(15, 20, 350, 55, "Common breakpoints");
      { checkVdpcontrol = new Fl_Check_Button(165, 50, 25, 25, "VDP Control");
        checkVdpcontrol->down_box(FL_DOWN_BOX);
        checkVdpcontrol->align(Fl_Align(FL_ALIGN_LEFT));
      } // Fl_Check_Button* checkVdpcontrol
      { checkPsgwrite = new Fl_Check_Button(265, 50, 25, 25, "PSG Write");
        checkPsgwrite->down_box(FL_DOWN_BOX);
        checkPsgwrite->align(Fl_Align(FL_ALIGN_LEFT));
      } // Fl_Check_Button* checkPsgwrite
      { checkVdpread = new Fl_Check_Button(165, 25, 25, 25, "VDP Read");
        checkVdpread->down_box(FL_DOWN_BOX);
        checkVdpread->align(Fl_Align(FL_ALIGN_LEFT));
      } // Fl_Check_Button* checkVdpread
      { checkVdpwrite = new Fl_Check_Button(265, 25, 25, 25, "VDP Write");
        checkVdpwrite->down_box(FL_DOWN_BOX);
        checkVdpwrite->align(Fl_Align(FL_ALIGN_LEFT));
      } // Fl_Check_Button* checkVdpwrite
      o->end();
    } // Fl_Group* o
    windowDialog->end();
  } // Fl_Double_Window* windowDialog
  //Initial values
  textAddress->value("0x0000");
  textNote->value("Pointless pointer breakpoint");
}

void DialogBreakpoints::set_breakpoint_table(uint8_t* p) {
  this->bp_table = p;
}