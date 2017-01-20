#include "DialogDebug.h"
#include <string>
#include <cassert>

#include <FL/fl_ask.H>

static bool startsWith(std::string s1, std::string s2) {
    size_t min_size = (s1.length() > s2.length()) ? s1.length() : s2.length();
    return s1.compare(0, min_size, s2) == 0;
}

DialogDebugger::DialogDebugger() : _DialogDebugger() {
    
}

DialogDebugger::~DialogDebugger() {

}

void DialogDebugger::onInputCommand(Fl_Input* o, void* v) {
    std::string cmd = std::string(o->value());
    if (cmd != "") {
        this->last_cmd = cmd;
    }

    o->value("");

    ///@todo do something with the data
}

///Flag checkboxes changed.
void DialogDebugger::onFlagsChanged() {
    uint8_t f = 0;

    //Calculate the new F register
    f = this->checkSignFlag->value() ? (f << 1) | 1 : (f << 1);
    f = this->checkZeroFlag->value() ? (f << 1) | 1 : (f << 1);
    f = this->checkUnk5Flag->value() ? (f << 1) | 1 : (f << 1);
    f = this->checkHalfCarryFlag->value() ? (f << 1) | 1 : (f << 1);

    f = this->checkUnk3Flag->value() ? (f << 1) | 1 : (f << 1);
    f = this->checkParityFlag->value() ? (f << 1) | 1 : (f << 1);
    f = this->checkSubtractFlag->value() ? (f << 1) | 1 : (f << 1);
    f = this->checkCarryFlag->value()    ? (f << 1) | 1 : (f << 1);

    ///@todo do something with the data
}

void DialogDebugger::onRegistersChanged() {
    uint16_t af, afp;
    uint16_t bc, bcp;
    uint16_t de, dep;
    uint16_t hl, hlp;
    uint16_t ix, iy;
    uint16_t sp, pc;
    uint16_t ir;

    //Read the input values
    try { af = std::stoi(inputAF->value(), 0, 16); } catch(std::invalid_argument e) { af = 0; }
    try { afp = std::stoi(inputAFp->value(), 0, 16); } catch(std::invalid_argument e) { afp = 0; }
    try { bc = std::stoi(inputBC->value(), 0, 16); } catch(std::invalid_argument e) { bc = 0; }
    try { bcp = std::stoi(inputBCp->value(), 0, 16); } catch(std::invalid_argument e) { bcp = 0; }
    try { de = std::stoi(inputDE->value(), 0, 16); } catch(std::invalid_argument e) { de = 0; }
    try { dep = std::stoi(inputDEp->value(), 0, 16); } catch(std::invalid_argument e) { dep = 0; }
    try { hl = std::stoi(inputHL->value(), 0, 16); } catch(std::invalid_argument e) { hl = 0; }
    try { hlp = std::stoi(inputHLp->value(), 0, 16); } catch(std::invalid_argument e) { hlp = 0; }
    try { ix = std::stoi(inputIX->value(), 0, 16); } catch(std::invalid_argument e) { ix = 0; }
    try { iy = std::stoi(inputIY->value(), 0, 16); } catch(std::invalid_argument e) { iy = 0; }
    try { sp = std::stoi(inputSP->value(), 0, 16); } catch(std::invalid_argument e) { sp = 0; }
    try { pc = std::stoi(inputPC->value(), 0, 16); } catch(std::invalid_argument e) { pc = 0; }
    try { ir = std::stoi(inputIR->value(), 0, 16); } catch(std::invalid_argument e) { ir = 0; }

    //Write back with proper formatting
    char num_str[6];
    sprintf(num_str, "%04X", af); inputAF->value(num_str);
    sprintf(num_str, "%04X", bc); inputBC->value(num_str);
    sprintf(num_str, "%04X", de); inputDE->value(num_str);
    sprintf(num_str, "%04X", hl); inputHL->value(num_str);
    sprintf(num_str, "%04X", afp); inputAFp->value(num_str);
    sprintf(num_str, "%04X", bcp); inputBCp->value(num_str);
    sprintf(num_str, "%04X", dep); inputDEp->value(num_str);
    sprintf(num_str, "%04X", hlp); inputHLp->value(num_str);
    sprintf(num_str, "%04X", ix); inputIX->value(num_str);
    sprintf(num_str, "%04X", iy); inputIY->value(num_str);
    sprintf(num_str, "%04X", sp); inputSP->value(num_str);
    sprintf(num_str, "%04X", pc); inputPC->value(num_str);
    sprintf(num_str, "%04X", ir); inputIR->value(num_str);

    ///@todo do something with the data
}

void DialogDebugger::onAddBreakpoint() {
    bool r, w, x, io;
    uint16_t address;
    std::string description;
    std::string mode_str;
    char addr_s[5];
    std::string addr_str;

    description = std::string(inputBreakDescription->value());
    try { 
        address = std::stoi(inputBreakAddress->value(), 0, 16); 
        sprintf(addr_s, "%04X", address);
        addr_str = std::string(addr_s);

        r  = checkRead->value() ? true : false;
        w  = checkWrite->value() ? true : false;
        x  = checkExecute->value() ? true : false;
        io = checkIO->value() ? true : false;
        mode_str = mode_str.append(r ? "R" : "-");
        mode_str = mode_str.append(w ? "W" : "-");
        mode_str = mode_str.append(x ? "X" : "-");
        mode_str = mode_str.append(io ? "I" : "-");

        ///@todo Add breakpoint to list and z80
        checkBrowserBreak->add(addr_str.append(" ").append(mode_str).append(": ").append(description).c_str(), 1);
        checkBrowserBreak->redraw();

        //Clear inputs
        inputBreakDescription->value("");
        inputBreakAddress->value("");
    } 
    catch (std::invalid_argument e) { 
        fl_alert("Invalid address [ %s ]", inputBreakAddress->value());
    }
}