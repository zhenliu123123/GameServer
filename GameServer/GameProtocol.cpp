#include "GameProtocol.h"

UserData* GameProtocol::raw2request(std::string _szInput)
{
    std::cout << _szInput << std::endl;
    return nullptr;
}

std::string* GameProtocol::response2raw(UserData& _oUserData)
{
    return nullptr;
}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
    return nullptr;
}

Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{
    return nullptr;
}
