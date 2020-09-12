#include "simplexview.h"
#include <stdio.h>
#include <vector>
#include <algorithm>

SimplexMatrix::SimplexMatrix()
{
    lastError = "Matriz não inicializada";
    currStep = SimplexStepInit;
}

SimplexMatrix::SimplexMatrix(const SimplexMatrix& other)
{
    header = other.header;
    column = other.column;
    matrix = other.matrix;
    maxFunc = other.maxFunc;
    lastError = other.lastError;
    currStep = other.currStep;
}

SimplexMatrix::~SimplexMatrix()
{
}

SimplexMatrix& SimplexMatrix::operator=(const SimplexMatrix& rhs)
{
    header = rhs.header;
    column = rhs.column;
    matrix = rhs.matrix;
    maxFunc = rhs.maxFunc;
    lastError = rhs.lastError;
    currStep = rhs.currStep;
    return *this;
}

int SimplexMatrix::findIn()
{
    int sel = 0;
    int i;
    Number max;

    for(i = 0; i < matrix.getColumns() - 1; i++)
    {
        if(maxFunc)// Max Z
        {
            if(matrix(0, i) < max)
            {
                max = matrix(0, i);
                sel = i + 1;
            }
        }
        else // Min Z
        {
            if(matrix(0, i) > max)
            {
                max = matrix(0, i);
                sel = i + 1;
            }
        }
    }
    return sel;
}

int SimplexMatrix::findOut(int in)
{
    int sel = -1;
    int i;
    Number max, a_b;

    for(i = 1; i < matrix.getRows(); i++)
    {
        a_b = matrix(i, matrix.getColumns() - 1) / matrix(i, in - 1);
        if(matrix(i, in - 1) > 0 && (a_b < max || sel == -1))
        {
            max = a_b;
            sel = i;
        }
    }
    return sel;
}

void SimplexMatrix::step4()
{
    int i, j;
    Number xcj, sel;

    xcj = matrix(ark, cj - 1);
    for(i = 0; i < matrix.getRows(); i++)
    {
        if(i == ark)
            continue;
        sel = matrix(i, cj - 1);
        for(j = 0; j < matrix.getColumns(); j++)
            matrix(i, j) -=  sel * matrix(ark, j) / xcj;
    }
    for(j = 0; j < matrix.getColumns(); j++)
        matrix(ark, j) /= xcj;
}

bool SimplexMatrix::isComplete()
{
    return currStep == SimplexStepGreat;
}

bool SimplexMatrix::checkMultiple()
{
    int j, count = 0;
    
    for(j = 0; j < matrix.getColumns() - 1; j++)
    {
        if(matrix(0, j) == 0)
            count++;
    }
    return (count > matrix.getRows() - 1);
}

bool SimplexMatrix::nextStep()
{
    if(currStep == SimplexStepInit)
    {
        cj = findIn();
        currStep = SimplexStepCheck;
    }
    else if(currStep == SimplexStepCheck)
    {
        cj = findIn();
        if(cj == 0)
        {
            // não encontrou
            currStep = SimplexStepGreat;
            return false;
        }
        currStep = SimplexStepSelectIn;
    }
    else if(currStep == SimplexStepSelectIn)
    {
        ark = findOut(cj);
        if(ark == -1)
        {
            // solução infinita
            currStep = SimplexStepInfinite;
            return false;
        }
        currStep = SimplexStepSelectOut;
    }
    else if(currStep == SimplexStepSelectOut)
    {
        Variable* var;
        //muda o índice da variável
        var = reinterpret_cast<Variable*>(column.get(ark));
        var->id = cj;
        step4();
        if(checkMultiple())
        {
            // solução múltipla
            currStep = SimplexStepMultiple;
            return false;
        }
        else
            currStep = SimplexStepNewBasic;
    }
    else if(currStep == SimplexStepNewBasic)
    {
        cj = findIn();
        currStep = SimplexStepCheck;
    }
    return true;
}

int SimplexMatrix::getCj()
{
    return cj;
}

int SimplexMatrix::getArk()
{
    return ark;
}

Number SimplexMatrix::getZ()
{
    return matrix(0, matrix.getColumns() - 1);
}

SimplexStep SimplexMatrix::getStep()
{
    return currStep;
}

