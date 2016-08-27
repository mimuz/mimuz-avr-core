// i2c841s.h
//
// I2C Slave Library for mi:muz:expr
//
// CC 2015 by D.F.Mac.
//
// base : http://www.mikrocontroller.net/topic/353654 (Autor Zepp 2014-12-31 21:01)
// 

#ifndef __I2CS841_h__
#define __I2CS841_h__

#include <avr/io.h>
#include <avr/interrupt.h>

#ifdef __cplusplus
extern "C"{
#endif 

// Callback
void (*onReceived)(uint8_t *pbuf, uint8_t size);

// Global Vals

volatile uint8_t tx_buf[4];
volatile uint8_t txd_exist = false;
volatile uint8_t tx_buf_index = 0;
volatile uint8_t rx_buf[16];
volatile uint8_t rx_buf_index = 0;

ISR( TWI_SLAVE_vect )
{  
  uint8_t status = TWSSRA & ((1 << TWDIF)|(1<< TWASIF));
  // TWDIF:  TWI Data Interrupt Flag
  // TWASIF: TWI Address/Stop Interrupt Flag 

  // When TWI Data Interruption is exist
  if(status & (1 << TWDIF)){
    if(TWSSRA & (1 << TWDIR)){  // TWDIR: (1: a master read operation is in progress)
      if(txd_exist){
        TWSD = tx_buf[tx_buf_index];
        tx_buf_index++;
        if(tx_buf_index >= 4){
          tx_buf_index = 0;
          txd_exist = false;
        }
      }else{
        TWSD = 0xff;
      }
    }else{                      // TWDIR: (0: a master write operation is in progress)
      rx_buf[rx_buf_index%16] = TWSD;
      rx_buf_index ++;
    }
    TWSCRB = (uint8_t) ((1<<TWCMD1)|(1<<TWCMD0));
  // When TWI Address/Stop Interruption is exist
  }else if (status & (1 << TWASIF)){    
    if (TWSSRA & (1<<TWC)){   // TWC: TWI Collision
      TWSCRB = (uint8_t) (1<<TWCMD1);
    }else{    
      if(TWSSRA & (1<<TWAS)){  // Start Condition
        if(!(TWSSRA & (1 << TWDIR))){ // Write Start
          rx_buf_index = 0;
          TWSCRB = (uint8_t) ((1<<TWCMD1)|(1<<TWCMD0)); // ACK   
        }else{                        // Read Start
          if(txd_exist){
            tx_buf_index = 0;
            TWSCRB = (uint8_t) ((1<<TWCMD1)|(1<<TWCMD0)); // ACK   
          }else{
            TWSCRB = (uint8_t) ((1<<TWAA)|(1<<TWCMD1)); // NACK   
          }
        }
      }else{                    // Stop Condition
        if(TWSSRA & (1 << TWDIR)){ // Read End
          txd_exist = false;
        }else{  // Write End
          if(onReceived != 0){
            (*onReceived)((uint8_t *)rx_buf,(uint8_t)rx_buf_index);
          }
          rx_buf_index = 0;
        }
        TWSCRB = (uint8_t) (1<<TWCMD1);
      }
    }
  }    
}

class i2c841slave {
public:
  i2c841slave(){}

  void init(uint8_t addr){
    onReceived = 0;

    DDRA |= 0x50; // SCL,SDA

    TWSA = addr;  // set to slaveaddr << 1; 
    TWSD = 0xFF;

    // enable twi slave    
    TWSCRA = (1<<TWSHE)|(1<<TWDIE)|(1<<TWASIE)|(1<<TWEN)|(1<<TWSIE);
    // × TWSHE  (0x80): TWI SDA Hold Time Enable
    // TWDIE  (0x20): TWI Data Interrupt Enable
    // TWASIE (0x10): TWI Address/Stop Interrupt Enable
    // TWEN   (0x08): Two-Wire Interface Enable
    // TWSIE  (0x04): TWI Stop Interrupt Enable

//    TWSCRA |= (1<<TWPME);  // for Debugging


  }

  void setOnReceived(void (*fptr)(uint8_t *pbuf, uint8_t size)){
    onReceived = fptr;
  }

  int setSendData(uint8_t *pbuf, uint8_t size){
    int cnt;
    if(size > sizeof(tx_buf)){
      return 0;
    }
    for(cnt = 0;cnt < size;cnt ++){
      tx_buf[cnt] = *(pbuf+cnt);
    }
    txd_exist = true;
    return size;
  }

};

#ifdef __cplusplus
} // extern "C"
#endif

i2c841slave i2cs;

#endif

