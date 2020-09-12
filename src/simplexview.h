#ifndef _SIMPLEXVIEW_H_
#define _SIMPLEXVIEW_H_
#include "../lib/win.h"
#include "matrix.h"
#include "expviewer.h"

enum SimplexStep
{
    SimplexStepInit,
    SimplexStepCheck,
    SimplexStepSelectIn,
    SimplexStepSelectOut,
    SimplexStepNewBasic,
    SimplexStepGreat,
    SimplexStepInfinite,
    SimplexStepMultiple    
};

class SimplexMatrix
{
private:
    friend class SimplexView;
    
    Expression header;
    Expression column;
    Matrix matrix;
    bool maxFunc;
    String lastError;
    SimplexStep currStep;
    int findIn();
    int findOut(int in);
    void step4();
    bool checkMultiple();
    int cj, ark;
public:
    SimplexMatrix();
    SimplexMatrix(const SimplexMatrix& other);
    ~SimplexMatrix();
    bool setMatrix(ExpViewer * viewer);
    String getLastError();
    bool isComplete();
    bool nextStep();
    SimplexMatrix& operator=(const SimplexMatrix& rhs);
    Number getZ();
    SimplexStep getStep();
    int getCj();
    int getArk();
};

class SimplexView: public Control
{
private:
    SimplexMatrix matrix;
    SimplexMatrix current;
    const String intToStr(int n);
    Font *numFont, *fracFont, *varFont, *idFont;
protected:
    LRESULT message(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void paint(HDC &hdc);
public:
    SimplexView();
    ~SimplexView();
    bool create(int x, int y, int width, int height, Control * parent);
    bool setMatrix(ExpViewer * viewer);
    String getLastError();
    bool nextStep();
    bool isComplete();
    void reset();
    Number getZ();
};

#endif

