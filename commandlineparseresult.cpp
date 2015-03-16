#include <QCommandLineParser>
#include "commandlineparseresult.h"

CommandLineParseResult::CommandLineParseResult()
{
}

int CommandLineParseResult::parseCommandLine()
{
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);

    const QCommandLineOption inputOption("i", "The torrent file.", "input");
    parser.addOption(inputOption);

    const QCommandLineOption inputMagnetOption("m", "The magnet torrent.", "input");
    parser.addOption(inputMagnetOption);

    const QCommandLineOption outputOption("o", "Output directory.", "output");
    parser.addOption(outputOption);

    const QCommandLineOption updateOption("u", "Max. kb/s up", "outkb");
    parser.addOption(updateOption);

    parser.addPositionalArgument("name", "The name to look up.");
    const QCommandLineOption helpOption = parser.addHelpOption();
    const QCommandLineOption versionOption = parser.addVersionOption();

    if (!parser.parse(QCoreApplication::arguments())) {
        return CommandLineError;
    }

    if (parser.isSet(versionOption)) {
        versionRequested();
        return CommandLineVersionRequested;
    }

    if (parser.isSet(helpOption)) {
        helpRequested();
        return CommandLineHelpRequested;
    }

    if (parser.isSet(updateOption)) {
        outkb = parser.value(updateOption);
    }

    if (parser.isSet(inputMagnetOption)) {
        input_magnet = parser.value(inputMagnetOption);
    } else {
        if (parser.isSet(inputOption)) {
            input = parser.value(inputOption);
        } else {
            if (parser.positionalArguments().size() <= 0) {
                this->helpRequested();
                return CommandLineError;
            } else {
                input = parser.positionalArguments().at(0);
            }
        }
    }

    if (parser.isSet(outputOption)) {
        output = parser.value(outputOption);
    } else {
        output = "./";
    }

    return CommandLineOk;
}

void CommandLineParseResult::helpRequested()
{
    parser.showHelp();
}

void CommandLineParseResult::versionRequested()
{
    printf("%s %s\n", qPrintable(QCoreApplication::applicationName()),
                   qPrintable(QCoreApplication::applicationVersion()));
}

void CommandLineParseResult::commandLineError(QString errorMessage)
{
    fputs(qPrintable(errorMessage), stderr);
    fputs("\n\n", stderr);
    fputs(qPrintable(parser.helpText()), stderr);
}

