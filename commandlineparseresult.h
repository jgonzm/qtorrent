#ifndef COMMANDLINEPARSERESULT_H
#define COMMANDLINEPARSERESULT_H

#include <QObject>
#include <QString>
#include <QCommandLineParser>

#define CommandLineOk               0
#define CommandLineError            1
#define CommandLineVersionRequested 2
#define CommandLineHelpRequested    3

class CommandLineParseResult : public QObject
{
public:
    CommandLineParseResult();
    QString input;
    QString input_magnet;
    QString output;
    QString outkb;


    int parseCommandLine();

private:
    QCommandLineParser parser;

    void helpRequested();
    void versionRequested();
    void commandLineError(QString errorMessage);
};

#endif // COMMANDLINEPARSERESULT_H
