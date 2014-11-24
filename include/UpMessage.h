#ifndef UPMESSAGE_H
#define UPMESSAGE_H

#include <string>

class UpMessage
{
public:
    UpMessage();
    virtual ~UpMessage();

    void reset();

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar << m_type << m_message << m_login;
    }

    int m_type;
    std::string	m_message;
    std::string	m_login;

protected:
private:
};

#endif // UPMESSAGE_H
