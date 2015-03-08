#include <QCoreApplication>
#include <QDebug>

#include "consolewindow.h"
#include "commandlineparseresult.h"

#define APP_VERSION   "1.0"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationVersion(APP_VERSION);

    CommandLineParseResult cmd;
    if (cmd.parseCommandLine())
        return -1;

    ConsoleWindow console;
    console.start(&cmd);

    return a.exec();
}
