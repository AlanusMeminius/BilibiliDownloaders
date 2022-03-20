#pragma once

#include <nlohmann/json.hpp>
#include <vector>

namespace aria2net
{
// 协议基类
class Protocol
{
public:
    Protocol() = default;
    Protocol(nlohmann::json json) : m_json(std::move(json)) {}
    virtual ~Protocol() = default;
    virtual const std::string toString() { m_json.dump(); };
    virtual void request() {};

    void SetJson(const nlohmann::json& json) { m_json = json; }
    const nlohmann::json& GetJson() const { return m_json; }

protected:
    nlohmann::json m_json;
};

// 支持json转任意类型
template<typename ProtocolType,
         typename std::enable_if<std::is_convertible<const ProtocolType&, Protocol>::value, int>::type = 0>
void to_json(nlohmann::json& j, const ProtocolType& p) {
    j = p.GetJson();
}

template<typename ProtocolType, 
         typename std::enable_if<std::is_convertible<const ProtocolType&, Protocol>::value, int>::type = 0>
void from_json(const nlohmann::json& j, ProtocolType& p) {
    p.SetJson(j);
}

// 错误协议
class AriaError : public Protocol
{
public:
    AriaError() = default;
    AriaError(nlohmann::json json) : Protocol(std::move(json)) { }
    ~AriaError() = default;

    // const std::string toString() override { m_json.dump(); }
    void request() override {};

    void SetCode(int code) { m_json["code"] = code; }
    const int GetCode() const { return m_json["code"]; };

    void SetMessage(const std::string& message) { m_json["message"] = message; }
    const std::string& GetMessage() const { return m_json["message"]; };
};

// Uri协议
class AriaUri : public Protocol
{
public:
    AriaUri() = default;
    AriaUri(nlohmann::json json) : Protocol(std::move(json)) { }
    ~AriaUri() = default;

    // const std::string toString() override { m_json.dump(); }
    void request() override {};

    void SetStatus(const std::string& status) { m_json["status"] = status; }
    const int GetStatus() const { return m_json["status"]; };

    void SetUri(const std::string& message) { m_json["uri"] = message; }
    const std::string& GetUri() const { return m_json["uri"]; };
};

// 与Aria通信的基本协议
template<typename Result>
class AriaBasicJson : public Protocol
{
public:
    AriaBasicJson() = default;;
    AriaBasicJson(nlohmann::json json) : Protocol(std::move(json)) { }
    ~AriaBasicJson() = default;

    const std::string toString() override { m_json.dump(); }
    void request() override {};

    void SetId(const std::string& id) { m_json["id"] = id; }
    const std::string& GetId() const { return m_json["id"]; };

    void SetJsonrpc(const std::string& jsonrpc) { m_json["jsonrpc"] = jsonrpc; }
    const std::string& GetJsonrpc() const { return m_json["jsonrpc"]; };

    void SetResult(const Result& result) { m_json["result"] = result; }
    const Result& GetResult() const { return m_json["result"]; };

    void SetError(const AriaError& error) { m_json["error"] = error; }
    const AriaError& GetError() const { return m_json["error"]; };
};

using AriaAddMetalink = AriaBasicJson<std::string>;
using AriaAddTorrent = AriaBasicJson<std::string>;
using AriaAddUri = AriaBasicJson<std::string>;
using AriaChangeOption = AriaBasicJson<std::string>;
using AriaChangePosition = AriaBasicJson<std::string>;
using AriaChangeUri = AriaBasicJson<std::list<int>>;

using AriaGetFiles = AriaBasicJson<std::list<AriaUri>>;
class AriaGetFilesResult : public Protocol
{
public:
    AriaGetFilesResult() = default;
    AriaGetFilesResult(nlohmann::json json) : Protocol(std::move(json)) { }
    ~AriaGetFilesResult() = default;

    const std::string toString() override { m_json.dump(); }
    void request() override {};

