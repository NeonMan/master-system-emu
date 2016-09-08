#include <sms/console.h>
#include <stdint.h>

static const char hello[] = " Hello world! ";

static void delay_loop(){
    volatile uint16_t i;
    for(i=0; i<5000; i++){
        i = i;
    }
}

void main(){
    delay_loop();
    con_init();
    con_put(hello);
    
    while(1){
        
    }
}
