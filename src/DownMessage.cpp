#include "DownMessage.h"

DownMessage::DownMessage()
{
    //ctor
}

DownMessage::~DownMessage()
{
    //dtor
}

void DownMessage::reset()
{
    m_type = DEFAULT_TYPE;
    m_area.clear();
    m_players.clear();
    m_info.clear();
}
