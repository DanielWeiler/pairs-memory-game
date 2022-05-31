#include "card.hh"

Card::Card(): QPushButton(),
    letter_()
{

}

void Card::set_letter(const char c)
{
    QChar l = c;
    letter_ = l;
}

QString Card::get_letter() const
{
    return letter_;
}

void Card::turn()
{
    // Turn face down
    if (is_face_up)
    {
        is_face_up = false;
        this->setText("♦♦♦");
        this->setStyleSheet("QPushButton {background-color: blue;}");
    }
    // Turn face up
    else
    {
        this->is_face_up = true;
        this->setText(letter_);
        this->setStyleSheet("QPushButton {background-color: cyan;}");
    }
}

void Card::remove()
{
    this->turn();
    this->setText("");
    this->setStyleSheet("QPushButton {background-color: white;}");
    this->setEnabled(false);
    removed = true;
}
