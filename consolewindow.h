#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QObject>
#include <QTimer>

#define DBOOST_ASIO_HASH_MAP_BUCKETS 1021
#define BOOST_EXCEPTION_DISABLE 1
#define BOOST_ASIO_DYN_LINK 0

#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/storage.hpp"
#include "libtorrent/file_storage.hpp"
#include "libtorrent/magnet_uri.hpp"
#include "commandlineparseresult.h"

using namespace libtorrent;

class ConsoleWindow : public QObject
{
    Q_OBJECT
public:
    ConsoleWindow();
    bool start(CommandLineParseResult *inputparams);
    bool addTorrent();

private:
    CommandLineParseResult *cmd;
    QTimer timer;
    session s;
    add_torrent_params p;
    torrent_handle handle;
    torrent_status status;

    void printing();

private slots:
    void showInfo(torrent_info &t);
    void updateState();

};

#endif // CONSOLEWINDOW_H
