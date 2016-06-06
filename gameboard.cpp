#include "gameboard.h"
#include "mainwindow.h"
#include "ui_gameboard.h"
#include "popupwindow.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QSpacerItem>
#include<QLabel>
#include<QPushButton>
#include<QGridLayout>


unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);


gameBoard::gameBoard(QWidget *parent, size_t board_sz, size_t dog_spd) :
    QWidget(parent),
    ui(new Ui::gameBoard), board_size(board_sz), dog_speed(dog_spd)
{
    ui->setupUi(this);

    QVBoxLayout* layout = new QVBoxLayout;

    Top = new QWidget;
    QHBoxLayout *top_bar = new QHBoxLayout(Top);
    layout->addLayout(top_bar);

    //Images
    QString fileName(":/images/avatar.jpg");
    avatar_image = new QPixmap(fileName);

    QString dog_filename(":/images/dog.jpg");
    dog_image = new QPixmap(dog_filename);

    QString grass_filename(":/images/grass.jpg");
    grass_image = new QPixmap(grass_filename);

    QString car_filename(":/images/car.jpg");
    car_image = new QPixmap(car_filename);

    QString asphalt_filename(":/images/asphalt.jpg");
    asphalt_image = new QPixmap(asphalt_filename);

    QString tree_filename(":/images/tree.jpg");
    tree_image = new QPixmap(tree_filename);

    //Game setting:
    if (board_sz == 10) { gametype_indicator = 1; } //easy game
    if (board_sz == 15) { gametype_indicator = 2; } //med game
    if (board_sz == 20) { gametype_indicator = 3; } //hard game


    // Life count
    QLabel* life_label = new QLabel("Lives left:");
    QFont f( "Helvetica", 15);
    life_label->setFont(f);
    top_bar->addWidget(life_label);

    life_count = 4;
    life_counter = new QLabel*[life_count-1];

    for(size_t i=0;i<life_count-1;i++) {
        life_counter[i] = new QLabel;
        life_counter[i]->setPixmap(*avatar_image);
        life_counter[i]->setMinimumSize(20,20);
        life_counter[i]->setMaximumSize(20,20);
        life_counter[i]->setAlignment(Qt::AlignHCenter);
        life_counter[i]->setScaledContents(true);
        top_bar->addWidget(life_counter[i]);
    }


    // Add in some horizontal space before next labels
    QSpacerItem* horizontal_space = new QSpacerItem(50,10);
    top_bar->addSpacerItem(horizontal_space);

    //Dog capture count
    QLabel* score = new QLabel("Dogs left:");
    score->setFont(f);
    top_bar->addWidget(score);

    if (gametype_indicator == 1) capture_count = 3;
    if (gametype_indicator == 2) capture_count = 5;
    if (gametype_indicator == 3) capture_count = 7;
    capture_counter = new QLabel*[capture_count];

    for(size_t i=0;i<capture_count;i++) {
        capture_counter[i] = new QLabel;
        capture_counter[i]->setPixmap(*dog_image);
        capture_counter[i]->setMinimumSize(20,20);
        capture_counter[i]->setMaximumSize(20,20);
        capture_counter[i]->setAlignment(Qt::AlignRight);
        capture_counter[i]->setScaledContents(true);
        top_bar->addWidget(capture_counter[i]);
    }

/*****************************************************************/


    // This code Creates the Board
    Board = new QWidget;
    labels = new QLabel*[board_size*board_size];
    QGridLayout *SquareGrid = new QGridLayout(Board);

    SquareGrid->setGeometry(QRect());
    SquareGrid->setSpacing(0);

    std::uniform_int_distribution<int> distribution(1,10);

    // Create each label for the board, initialize with random value.
    for(size_t i=0;i<board_size;i++) {
        for(size_t j=0;j<board_size;j++) {

            // Create label and set properties.
            labels[i*board_size+j] = new QLabel;
            labels[i*board_size+j]->setMinimumSize(40,40);
            labels[i*board_size+j]->setMaximumSize(40,40);
            labels[i*board_size+j]->setPixmap(*grass_image);
            labels[i*board_size+j]->setAlignment(Qt::AlignHCenter);
            labels[i*board_size+j]->setScaledContents(true);

            // Add label to the layout
            SquareGrid -> addWidget(labels[i*board_size+j] ,i,j);

        }
    }

    // Initialize the avatar to center bottom
    avatar_position = new QPoint;
   if (gametype_indicator==1) {avatar_position = new QPoint(5,9);}
   if (gametype_indicator==2) {avatar_position = new QPoint(8,14);}
   if (gametype_indicator==3) {avatar_position = new QPoint(10,19);}


    //    Top
    // --------
    //   Board

      setFixedSize(500,550);

      // Create a vertical box layout for the two pieces
      QVBoxLayout *piece_together = new QVBoxLayout;

      piece_together->addWidget(Top,0,Qt::AlignCenter);
      piece_together->addWidget(Board,0,Qt::AlignCenter);

      QPushButton* quit = new QPushButton("Quit");
      QObject::connect(quit, SIGNAL(clicked()), parent, SLOT(returnMenu()));

      piece_together->addWidget(quit);

      this->setLayout(piece_together);

      //roads = new QLabel;

      //Cars - diff # for each difficulty level
      if (gametype_indicator==1) { number_cars = 3; }
      if (gametype_indicator==2) { number_cars = 5; }
      if (gametype_indicator==3) { number_cars = 7; }

      car_positions = new QPoint[number_cars];

      //Start cars at left of board
      for (size_t i =0; i<number_cars; ++i) {
          car_positions[i].setX(-1);

          size_t rand = distribution(generator)%board_size;
          car_positions[i].setY(rand);

          yvals.push_back(car_positions[i].y());

          //Draw roads
          for (size_t j = 0; j<board_size; ++j) {
              labels[rand*board_size+j]->setMinimumSize(40,40);
              labels[rand*board_size+j]->setMaximumSize(40,40);
              labels[rand*board_size+j]->setPixmap(*asphalt_image);
              labels[rand*board_size+j]->setAlignment(Qt::AlignCenter);
              labels[rand*board_size+j]->setScaledContents(true);
          }
      }


      //Dogs
      if (gametype_indicator == 1) number_dog = 3;
      if (gametype_indicator == 2) number_dog = 5;
      if (gametype_indicator == 3) number_dog = 7;
      dog_positions = new QPoint[number_dog];


      // Set dogs randomly around board
      for (size_t i =0; i<number_dog; ++i) {
          dog_positions[i].setX(distribution(generator)%board_size);
          dog_positions[i].setY(distribution(generator)%board_size);
      }

      //Trees
     if (gametype_indicator == 1) number_trees = 10;
     if (gametype_indicator == 2) number_trees = 15;
     if (gametype_indicator == 3) number_trees = 30;
     tree_positions = new QPoint[number_trees];

     for (size_t i =0; i<number_trees; ++i) {
         int x = distribution(generator)%board_size;

         int y ;
         do {
             y = distribution(generator)%board_size;
         } while (y == yvals[0] || y == yvals[1] || y == yvals[2] || y == yvals[3] || y == yvals[4]
                  || y == yvals[5] || y == yvals[6]);


         treex.push_back(x);
         treey.push_back(y);
         tree_positions[i].setX(x);
         tree_positions[i].setY(y);
         labels[y*board_size+i]->setPixmap(*tree_image);
         labels[y*board_size+i]->setAlignment(Qt::AlignCenter);
     }


     connect(this, SIGNAL(gameOver()), parent, SLOT(gameOver()));
     connect(this, SIGNAL(lostGame()), parent, SLOT(lostGame()));
     connect(this, SIGNAL(wonGame()), parent, SLOT(wonGame()));
       connect(this, SIGNAL(mediumGame()), parent, SLOT(mediumGame()));
       connect(this, SIGNAL(hardGame()), parent, SLOT(hardGame()));
       connect(this, SIGNAL(returnMenu()), parent, SLOT(returnMenu()));


      QTimer *timer = new QTimer(this);
      connect(timer, SIGNAL(timeout()), this, SLOT(updateDogs()));
      timer->start(dog_speed);


}

