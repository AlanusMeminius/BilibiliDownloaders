#include <QUuid>

#include "AriaClient.h"
#include "loger/Loger.h"

namespace aria2net
{

const std::string GetRpcUri(int listenPort)
{
    return QString("http://localhost:%1/jsonrpc").arg(listenPort).toStdString();
}


AriaClient::AriaClient(QObject *parent)
    : QObject(parent)

{

}

AriaClient::~AriaClient()
{
}

SystemListNotifications AriaClient::listNotificationsAsync()
{
    AriaSendData ariaSend;
    QUuid id = QUuid::createUuid();
    ariaSend.SetId(id.toString().toStdString());
    ariaSend.SetJsonrpc(JSONRPC);
    ariaSend.SetMethod("system.listNotifications");

    return GetResult<SystemListNotifications>(ariaSend);
}

SystemListMethods AriaClient::ListMethodsAsync()
{
    AriaSendData ariaSend;
    QUuid id = QUuid::createUuid();
    ariaSend.SetId(id.toString().toStdString());
    ariaSend.SetJsonrpc(JSONRPC);
    ariaSend.SetMethod("system.listMethods");

    return GetResult<SystemListMethods>(ariaSend);
}

std::list<SystemMulticall> AriaClient::MulticallAsync(const std::list<SystemMulticallMathod>& systemMulticallMathods)
{
    std::list<std::string> listMethod;
    for (const auto method : systemMulticallMathods)
    {
        listMethod.emplace_back(method.toString());
    }

    AriaSendData ariaSend;
    QUuid id = QUuid::createUuid();
    ariaSend.SetId(id.toString().toStdString());
    ariaSend.SetJsonrpc(JSONRPC);
    ariaSend.SetMethod("system.multicall");
    ariaSend.SetParams(listMethod);

    nlohmann::json jsonResult = GetResult<nlohmann::json>(ariaSend);
    std::list<SystemMulticall> result;
    for (const auto& sigleJson : jsonResult)
    {
        result.emplace_back(sigleJson);
    }

    return result;
}

AriaSaveSession AriaClient::SaveSessionAsync()
{
    ListString listString;
    listString.emplace_back(std::string("token:") + TOKEN);

    AriaSendData ariaSend;
    QUuid id = QUuid::createUuid();
    ariaSend.SetId(id.toString().toStdString());
    ariaSend.SetJsonrpc(JSONRPC);
    ariaSend.SetMethod("aria2.saveSession");
    ariaSend.SetParams(listString);
    return GetResult<AriaSaveSession>(ariaSend);
}

AriaShutdown AriaClient::ForceShutdownAsync()
{
    ListString listString;
    listString.emplace_back(std::string("token:") + TOKEN);

    AriaSendData ariaSend;
    QUuid id = QUuid::createUuid();
    ariaSend.SetId(id.toString().toStdString());
    ariaSend.SetJsonrpc(JSONRPC);
    ariaSend.SetMethod("aria2.forceShutdown");
    ariaSend.SetParams(listString);

    return GetResult<AriaShutdown>(ariaSend);
}

AriaShutdown AriaClient::ShutdownAsync()
{
    ListString listString;
    listString.emplace_back(std::string("token:") + TOKEN);

    AriaSendData ariaSend;
    QUuid id = QUuid::createUuid();
    ariaSend.SetId(id.toString().toStdString());
    ariaSend.SetJsonrpc(JSONRPC);
    ariaSend.SetMethod("aria2.shutdown");
    ariaSend.SetParams(listString);

    return GetResult<AriaShutdown>(ariaSend);
}

AriaGetSessionInfo AriaClient::GetSessionInfoAsync()
{
    ListString listString;
    listString.emplace_back(std::string("token:") + TOKEN);

    AriaSendData ariaSend;
    QUuid id = QUuid::createUuid();
    ariaSend.SetId(id.toString().toStdString());
    ariaSend.SetJsonrpc(JSONRPC);
    ariaSend.SetMethod("aria2.getSessionInfo");
    ariaSend.SetParams(listString);

    return GetResult<AriaGetSessionInfo>(ariaSend);
}

AriaVersion AriaClient::GetAriaVersionAsync()
{
    ListString listString;
    listString.emplace_back(std::string("token:") + TOKEN);

    AriaSendData ariaSend;
    QUuid id = QUuid::createUuid();
    ariaSend.SetId(id.toString().toStdString());
    ariaSend.SetJsonrpc(JSONRPC);
    ariaSend.SetMethod("aria2.getVersion");
    ariaSend.SetParams(listString);

    return GetResult<AriaVersion>(ariaSend);
}

AriaRemove AriaClient::RemoveDownloadResultAsync(const std::string& gid)
{
    ListString listString;
    listString.emplace_back(std::string("token:") + TOKEN);
    listString.emplace_back(gid);

    AriaSendData ariaSend;
    QUuid id = QUuid::createUuid();
    ariaSend.SetId(id.toString().toStdString());
    ariaSend.SetJsonrpc(JSONRPC);
    ariaSend.SetMethod("aria2.removeDownloadResult");
    ariaSend.SetParams(listString);

    return GetResult<AriaRemove>(ariaSend);
}

AriaRemove AriaClient::PurgeDownloadResultAsync()
{
    ListString listString;
    listString.emplace_back(std::string("token:") + TOKEN);

    AriaSendData ariaSend;
    QUuid id = QUuid::createUuid();
    ariaSend.SetId(id.toString().toStdString());
    ariaSend.SetJsonrpc(JSONRPC);
    ariaSend.SetMethod("aria2.purgeDownloadResult");
    ariaSend.SetParams(listString);

    return GetResult<AriaRemove>(ariaSend);
}

AriaGetGlobalStat AriaClient::GetGlobalStatAsync()
{
    ListString listString;
    listString.emplace_back(std::string("token:") + TOKEN);

    AriaSendData ariaSend;
    QUuid id = QUuid::createUuid();
    ariaSend.SetId(id.toString().toStdString());
    ariaSend.SetJsonrpc(JSONRPC);
    ariaSend.SetMethod("aria2.getGlobalStat");
    ariaSend.SetParams(listString);

    return GetResult<AriaGetGlobalStat>(ariaSend);
}

AriaChangeOption AriaClient::ChangeGlobalOptionAsync(const ListString& option)
{
    ListString listString;
    listString.emplace_back(std::string("token:") + TOKEN);
    listString.insert(listString.end(), option.begin(), option.end());

    AriaSendData ariaSend;
    QUuid id = QUuid::createUuid();
    ariaSend.SetId(id.toString().toStdString());
    ariaSend.SetJsonrpc(JSONRPC);
    ariaSend.SetMethod("aria2.getGlobalStat");
    ariaSend.SetParams(listString);

    return GetResult<AriaChangeOption>(ariaSend);
}

AriaGetOption AriaClient::GetGlobalOptionAsync()
{
    ListString listString;
    listString.emplace_back(std::string("token:") + TOKEN);

    AriaSendData ariaSend;
    QUuid id = QUuid::createUuid();
    ariaSend.SetId(id.toString().toStdString());
    ariaSend.SetJsonrpc(JSONRPC);
    ariaSend.SetMethod("aria2.getGlobalOption");
    ariaSend.SetParams(listString);

    return GetResult<AriaGetOption>(ariaSend);
}

AriaChangeOption AriaClient::ChangeOptionAsync(const std::string& gid, const ListString& option)
{
    ListString listString;
    listString.emplace_back(std::string("token:") + TOKEN);
    listString.emplace_back(gid);
    listString.insert(listString.end(), option.begin(), option.end());

    AriaSendData ariaSend;
    QUuid id = QUuid::createUuid();
    ariaSend.SetId(id.toString().toStdString());
    ariaSend.SetJsonrpc(JSONRPC);
    ariaSend.SetMethod("aria2.getGlobalStat");
    ariaSend.SetParams(listString);

    return GetResult<AriaChangeOption>(ariaSend);
}

std::string AriaClient::Request(const std::string& url, const std::string& param)
{
    std::string strResponse;
    CNetWork::GetInstance().HttpPost(url, param, strResponse);
    return strResponse;
}

} // namespace aria2net
