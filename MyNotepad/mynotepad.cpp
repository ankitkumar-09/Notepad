#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QTemporaryFile>

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif // QT_CONFIG(printdialog)
#include <QPrinter>
#endif // QT_CONFIG(printer)
#endif // QT_PRINTSUPPORT_LIB
#include <QFont>
#include <QFontDialog>

#include "mynotepad.h"
#include "ui_mynotepad.h"

extern "C"
{
#include "../FILE_MANAGER/include/encryption.h"
}
MyNotepad::MyNotepad(QWidget *parent) : QMainWindow(parent),
                                        ui(new Ui::MyNotepad)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);

    connect(ui->actionNew, &QAction::triggered, this, &MyNotepad::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &MyNotepad::open);
    connect(ui->actionSave, &QAction::triggered, this, &MyNotepad::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &MyNotepad::saveAs);
    connect(ui->actionPrint, &QAction::triggered, this, &MyNotepad::print);
    connect(ui->actionExit, &QAction::triggered, this, &MyNotepad::exit);
    connect(ui->actionCopy, &QAction::triggered, this, &MyNotepad::copy);
    connect(ui->actionCut, &QAction::triggered, this, &MyNotepad::cut);
    connect(ui->actionPaste, &QAction::triggered, this, &MyNotepad::paste);
    connect(ui->actionUndo, &QAction::triggered, this, &MyNotepad::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &MyNotepad::redo);
    connect(ui->actionFont, &QAction::triggered, this, &MyNotepad::selectFont);
    connect(ui->actionBold, &QAction::triggered, this, &MyNotepad::setFontBold);
    connect(ui->actionUnderline, &QAction::triggered, this, &MyNotepad::setFontUnderline);
    connect(ui->actionItalic, &QAction::triggered, this, &MyNotepad::setFontItalic);
    connect(ui->actionAbout, &QAction::triggered, this, &MyNotepad::about);

    QToolBar *toolbar = addToolBar("Main Toolbar");

    QAction *secureModeAction = new QAction("🔒 Secure Mode", this);
    secureModeAction->setCheckable(true);
    secureModeAction->setToolTip("Toggle automatic encryption on save");
    secureModeAction->setShortcut(QKeySequence("Ctrl+M"));

    connect(secureModeAction, &QAction::toggled, this, [=](bool enabled)
            {
    secureModeEnabled = enabled;
    QString msg = enabled ? "Secure Mode ENABLED.\nAll saves will be encrypted." 
                          : "Secure Mode DISABLED.\nNormal saves will be used.";
    QMessageBox::information(this, "Secure Mode", msg); });

    toolbar->addAction(secureModeAction);

    QAction *openEncAction = new QAction("Open Encrypted", this);
    QAction *saveEncAction = new QAction("Save as Encrypted", this);

    openEncAction->setShortcut(QKeySequence("Ctrl+Shift+O"));
    saveEncAction->setShortcut(QKeySequence("Ctrl+Shift+S"));

    connect(openEncAction, &QAction::triggered, this, &MyNotepad::openEncrypted);
    connect(saveEncAction, &QAction::triggered, this, &MyNotepad::saveEncrypted);

    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(ui->actionNew);
    fileMenu->addAction(ui->actionOpen);
    fileMenu->addAction(openEncAction);
    fileMenu->addAction(saveEncAction);
    fileMenu->addAction(ui->actionExit);

#if !QT_CONFIG(printer)
    ui->actionPrint->setEnabled(false);
#endif
#if !QT_CONFIG(clipboard)
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
#endif
}
MyNotepad::~MyNotepad()
{
    delete ui;
}
void MyNotepad::setText(const QString &text)
{
    ui->textEdit->setPlainText(text);
}

void MyNotepad::newDocument()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
}