/** Gameboard destructor;
 */
gameBoard::~gameBoard()
{
    delete [] dog_positions;
    delete [] avatar_position;
    delete [] car_positions;
    delete [] tree_positions;

    delete ui;
}

/** Key input for movement and capture
 * @param *event is key press
*/
void gameBoard::keyPressEvent(QKeyEvent *event){

    int x = avatar_position->rx();
    int y = avatar_position->ry();

    switch (event->key()) {
    case Qt::Key_Left:
        if(avatar_position->rx() != 0)
            updateAvatar(x,y,x-1,y);

        break;
    case Qt::Key_Right:
        /*for (size_t i=0;i<number_trees;i++) {
            int px = tree_positions[i].rx();
            int py = tree_positions[i].ry();
            if (x == px && y == py) {
            avatar_position->setX(x);
            avatar_position->setY(y);
        }
            else if(x != px && y != py && avatar_position->rx() != board_size-1)
                updateAvatar(x,y,x+1,y);
        }
        */
        /*for (size_t i=0;i<number_trees;i++) {
         int px = treex[i];
         int py = treey[i];
         if ( x != px && y != py)
             updateAvatar(x,y,x+1,y);
        }*/

        if (avatar_position->rx() != board_size-1)
                updateAvatar(x,y,x+1,y);

        break;
    case Qt::Key_Up:
        if(avatar_position->ry() != 0)
            updateAvatar(x,y,x,y-1);

        break;
    case Qt::Key_Down:
        if(avatar_position->ry() != board_size-1)
            updateAvatar(x,y,x,y+1);

        break;

    case Qt::Key_Space:
        catchDog(x,y);

        break;

    default:
        QWidget::keyPressEvent(event);
    }
    return;
}

