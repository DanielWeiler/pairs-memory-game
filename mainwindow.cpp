#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Pairs Memory Game");

    central_ = new QWidget(this);
    g_Layout_ = new QGridLayout(central_);
    setCentralWidget(central_);

    // Format main grid layout so that widgets are in fixed positions.
    g_Layout_->setContentsMargins(MARGIN, MARGIN, MARGIN, MARGIN);
    g_Layout_->setColumnMinimumWidth(2, HEADER_WIDTH);
    g_Layout_->setColumnMinimumWidth(3, MARGIN);
    g_Layout_->setRowMinimumHeight(1, MARGIN);
    g_Layout_->setRowMinimumHeight(9, BOARD_HEIGHT);

    score_board_header_Label_ = new QLabel;
    score_board_header_Label_->setText("Score Board");
    score_board_header_Label_->setAlignment(Qt::AlignCenter);
    g_Layout_->addWidget(score_board_header_Label_, 2, 4, 1, 2);

    init_header_Label();

    init_card_amount_Label();
    init_card_amount_LineEdit();

    init_seed_value_Label();
    init_seed_value_LineEdit();

    init_player_amount_Label();
    init_player_amount_LineEdit();

    init_player_names_input_Label();
    init_player_names_input_LineEdit();

    init_enter_Button();
    init_end_turn_Button();
}

MainWindow::~MainWindow()
{
    delete ui;

    // Deallocating cards
    for (auto row : game_board_)
    {
        for(auto card : row)
        {
            delete card;
        }
    }

    // Deallocating players
    for (auto player : players_)
    {
        delete player;
    }
}

void MainWindow::on_CardAmountLineEditEditingFinished()
{
    QString input = card_amount_LineEdit_->text();
    card_amount_ = input.toInt();
}

void MainWindow::on_SeedValueLineEditEditingFinished()
{
    QString input = seed_value_LineEdit_->text();
    seed_value_ = input.toInt();
}

void MainWindow::on_PlayerAmountLineEditEditingFinished()
{
    QString input = player_amount_LineEdit_->text();
    player_amount_ = input.toInt();
}

void MainWindow::on_EnterButtonClicked()
{
    if (card_amount_LineEdit_->text().isEmpty() or
            not (card_amount_ > 0 and card_amount_ % 2 == 0))
    {
        header_Label_->setText("The amount of cards must be even and "
                               "positive.");
        return;
    }
    else if (card_amount_ > CARD_AMOUNT_LIMIT)
    {
        header_Label_->setText("Card amount maximum is 20.");
        return;
    }

    else if (player_amount_LineEdit_->text().isEmpty() or player_amount_ < 2
             or player_amount_ > 6)
    {
        header_Label_->setText("There must be between two to six players.");
        return;
    }

    card_amount_LineEdit_->setEnabled(false);
    seed_value_LineEdit_->setEnabled(false);
    player_amount_LineEdit_->setEnabled(false);

    player_names_input_LineEdit_->setEnabled(true);
    header_Label_->setText("Enter player names one by one.");

    // Reading input for player name
    if (not player_names_input_LineEdit_->text().isEmpty())
    {
        if (player_names_input_LineEdit_->text().size() > 10)
        {
            header_Label_->setText("Max character length for name is 10.");
            player_names_input_LineEdit_->setText("");
            return;
        }
        if (player_names_.size() < 1)
        {
            player_names_.push_back(player_names_input_LineEdit_->text());
        }
        // Checking for duplicate names
        else
        {
            bool duplicate_name = false;
            for (auto name : player_names_)
            {
                if (player_names_input_LineEdit_->text() == name)
                {
                    duplicate_name = true;
                }
            }

            if (duplicate_name)
            {
                header_Label_->setText("Two players cannot have the same "
                                       "name.");
            }
            else
            {
                player_names_.push_back(player_names_input_LineEdit_->text());
            }
        }

        player_names_input_LineEdit_->setText("");
    }

    if (player_names_.size() == player_amount_)
    {
        // Initiating game
        player_names_input_LineEdit_->setEnabled(false);
        create_players();
        init_score_board();
        init_game_board();
        init_card_letters();
        enter_Button_->setEnabled(false);
        header_Label_->setText(players_.at(current_player_turn_)->get_name()
                               + "'s turn");
    }
}

