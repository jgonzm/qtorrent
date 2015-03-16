#include <QDebug>

#include "consolewindow.h"

int allocation_mode = libtorrent::storage_mode_sparse;
/*
if (strcmp(arg, "allocate") == 0) allocation_mode = storage_mode_allocate;
else if (strcmp(arg, "full") == 0) allocation_mode = storage_mode_allocate;
else if (strcmp(arg, "sparse") == 0) allocation_mode = storage_mode_sparse;
*/

ConsoleWindow::ConsoleWindow()
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateState()));
    timer.start(2000);
}

ConsoleWindow::~ConsoleWindow()
{
    s.stop_dht();
    s.remove_torrent(handle);
}

bool ConsoleWindow::start(CommandLineParseResult *inputparams)
{
    this->cmd = inputparams;

    session_settings settings;
    settings.disk_cache_algorithm = session_settings::lru;
    s.set_settings(settings);

    s.set_local_upload_rate_limit(cmd->outkb.toInt());
    error_code ec;
    s.listen_on(std::make_pair(55535, 65535), ec);
    if (ec)
    {
        qDebug() << "failed to open listen socket:" << ec.message().c_str();
        return false;
    }
    s.start_dht();

    p.save_path = "./";
    if (!cmd->input.isEmpty()) {
        p.ti = new torrent_info(cmd->input.toLocal8Bit().data(), ec);
        if (ec) {
            qDebug() << ec.message().c_str();
            return false;
        }
        p.storage_mode = (storage_mode_t)allocation_mode;
        handle = s.add_torrent(p, ec);
        handle.set_sequential_download(true);
    } else if (!cmd->input_magnet.isEmpty()) {
        p.url = cmd->input_magnet.toStdString().c_str();
        p.storage_mode = (storage_mode_t)allocation_mode;
        parse_magnet_uri(cmd->input_magnet.toStdString().c_str(), p, ec);
        torrent_handle a = add_magnet_uri(s, cmd->input_magnet.toStdString(), p, ec);
        s.add_torrent(p, ec);
    }

    if (ec) {
        qDebug() << ec.message().c_str();
        return false;
    }

    if (handle.is_valid()) {
        torrent_info ti = handle.get_torrent_info();
        showInfo(ti);
    }

    return true;
}

bool ConsoleWindow::addTorrent()
{
    return true;
}

void ConsoleWindow::updateState()
{
    if (!handle.is_valid()) {
        if (s.get_torrents().size()) {
            handle = s.get_torrents().at(0);
            handle.set_sequential_download(true);
        }
        return;
    }

    if (handle.status().state == torrent_status::downloading) {
        printing();
        return;
    }

    if (handle.status().state == status.state)
        return;

    status = handle.status();

    switch (handle.status().state)
    {
    case torrent_status::downloading:
        printing();
        break;
    case torrent_status::downloading_metadata:
        qDebug() << "Downloading metadata";
        break;
    case torrent_status::finished:
        qDebug() << "finished";
        break;
    case torrent_status::seeding:
        qDebug() << "seeding";
        break;
    case torrent_status::allocating:
        qDebug() << "allocating";
        break;
    case torrent_status::checking_files:
        qDebug() << "checking_files";
        break;
    case torrent_status::checking_resume_data:
        qDebug() << "checking_resume_data";
        break;
    default:
        qDebug() << handle.status().state;
        break;
    }
}

void ConsoleWindow::printing()
{
    double download_rate = s.status().download_rate / 1024;
    if (download_rate == 0) return;
    double total_size = handle.get_torrent_info().total_size()/ 1024;

    std::vector<size_type> progress;
    handle.file_progress(progress);
    unsigned long download_total = 0;
    for (int x=0; x < (int)progress.size(); x++)
        download_total += progress.at(x);
    download_total /= 1024;

    double size_remain = total_size - download_total;
    unsigned long time_remain = 0;
    if (download_rate > 0.1)
        time_remain = (size_remain / download_rate) / 60;
    QString time_units = "min.";
    if (!time_remain) time_remain = 0;
    if (time_remain > 60) {
        time_remain /= 60;
        time_units = "h.";
    }

    double percent = roundf( (download_total/total_size * 100)*10 / 10 );

    qDebug() << percent << "%" << " .... .... .... .... .... " << download_rate << "Kb"
                << time_remain << time_units.toLocal8Bit().data()
                << "(" << s.status().num_peers << ")";
}

void ConsoleWindow::showInfo(torrent_info &ti)
{
    qDebug() << "";
    qDebug() << "Name:    "<< ti.name().data();
    qDebug() << "Size:    " << ti.total_size() / 1048576 << "Mb.";
    qDebug() << "Comments:" << ti.comment().data();
    qDebug() << "Trackers:";
    for (std::vector<announce_entry>::const_iterator i = ti.trackers().begin(); i != ti.trackers().end(); ++i)
    {
      qDebug() <<  "         " << i->url.c_str();
    }
    qDebug() << "Files:";
    for (int x=0; x < ti.num_files(); x++)
        qDebug() << "         " << ti.files().file_name(x).data() << ti.files().file_size(x) / 1048576 << "Mb.";
    qDebug() << "";
}
