#ifndef UPMESSAGE_H
#define UPMESSAGE_H

#include <string>
#include "Player.h"

class UpMessage
{
public:
    UpMessage();
    virtual ~UpMessage();

    void reset();

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & m_move & m_action;
    }

    Player::Move m_move;
    Player::Action m_action;

protected:
private:
};

#endif // UPMESSAGE_H
