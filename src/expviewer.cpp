#include "expviewer.h"

ExpViewer::ExpViewer()
{
    func = NULL;
    numFont = new Font(NULL, "Times New Roman", 20);
    varFont = new Font(NULL, "Times New Roman", 16);
    fracFont = new Font(NULL, "Times New Roman", 16);
    idFont  = new Font(NULL, "Times New Roman", 14);
}

ExpViewer::~ExpViewer()
{
    std::vector<Expression*>::iterator it;

    for(it = items.begin(); it != items.end(); it++)
        delete *it;
    delete numFont;
    delete varFont;
    delete fracFont;
    delete idFont;
    delete parFont;
    if(func != NULL)
        delete func;
}

Expression* ExpViewer::getFunction()
{
    return func;
}

const String ExpViewer::intToStr(int n)
{
    String s;

    s.format("%d", n);
    return s;
}

bool ExpViewer::create(int x, int y, int width, int height, Control* parent)
{
    if(!registerClass("ExpViewer"))
        return false;
    if(!createControl(0, 0, "ExpViewer", "", x, y, width, height, parent))
        return false;
    parFont = new Font(NULL, "Courier New", height * 0.8);
    return true;
}

void ExpViewer::addItem(Expression* item)
{
    if(itemCount() > 0 && getItem(itemCount() - 1)->isBasicRestriction())
    {
        Expression* exp = getItem(itemCount() - 1);
        if(item->isBasicRestriction())
            *exp = *item;
        else
            items.insert(items.begin() + (itemCount() - 1), new Expression(*item));
    }
    else
        items.push_back(new Expression(*item));
    invalidate();
}

void ExpViewer::deleteItem(int index)
{
    delete items[index];
    items.erase(items.begin() + index);
    invalidate();
}

Expression* ExpViewer::getItem(int index)
{
    return items[index];
}

int ExpViewer::itemCount()
{
    return items.size();
}

void ExpViewer::clear()
{
    std::vector<Expression*>::iterator it;

    for(it = items.begin(); it != items.end(); it++)
        delete *it;
    items.clear();
    setFunction(NULL);
    invalidate();
}

void ExpViewer::setFunction(Expression* func)
{
    if(func != NULL && !func->isFunction())
        return;
    if(this->func != NULL)
        delete this->func;
    if(func != NULL)
        this->func = new Expression(*func);
    else
        this->func = NULL;
    invalidate();
}

LRESULT ExpViewer::message(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(uMsg == WM_PAINT)
    {
        PAINTSTRUCT ps;
        HDC hdc;

        hdc = BeginPaint(getHandle(), &ps);
        paint(hdc);
        EndPaint(getHandle(), &ps);
        return 0;
    }
    return Control::message(uMsg, wParam, lParam);
}

