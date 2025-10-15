#include "Channel.hpp"


Channel::~Channel()
{
    std::cout << "Channel Destructor Called\n";
}

Channel::Channel()
{
    std::cout << "Channel Default Constructor Called\n";
}


Channel::Channel(std::string& Channel_name, std::string& key)
{
    _name = Channel_name;
    _key = key;
    _inviteOnly = false;
    _topicLock = false;

    std::cout << "Channel ARGS Constructor Called\n";
}

void Channel::addClient(int& client_fd)
{
    _users.push_back(client_fd);
}


void Channel::addOperator(int& client_fd)
{
    _operators.push_back(client_fd);
}

void Channel::addInvited(int& fd)
{
    _invited_users.push_back(fd);
}


void Channel::removeOperator(int& client_fd)
{
	//std::set<int>::iterator it;
	// it = _operators.find(client_fd);
	// if (it != _operators.end())
	// 	_operators.erase(it);

    if(_operators.empty() == true)
        return;
    if(_operators.size() == 1)
    {
        _operators.pop_back();
        return ;
    }
    else
    {
        //must be checked
        std::vector<int>::iterator it;
        std::vector<int>::iterator tmp;
        it = std::find(_operators.begin(), _operators.end(), client_fd);
        tmp = _operators.end();
        tmp--;
        if(it != _operators.end() && tmp != it)
        {
            std::cout << "enter to tmp != it\n";
            std::swap(*it, *tmp);
            _operators.pop_back();
        }
    }
}

bool Channel::removeClientFromChannel(int client_fd)
{
    for (std::vector<int>::iterator it = _users.begin(); it != _users.end(); ++it)
    {
        if (*it == client_fd)
        {
            _users.erase(it);
            return true;
        }
    }
    return false;
}



/// Channel Getters

std::string& Channel::Get_topicDate()
{
    return _topicDate;
}

std::string& Channel::Get_topicSetter()
{
    return _topicSetter;
}


std::string& Channel::Get_topic()
{
    return _topic;
}

bool Channel::Get_topicLock()const
{
    return _topicLock;
}


bool& Channel::Get_inviteOnly()
{
    return _inviteOnly;
}

size_t& Channel::Get_userLimit()
{
    return _userLimit;
}

size_t Channel::Get_userCount()
{
    return _users.size();
}

const std::string& Channel::Get_key()const
{
    return _key;
}

std::vector<int>& Channel::Get_users()
{
    return _users;
}

std::vector<int>& Channel::Get_operators()
{
    return _operators;
}

std::string& Channel::Get_name()
{
    return _name;
}


std::string Channel::Get_ChannelModes()
{
    std::string modes = "+";

    if (_inviteOnly)
        modes += "i";
    if (_topicLock)
        modes += "t";
    if (!_key.empty())
        modes += "k";
    if (_userLimit > 0)
        modes += "l";

    return modes;
}

std::string& Channel::Get_creationDate()
{
    return _creationDate;
}

void Channel::Set_inviteOnly(bool status)
{
    _inviteOnly = status;
}

void Channel::Set_key(const std::string& key)
{
    _key = key;
}


void Channel::Set_topicLock(bool status)
{
    _topicLock = status;
}









