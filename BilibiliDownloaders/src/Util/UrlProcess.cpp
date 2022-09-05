#include "UrlProcess.h"

#include <QString>
#include <QStringList>

bool util::UrlProcess::IsUrl(const QString& text)
{
    return text.startsWith("http://") || text.startsWith("https://");
}

QString util::UrlProcess::DeleteUrlParam(const QString& url)
{
    QStringList strList = url.split("?");
    return strList[0].endsWith("/") ? strList[0].remove(strList[0].size() - 1, 1) : strList[0];
}

QString util::UrlProcess::GetId(const QString& url)
{
    QString tempUrl = EnableHttps(url);
    tempUrl = DeleteUrlParam(url);

    return tempUrl.replace(QString::fromLocal8Bit(VideoUrl.data()), "");
}

QString util::UrlProcess::EnableHttps(const QString& url)
{
    QString result = url;
    if (IsUrl(url))
    {
        return result.replace("htt[://", "https://");
    }
    return {};
}