    void SetCompletedLength(const std::string& id) { m_json["completedLength"] = id; }
    const std::string& GetcompletedLength() const { return m_json["completedLength"]; };

    void SetIndex(const std::string& index) { m_json["index"] = index; }
    const std::string& GetIndex() const { return m_json["index"]; };

    void SetLength(const std::string& length) { m_json["length"] = length; }
    const std::string& GetLength() const { return m_json["length"]; };

    void SetPath(const AriaError& path) { m_json["path"] = path; }
    const AriaError& GetPath() const { return m_json["path"]; };

    void SetSelected(const AriaError& selected) { m_json["selected"] = selected; }
    const AriaError& GetSelected() const { return m_json["selected"]; };

    void SetUris(const std::list<AriaUri>& uris) { m_json["uris"] = uris; }
    const std::list<AriaUri>& GetUris() const { return m_json["uris"]; };
};

class AriaGetGlobalStatResult : public Protocol
{
public:
    AriaGetGlobalStatResult() = default;
    AriaGetGlobalStatResult(nlohmann::json json) : Protocol(std::move(json)) { }
    ~AriaGetGlobalStatResult() = default;

    const std::string toString() override { m_json.dump(); }
    void request() override {};

    void SetDownloadSpeed(const std::string& downloadSpeed) { m_json["downloadSpeed"] = downloadSpeed; }
    const std::string& GetDownloadSpeed() const { return m_json["downloadSpeed"]; };

    void SetNumActive(const std::string& numActive) { m_json["numActive"] = numActive; }
    const std::string& GetNumActive() const { return m_json["numActive"]; };

    void SetNumStopped(const std::string& numStopped) { m_json["numStopped"] = numStopped; }
    const std::string& GetNumStopped() const { return m_json["numStopped"]; };

    void SetNumStoppedTotal(const AriaError& numStoppedTotal) { m_json["numStoppedTotal"] = numStoppedTotal; }
    const AriaError& GetNumStoppedTotal() const { return m_json["numStoppedTotal"]; };

    void SetNumWaiting(const AriaError& numWaiting) { m_json["numWaiting"] = numWaiting; }
    const AriaError& GetNmWaiting() const { return m_json["numWaiting"]; };

    void SetUploadSpeed(const AriaError& uploadSpeed) { m_json["uploadSpeed"] = uploadSpeed; }
    const AriaError& GetuUploadSpeed() const { return m_json["uploadSpeed"]; };

};
using AriaGetGlobalStat = AriaBasicJson<AriaGetGlobalStatResult>;


class AriaOption : public Protocol
{
public:
    AriaOption() = default;
    AriaOption(nlohmann::json json) : Protocol(std::move(json)) { }
    ~AriaOption() = default;

    const std::string toString() override { m_json.dump(); }
    void request() override {};

    void SetAllProxy(const std::string& allProxy) { m_json["all-proxy"] = allProxy; }
    const std::string& GetAllProxy() const { return m_json["all-proxy"]; }

    void SetAllowOverwrite(const std::string& allowOverwrite) { m_json["allow-overwrite"] = allowOverwrite; }
    const std::string& GetAllowOverwrite() const { return m_json["allow-overwrite"]; }

    void SetAllowPieceLengthChange(const std::string& allowPieceLengthChange) { m_json["allow-piece-length-change"] = allowPieceLengthChange; }
    const std::string& GetAllowPieceLengthChange() const { return m_json["allow-piece-length-change"]; }

    void SetAlwaysResumel(const AriaError& alwaysResume) { m_json["always-resume"] = alwaysResume; }
    const AriaError& GetAlwaysResume() const { return m_json["always-resume"]; }

    void SetAsyncDns(const std::string& asyncDns) { m_json["async-dns"] = asyncDns; }
    const std::string& GetAsyncDns() const { return m_json["async-dns"]; }