bool SimplexMatrix::setMatrix(ExpViewer* viewer)
{
    int max_var, i, j, new_var_count, var_count;
    Expression *exp;
    Term * term;
    Variable * var;
    bool has_comp;
    String letter = "X";
    int tmp_count;
    bool has_basic_rest = false;
    std::vector<int> all_vars, vars_not_in;
    std::vector<int>::iterator it;
    String v_str;

    if(viewer->getFunction() == NULL)
    {
        lastError = "Função objetivo não existe";
        return false;
    }
    if(viewer->itemCount() == 0)
    {
        lastError = "Não existe restrições";
        return false;
    }
    currStep = SimplexStepInit;
    // transformando na forma normal
    max_var = 0;
    new_var_count = 0;
    tmp_count = viewer->itemCount();

    if(viewer->getItem(tmp_count - 1)->isBasicRestriction())
    {
        tmp_count--;
        has_basic_rest = true;
        exp = viewer->getItem(tmp_count);
        for(i = 0; i < exp->count(); i++)
        {
            term = exp->get(i);
            if(term->type == TYPE_VARIABLE)
            {
                var = reinterpret_cast<Variable*>(term);
                if(var->id > 0)
                    all_vars.push_back(var->id);
            }
            else
            {
                if(term->letter != ">=" && term->letter != ",")
                {
                    lastError = "Sinal de comparação " + term->letter +
                                " não suportado!";
                    return false;
                }
            }
        }
    }
    for(i = 0; i < tmp_count; i++)
    {
        exp = viewer->getItem(i);
        for(j = 0; j < exp->count(); j++)
        {
            term = exp->get(j);
            if(term->type == TYPE_VARIABLE)
            {
                var = reinterpret_cast<Variable*>(term);
                if(var->letter != "")
                {
                    letter = var->letter;
                    if(has_basic_rest)
                    {
                        it = find(all_vars.begin(), all_vars.end(), var->id);
                        if(it == all_vars.end())
                        {
                            it = find(vars_not_in.begin(), vars_not_in.end(), var->id);
                            if(it == vars_not_in.end())
                                vars_not_in.push_back(var->id);
                        }
                    }
                    if(var->id > max_var)
                    {
                        max_var = var->id;
                    }
                }
            }
            else
            {
                if(term->letter == "<=" || term->letter == ">=")
                {
                    new_var_count++;
                }
            }
        }
    }
    var_count = max_var + new_var_count + vars_not_in.size();
    lastError.format("numeros de variáveis: %d", var_count);
    matrix.set(tmp_count + 1, var_count + 1);
    var = new Variable();
    var->id = 0;
    var->letter = "Base";
    column.clear();
    column.add(var);
    delete var;
    exp = viewer->getFunction();
    has_comp = false;
    //Função
    maxFunc = exp->get(0)->letter == "MAXZ";
    for(j = 0; j < exp->count(); j++)
    {
        term = exp->get(j);
        if(term->type == TYPE_VARIABLE)
        {
            var = reinterpret_cast<Variable*>(term);
            if(var->letter != "")
            {
                if(has_comp)
                    matrix(0, var->id - 1) = -var->number;
                else
                    matrix(0, var->id - 1) = var->number;
            }
            else
            {
                //passa os termos independentes para o lado direito
                if(has_comp)
                    matrix(0, var_count) += var->number;
                else
                    matrix(0, var_count) -= var->number;
            }
        }
        else
        {
            if(term->letter == "=")
                has_comp = true;
        }
    }
    for(i = 0; i < tmp_count; i++)
    {
        exp = viewer->getItem(i);
        has_comp = false;
        for(j = 0; j < exp->count(); j++)
        {
            term = exp->get(j);
            if(term->type == TYPE_VARIABLE)
            {
                var = reinterpret_cast<Variable*>(term);
                if(var->letter != "")
                {
                    if(has_comp)
                        matrix(i + 1, var->id - 1) -= var->number;
                    else
                        matrix(i + 1, var->id - 1) += var->number;
                }
                else
                {
                    //passa os termos independentes para o lado direito
                    if(has_comp)
                        matrix(i + 1, var_count) += var->number;
                    else
                        matrix(i + 1, var_count) -= var->number;
                }
            }
            else
            {
                if(term->letter == "<=" || term->letter == ">=" ||
                        term->letter == ">" || term->letter == "<" ||
                        term->letter == "=")
                {
                    new_var_count--;
                    max_var++;
                    var = new Variable();
                    var->id = max_var;
                    var->letter = letter;
                    column.add(var);
                    delete var;
                    if(term->letter == "<=")
                        matrix(i + 1, max_var - 1) = 1;
                    else if(term->letter == ">=")
                        matrix(i + 1, max_var - 1) = -1;
                    else
                    {
                        lastError = "Sinal de comparação " + term->letter +
                                    " não suportado!";
                        return false;
                    }
                    has_comp = true;
                }
            }
        }
    }
    var = new Variable();
    header.clear();
    for(i = 0; i < var_count; i++)
    {
        var->id = i + 1;
        var->letter = letter;
        header.add(var);
    }
    var->id = 0;
    var->letter = "b";
    header.add(var);
    delete var;
    lastError = "Sem erros";
    return true;
}

