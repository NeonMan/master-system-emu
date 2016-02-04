
#include "savestate.h"
#include <stdio.h>

int main(){
    FILE* f;
    
    f = fopen("Sample.sav", "wb");
    ss_save(f, "SAMPLE.SMS");
    fclose(f);

    f = fopen("Sample.sav", "rb");
    ss_restore(f);
    fclose(f);

    return 0;
}