    void SetAutoFileRenaming(const std::string& autoFileRenaming) { m_json["auto-file-renaming"] = autoFileRenaming; }
    const std::string& GetAutoFileRenaming() const { return m_json["auto-file-renaming"]; }

    void SetBtEnableHookAfterHashCheck(const std::string& btEnableHookAfterHashCheck) { m_json["bt-enable-hook-after-hash-check"] = btEnableHookAfterHashCheck; }
    const std::string& GetBtEnableHookAfterHashCheck() const { return m_json["bt-enable-hook-after-hash-check"]; };

    void SetBtEnableLpd(const std::string& btEnableLpd) { m_json["bt-enable-lpd"] = btEnableLpd; }
    const std::string& GetBtEnableLpd() const { return m_json["bt-enable-lpd"]; }

    void SetBtForceEncryption(const std::string& btForceEncryption) { m_json["bt-force-encryption"] = btForceEncryption; }
    const std::string& GetBtForceEncryption() const { return m_json["bt-force-encryption"]; }

    void SetBtHashCheckSeed(const std::string& btHashCheckSeed) { m_json["bt-hash-check-seed"] = btHashCheckSeed; }
    const std::string& GetBtHashCheckSeed() const { return m_json["bt-hash-check-seed"]; }

    void SetBtLoadSavedMetadata(const std::string& btLoadSavedMetadata) { m_json["bt-load-saved-metadata"] = btLoadSavedMetadata; }
    const std::string& GetBtLoadSavedMetadata() const { return m_json["bt-load-saved-metadata"]; }

    void SetBtMaxPeers(const std::string& btMaxPeers) { m_json["bt-max-peers"] = btMaxPeers; }
    const std::string& GetBtMaxPeers() const { return m_json["bt-max-peers"]; }

    void SetBtMetadataOnly(const std::string& btMetadataOnly) { m_json["bt-metadata-only"] = btMetadataOnly; }
    const std::string& GetBtMetadataOnly() const { return m_json["bt-metadata-only"]; }

    void SetBtMinCryptoLevel(const std::string& btMinCryptoLevel) { m_json["bt-min-crypto-level"] = btMinCryptoLevel; }
    const std::string& GetBtMinCryptoLevel() const { return m_json["bt-min-crypto-level"]; }


    void SetBtRemoveUnselectedFile(const std::string& btRemoveUnselectedFile) { m_json["bt-remove-unselected-file"] = btRemoveUnselectedFile; }
    const std::string& GetSetBtRemoveUnselectedFile() const { return m_json["bt-remove-unselected-file"]; }

    void SetBtRequestPeerSpeedLimit(const std::string& btRequestPeerSpeedLimit) { m_json["bt-request-peer-speed-limit"] = btRequestPeerSpeedLimit; }
    const std::string& GetBtRequestPeerSpeedLimit() const { return m_json["bt-request-peer-speed-limit"]; }


    void SetBtRequireCrypto(const std::string& btRequireCrypto) { m_json["bt-require-crypto"] = btRequireCrypto; }
    const std::string& GetBtRequireCrypto() const { return m_json["bt-require-crypto"]; }

    void SetBtSaveMetadata(const std::string& btSaveMetadata) { m_json["bt-save-metadata"] = btSaveMetadata; }
    const std::string& GetBtSaveMetadata() const { return m_json["bt-save-metadata"]; }

    void SetBtSeedUnverified(const std::string& btSeedUnverified) { m_json["bt-seed-unverified"] = btSeedUnverified; }
    const std::string& GetBtSeedUnverified() const { return m_json["bt-seed-unverified"]; }

    void SetBtStopTimeout(const std::string& btStopTimeout) { m_json["bt-stop-timeout"] = btStopTimeout; }
    const std::string& GetBtStopTimeout() const { return m_json["bt-stop-timeout"]; }

    void SetBtRebtTrackerConnectTimeout(const std::string& btRebtTrackerConnectTimeout) { m_json["bt-tracker-connect-timeout"] = btRebtTrackerConnectTimeout; }
    const std::string& GetBtRebtTrackerConnectTimeout() const { return m_json["bt-tracker-connect-timeout"]; }

