#include <AHT20.h>
#include <Wire.h>
#include "data_send.h"
AHT20 aht20;


  void data_collect(datapack *collection){
    collection->protocolVersion=1;
    collection->nodeID=1;//default
    collection->sensorType=1;//temp
    collection->temperature=aht20.getTemperature();
    collection->humidity=aht20.getHumidity();
    collection->outdoorTemp=0;
    collection->timestamp=0;//reciever will add later
    collection->heatingState=0;//not applicable to this sensor
}
