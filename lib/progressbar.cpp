#include "progressbar.h"
#define PBS_MARQUEE 8
#define PBM_SETMARQUEE WM_USER + 10
#define PBM_GETPOS	1032
#define PBM_SETPOS	(WM_USER+2)
#define PBS_SMOOTH	1
#define PBM_SETRANGE32	1030

ProgressBar::ProgressBar()
{
    marquee_enabled = false;
    start = 0;
    end = 100;
    position = 0;
}

bool ProgressBar::create(int x, int y, int w, int h, Control* parent)
{
    if(!createControl(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,
                      WS_OVERLAPPED, "msctls_progress32", "", x, y, w, h, parent))
        return false;
    if(position > 0)
        setPos(position);
    if(marquee_enabled)
        setMarquee(marquee_enabled);
    setRange(start, end);
    return true;
}

int ProgressBar::getPos()
{
    position = SendMessage(getHandle(), PBM_GETPOS, 0, 0);
    return position;
}

void ProgressBar::setPos(int position)
{
    this->position = position;
    SendMessage(getHandle(), PBM_SETPOS, position, 0);
}

void ProgressBar::setMarquee(bool enable)
{
    this->marquee_enabled = enable;
    if(enable)
    {
        addStyle(PBS_MARQUEE);
        SendMessage(getHandle(), PBM_SETMARQUEE, TRUE, 0);
    }
    else
    {
        delStyle(PBS_MARQUEE);
        SendMessage(getHandle(), PBM_SETMARQUEE, FALSE, 0);
    }
}

void ProgressBar::setRange(int start, int end)
{
    this->start = start;
    this->end = end;
    SendMessage(getHandle(), PBM_SETRANGE32, start, end);
}
