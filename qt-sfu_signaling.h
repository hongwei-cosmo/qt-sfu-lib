#ifndef QSFUSIGNALING_H
#define QSFUSIGNALING_H

#include <functional>
#include <memory>
#include <string>

#include "Client.h"
#include "qtsfuclientlib_global.h"

// NOTE: Must include QObject after Client.h
#include <QObject>
/**
 * @brief Qt Wrapper of class Client of dm-sfu-client-lib
 */
class QTSFUCLIENTLIBSHARED_EXPORT QSfuSignaling :
    public QObject,
    public dm::Client::Transport
{
  Q_OBJECT
public:
  enum CmdId {
    CreateRoom,
    CreateAuditRoom,
    DestroyRoom,
    JoinRoom,
    LeaveRoom
  };
  explicit QSfuSignaling(QObject *parent = nullptr);
  virtual ~QSfuSignaling() = default;

  void initialize();
  void createRoom();
  void createAuditRoom(const std::string &recodingId);
  void destroyRoom();
  void joinRoom(const std::string &sdp);
  void leaveRoom();

  void setRoomId(const std::string &roomId_);
  std::string getRoomId() const;
  void setRoomAccessPin(const std::string &pin);
  std::string getAnswerSdp() const;

private:
  std::unique_ptr<dm::Client> sfu_ = nullptr;
  std::string roomId_;
  std::string roomAccessPin_ = "pin";
  SDPInfo::shared sdpInfo_;

  virtual void send(const std::string &message) override;
  virtual void onmessage(const std::function<bool(const std::string &message)> &callback) override;

signals:
  /**
   * @brief Signal used to get the status of the command
   * @param cmdId Command ID
   * @param result "OK" if command is successful. Or it is the error message
   */
  void commandFinished(CmdId cmdId, const std::string &result);
  void streamPublished();
  void streamUnpublished(const std::string &streamId);
  void participantJoined(const std::string &roomId, const std::string &clientId, const std::string &reason);
  void participantLeft(const std::string &roomId, const std::string &clientId, const std::string &reason);
  void participantKicked(const std::string &roomId, const std::string &reason);
  void activeSpeakerChanged(const std::string &roomId, const std::string &clientId);

public slots:
};

#endif // QSFUSIGNALING_H