String SimplexMatrix::getLastError()
{
    //TODO Generated function
    return lastError;
}

SimplexView::SimplexView()
{
    numFont = new Font(NULL, "Times New Roman", 20);
    fracFont = new Font(NULL, "Times New Roman", 16);
    varFont = new Font(NULL, "Times New Roman", 16);
    idFont  = new Font(NULL, "Times New Roman", 14);
}
SimplexView::~SimplexView()
{
    delete numFont;
    delete fracFont;
    delete varFont;
    delete idFont;
}

bool SimplexView::create(int x, int y, int width, int height, Control* parent)
{
    if(!registerClass("SimplexView"))
        return false;
    if(!createControl(0, 0, "SimplexView", "", x, y, width, height, parent))
        return false;
    return true;
}

bool SimplexView::setMatrix(ExpViewer* viewer)
{
    if(current.setMatrix(viewer))
    {
        matrix = current;
        invalidate();
    }
    else
        return false;
    return true;
}

String SimplexView::getLastError()
{
    return current.getLastError();
}

bool SimplexView::nextStep()
{
    bool result;

    result = current.nextStep();
    invalidate();
    return result;
}

bool SimplexView::isComplete()
{
    return current.isComplete();
}

void SimplexView::reset()
{
    current = matrix;
    invalidate();
}

Number SimplexView::getZ()
{
    return current.getZ();
}

