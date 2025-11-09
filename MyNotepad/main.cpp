#include "mynotepad.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QString>

int main(int argc, char *argv[]) {
    QApplication EditorApp(argc, argv);
    MyNotepad Editor;

    if (argc > 1) {
        QString filePath = argv[1];
        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString content = in.readAll();
            file.close();

            Editor.setWindowTitle(filePath);      
            Editor.setText(content);          
        } else {
            Editor.setWindowTitle("Failed to open file: " + filePath);
        }
    }

    Editor.show();
    return EditorApp.exec();
}