void MainWindow::on_EndTurnButtonClicked()
{

    Player* current_player = players_.at(current_player_turn_);
    Player* next_player = players_.at(next_player_turn_);

    // Checking for the cards that the player turned over.
    for (auto row : game_board_)
    {
        for(auto card : row)
        {
            if (card->is_face_up)
            {
                visible_cards_.push_back(card);
            }
        }
    }

    // Checking if a matching pair of cards was found.
    if (visible_cards_.at(0)->get_letter() ==
            visible_cards_.at(1)->get_letter())
    {
        header_Label_->setText(current_player->get_name()
                               + " found a pair! It is now your turn "
                               + next_player->get_name() + ".");

        // Adding points to the score board.
        current_player->add_pair();
        for (auto row : score_board_)
        {
            if (row.at(0)->text() == current_player->get_name())
            {
                row.at(1)->setText(
                            QString::fromStdString(
                                to_string(
                                    current_player->
                                    get_number_of_pairs())));
            }
        }

        // Cards are removed from the game board.
        visible_cards_.at(0)->remove();
        visible_cards_.at(1)->remove();
    }
    // A matching pair was not found.
    else
    {
        header_Label_->setText(next_player->get_name()
                               + "'s turn");

        visible_cards_.at(0)->turn();
        visible_cards_.at(1)->turn();
    }

    // Setting the game for the next player's turn.
    visible_cards_.clear();
    visible_cards_count_ = 0;
    end_turn_Button_->setEnabled(false);
    player_turn_counter_ += 1;
    current_player_turn_ += 1;
    next_player_turn_ += 1;

    // Resetting the variables that track turns for when one round of turns
    // ends.
    if (player_turn_counter_ > players_.size())
    {
        player_turn_counter_ = 1;
        current_player_turn_ = 0;
    }
    if (next_player_turn_ == players_.size())
    {
        next_player_turn_ = 0;
    }

    check_game_over();
}

void MainWindow::on_CardButtonClicked()
{
    // The maximum amount of cards that may be turned per turn is two.
    if (visible_cards_count_ == 2)
    {
        return;
    }

    // Storing global cursor position
    QPoint global_click_position = QCursor::pos();

    // Counting local cursor position, i.e. decreasing
    // Main Window's location from the global one
    int local_x = global_click_position.x() - geometry().x();
    int local_y = global_click_position.y() - geometry().y();
    QPoint local_click_position = QPoint(local_x, local_y);

    // Searching for the button clicked
    for (auto row : game_board_)
    {
        for(auto card : row)
        {
            if(card->geometry().contains(local_click_position) and
                    card->is_face_up == false)
            {
                card->turn();
                visible_cards_count_ += 1;

                if (visible_cards_count_ == 2)
                {
                    end_turn_Button_->setEnabled(true);
                }

                return; // For efficiency reasons
                        // (only one button can be clicked at a time)
            }
        }
    }
}

void MainWindow::calculate_nearest_factors()
{
    int product = card_amount_;

    for(int i = 1; i * i <= product; ++i)
    {
        if(product % i == 0)
        {
            row_factor_ = i;
        }
    }
    column_factor_ = product / row_factor_;
}

void MainWindow::create_players()
{
    for (QString name : player_names_)
    {
        Player* new_player = new Player(name);
        players_.push_back(new_player);
    }
}

void MainWindow::check_game_over()
{
    for (auto row : game_board_)
    {
        for(auto card : row)
        {
            if (card->removed == false)
            {
                return;
            }
        }
    }

    game_over();
}

void MainWindow::game_over()
{
    // This loop iterates through all the players to find the highest score.
    unsigned int high_score = 0;
    for (Player* player : players_)
    {
        if (player->get_number_of_pairs() > high_score)
        {
            high_score = player->get_number_of_pairs();
        }
    }

    // A vector to hold only the player(s) who had the highest score.
    vector<Player*> winners;
    for (Player* player : players_)
    {
        if (player->get_number_of_pairs() == high_score)
        {
            winners.push_back(player);
        }
    }

    // The result if only one player got the highest score.
    if (winners.size() == 1)
    {
        header_Label_->setText("Game over! \n" + winners.at(0)->get_name()
                               + " has won with "
                               + QString::fromStdString(
                                   to_string(high_score))
                               + " pair(s).");
    }
    // The result if multiple players got the highest score.
    else
    {
        QString tied_players;
        for (auto player : winners)
        {
            // Handling the grammar cases for different amount of tied players.
            if (player == winners.back())
            {
                tied_players += "and " + player->get_name();
            }
            else
            {
                if (winners.size() == 2)
                {
                    tied_players += player->get_name() + " ";
                }
                else
                {
                    tied_players += player->get_name() + ", ";
                }
            }
        }
        header_Label_->setText("Game over! \n" + tied_players
                               + " tie with a score of "
                               + QString::fromStdString(
                                   to_string(high_score))
                               + " pair(s).");
    }
}

void MainWindow::init_header_Label()
{
    header_Label_ = new QLabel;
    g_Layout_->addWidget(header_Label_, 0, 2);
    header_Label_->setText("Who can find the most pairs of matching cards? \n "
                           "Enter the following to start: ");
    header_Label_->setAlignment(Qt::AlignCenter);
}

void MainWindow::init_card_amount_Label()
{
    card_amount_Label_ = new QLabel;
    g_Layout_->addWidget(card_amount_Label_, 2, 1);
    card_amount_Label_->setText("Amount of cards (even number): ");
}

void MainWindow::init_card_amount_LineEdit()
{
    card_amount_LineEdit_ = new QLineEdit;
    g_Layout_->addWidget(card_amount_LineEdit_, 2, 2);
    card_amount_LineEdit_->setFixedWidth(NUMBER_WIDTH);

    connect(card_amount_LineEdit_, &QLineEdit::editingFinished, this,
            &MainWindow::on_CardAmountLineEditEditingFinished);
}