void ExpViewer::drawExpression(HDC& hdc, int x, int y, Expression * e,
                               int max_jump)
{
    int j;
    Term * t;
    Variable * v;
    int y_letter, y_frac, max_w;
    bool has_var = false;
    String s;

    for(j = 0; j < e->count(); j++)
    {
        t = e->get(j);
        if(t->type == TYPE_TERM)
        {
            if(t->letter == "MINZ" || t->letter == "MAXZ")
            {
                if(t->letter == "MAXZ")
                    s = "Max";
                else
                    s = "Min";
                SetTextColor(hdc, RGB(0, 128, 0));
                TextOut(hdc, x, y, s.c_str(), s.length());
                x += numFont->getWidth(hdc, s + " ");
                s = "Z";
                SetTextColor(hdc, RGB(0, 128, 128));
                TextOut(hdc, x, y, s.c_str(), s.length());
                x += numFont->getWidth(hdc, s + " ");
            }
            else if(t->letter == "=" || t->letter == ">" || t->letter == "<")
            {
                s = t->letter;
                x += 4;
                SetTextColor(hdc, RGB(0, 0, 0));
                TextOut(hdc, x, y, s.c_str(), s.length());
                x += numFont->getWidth(hdc, s + " ");
            }
            else if(t->letter == "<=")
            {
                SetTextColor(hdc, RGB(0, 0, 0));
                x += 4;
                TextOut(hdc, x, y, "<", 1);
                MoveToEx(hdc, x + 1, y + 8 + numFont->getHeight(hdc) / 2, NULL);
                LineTo(hdc, x + numFont->getWidth(hdc, "="), y + 8 + numFont->getHeight(hdc) / 2);
                x += numFont->getWidth(hdc, "= ") + 2;
            }
            else if(t->letter == ">=")
            {
                SetTextColor(hdc, RGB(0, 0, 0));
                x += 4;
                TextOut(hdc, x, y, ">", 1);
                MoveToEx(hdc, x, y + 8 + numFont->getHeight(hdc) / 2, NULL);
                LineTo(hdc, x + numFont->getWidth(hdc, "=") - 1, y + 8 + numFont->getHeight(hdc) / 2);
                x += numFont->getWidth(hdc, "= ") + 2;
            }
            else if(t->letter == ",")
            {
                s = t->letter;
                x += 4;
                SetTextColor(hdc, RGB(0, 0, 0));
                TextOut(hdc, x, y, s.c_str(), s.length());
                x += numFont->getWidth(hdc, s + " ");
            }
            has_var = false;
        }
        else
        {
            v = reinterpret_cast<Variable*>(t);

            if(has_var || v->number.getUp() < 0)
            {
                //draw signal
                if(v->number.getUp() > 0)
                    s = "+";
                else
                    s = "-";
                SetTextColor(hdc, RGB(0, 0, 0));
                TextOut(hdc, x, y, s.c_str(), s.length());
                x += numFont->getWidth(hdc, s + " ") + 3;
            }

            if(j == 3 && e->count() > max_jump && j < e->count() - 4)
            {
                j = e->count() - 4;
                TextOut(hdc, x, y, "...", 3);
                x += 3 + numFont->getWidth(hdc, "...");
                continue;
            }

            if(v->number.getDown() != 1)
            {
                SelectObject(hdc, fracFont->getHandle());
                //draw numerator
                if(v->number.getUp() < 0)
                    s = intToStr(-v->number.getUp());
                else
                    s = intToStr(v->number.getUp());
                y_frac = 2 + y - fracFont->getHeight(hdc) / 2;
                SetTextColor(hdc, RGB(0, 0, 255));
                max_w = fracFont->getWidth(hdc, s);
                if(fracFont->getWidth(hdc, intToStr(v->number.getDown())) > max_w)
                    max_w = fracFont->getWidth(hdc, intToStr(v->number.getDown()));
                TextOut(hdc, x + (max_w - fracFont->getWidth(hdc, s)) / 2,
                        y_frac, s.c_str(), s.length());
                //draw line
                //RGB(0, 0, 0)
                MoveToEx(hdc, x - 2, y_frac + fracFont->getHeight(hdc), NULL);
                LineTo(hdc, x + max_w,
                       y_frac + fracFont->getHeight(hdc));
                // draw denominator
                s = intToStr(v->number.getDown());
                SetTextColor(hdc, RGB(0, 0, 255));
                TextOut(hdc, x + (max_w - fracFont->getWidth(hdc, s)) / 2,
                        y_frac + fracFont->getHeight(hdc) + 1, s.c_str(), s.length());
                x += max_w + 3;
                SelectObject(hdc, numFont->getHandle());
            }
            else
            {
                if((v->number.getUp() != 1 && v->number.getUp() != -1)
                        || v->letter == "")
                {
                    // draw numerator
                    if(v->number.getUp() < 0)
                        s = intToStr(-v->number.getUp());
                    else
                        s = intToStr(v->number.getUp());
                    SetTextColor(hdc, RGB(0, 0, 255));
                    TextOut(hdc, x, y, s.c_str(), s.length());
                    x += numFont->getWidth(hdc, s) + 3;
                }

            }
            if(v->letter != "")
            {
                // draw letter
                s = v->letter;
                y_letter = y + numFont->getHeight(hdc) - varFont->getHeight(hdc);
                SelectObject(hdc, varFont->getHandle());
                SetTextColor(hdc, RGB(0, 0, 0));
                TextOut(hdc, x, y_letter, s.c_str(), s.length());
                x += varFont->getWidth(hdc, s) + 3;
                // draw var id
                s = intToStr(v->id);
                SelectObject(hdc, idFont->getHandle());
                SetTextColor(hdc, RGB(128, 0, 0));
                TextOut(hdc, x, y_letter + 5, s.c_str(), s.length());
                SelectObject(hdc, numFont->getHandle());
                x += idFont->getWidth(hdc, s) + 3;
            }
            has_var = true;
        }
    }
}

void ExpViewer::paint(HDC& hdc)
{
    RECT rect;
    HFONT oldFont;
    Expression * e;
    int x, y, i;

    getClientRect(rect);
    FillRect(hdc, &rect, GetSysColorBrush(COLOR_BTNFACE));
    //Rectangle(hdc, 0, 0, getClientWidth(), getClientHeight());
    SetBkMode(hdc, TRANSPARENT);
    oldFont = (HFONT)SelectObject(hdc, parFont->getHandle());
    SetTextColor(hdc, RGB(64, 128, 255));
    TextOut(hdc, -60, -15, "{", 1);
    x = parFont->getWidth(hdc, "{") / 2 - 40;
    y = rect.bottom * 0.25;
    SelectObject(hdc, numFont->getHandle());
    if(func != NULL)
        drawExpression(hdc, 10, 10, func);
    for(i = 0; i < itemCount(); i++)
    {
        if(i > 1 && itemCount() > 5 && i < itemCount() - 2)
        {
            i = itemCount() - 2;
            TextOut(hdc, x, y, "...", 3);
            y += 8 + numFont->getHeight(hdc);
        }
        e = getItem(i);
        drawExpression(hdc, x, y, e);
        y += 8 + numFont->getHeight(hdc);
    }
}

