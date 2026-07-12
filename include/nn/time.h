#pragma once

typedef long Tick;
typedef long TimeSpan;

extern Tick nnosGetSystemTick();
extern long nnosGetSystemTickFrequency();
extern TimeSpan nnosConvertTickToNanoSeconds(Tick tick);