void MainWindow::init_seed_value_Label()
{
    seed_value_Label_ = new QLabel;
    g_Layout_->addWidget(seed_value_Label_, 3, 1);
    seed_value_Label_->setText("Seed value: ");
}

void MainWindow::init_seed_value_LineEdit()
{
    seed_value_LineEdit_ = new QLineEdit;
    seed_value_LineEdit_->setFixedWidth(NUMBER_WIDTH);
    g_Layout_->addWidget(seed_value_LineEdit_, 3, 2);

    connect(seed_value_LineEdit_, &QLineEdit::editingFinished, this,
            &MainWindow::on_SeedValueLineEditEditingFinished);
}

void MainWindow::init_player_amount_Label()
{
    player_amount_Label_ = new QLabel;
    g_Layout_->addWidget(player_amount_Label_, 4, 1);
    player_amount_Label_->setText("Amount of players (2 - 6): ");
}

void MainWindow::init_player_amount_LineEdit()
{
    player_amount_LineEdit_ = new QLineEdit;
    player_amount_LineEdit_->setFixedWidth(NUMBER_WIDTH);
    g_Layout_->addWidget(player_amount_LineEdit_, 4, 2);

    connect(player_amount_LineEdit_, &QLineEdit::editingFinished, this,
            &MainWindow::on_PlayerAmountLineEditEditingFinished);
}

void MainWindow::init_player_names_input_Label()
{
    player_names_input_Label_ = new QLabel;
    g_Layout_->addWidget(player_names_input_Label_, 5, 1);
    player_names_input_Label_->setText("Names of players: ");
}

void MainWindow::init_player_names_input_LineEdit()
{
    player_names_input_LineEdit_ = new QLineEdit;
    player_names_input_LineEdit_->setEnabled(false);
    player_names_input_LineEdit_->setFixedWidth(PLAYER_NAME_WIDTH);
    g_Layout_->addWidget(player_names_input_LineEdit_, 5, 2);
}

void MainWindow::init_enter_Button()
{
    enter_Button_ = new QPushButton;
    enter_Button_->setText("Enter");
    g_Layout_->addWidget(enter_Button_, 6, 2);
    enter_Button_->setFixedWidth(BUTTON_WIDTH);

    connect(enter_Button_, &QPushButton::clicked, this,
            &MainWindow::on_EnterButtonClicked);
}

void MainWindow::init_end_turn_Button()
{
    end_turn_Button_ = new QPushButton;
    end_turn_Button_->setText("End turn");
    end_turn_Button_->setFixedWidth(BUTTON_WIDTH);
    g_Layout_->addWidget(end_turn_Button_, 6, 2, Qt::AlignRight);
    end_turn_Button_->setEnabled(false);

    connect(end_turn_Button_, &QPushButton::clicked, this,
            &MainWindow::on_EndTurnButtonClicked);
}

void MainWindow::init_score_board()
{
    vector<QLabel*> score_board_row;
    int y_pos_factor = 0;
    for (auto player : players_)
    {
        score_board_row.clear();

        QLabel* player_name = new QLabel;
        player_name->setText(player->get_name());
        g_Layout_->addWidget(player_name, 3 + y_pos_factor, 4);

        QLabel* player_score = new QLabel;
        player_score->setText("0");
        g_Layout_->addWidget(player_score, 3 + y_pos_factor, 5,
                             Qt::AlignRight);

        score_board_row.push_back(player_name);
        score_board_row.push_back(player_score);

        score_board_.push_back(score_board_row);

        y_pos_factor += 1;
    }
}

void MainWindow::init_game_board()
{
    calculate_nearest_factors();

    // A grid layout to contain the game board.
    board_grid_ = new QGridLayout;
    g_Layout_->addLayout(board_grid_, 9, 1, BOARD_ROWSPAN, BOARD_COLUMNSPAN);

    Game_row_type row;
    for(int i = 0; i < row_factor_; ++i)
    {
        row.clear();

        for(int j = 0; j < column_factor_; ++j)
        {
            Card* card_button = new Card();
            row.push_back(card_button);
            card_button->setFixedWidth(BUTTON_WIDTH);
            card_button->setStyleSheet(
                        "QPushButton {background-color: blue;}");
            card_button->setText("♦♦♦");
            board_grid_->addWidget(card_button, i, j);

            connect(card_button, &QPushButton::clicked,
                    this, &MainWindow::on_CardButtonClicked);
        }

        game_board_.push_back(row);
    }
}

void MainWindow::init_card_letters()
{
    // Creating a string that holds all the letters to be set to the cards.
    string letters = "";
    for (int i = 0, c = 'A'; i < card_amount_; i += 2, ++c)
    {
        for (int j = 0; j < 2; ++j)
        {
            letters += c;
        }
    }

    // Generating a random number to shuffle the letters with. The seed value
    // is used to change the outcome of the random number generated.
    rand_generator.seed(seed_value_);
    shuffle(letters.begin(), letters.end()-1, rand_generator);

    // Setting letters to the cards from the shuffled string of letters.
    int i = 0;
    for (auto row : game_board_)
    {
        for (auto card : row)
        {
            card->set_letter(letters.at(i));
            i += 1;
        }
    }
}
