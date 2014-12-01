#include "UpMessage.h"

UpMessage::UpMessage()
{
    //ctor
}

UpMessage::~UpMessage()
{
    //dtor
}

void UpMessage::reset()
{
    m_move = Player::MOVE_NOT;
    m_action = Player::ACTION_NONE;
}
