#include "qt-sfu_signaling.h"

QSfuSignaling::QSfuSignaling(QObject *parent) : QObject(parent)  { }

void QSfuSignaling::initialize()
{
  sfu_ = std::make_unique<dm::Client>(*this);
  // Set event handlers for sfu_
  sfu_->on<dm::Stream::Event::Published>([=](dm::Stream::Event::Published &r) {
    this->sdpInfo_->addStream(r.streamInfo);
    emit streamPublished();
  })
  .on<dm::Stream::Event::Unpublished>([=](dm::Stream::Event::Unpublished &r) {
    sdpInfo_->removeStream(r.streamId);
    emit streamUnpublished(r.streamId);
  })
  .on<dm::Participant::Event::Joined>([=](dm::Participant::Event::Joined &r) {
    emit participantJoined(r.roomId, r.clientId, r.reason);
  })
  .on<dm::Participant::Event::Left>([=](dm::Participant::Event::Left &r) {
    emit participantLeft(r.roomId, r.clientId, r.reason);
  })
  .on<dm::Participant::Event::Kicked>([=](dm::Participant::Event::Kicked &r) {
    emit participantKicked(r.roomId, r.reason);
  })
  .on<dm::Participant::Event::ActiveSpeakerChanded>([=](dm::Participant::Event::ActiveSpeakerChanded &r) {
    emit activeSpeakerChanged(r.roomId, r.clientId);
  });
}

void QSfuSignaling::createRoom()
{
  sfu_->createRoom(roomAccessPin_,
                        [this](const dm::Room::Created &r) {
    if (!r.error) {
      roomId_ = r.result->id;
    }
    emit commandFinished(CreateRoom, r.toString());
  });
}

void QSfuSignaling::createAuditRoom(const std::string& recodingId)
{
  sfu_->createAuditRoom(roomAccessPin_, recodingId,
                              [=](const dm::Room::Created &r) {
    if (!r.error) {
      roomId_ = r.result->id;
    }
    emit commandFinished(CreateAuditRoom, r.toString());
    return;
  });
}

void QSfuSignaling::destroyRoom()
{
  sfu_->destroyRoom(roomId_, [this](const dm::Room::Destroyed &r) {
    emit commandFinished(DestroyRoom, r.toString());
  });
}

void QSfuSignaling::joinRoom(const std::string& sdp)
{
  auto offerInfo = SDPInfo::parse(sdp);
  sfu_->join(roomId_, roomAccessPin_, offerInfo,
                  [this](const dm::Participant::Joined &r) {
    if (!r.error) {
      sdpInfo_ = r.result->sdpInfo;
      for (auto stream : r.result->streams) {
        sdpInfo_->addStream(stream);
      }
    }
    emit commandFinished(JoinRoom, r.toString());
  });
}

void QSfuSignaling::leaveRoom()
{
  sfu_->leave(roomId_, [this](const dm::Participant::Left &r) {
    emit commandFinished(LeaveRoom, r.toString());
  });
}

void QSfuSignaling::setRoomId(const std::string& roomId)
{
  roomId_ = roomId;
}

std::string QSfuSignaling::getRoomId() const
{
  return roomId_;
}

void QSfuSignaling::setRoomAccessPin(const std::string& pin)
{
  roomAccessPin_ = pin;
}

std::string QSfuSignaling::getAnswerSdp() const
{
  return sdpInfo_->toString();
}

void send(const std::string& message)
{

}

void onmessage(const std::function<bool (const std::string&)>& callback)
{

}
