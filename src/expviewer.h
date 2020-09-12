#ifndef _EXPVIEWER_H_
#define _EXPVIEWER_H_
#include "../lib/win.h"
#include "parser.h"
#include "matrix.h"

class ExpViewer: public Control
{
private:
    Expression* func; /* Função ojetivo */
    std::vector<Expression*> items; /* Restrições */
    Font *numFont, *varFont, *fracFont, *idFont, *parFont;
    const String intToStr(int n);
    void drawExpression(HDC& hdc, int x, int y, Expression * e,
                        int max_jump = 7);
protected:
    LRESULT message(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void paint(HDC &hdc);
public:
    ExpViewer();
    ~ExpViewer();
    Expression* getFunction();
    bool create(int x, int y, int width, int height, Control * parent);
    void addItem(Expression* item);
    void deleteItem(int index);
    void clear();
    Expression* getItem(int index);
    int itemCount();
    void setFunction(Expression* func);
};

#endif

