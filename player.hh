/* The Player class creates objects, which are players, that have a name and
 * that can keep track of their score in the game.
 *
 *
 * Class: Player
 * -------------
 * Represents a single player in pairs (memory) game.
 * */

#ifndef PLAYER_HH
#define PLAYER_HH

#include <QString>

class Player
{
public:
    Player(const QString& name, unsigned int number_of_pairs = 0);

    // Returns the name of the player.
    QString get_name() const;

    // Increases the number of pairs the player has by one.
    void add_pair();

    // Returns the number of pairs collected by the player so far.
    unsigned int get_number_of_pairs() const;

private:
    QString name_;
    unsigned int number_of_pairs_;
};

#endif // PLAYER_HH