/** Code for QPaintEvent, show pixmaps for avatar, dogs, cars
 * @param *e is QPaintEvent
*/
void gameBoard::paintEvent(QPaintEvent *e) {

    int x = avatar_position->rx();
    int y = avatar_position->ry();

    labels[y*board_size+x]->setPixmap(*avatar_image);
    labels[y*board_size+x]->setAlignment(Qt::AlignHCenter);
    labels[y*board_size+x]->setScaledContents(true);

    for(size_t i=0;i<number_dog;i++) {
        int px = dog_positions[i].rx();
        int py = dog_positions[i].ry();

        if(px >= 0 && py >= 0 && px < (int)board_size && py < (int)board_size) {
           // scaledPix = new QPixmap(*dog_image->scaled((width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation))
            labels[py*board_size+px]->setPixmap(*dog_image);
            labels[py*board_size+px]->setAlignment(Qt::AlignRight);
            labels[py*board_size+px]->setScaledContents(true);
        }
    }

    for(size_t j=0;j<number_cars;j++) {
        int px = car_positions[j].rx();
        int py = car_positions[j].ry();

        if(px >= 0 && py >= 0 && px < (int)board_size && py < (int)board_size) {

            labels[py*board_size+px]->setPixmap(*car_image);
            labels[py*board_size+px]->setScaledContents(true);
            labels[py*board_size+px]->setAlignment(Qt::AlignCenter);
        }
    }


}

/** shows paint event
 * @param *e is QPaintEvent
*/
void gameBoard::showEvent(QShowEvent *e) {

    this->activateWindow();
    this->setFocus();
    QWidget::showEvent(e);
}

/** Update avatar position and subtract a life if it encounters a car
 * @param px is avatar current x position
 * @param py is avatar current y position
 * @param nx is new avatar x position
 * @param ny is new avatar y position
*/
void gameBoard::updateAvatar(int px, int py, int nx, int ny) {


   /* std::vector<int> yvals (7);
    for (int x =0 ; x<number_cars; ++x){
        yvals.push_back(car_positions[x].y());
    }*/

    //set back to asphalt for roads
    if (gametype_indicator==1) {
    if ( py == yvals[0] || py == yvals[1] || py == yvals[2]) {
         labels[py*board_size+px]->setPixmap(*asphalt_image);
    }
    //or set back to grass
    else labels[py*board_size+px]->setPixmap(*grass_image);
    }

    if (gametype_indicator==2) {
    if ( py == yvals[0] || py == yvals[1] || py == yvals[2] || py == yvals[3] || py == yvals[4]) {
         labels[py*board_size+px]->setPixmap(*asphalt_image);
    }
    //or set back to grass
    else labels[py*board_size+px]->setPixmap(*grass_image);
    }

    if (gametype_indicator==3) {
    if ( py == yvals[0] || py == yvals[1] || py == yvals[2] || py == yvals[3] || py == yvals[4]
         || py == yvals[5] || py == yvals[6]) {
         labels[py*board_size+px]->setPixmap(*asphalt_image);
    }
    //or set back to grass
    else labels[py*board_size+px]->setPixmap(*grass_image);
    }


    avatar_position->setX(nx);
    avatar_position->setY(ny);

    for (size_t j = 0; j<number_cars; j++) {
        int jx = car_positions[j].x();
        int jy = car_positions[j].y();
        if (jx == nx && jy == ny) {
            if (life_count > 1) {
                life_count--;
                life_counter[life_count-1]->clear();
            }
            else {
                //this->lostGame();
                this->gameOver();
                life_count = 4;

            }

        }
     }
 }



/** Called to attempt dog catch
 * @returns true if dog is caught
*/
bool gameBoard::catchDog() {
    int x = avatar_position->rx();
    int y = avatar_position->ry();


    if(isValidCatch(x,y)) {
      updateAfterCatch(true);
      return true;
    }
    else {
      updateAfterCatch(false);
      return false;
    }
}

/** check if dog is caught, calls updateAfterCatch if true
 * @param x is avatar x position
 * @param y is avatar y position
 * @returns true if catch is valid
*/
bool gameBoard::catchDog(int x, int y) {

    if(isValidCatch(x,y)) {
        updateAfterCatch(true);
        return true;
     }
    else {
        updateAfterCatch(false);
        return false;
    }
}

