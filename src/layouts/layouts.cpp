#include <Arduino.h>

#include "layouts.h"
#include "layout_de.h"
#include "layout_en.h"

const TLayout* getLayoutMap(int layout)
{
    Serial.print("getLayoutMap ");
    Serial.println(layout);
    if(layout==0) return getLayoutMapEN();
    if(layout==1) return getLayoutMapDE();
    return nullptr;
}
