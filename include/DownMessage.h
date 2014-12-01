#ifndef DOWNMESSAGE_H
#define DOWNMESSAGE_H

#include <string>
#include <vector>

#include "Player.h"

class DownMessage
{
public:
    DownMessage();
    virtual ~DownMessage();

    void reset();

    enum Type
    {
        PLAYER_CONNECTED, PLAYER_LEFT, WORLD_STATE, DEFAULT_TYPE
    };

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & m_type & m_info & m_area & m_players;
    }

    Type m_type;
    std::string	m_area;
    std::vector<Player> m_players;
    std::string m_info;
protected:
private:
};

#endif // DOWNMESSAGE_H
