#include "../lib/win.h"
#include "../lib/image.h"
#include "matrix.h"
#include "number.h"
#include "expviewer.h"
#include "simplexview.h"
#include <list>

class ClipboardEvent
{
public:
    virtual void paste(const String& text, bool &canPaste) = 0;
};

class EditEx: public Edit
{
private:
    std::list<ClipboardEvent*> clipbrd_list;
protected:
    LRESULT message(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if(uMsg == WM_PASTE)
        {

            if(OpenClipboard(getHandle()))
            {
                HGLOBAL hglb;
                char* str;
                String text;
                ClipboardEvent * clipbrd;
                std::list<ClipboardEvent*>::iterator it;
                bool canPaste = true;

                hglb = GetClipboardData(CF_TEXT);
                if(hglb != NULL)
                {
                    str = reinterpret_cast<char*>(GlobalLock(hglb));
                    if(str != NULL)
                    {
                        text = str;
                        for(it = clipbrd_list.begin(); it != clipbrd_list.end(); it++)
                        {
                            clipbrd = *it;
                            clipbrd->paste(text, canPaste);
                        }
                        GlobalUnlock(hglb);
                    }
                }
                CloseClipboard();
                if(!canPaste)
                    return 0;
            }
        }
        return Edit::message(uMsg, wParam, lParam);
    }
public:
    void addPasteEvent(ClipboardEvent * clipbrd)
    {
        clipbrd_list.push_back(clipbrd);
    }
};

/* classe que implementa uma flag para quando a janela mover não mover outra
 * janela gerando assim um loop infinito
 */
class BaseWindow: public Window
{
private:
    bool moving;
public:
    BaseWindow()
    {
        moving = false;
    }

    void setMoving(bool moving)
    {
        this->moving = moving;
    }

    bool getMoving()
    {
        return moving;
    }
};

class ResultWindow: public BaseWindow
{
private:
    BaseWindow * parent;
    SimplexView spxview;
protected:
    DWORD getStyle()
    {
        return WS_DLGFRAME;
    }

    void move(int x, int y)
    {
        if(!getMoving())
        {
            parent->setMoving(true);
            parent->setPosition(x - (parent->getWidth() + GetSystemMetrics(SM_CXFIXEDFRAME) * 4 + 1),
                                y - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFIXEDFRAME));
            parent->setMoving(false);
        }
    }
public:
    ResultWindow(BaseWindow * parent)
    {
        setResizable(false);
        this->parent = parent;
        create("ResultWindowClass", 480, 480, "Resultado", NULL);
        setButtonEnabled(BUTTON_ALL, false);
        spxview.create(5, 5, getClientWidth() - 10,
                       getClientHeight() - 10, this);
        spxview.show();
    }

    SimplexView& getSimplexView()
    {
        return spxview;
    }

    bool setMatrix(ExpViewer * viewer)
    {
        if(!spxview.setMatrix(viewer))
        {
            messageBox(spxview.getLastError(), "Erro", MB_ICONERROR);
            return false;
        }
        return true;
    }
};

class SimplexWindow: public BaseWindow, ClickEvent, KeyEvent, ChangeEvent,
    public ClipboardEvent, DragDropEvent
{
private:
    ExpViewer expviewer;
    EditEx edit;
    Image imgUFPI;
    Button btnAdd;
    Button btnSol;
    Button btnCls;
    Button btnReset;
    Button btnOpen;
    Label lblaviso;
    ResultWindow * resultWindow;
    bool solving;
private:
    bool openFile(String& dest)
    {
        OPENFILENAME ofn;
        char szFile[MAX_PATH];

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = getHandle();
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "Todos os arquivos\0*.*\0Arquivos de texto\0*.txt\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        if(GetOpenFileName(&ofn))
        {
            dest = ofn.lpstrFile;
            return true;
        }
        return false;
    }
protected:
    void move(int x, int y)
    {
        if(!getMoving())
        {
            resultWindow->setMoving(true);
            resultWindow->setPosition(x + getWidth() +
                                      GetSystemMetrics(SM_CXFIXEDFRAME) * 2 + 1,
                                      y - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFIXEDFRAME));
            resultWindow->setMoving(false);
        }
    }
