#ifndef _PROGRESSBAR_H_
#define _PROGRESSBAR_H_
#include "control.h"

class ProgressBar: public Control
{
private:
    int position;
    bool marquee_enabled;
    int start, end;
public:
    ProgressBar();
    bool create(int x, int y, int w, int h, Control * parent);
    int getPos();
    void setPos(int position);
    void setMarquee(bool enable);
    void setRange(int start, int end);
};
#endif