    void SetBtTrackerInterval(const std::string& btTrackerInterval) { m_json["bt-tracker-interval"] = btTrackerInterval; }
    const std::string& GetBtTrackerInterval() const { return m_json["bt-tracker-interval"]; }

    void SetBtTrackerTimeout(const std::string& btTrackerTimeout) { m_json["bt-tracker-timeout"] = btTrackerTimeout; }
    const std::string& GetBtTrackerTimeout() const { return m_json["bt-tracker-timeout"]; }

    void SetCheckIntegrity(const std::string& checkIntegrity) { m_json["check-integrity"] = checkIntegrity; }
    const std::string& GetCheckIntegrity() const { return m_json["bt-require-crypto"]; }

    void SetConditionalGet(const std::string& conditionalGet) { m_json["conditional-get"] = conditionalGet; }
    const std::string& GetConditionalGet() const { return m_json["conditional-get"]; }

    void SetConnectTimeout(const std::string& connectTimeout) { m_json["connect-timeout"] = connectTimeout; }
    const std::string& GetConnectTimeout() const { return m_json["connect-timeout"]; }

    void SetContinue(const std::string& strContinue) { m_json["continue"] = strContinue; }
    const std::string& GetContinue() const { return m_json["continue"]; }

    void SetDir(const std::string& dir) { m_json["dir"] = dir; }
    const std::string& GetDir() const { return m_json["dir"]; }

    void SetDryRun(const std::string& dryRun) { m_json["dry-run"] = dryRun; }
    const std::string& GetDryRun() const { return m_json["dry-run"]; }

    void SetEnableHttpKeepAlive(const std::string& enableHttpKeepAlive) { m_json["enable-http-keep-alive"] = enableHttpKeepAlive; }
    const std::string& GetEnableHttpKeepAlive() const { return m_json["enable-http-keep-alive"]; }

    void SetEnableHttpPipelining(const std::string& enableHttpPipelining) { m_json["enable-http-pipelining"] = enableHttpPipelining; }
    const std::string& GetEnableHttpPipelining() const { return m_json["enable-http-pipelining"]; }

    void SetEnableMmap(const std::string& enableMmap) { m_json["enable-mmap"] = enableMmap; }
    const std::string& GetEnableMmap() const { return m_json["enable-mmap"]; }

    void SetEnablePeerExchange(const std::string& enablePeerExchange) { m_json["enable-peer-exchange"] = enablePeerExchange; }
    const std::string& GetEnablePeerExchange() const { return m_json["enable-peer-exchange"]; }

    void SetFileAllocation(const std::string& fileAllocation) { m_json["file-allocation"] = fileAllocation; }
    const std::string& GetFileAllocation() const { return m_json["file-allocation"]; }

    void SetFollowMetalink(const std::string& followMetalink) { m_json["follow-metalink"] = followMetalink; }
    const std::string& GetFollowMetalink() const { return m_json["follow-metalink"]; }

    void SetFollowTorrent(const std::string& followTorrent) { m_json["follow-torrent"] = followTorrent; }
    const std::string& GetFollowTorrent() const { return m_json["follow-torrent"]; }

    void SetForceSave(const std::string& forceSave) { m_json["force-save"] = forceSave; }
    const std::string& GetForceSave() const { return m_json["force-save"]; }

    void SetFtpPasv(const std::string& ftpPasv) { m_json["ftp-pasv"] = ftpPasv; }
    const std::string& GetFtpPasv() const { return m_json["ftp-pasv"]; }

    void SetFtpReuseConnection(const std::string& ftpReuseConnection) { m_json["ftp-reuse-connection"] = ftpReuseConnection; }
    const std::string& GetFtpReuseConnection() const { return m_json["follow-metalink"]; }

