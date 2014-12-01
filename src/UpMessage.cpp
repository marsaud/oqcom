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
    m_move = oq::MOVE_NOT;
    m_action = oq::ACTION_NONE;
}
