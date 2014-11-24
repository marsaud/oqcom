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
    m_message.clear();
    m_login.clear();
}