    void SetFtpType(const std::string& ftpType) { m_json["ftp-type"] = ftpType; }
    const std::string& GetFtpType() const { return m_json["ftp-type"]; }

    void SetHashCheckOnly(const std::string& hashCheckOnly) { m_json["hash-check-only"] = hashCheckOnly; }
    const std::string& GetHashCheckOnly() const { return m_json["hash-check-only"]; }

    void SetHttpAcceptGzip(const std::string& httpAcceptGzip) { m_json["http-accept-gzip"] = httpAcceptGzip; }
    const std::string& GetHttpAcceptGzip() const { return m_json["http-accept-gzip"]; }

    void SetHttpAuthChallenge(const std::string& httpAuthChallenge) { m_json["http-auth-challenge"] = httpAuthChallenge; }
    const std::string& GetHttpAuthChallenge() const { return m_json["http-auth-challenge"]; }

    void SetHttpNoCache(const std::string& httpNoCache) { m_json["http-no-cache"] = httpNoCache; }
    const std::string& GetHttpNoCache() const { return m_json["http-no-cache"]; }

    void SetLowestSpeedLimit(const std::string& lowestSpeedLimit) { m_json["lowest-speed-limit"] = lowestSpeedLimit; }
    const std::string& GetLowestSpeedLimit() const { return m_json["lowest-speed-limit"]; }


    //void SetLowestSpeedLimit(const std::string& lowestSpeedLimit) { m_json["lowest-speed-limit"] = lowestSpeedLimit; }
    //const std::string& GetLowestSpeedLimit() const { return m_json["lowest-speed-limit"]; }

    //void SetLowestSpeedLimit(const std::string& lowestSpeedLimit) { m_json["lowest-speed-limit"] = lowestSpeedLimit; }
    //const std::string& GetLowestSpeedLimit() const { return m_json["lowest-speed-limit"]; }

    //void SetLowestSpeedLimit(const std::string& lowestSpeedLimit) { m_json["lowest-speed-limit"] = lowestSpeedLimit; }
    //const std::string& GetLowestSpeedLimit() const { return m_json["lowest-speed-limit"]; }

    //void SetLowestSpeedLimit(const std::string& lowestSpeedLimit) { m_json["lowest-speed-limit"] = lowestSpeedLimit; }
    //const std::string& GetLowestSpeedLimit() const { return m_json["lowest-speed-limit"]; }

    //void SetLowestSpeedLimit(const std::string& lowestSpeedLimit) { m_json["lowest-speed-limit"] = lowestSpeedLimit; }
    //const std::string& GetLowestSpeedLimit() const { return m_json["lowest-speed-limit"]; }

};

using AriaGetOption = AriaBasicJson<AriaOption>;





    //    [JsonProperty("max-connection-per-server")]
    //public string MaxConnectionPerServer{ get; set; }

    //    [JsonProperty("max-download-limit")]
    //public string MaxDownloadLimit{ get; set; }

    //    [JsonProperty("max-file-not-found")]
    //public string MaxFileNotFound{ get; set; }

    //    [JsonProperty("max-mmap-limit")]
    //public string MaxMmapLimit{ get; set; }

    //    [JsonProperty("max-resume-failure-tries")]
    //public string MaxResumeFailureTries{ get; set; }

    //    [JsonProperty("max-tries")]
    //public string MaxTries{ get; set; }

    //    [JsonProperty("max-upload-limit")]
    //public string MaxUploadLimit{ get; set; }

    //    [JsonProperty("metalink-enable-unique-protocol")]
    //public string MetalinkEnableUniqueProtocol{ get; set; }

    //    [JsonProperty("metalink-preferred-protocol")]
    //public string MetalinkPreferredProtocol{ get; set; }

    //    [JsonProperty("min-split-size")]
    //public string MinSplitSize{ get; set; }

    //    [JsonProperty("no-file-allocation-limit")]
    //public string NoFileAllocationLimit{ get; set; }

