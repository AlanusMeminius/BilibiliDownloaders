#pragma once
#pragma execution_character_set("utf-8")

#include <QObject>

#include <nlohmann/json.hpp>

#include "NetWork/CNetWork.h"
#include "Aria2Net/Protocol/Protocol.h"

namespace aria2net
{
const std::string GetRpcUri(int listenPort = 6800);

class AriaClient : public QObject
{
    Q_OBJECT

    using ListString = std::list<std::string>;
public:
    AriaClient(QObject* parent = nullptr);
    ~AriaClient();

    SystemListNotifications listNotificationsAsync();
    SystemListMethods ListMethodsAsync();
    std::list<SystemMulticall> MulticallAsync(const std::list<SystemMulticallMathod>& systemMulticallMathods);
    AriaSaveSession SaveSessionAsync();
    AriaShutdown ForceShutdownAsync();
    AriaShutdown ShutdownAsync();
    AriaGetSessionInfo GetSessionInfoAsync();
    AriaVersion GetAriaVersionAsync();
    AriaRemove RemoveDownloadResultAsync(const std::string& gid);
    AriaRemove PurgeDownloadResultAsync();
    AriaGetGlobalStat GetGlobalStatAsync();
    AriaChangeOption ChangeGlobalOptionAsync(const ListString& option);
    AriaGetOption GetGlobalOptionAsync();
    AriaChangeOption ChangeOptionAsync(const std::string& gid, const ListString& option);


private:
    static constexpr char JSONRPC[] = "2.0";
    static constexpr char TOKEN[] = "downkyi";

    template<typename Result>
    Result GetResult(const AriaSendData& sendData);

public:
    std::string Request(const std::string& url, const std::string& params);
};

template<typename Result>
inline Result AriaClient::GetResult(const AriaSendData& sendData)
{
    std::string strParams = sendData.toString();
    std::string strResponse = Request(GetRpcUri(), strParams);
    nlohmann::json result = nlohmann::json::parse(strResponse);
    return Result(result);
}

} // namespace aria2net
