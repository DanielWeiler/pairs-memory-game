/* The Card class creates objects that hold the information of the card.
 * A card contains a letter that matches the letter of another card on
 * the board. The card can be flipped to reveal its letter.
 *
 * Class: Card
 * -----------
 * Represents a single card in pairs (memory) game.
 * */

#ifndef CARD_HH
#define CARD_HH
#include <QPushButton>
#include <QChar>

class Card: public QPushButton
{
public:
    Card();

    // Assigns the given letter to the card.
    void set_letter(const char c);

    // Returns the letter assigned to the card.
    QString get_letter() const;

    // Identifies if the card is in a face up position or not.
    bool is_face_up = false;

    // Turns the card either face up or face down depending on its current
    // position.
    void turn();

    // Identifies if the card has been removed from the game board or not.
    bool removed = false;

    // Removes the card from the game board.
    void remove();

private:
    QString letter_ = "";

};

#endif // CARD_HH