    //    [JsonProperty("no-netrc")]
    //public string NoNetrc{ get; set; }

    //    [JsonProperty("out")]
    //public string Out{ get; set; }

    //    [JsonProperty("parameterized-uri")]
    //public string ParameterizedUri{ get; set; }

    //    [JsonProperty("pause-metadata")]
    //public string PauseMetadata{ get; set; }

    //    [JsonProperty("piece-length")]
    //public string PieceLength{ get; set; }

    //    [JsonProperty("proxy-method")]
    //public string ProxyMethod{ get; set; }

    //    [JsonProperty("realtime-chunk-checksum")]
    //public string RealtimeChunkChecksum{ get; set; }

    //    [JsonProperty("remote-time")]
    //public string RemoteTime{ get; set; }

    //    [JsonProperty("remove-control-file")]
    //public string RemoveControlFile{ get; set; }

    //    [JsonProperty("retry-wait")]
    //public string RetryWait{ get; set; }

    //    [JsonProperty("reuse-uri")]
    //public string ReuseUri{ get; set; }

    //    [JsonProperty("rpc-save-upload-metadata")]
    //public string RpcSaveUploadMetadata{ get; set; }

    //    [JsonProperty("save-not-found")]
    //public string SaveNotFound{ get; set; }

    //    [JsonProperty("seed-ratio")]
    //public string SeedRatio{ get; set; }

    //    [JsonProperty("split")]
    //public string Split{ get; set; }

    //    [JsonProperty("stream-piece-selector")]
    //public string StreamPieceSelector{ get; set; }

    //    [JsonProperty("timeout")]
    //public string Timeout{ get; set; }

    //    [JsonProperty("uri-selector")]
    //public string UriSelector{ get; set; }

    //    [JsonProperty("use-head")]
    //public string UseHead{ get; set; }

    //    [JsonProperty("user-agent")]
    //public string UserAgent{ get; set; }

class AriaPeer : public Protocol
{
public:
    AriaPeer() = default;
    AriaPeer(nlohmann::json json) : Protocol(std::move(json)) { }
    ~AriaPeer() = default;

    const std::string toString() override { m_json.dump(); }
    void request() override {};

    void SetAmChoking(const std::string& amChoking) { m_json["amChoking"] = amChoking; }
    const std::string& GetAmChoking() const { return m_json["amChoking"]; }

    void SetBitField(const std::string& bitField) { m_json["bitfield"] = bitField; }
    const std::string& GetBitField() const { return m_json["bitfield"]; }

    void SetDownloadSpeed(const std::string& downloadSpeed) { m_json["downloadSpeed"] = downloadSpeed; }
    const std::string& GetDownloadSpeed() const { return m_json["downloadSpeed"]; }

    void SetIp(const std::string& ip) { m_json["ip"] = ip; }
    const std::string& GetIp() const { return m_json["ip"]; }

    void SetPeerChoking(const std::string& peerChoking) { m_json["peerChoking"] = peerChoking; }
    const std::string& GetPeerChoking() const { return m_json["peerChoking"]; }

    void SetPeerId(const std::string& peerId) { m_json["peerId"] = peerId; }
    const std::string& GetPeerId() const { return m_json["peerId"]; }

    void SetPort(const std::string& port) { m_json["port"] = port; }
    const std::string& GetPort() const { return m_json["port"]; }

    void SetSeeder(const std::string& seeder) { m_json["seeder"] = seeder; }
    const std::string& GetSeeder() const { return m_json["seeder"]; }

    void SeTuploadSpeed(const std::string& uploadSpeed) { m_json["uploadSpeed"] = uploadSpeed; }
    const std::string& GetUploadSpeed() const { return m_json["uploadSpeed"]; }

};
using AriaGetPeers = AriaBasicJson<std::list<AriaPeer>>;
   
class AriaResultServer : public Protocol
{
public:
    AriaResultServer() = default;
    AriaResultServer(nlohmann::json json) : Protocol(std::move(json)) { }
    ~AriaResultServer() = default;

