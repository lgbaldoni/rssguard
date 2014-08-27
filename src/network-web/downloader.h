// This file is part of RSS Guard.
//
// Copyright (C) 2011-2014 by Martin Rotter <rotter.martinos@gmail.com>
//
// RSS Guard is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RSS Guard is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RSS Guard. If not, see <http://www.gnu.org/licenses/>.

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>

#include "definitions/definitions.h"

#include <QNetworkReply>
#include <QSslError>


class SilentNetworkAccessManager;
class QTimer;

class Downloader : public QObject {
    Q_OBJECT

  public:
    // Constructors and destructors.
    explicit Downloader(QObject *parent = 0);
    virtual ~Downloader();

    // Access to last received full output data/error.
    QByteArray lastOutputData() const;
    QNetworkReply::NetworkError lastOutputError() const;

  public slots:
    // Performs asynchronous download of given file. Redirections are handled.
    void downloadFile(const QString &url, int timeout = DOWNLOAD_TIMEOUT, bool protected_contents = false,
                      const QString &username = QString(), const QString &password = QString());

  signals:
    // Emitted when new progress is known.
    void progress(qint64 bytes_received, qint64 bytes_total);
    void completed(QNetworkReply::NetworkError status, QByteArray contents = QByteArray());

  private slots:
    // Called when current reply is processed.
    void finished(QNetworkReply *reply);

    // Called when progress of downloaded file changes.
    void progressInternal(qint64 bytes_received, qint64 bytes_total);

    // Called when current operation times out.
    void timeout();

  private:
    void runGetRequest(const QNetworkRequest &request);

  private:
    QNetworkReply *m_activeReply;
    SilentNetworkAccessManager *m_downloadManager;
    QTimer *m_timer;

    QByteArray m_lastOutputData;
    QNetworkReply::NetworkError m_lastOutputError;
};

#endif // DOWNLOADER_H