public:
    SimplexWindow()
    {
        solving = false;
        setResizable(false);
        create("SimplexWindowClass", 640, 480, "Simplex", NULL);
        resultWindow = new ResultWindow(this);
        setPosition((GetSystemMetrics(SM_CXSCREEN) - (getWidth() + resultWindow->getWidth())) / 2, getY());
        setButtonEnabled(BUTTON_MAXIMIZE, false);
        edit.create((getClientWidth() - (getClientWidth() - 50)) / 2,
                    20, getClientWidth() - 120, 50, "Min Z = 2x1 + 5x2 + 5x6", this);
        edit.getFont().setName("Times New Roman");
        edit.getFont().setSize(28);
        edit.getFont().setQuality(ANTIALIASED_QUALITY);
        edit.addKeyEvent(this);
        edit.addChangeEvent(this);
        edit.addPasteEvent(this);
        lblaviso.create((getClientWidth() - (getClientWidth() - 50)) / 2,
                        20 + edit.getHeight() + 5, 100, 30, "", this);
        btnAdd.create((getClientWidth() - (getClientWidth() - 50)) / 2 +
                      getClientWidth() - 108, 20, 75, 50, "Add", this);
        btnAdd.addClickEvent(this);
        btnAdd.setImageResource("image_add");
        btnSol.create((getClientWidth() - (getClientWidth() - 50)) / 2 +
                      getClientWidth() - 108, 75, 75, 50, "Resolver", this);
        btnSol.addClickEvent(this);
        btnSol.setImageResource("image_solve");
        btnSol.setEnabled(false);
        btnCls.create((getClientWidth() - (getClientWidth() - 50)) / 2 +
                      getClientWidth() - 108, 130, 75, 50, "Limpar", this);
        btnCls.addClickEvent(this);
        btnCls.setImageResource("image_cls");
        btnReset.create((getClientWidth() - (getClientWidth() - 50)) / 2 +
                        getClientWidth() - 108, 185, 75, 50, "Reiniciar", this);
        btnReset.addClickEvent(this);
        btnReset.hide();
        btnOpen.create((getClientWidth() - (getClientWidth() - 50)) / 2 +
                       getClientWidth() - 108, 240, 75, 50, "Carregar", this);
        btnOpen.addClickEvent(this);
        expviewer.create(100, 100, 438, 300, this);
        expviewer.addDragDropEvent(this);
        expviewer.setDragDrop(true);
        expviewer.show();
        imgUFPI.create(-40, 50 + getClientHeight() - 200, 100, 100, this);
        if(!imgUFPI.loadResource("MARCA_DAGUA"))
        {
            messageBox("Recurso 'MARCA_DAGUA' não encontrado", "Erro",
                       MB_ICONERROR);
            terminate(1);
            return;
        }
        show();
        SetForegroundWindow(getHandle());
        edit.setFocus();
        edit.setSelStart(edit.getText().length());
    }

    ~SimplexWindow()
    {
        delete resultWindow;
    }

    void click(Object & sender)
    {
        if(sender == btnAdd)
        {
            String text = edit.getText();
            Parser p;
            int r = p.execute(text);
            if(r == 0)
            {
                Expression& eref = p.getExpression();
                if(eref.isFunction())
                    expviewer.setFunction(&eref);
                else
                    expviewer.addItem(&eref);
                if(expviewer.getFunction() != NULL && expviewer.itemCount() > 0)
                    btnSol.setEnabled(true);
                edit.clear();
            }
            else
            {
                String msg;

                msg.format("Erro código: %d", r);
                messageBox(msg, "Erro", MB_ICONERROR);
            }
            edit.setFocus();
        }
        else if(sender == btnSol)
        {
            if(solving)
            {
                if(!resultWindow->getSimplexView().nextStep())
                    btnSol.setEnabled(false);
                btnReset.show();
            }
            else
            {
                if(!resultWindow->setMatrix(&expviewer))
                {
                    edit.setFocus();
                }
                else
                {
                    resultWindow->show();
                    btnSol.setText("Próximo");
                    solving = true;
                }
            }
        }
        else if(sender == btnCls)
        {
            btnSol.setEnabled(false);
            expviewer.clear();
            edit.clear();
            resultWindow->hide();
            btnReset.hide();
            btnSol.setText("Resolver");
            edit.setFocus();
            solving = false;
        }
        else if(sender == btnReset)
        {
            resultWindow->getSimplexView().reset();
            btnSol.setEnabled(true);
            btnReset.hide();
        }
        else if(sender == btnOpen)
        {
            String path;
            Strings arq;
            if(openFile(path))
            {
                if(arq.load(path))
                {
                    addExpressions(arq);
                }
                else
                    messageBox("Não foi possível carregar o arquivo.", "Erro", MB_ICONERROR);
            }
        }
    }

    void addExpressions(Strings& arq)
    {
        click(btnCls);
        while(arq.count() != 0)
        {
            Parser p;
            String text = arq.get(0);
            int r;

            if(text != "")
            {
                r = p.execute(text);
                if(r == 0)
                {
                    Expression& eref = p.getExpression();
                    if(eref.isFunction())
                        expviewer.setFunction(&eref);
                    else
                        expviewer.addItem(&eref);
                    if(expviewer.getFunction() != NULL && expviewer.itemCount() > 0)
                        btnSol.setEnabled(true);
                }
                else
                {
                    String msg;

                    msg.format("Erro código: %d", r);
                    messageBox(msg, "Erro", MB_ICONERROR);
                }
            }
            arq.erase(0);
        }
    }

    void dblClick(Object & sender) {}
    void keyDown(Object & sender, int key, int state) {}
    void keyUp(Object & sender, int key, int state)
    {
        if(sender == edit)
        {
            if(key == VK_RETURN)
            {
                click(btnAdd);
            }
        }
    }
    void keyPress(Object & sender, char key, int state) {}
    void change(Object & sender)
    {
        String text = edit.getText();
        Parser p;
        int r;

        if(text == "")
        {
            if(expviewer.getFunction() == NULL)
                lblaviso.setText("Digite uma função objetivo");
            else
                lblaviso.setText("Digite uma restrição");
            return;
        }
        r = p.execute(text);
        if(r == 0)
        {
            Expression& eref = p.getExpression();
            String msg;


            if(eref.isFunction())
                msg = "Função ok";
            else
                msg = "Restrição ok";
            lblaviso.setText(msg);
        }
        else
        {
            lblaviso.setText(p.getErrorMsg());
        }
    }

    void paste(const String& text, bool &canPaste)
    {
        Strings clipbrd;

        clipbrd.setText(text);
        if(clipbrd.count() > 1)
        {
            addExpressions(clipbrd);
            canPaste = false;
        }
    }

    void drop(const String& urls)
    {
        Strings files, arq;

        files.setText(urls);
        if(files.count() == 1)
        {
            if(arq.load(files.get(0)))
                addExpressions(arq);
        }
    }
};

int main(int argc, char *argv[])
{
    SimplexWindow sw;

    return sw.run();
}

