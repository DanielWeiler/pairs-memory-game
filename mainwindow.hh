#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <vector>
#include <random>
#include "card.hh"
#include "player.hh"

using namespace std;
using Game_row_type = vector<Card*>;
using Game_board_type = vector<vector<Card*>>;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // Captures the number of cards to be in the game.
    void on_CardAmountLineEditEditingFinished();

    // Captures the seed value used to randomize the letters of the cards.
    void on_SeedValueLineEditEditingFinished();

    // Captures the number of players to be in the game.
    void on_PlayerAmountLineEditEditingFinished();

    // Checks for correct user inputs, handles player name entry, and
    // initializes the game.
    void on_EnterButtonClicked();

    // Checks the cards that the player turned over to see if a matching pair
    // was found or not and sets the game for the next turn or for the end of
    // the game.
    void on_EndTurnButtonClicked();

    // Turns the card over that was clicked and stores this information.
    void on_CardButtonClicked();

private:
    Ui::MainWindow *ui;

    // Space between elements, both horizontally and vertically
    const int MARGIN = 20;

    // Width for the header label
    const int HEADER_WIDTH = 335;

    // Width for all number input line edit fields
    const int NUMBER_WIDTH = 25;

    // Width for player name input line edit field
    const int PLAYER_NAME_WIDTH = 125;

    // Width for push buttons
    const int BUTTON_WIDTH = 75;

    // Sizes for the game board
    const int BOARD_ROWSPAN = 10;
    const int BOARD_COLUMNSPAN = 5;
    const int BOARD_HEIGHT = 280;

    // Prime numbers beyond 20 that do not split into rows
    // nicely stretch the window too much.
    const int CARD_AMOUNT_LIMIT = 20;

    QWidget* central_;

    QGridLayout* g_Layout_;
    QGridLayout* board_grid_;

    QLabel* header_Label_;

    QLabel* score_board_header_Label_;

    QLabel* card_amount_Label_;
    QLineEdit* card_amount_LineEdit_;

    QLabel* seed_value_Label_;
    QLineEdit* seed_value_LineEdit_;

    QLabel* player_amount_Label_;
    QLineEdit* player_amount_LineEdit_;

    QLabel* player_names_input_Label_;
    QLineEdit* player_names_input_LineEdit_;

    QPushButton* enter_Button_;
    QPushButton* end_turn_Button_;

    vector<vector<QLabel*>> score_board_;

    // Factors to be used to create the game board dimensions.
    int row_factor_ = 1;
    int column_factor_ = 1;
    Game_board_type game_board_;

    // The amount of cards to be in the game given by the user.
    int card_amount_ = 0;

    // Value to randomize the order of the cards.
    int seed_value_ = 0;
    default_random_engine rand_generator;

    // The amount of players to be in the game given by the user.
    size_t player_amount_ = 0;

    // The player names given by the user used to create the players.
    vector<QString> player_names_;
    vector<Player*> players_;

    size_t player_turn_counter_ = 1;
    int current_player_turn_ = 0;
    size_t next_player_turn_ = 1;

    // Amount of cards turned over
    int visible_cards_count_ = 0;
    // Stores the cards that are currently turned over.
    vector<Card*> visible_cards_;

    // Calculates the factors of the number of cards that are as near to each
    // other as possible. This is used to create the dimensions of the game
    // board.
    void calculate_nearest_factors();

    // Uses the names given by the user to create the players of the game.
    void create_players();

    // Checks if all cards have been removed from the game board for the end
    // of the game.
    void check_game_over();

    // Ends the game and announces the winning players or the players that
    // tied.
    void game_over();

    // Initialization methods for labels, line edits, buttons, the score board,
    // the game board, and the letters of the cards.
    void init_header_Label();

    void init_card_amount_Label();
    void init_card_amount_LineEdit();

    void init_seed_value_Label();
    void init_seed_value_LineEdit();

    void init_player_amount_Label();
    void init_player_amount_LineEdit();

    void init_player_names_input_Label();
    void init_player_names_input_LineEdit();

    void init_enter_Button();
    void init_end_turn_Button();

    // Fills the score board with the player names.
    void init_score_board();

    // Fills the game board, the size of which is rows * columns, with empty
    // cards.
    void init_game_board();

    // Fills the cards with their assigned letters so that there are pairs of
    // letters.
    void init_card_letters();
};
#endif // MAINWINDOW_HH