    const std::string toString() override { m_json.dump(); }
    void request() override {};

    void SetCurrentUri(const std::string& currentUri) { m_json["currentUri"] = currentUri; }
    const std::string& GetCurrentUri() const { return m_json["currentUri"]; }

    void SetDownloadSpeed(const std::string& downloadSpeed) { m_json["downloadSpeed"] = downloadSpeed; }
    const std::string& GetDownloadSpeed() const { return m_json["downloadSpeed"]; }

    void SetUri(const std::string& message) { m_json["uri"] = message; }
    const std::string& GetUri() const { return m_json["uri"]; };
};

class AriaGetServersResult : public Protocol
{
public:
    AriaGetServersResult() = default;
    AriaGetServersResult(nlohmann::json json) : Protocol(std::move(json)) { }
    ~AriaGetServersResult() = default;

    const std::string toString() override { m_json.dump(); }
    void request() override {};

    void SetIndex(const std::string& index) { m_json["index"] = index; }
    const std::string& GetIndex() const { return m_json["index"]; }

    void SetServers(const std::list<AriaResultServer>& servers) { m_json["servers"] = servers; }
    const std::list<AriaResultServer>& GetServers() const { return m_json["servers"]; }
};

using AriaGetServers = AriaBasicJson<std::list<AriaGetServersResult>>;

class AriaGetSessionInfoResult : public Protocol
{
public:
    AriaGetSessionInfoResult() = default;
    AriaGetSessionInfoResult(nlohmann::json json) : Protocol(std::move(json)) { }
    ~AriaGetSessionInfoResult() = default;

    const std::string toString() override { m_json.dump(); }
    void request() override {};

    void SetSessionId(const std::string& sessionId) { m_json["sessionId"] = sessionId; }
    const std::string& GetSessionId() const { return m_json["sessionId"]; }

};

using AriaGetSessionInfo = AriaBasicJson<AriaGetSessionInfoResult>;
using AriaPause = AriaBasicJson<std::string>;
using AriaRemove = AriaBasicJson<std::string>; 
using AriaSaveSession = AriaBasicJson<std::string>;
using AriaShutdown = AriaBasicJson<std::string>;
using AriaShutdown = AriaBasicJson<std::string>;

class AriaVersionResult : public Protocol
{
public:
    AriaVersionResult() = default;
    AriaVersionResult(nlohmann::json json) : Protocol(std::move(json)) { }
    ~AriaVersionResult() = default;

    const std::string toString() override { m_json.dump(); }
    void request() override {};

    void SetEnabledFeatures(const std::string& enabledFeatures) { m_json["enabledFeatures"] = enabledFeatures; }
    const std::string& GetEnabledFeatures() const { return m_json["enabledFeatures"]; }

    void SetServers(const std::list<AriaResultServer>& servers) { m_json["servers"] = servers; }
    const std::list<AriaResultServer>& GetServers() const { return m_json["servers"]; }
};
using AriaVersion = AriaBasicJson<AriaVersionResult>;

using SystemListMethods = AriaBasicJson<std::list<std::string>>;
using SystemListNotifications = AriaBasicJson<std::list<std::string>>;
using SystemMulticall = AriaBasicJson<std::string>;
class SystemMulticallMathod : public Protocol
{
public:
    SystemMulticallMathod() = default;
    SystemMulticallMathod(nlohmann::json json) : Protocol(std::move(json)) { }
    ~SystemMulticallMathod() = default;

    const std::string toString() override { m_json.dump(); }
    void request() override {};

    void SetMethod(const std::string& method) { m_json["method"] = method; }
    const std::string& GetMethod() const { return m_json["method"]; }

    void SetServers(const std::list<std::string>& params) { m_json["params"] = params; }
    const std::list<std::string>& GetServers() const { return m_json["params"]; }
};

} // namespace aria2net