LRESULT SimplexView::message(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

const String SimplexView::intToStr(int n)
{
    String s;

    s.format("%d", n);
    return s;
}

void SimplexView::paint(HDC& hdc)
{
    //TODO Generated function
    int numRows = current.matrix.getRows();
    int numCols = current.matrix.getColumns();
    int w, h, y_frac, x_curr, y_curr, y_letter;
    int i, j, x_offset, y_offset, x, y;
    int space, max_w, sig_w;
    RECT rect;
    Number n;
    String s, sig;
    Variable* v;
    HPEN oldPen, newPen, newBluePen;
    int cj = 0, ark = -1;

    newPen = CreatePen(PS_DOT, 1, RGB(255, 0, 0));
    newBluePen = CreatePen(PS_DOT, 1, RGB(0, 0, 255));
    if(current.getStep() == SimplexStepSelectIn ||
            current.getStep() == SimplexStepCheck)
        cj = current.getCj();
    else if(current.getStep() == SimplexStepSelectOut)
    {
        cj = current.getCj();
        ark = current.getArk();
    }
    w = getClientWidth();
    h = getClientHeight();
    space = 50;
    if((numCols + 1) * space > w)
        space = w / (numCols + 1);
    if((numRows + 1) * space > h)
        space = h / (numRows + 1);
    x_offset = (w - (numCols + 1) * space) / 2;
    y_offset = (h - (numRows + 1) * space) / 2;
    rect.left = 0;
    rect.top = 0;
    rect.right = w;
    rect.bottom = h;
    FillRect(hdc, &rect, GetSysColorBrush(COLOR_WINDOW));

    if(current.getStep() == SimplexStepInit)
    {
        s = "Passo 1: Começar com o quadro na forma canônica";
        TextOut(hdc, 10, y_offset - numFont->getHeight(hdc),
                s.c_str(), s.length());
    }
    else if(current.getStep() == SimplexStepCheck)
    {
        s = "Passo 2: Realizar o teste de otimização";
        TextOut(hdc, 10, y_offset - numFont->getHeight(hdc) * 2,
                s.c_str(), s.length());
        if(cj == 0)
            s = "Passo 2: a solução obtida é ótima";
        else
            s = "Passo 2: a solução não é ótima";
        TextOut(hdc, 10, y_offset - numFont->getHeight(hdc),
                s.c_str(), s.length());
        cj = 0;
    }
    else if(current.getStep() == SimplexStepSelectIn)
    {
        v = reinterpret_cast<Variable*>(current.header.get(cj - 1));
        s.format("%s%d", v->letter.c_str(), v->id);
        s = "Passo 2: A variável " + s + " entra na base";
        SetTextColor(hdc, RGB(255, 0, 0));
        TextOut(hdc, 10, y_offset - numFont->getHeight(hdc),
                s.c_str(), s.length());
    }
    else if(current.getStep() == SimplexStepSelectOut)
    {
        v = reinterpret_cast<Variable*>(current.header.get(cj - 1));
        s.format("%s%d", v->letter.c_str(), v->id);
        s = "Passo 2: A variável " + s + " entra na base";
        SetTextColor(hdc, RGB(255, 0, 0));
        TextOut(hdc, 10, y_offset - numFont->getHeight(hdc) * 2,
                s.c_str(), s.length());
        v = reinterpret_cast<Variable*>(current.column.get(ark));
        s.format("%s%d", v->letter.c_str(), v->id);
        s = "Passo 3: A variável " + s + " sai da base";
        SetTextColor(hdc, RGB(0, 0, 255));
        TextOut(hdc, 10, y_offset - numFont->getHeight(hdc),
                s.c_str(), s.length());
    }
    else if(current.getStep() == SimplexStepNewBasic)
    {
        n = getZ();
        s = "Passo 4: Novo valor de Z: " + n.toString();
        TextOut(hdc, 10, y_offset - numFont->getHeight(hdc),
                s.c_str(), s.length());
    }
    else if(current.getStep() == SimplexStepGreat)
    {
        n = getZ();
        s = "Concluido: Solução ótima encontrada valor de Z*: " + n.toString();
        SetTextColor(hdc, RGB(0, 0, 255));
        TextOut(hdc, 10, y_offset - numFont->getHeight(hdc),
                s.c_str(), s.length());
    }
    else if(current.getStep() == SimplexStepInfinite)
    {
        s = "Concluido: Solução infinita";
        SetTextColor(hdc, RGB(255, 0, 0));
        TextOut(hdc, 10, y_offset - numFont->getHeight(hdc),
                s.c_str(), s.length());
    }
    else if(current.getStep() == SimplexStepMultiple)
    {
        s = "Concluido: Multiplas soluções";
        SetTextColor(hdc, RGB(255, 0, 0));
        TextOut(hdc, 10, y_offset - numFont->getHeight(hdc),
                s.c_str(), s.length());
    }
    for(i = 0; i < numRows; i++)
    {
        //draw row data
        x = x_offset + space;
        y = y_offset + (i + 1) * space;
        for(j = 0; j < numCols; j++)
        {
            n = current.matrix(i, j);
            sig_w = 0;
            if(n.getUp() < 0)
                sig_w = numFont->getWidth(hdc, "-");
            if(n.getDown() != 1)
            {
                // draw numerator
                if(n.getUp() < 0)
                    s = intToStr(-n.getUp());
                else
                    s = intToStr(n.getUp());
                y_curr = y + (space - fracFont->getHeight(hdc)) / 2;
                y_frac = 2 + y_curr - fracFont->getHeight(hdc) / 2;
                max_w = fracFont->getWidth(hdc, s);
                if(fracFont->getWidth(hdc, intToStr(n.getDown())) > max_w)
                    max_w = fracFont->getWidth(hdc, intToStr(n.getDown()));
                x_curr = x + (space - (max_w + sig_w)) / 2;
                //draw signal
                if(n.getUp() < 0)
                {
                    //draw signal
                    sig = "-";
                    SelectObject(hdc, numFont->getHandle());
                    SetTextColor(hdc, RGB(0, 0, 0));
                    TextOut(hdc, x_curr - 2, y_curr - 4, sig.c_str(), sig.length());
                }
                x_curr += sig_w;
                SelectObject(hdc, fracFont->getHandle());
                SetTextColor(hdc, RGB(0, 0, 255));
                TextOut(hdc, x_curr + (max_w - fracFont->getWidth(hdc, s)) / 2,
                        y_frac, s.c_str(), s.length());
                //draw line
                //RGB(0, 0, 0)
                MoveToEx(hdc, x_curr - 2, y_frac + fracFont->getHeight(hdc), NULL);
                LineTo(hdc, x_curr + max_w,
                       y_frac + fracFont->getHeight(hdc));
                // draw denominator
                s = intToStr(n.getDown());
                SetTextColor(hdc, RGB(0, 0, 255));
                TextOut(hdc, x_curr + (max_w - fracFont->getWidth(hdc, s)) / 2,
                        y_frac + fracFont->getHeight(hdc) + 1, s.c_str(), s.length());
                SelectObject(hdc, numFont->getHandle());
            }
            else
            {
                SelectObject(hdc, numFont->getHandle());
                // draw numerator
                if(n.getUp() < 0)
                    s = intToStr(-n.getUp());
                else
                    s = intToStr(n.getUp());
                x_curr = x + (space - (numFont->getWidth(hdc, s) + sig_w)) / 2;
                y_curr = y + (space - numFont->getHeight(hdc)) / 2;
                //draw signal
                if(n.getUp() < 0)
                {
                    //draw signal
                    sig = "-";
                    SetTextColor(hdc, RGB(0, 0, 0));
                    TextOut(hdc, x_curr, y_curr, sig.c_str(), sig.length());
                }
                x_curr += sig_w;
                SetTextColor(hdc, RGB(0, 0, 255));
                TextOut(hdc, x_curr, y_curr, s.c_str(), s.length());
            }
            x += space;
        }
    }
    x = x_offset;
    for(i = 0; i < numRows; i++)
    {
        MoveToEx(hdc, x_offset, y_offset + (i + 1) * space, NULL);
        LineTo(hdc, x_offset + space * (numCols + 1), y_offset + (i + 1) * space);
        y = y_offset + space * (i + 1);
        v = reinterpret_cast<Variable*>(current.column.get(i));
        if(v->id > 0)
        {
            if(i == ark)
            {
                oldPen = (HPEN)SelectObject(hdc, newBluePen);
                RoundRect(hdc, x + 1, y + 1, x + space, y + space, 10, 10);
                SelectObject(hdc, oldPen);
            }
            // draw letter
            s = intToStr(v->id);
            sig_w = idFont->getWidth(hdc, s) + 3;
            s = v->letter;
            x_curr = x + (space - (numFont->getWidth(hdc, s) + sig_w)) / 2;
            y_curr = y + (space - (numFont->getHeight(hdc) + 5)) / 2;

            y_letter = y_curr + numFont->getHeight(hdc) - varFont->getHeight(hdc);
            SelectObject(hdc, varFont->getHandle());
            SetTextColor(hdc, RGB(0, 0, 0));
            TextOut(hdc, x_curr, y_letter, s.c_str(), s.length());
            x_curr += varFont->getWidth(hdc, s) + 3;
            // draw var id
            s = intToStr(v->id);
            SelectObject(hdc, idFont->getHandle());
            SetTextColor(hdc, RGB(128, 0, 0));
            TextOut(hdc, x_curr, y_letter + 5, s.c_str(), s.length());
        }
        else
        {
            SelectObject(hdc, varFont->getHandle());
            s = v->letter;
            x_curr = x + (space - varFont->getWidth(hdc, s)) / 2;
            y_curr = y + (space - varFont->getHeight(hdc)) / 2;
            SetTextColor(hdc, RGB(0, 0, 0));
            TextOut(hdc, x_curr, y_curr, s.c_str(), s.length());
        }
    }
    y = y_offset;
    for(j = 0; j < numCols; j++)
    {
        MoveToEx(hdc, x_offset + (j + 1) * space, y_offset, NULL);
        LineTo(hdc, x_offset + (j + 1) * space, y_offset + space * (numRows + 1));
        x = x_offset + space * (j + 1);
        v = reinterpret_cast<Variable*>(current.header.get(j));
        if(v->id > 0)
        {
            if(v->id == cj)
            {
                oldPen = (HPEN)SelectObject(hdc, newPen);
                RoundRect(hdc, x + 1, y + 1, x + space, y + space, 10, 10);
                SelectObject(hdc, oldPen);
            }
            // draw letter
            s = intToStr(v->id);
            sig_w = idFont->getWidth(hdc, s) + 3;
            s = v->letter;
            x_curr = x + (space - (numFont->getWidth(hdc, s) + sig_w)) / 2;
            y_curr = y + (space - (numFont->getHeight(hdc) + 5)) / 2;

            y_letter = y_curr + numFont->getHeight(hdc) - varFont->getHeight(hdc);
            SelectObject(hdc, varFont->getHandle());
            SetTextColor(hdc, RGB(0, 0, 0));
            TextOut(hdc, x_curr, y_letter, s.c_str(), s.length());
            x_curr += varFont->getWidth(hdc, s) + 3;
            // draw var id
            s = intToStr(v->id);
            SelectObject(hdc, idFont->getHandle());
            SetTextColor(hdc, RGB(128, 0, 0));
            TextOut(hdc, x_curr, y_letter + 5, s.c_str(), s.length());
        }
        else
        {
            SelectObject(hdc, varFont->getHandle());
            s = v->letter;
            x_curr = x + (space - varFont->getWidth(hdc, s)) / 2;
            y_curr = y + (space - varFont->getHeight(hdc)) / 2;
            SetTextColor(hdc, RGB(0, 0, 0));
            TextOut(hdc, x_curr, y_curr, s.c_str(), s.length());
        }
    }
    DeleteObject(newPen);
    DeleteObject(newBluePen);
}

