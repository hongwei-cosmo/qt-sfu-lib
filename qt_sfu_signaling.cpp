#include "qt_sfu_signaling.h"

QSfuSignaling::QSfuSignaling(QObject *parent) : QObject(parent), sfu_(*this)
{
  qDebug("%s is called", __func__);
  // Set event handlers for sfu_
  sfu_.on<dm::Stream::Event::Published>([=](dm::Stream::Event::Published &r) {
    this->sdpInfo_->addStream(r.streamInfo);
    Q_EMIT streamPublished();
  })
  .on<dm::Stream::Event::Unpublished>([=](dm::Stream::Event::Unpublished &r) {
    sdpInfo_->removeStream(r.streamId);
    Q_EMIT streamUnpublished(r.streamId);
  })
  .on<dm::Participant::Event::Joined>([=](dm::Participant::Event::Joined &r) {
    Q_EMIT participantJoined(r.roomId, r.clientId, r.reason);
  })
  .on<dm::Participant::Event::Left>([=](dm::Participant::Event::Left &r) {
    Q_EMIT participantLeft(r.roomId, r.clientId, r.reason);
  })
  .on<dm::Participant::Event::Kicked>([=](dm::Participant::Event::Kicked &r) {
    Q_EMIT participantKicked(r.roomId, r.reason);
  })
  .on<dm::Participant::Event::ActiveSpeakerChanded>([=](dm::Participant::Event::ActiveSpeakerChanded &r) {
    Q_EMIT activeSpeakerChanged(r.roomId, r.clientId);
  });
}

void QSfuSignaling::createRoom()
{
  qDebug("%s is called", __func__);
  sfu_.createRoom(roomAccessPin_,
                        [this](const dm::Room::Created &r) {
    if (!r.error) {
      roomId_ = r.result->id;
    }
    Q_EMIT commandFinished(CreateRoom, r.toString());
  });
}

void QSfuSignaling::createAuditRoom(const std::string& recodingId)
{
  qDebug("%s is called", __func__);
  sfu_.createAuditRoom(roomAccessPin_, recodingId,
                              [=](const dm::Room::Created &r) {
    if (!r.error) {
      roomId_ = r.result->id;
    }
    Q_EMIT commandFinished(CreateAuditRoom, r.toString());
    return;
  });
}

void QSfuSignaling::destroyRoom()
{
  qDebug("%s is called", __func__);
  sfu_.destroyRoom(roomId_, [this](const dm::Room::Destroyed &r) {
    Q_EMIT commandFinished(DestroyRoom, r.toString());
  });
}

void QSfuSignaling::joinRoom(const std::string& sdp)
{
  qDebug("%s is called", __func__);
  auto offerInfo = SDPInfo::parse(sdp);
  sfu_.join(roomId_, roomAccessPin_, offerInfo,
                  [this](const dm::Participant::Joined &r) {
    if (!r.error) {
      sdpInfo_ = r.result->sdpInfo;
      for (auto stream : r.result->streams) {
        sdpInfo_->addStream(stream);
      }
    }
    Q_EMIT commandFinished(JoinRoom, r.toString());
  });
}

void QSfuSignaling::leaveRoom()
{
  qDebug("%s is called", __func__);
  sfu_.leave(roomId_, [this](const dm::Participant::Left &r) {
    Q_EMIT commandFinished(LeaveRoom, r.toString());
  });
}

void QSfuSignaling::setRoomId(const std::string &roomId)
{
  qDebug("%s is called", __func__);
  roomId_ = roomId;
}

std::string QSfuSignaling::getRoomId() const
{
  qDebug("%s is called", __func__);
  return roomId_;
}

void QSfuSignaling::setRoomAccessPin(const std::string &pin)
{
  qDebug("%s is called", __func__);
  roomAccessPin_ = pin;
}

std::string QSfuSignaling::getAnswerSdp() const
{
  qDebug("%s is called", __func__);
  return sdpInfo_->toString();
}

void QSfuSignaling::gotMsgFromSfu(const std::string &message) const
{
  qDebug("%s is called", __func__);
  callback_(message);
}

void QSfuSignaling::send(const std::string& message)
{
  qDebug("%s is called", __func__);
  Q_EMIT sendMessgeToSfu(message);
}

void QSfuSignaling::onmessage(const std::function<bool (const std::string&)> &callback)
{
  qDebug("%s is called", __func__);
  this->callback_ = callback;
}