/** check if dog is caught
 * @param x is avatar x position
 * @param y is avatar y position
 * @returns true if catch is valid
*/
bool gameBoard::isValidCatch(int x, int y) {
    for (size_t j = 0; j<number_dog; j++) {
        int jx = dog_positions[j].x();
        int jy = dog_positions[j].y();
        if (jx == x && jy == y) {
            dog_positions[j].setX(-100);
            dog_positions[j].setY(-100);
            return true;
        }
    }
    return false;
}

/** update dog count if one is captured
 * @param flag reflects if catch is valid
*/
void gameBoard::updateAfterCatch(bool flag) {
    int x = avatar_position->rx();
    int y = avatar_position->ry();

    if(flag) {
        if(capture_count > 1) {
         capture_count--;
         capture_counter[capture_count-1]->clear();
         //number_dog--;
        }
         else {
            wonGame();
            if (gametype_indicator == 1) {
                this->close();
                life_count = 4;
                this->mediumGame();

            }
            if (gametype_indicator == 2) {
                this->close();
                 life_count = 4;
                this->hardGame();

            }
            if (gametype_indicator == 3) {
                this->returnMenu();
            }
        }
    }
    else return;
}

/** Move dogs and cars

*/
void gameBoard::updateDogs() {

    std::uniform_real_distribution<double> p(0,1);
    std::uniform_int_distribution<int> x_or_y(0,1);
    std::uniform_int_distribution<int> increment(0,1);

    for(size_t i=0;i<number_dog;i++) {

        int px = dog_positions[i].rx();
        int py = dog_positions[i].ry();

        if(px>=0 && py >= 0 && px<(int)board_size && py<(int)board_size) {

            //set back to asphalt for roads
            if (gametype_indicator==1) {
            if ( py == yvals[0] || py == yvals[1] || py == yvals[2]) {
                 labels[py*board_size+px]->setPixmap(*asphalt_image);
            }
            //or set back to grass
            else labels[py*board_size+px]->setPixmap(*grass_image);
            }

            //set back to asphalt for roads
            if (gametype_indicator==2) {
            if ( py == yvals[0] || py == yvals[1] || py == yvals[2] || py == yvals[3] || py == yvals[4]) {
                 labels[py*board_size+px]->setPixmap(*asphalt_image);
            }
            //or set back to grass
            else labels[py*board_size+px]->setPixmap(*grass_image);
            }

            //set back to asphalt for roads
            if (gametype_indicator==3) {
            if ( py == yvals[0] || py == yvals[1] || py == yvals[2] || py == yvals[3] || py == yvals[4]
                 || py == yvals[5] || py == yvals[6]) {
                 labels[py*board_size+px]->setPixmap(*asphalt_image);
            }
            //or set back to grass
            else labels[py*board_size+px]->setPixmap(*grass_image);
            }
        }

        int nx = px;
        int ny = py;

        // If it is time to move this dog, then
        if(p(generator) < 0.1) {

            if(x_or_y(generator) == 0) {
                nx = px + 2*increment(generator)-1;
                if(nx >= -1 && nx <= (int)board_size)
                    dog_positions[i].setX(nx);
            }
            else {
                ny = py + 2*increment(generator)-1;
                if(ny >= -1 && ny <= (int)board_size)
                    dog_positions[i].setY(ny);

            }

        }


        //Update cars
        for(size_t j=0;j<number_cars;j++) {

            int px = car_positions[j].rx();
            int py = car_positions[j].ry();

            if(px>=0 && py >= 0 && px<(int)board_size && py<(int)board_size) {
                labels[py*board_size+px]->setMinimumSize(40,40);
                labels[py*board_size+px]->setMaximumSize(40,40);
                labels[py*board_size+px]->setPixmap(*asphalt_image);
                labels[py*board_size+px]->setAlignment(Qt::AlignCenter);
                labels[py*board_size+px]->setScaledContents(true);
           }

            int nx = px;
            int ny = py;

            // If it is time to move this car, then
            if(p(generator) < 0.1) {

                if(x_or_y(generator) == 0) {
                nx = px + 1;
                //send car back to left if it goes off board
                if (nx > board_size) {car_positions[j].setX(-1);}
                    else {
                        car_positions[j].setX(nx);
                    }

                //lose life if car hits avatar position
                int ax = avatar_position->rx();
                int ay = avatar_position->ry();

                for (size_t j = 0; j<number_cars; j++) {
                    int jx = car_positions[j].x();
                    int jy = car_positions[j].y();
                    if (jx == ax && jy == ay) {
                        if (life_count > 1) {
                            life_count--;
                            life_counter[life_count-1]->clear();
                        }
                        else {
                            this->gameOver();
                            life_count = 4;
                            //this->lostGame();

                       }
                    }
                 }

            }

                else {
                    break;
                }
            }
}

    repaint();
    QCoreApplication::processEvents();

}
}
