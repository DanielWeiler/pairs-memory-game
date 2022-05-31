#include "player.hh"

Player::Player(const QString& name, unsigned int number_of_pairs):
    name_(name), number_of_pairs_(number_of_pairs)
{

}

QString Player::get_name() const
{
    return name_;
}

void Player::add_pair()
{
    number_of_pairs_ += 1;
}

unsigned int Player::get_number_of_pairs() const
{
    return number_of_pairs_;
}