void MyNotepad::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void MyNotepad::save()
{
    QString fileName;

    // --------------------------------------------------------------If no filename, ask the user
    if (currentFile.isEmpty())
    {
        fileName = QFileDialog::getSaveFileName(this, "Save File");
        currentFile = fileName;
    }
    else
    {
        fileName = currentFile;
    }

    if (fileName.isEmpty())
        return;

    //--------------------------------------------------------------- If secure mode is ON, encrypt instead of plain save
    if (secureModeEnabled)
    {
        QString password = QInputDialog::getText(this, "Encryption Password",
                                                 "Enter password:", QLineEdit::Password);
        if (password.isEmpty())
            return;

        QTemporaryFile tempFile;
        if (!tempFile.open())
        {
            QMessageBox::warning(this, "Error", "Cannot create temporary file.");
            return;
        }

        QTextStream out(&tempFile);
        out << ui->textEdit->toPlainText();
        out.flush();
        tempFile.close();

        if (aes_encrypt_file(tempFile.fileName().toStdString().c_str(),
                             fileName.toStdString().c_str(),
                             password.toStdString().c_str()))
        {
            QMessageBox::information(this, "Secure Save",
                                     "File saved securely (AES Encrypted).");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Encryption failed!");
        }
        return;
    }

    //---------------------------------------------------------------------- If Secure Mode is off → save normally
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    setWindowTitle(fileName);
    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();
}


//-------------------------------------------------------------------------- Saveas user wants to
void MyNotepad::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void MyNotepad::print()
{
    #if QT_CONFIG(printer)
    QPrinter printDev;
    #if QT_CONFIG(printdialog)
    QPrintDialog dialog(&printDev, this);
    if (dialog.exec() == QDialog::Rejected)
        return;
    #endif // QT_CONFIG(printdialog)
    ui->textEdit->print(&printDev);
    #endif // QT_CONFIG(printer)
}

void MyNotepad::exit()
{
    QCoreApplication::quit();
}

void MyNotepad::copy()
{
    #if QT_CONFIG(clipboard)
    ui->textEdit->copy();
    #endif
}

void MyNotepad::cut()
{
    #if QT_CONFIG(clipboard)
    ui->textEdit->cut();
    #endif
}

void MyNotepad::paste()
{
    #if QT_CONFIG(clipboard)
    ui->textEdit->paste();
    #endif
}

void MyNotepad::undo()
{
    ui->textEdit->undo();
}

void MyNotepad::redo()
{
    ui->textEdit->redo();
}

void MyNotepad::selectFont()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        ui->textEdit->setFont(font);
}

void MyNotepad::setFontUnderline(bool underline)
{
    ui->textEdit->setFontUnderline(underline);
}

void MyNotepad::setFontItalic(bool italic)
{
    ui->textEdit->setFontItalic(italic);
}

void MyNotepad::setFontBold(bool bold)
{
    bold ? ui->textEdit->setFontWeight(QFont::Bold) : ui->textEdit->setFontWeight(QFont::Normal);
}

void MyNotepad::about()
{
    QMessageBox::about(this, tr("About Developers"),
                       tr("Made By Ankit"));
}

// ---------------------------------------------------------------------------Encryption
void MyNotepad::saveEncrypted()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Encrypted File", "", "Encrypted Files (*.enc)");
    if (filename.isEmpty())
        return;

    QString password = QInputDialog::getText(this, "Encryption Password", "Enter password:", QLineEdit::Password);
    if (password.isEmpty())
        return;

    QTemporaryFile tempFile;
    if (!tempFile.open())
    {
        QMessageBox::warning(this, "Error", "Cannot create temporary file.");
        return;
    }

    QTextStream out(&tempFile);
    out << ui->textEdit->toPlainText();
    out.flush();
    tempFile.close();

    if (aes_encrypt_file(tempFile.fileName().toStdString().c_str(),
                         filename.toStdString().c_str(),
                         password.toStdString().c_str()))
    {
        QMessageBox::information(this, "Success", "File encrypted and saved successfully!");
    }
    else
    {
        QMessageBox::warning(this, "Error", "Encryption failed!");
    }
}



void MyNotepad::openEncrypted()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Encrypted File", "", "Encrypted Files (*.enc)");
    if (filename.isEmpty())
        return;

    QString password = QInputDialog::getText(this, "Decryption Password", "Enter password:", QLineEdit::Password);
    if (password.isEmpty())
        return;

    QTemporaryFile tempFile;
    if (!tempFile.open())
    {
        QMessageBox::warning(this, "Error", "Cannot create temporary file.");
        return;
    }
    tempFile.close();

    if (aes_decrypt_file(filename.toStdString().c_str(),
                         tempFile.fileName().toStdString().c_str(),
                         password.toStdString().c_str()))
    {

        QFile file(tempFile.fileName());
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            ui->textEdit->setPlainText(in.readAll());
            file.close();
            QMessageBox::information(this, "Success", "File decrypted and loaded successfully!");
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Decryption failed! Possibly wrong password.");
    }
}
