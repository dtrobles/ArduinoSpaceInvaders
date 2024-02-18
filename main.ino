#include <gamma.h>

#include <RGBmatrixPanel.h>

#include <Adafruit_GFX.h>

// define the wiring of the LED screen
const uint8_t CLK = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;

// global constant for the number of Invaders in the game
const int NUM_ENEMIES = 16;

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// the following functions are for printing messages
void print_welcome_with_invaders();
void print_level(int level);
void print_lives(int lives);
void game_over();

class Color {
  public: int red;
  int green;
  int blue;
  Color() {
    red = 0;
    green = 0;
    blue = 0;
  }
  Color(int r, int g, int b) {
    red = r;
    green = g;
    blue = b;
  }
  uint16_t to_333() const {
    return matrix.Color333(red, green, blue);
  }
};

const Color BLACK(0, 0, 0);
const Color RED(4, 0, 0);
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);

class Invader {
  public:
    // Constructors
    Invader() {
      x = 0;
      y = 0;
      strength = 0;
      hitTimes = 0;
    }
  // sets values for private date members x and y
  Invader(int x_arg, int y_arg) {
    x = x_arg;
    y = y_arg;
  }
  // sets values for private data members
  Invader(int x_arg, int y_arg, int strength_arg) {
    x = x_arg;
    y = y_arg;
    strength = strength_arg;
  }

  // sets values for private data members
  void initialize(int x_arg, int y_arg, int strength_arg) {
    x = x_arg;
    y = y_arg;
    strength = strength_arg;
  }

int addHit() {
  hitTimes++;
}

int get_hitTimes() {
  return hitTimes;
}
  // getters
  int get_x() const {
    return x;
  }
  int get_y() const {
    return y;
  }
  int get_strength() const {
    return strength;
  }

  // Moves the Invader down the screen by one row
  // Modifies: y
  void move() {
    erase();
    initialize(x, y - 1, strength);
    draw();
  }

  // draws the Invader if its strength is greater than 0
  // calls: draw_with_rgb
  void draw() {
    if (strength > 0) {
      Color body_color;
      if (strength == 1) {
        body_color = RED;
      } else if (strength == 2) {
        body_color = ORANGE;
      } else if (strength == 3) {
        body_color = YELLOW;
      } else if (strength == 4) {
        body_color = GREEN;
      } else if (strength == 5) {
        body_color = BLUE;
      } else if (strength == 6) {
        body_color = PURPLE;
      } else if (strength == 7) {
        body_color = WHITE;
      } else {
        body_color = BLACK; // Default or error case
      }

      draw_with_rgb(body_color, BLUE);
    }
  }

  // draws black where the Invader used to be
  // calls: draw_with_rgb
  void erase() {
    draw_with_rgb(BLACK, BLACK);
  }

  // Invader is hit by a Cannonball.
  // Modifies: strength
  // calls: draw, erase
  void hit() {
    strength--;
    if (strength <= 0) {
      erase();
    } else {
      draw();
    }
  }

  private: int x;
  int y;
  int strength;
  int hitTimes;

  // draws the Invader
  void draw_with_rgb(Color body_color, Color eye_color) {
    //  ##
    // #**#
    // ####
    // #  #

    // Top row
    matrix.drawPixel(x, y, body_color.to_333());
    matrix.drawPixel(x - 1, y, body_color.to_333());

    // Second row
    matrix.drawPixel(x + 1, y - 1, body_color.to_333());
    matrix.drawPixel(x, y - 1, eye_color.to_333());
    matrix.drawPixel(x - 1, y - 1, eye_color.to_333());
    matrix.drawPixel(x - 2, y - 1, body_color.to_333());

    // Third row
    for (int i = 0; i < 4; i++) {
      matrix.drawPixel(x - i + 1, y - 2, body_color.to_333());
    }

    // Bottom row
    matrix.drawPixel(x + 1, y - 3, body_color.to_333());
    matrix.drawPixel(x - 2, y - 3, body_color.to_333());
  }
};

class Boss {
  public: Boss() {
    x = 0;
    y = 0;
    hitPoints = 0;
    eyeColor = BLUE;
  }

  void initialize(int x_arg, int y_arg, int hitPoints_arg) {
    x = x_arg;
    y = y_arg;
    hitPoints = hitPoints_arg;
  }

  void draw() {
    if (hitPoints > 0) {
      Color body_color = GREEN; // Boss color
      draw_with_rgb(body_color, eyeColor);
    }
  }

  void set_eyeColor(Color value) {
    eyeColor = value;
  }

  void move() {
    erase();
    initialize(x, y - 1, hitPoints);
    draw();
  }

  void hit() {
    hitPoints--;
    Color body_color = RED;
    draw_with_rgb(body_color, eyeColor); //previously said blue
    delay(40);
    body_color = GREEN;
    draw_with_rgb(body_color, eyeColor);
    if (hitPoints <= 0) {
      erase();
    }
  }

  int get_hitPoints() const {
    return hitPoints;
  }

  void erase() {
    draw_with_rgb(BLACK, BLACK);
  }

  int get_x() {
    return x;
  }
  
  int get_y() {
    return y;
  }

  private: int x,
  y,
  hitPoints;
  Color eyeColor;

  void draw_with_rgb(Color body_color, Color eye_color) {

        // design:
    // XXRXXRXX
    // XRRXXRRX
    // RRRRRRRR
    // RBBRRBBR
    // RBRRRRBR
    // RRRRRRRR
    // RRRRRRRR
    // XRRRRRRX

    int offsetX = x - 3; // Adjust as needed to center the boss
    int offsetY = y - 3; // Adjust as needed to position the boss vertically

    char pattern[8][8] = {
        {'X', 'X', 'R', 'X', 'X', 'R', 'X', 'X'},
        {'X', 'R', 'R', 'X', 'X', 'R', 'R', 'X'},
        {'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R'},
        {'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R'},
        {'R', 'B', 'B', 'R', 'R', 'B', 'B', 'R'},
        {'R', 'B', 'R', 'R', 'R', 'R', 'B', 'R'},
        {'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R'},
        {'X', 'R', 'R', 'R', 'R', 'R', 'R', 'X'}
    };

    // Draw the pattern using if-else statements
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (pattern[i][j] == 'R') {
                matrix.drawPixel(offsetX + j, offsetY + i, body_color.to_333());
            } else if (pattern[i][j] == 'B') {
                matrix.drawPixel(offsetX + j, offsetY + i, eye_color.to_333());
            } else if (pattern[i][j] == 'X') {
                matrix.drawPixel(offsetX + j, offsetY + i, BLACK.to_333());
            }
        }
    }
   
  }

};

class Cannonball {
  public: Cannonball() {
    x = 0;
    y = 0;
    fired = false;
    speed = 60;
  }

  // resets private data members to initial values
  void reset() {
    x = 0;
    y = 0;
    fired = false;
  }

  // getters
  int get_x() const {
    return x;
  }
  int get_y() const {
    return y;
  }
  bool has_been_fired() const {
    return fired;
  }

  // sets private data members
  void fire(int x_arg, int y_arg) {
    x = x_arg;
    y = y_arg;
    fired = true;
  }

  void set_speed(int speedNum) {
    speed = speedNum;
  }

  // moves the Cannonball and detects if it goes off the screen
  // Modifies: y, fired
  void move() {
    if (y < 15) {
      draw();
      delay(speed);
      fire(x, y + 1);
      erase();
      if (y == 15) {
        fire(x, y + 1);
        erase();
        fired = false;
      }
    }
  }

  // resets private data members to initial values
  void hit() {
    erase();
    fire(x, y + 1);
    erase();
    fired = false;
  }

  // draws the Cannonball, if it is fired
  void draw() {
    matrix.drawPixel(x, y, YELLOW.to_333());
    matrix.drawPixel(x, y + 1, YELLOW.to_333());
  }

  // draws black where the Cannonball used to be
  void erase() {
    matrix.drawPixel(x, y - 1, BLACK.to_333());
  }

  private: int x;
  int y;
  bool fired;
  int speed;
};

class Dot {
  public: 
  Dot() {
    x = 0;
    y = 0;
  }

  void draw() {
    matrix.drawPixel(x, y, AQUA.to_333());
    matrix.drawPixel(x + 1, y, AQUA.to_333());
    matrix.drawPixel(x, y + 1, AQUA.to_333());
    matrix.drawPixel(x - 1, y, AQUA.to_333());
    matrix.drawPixel(x, y - 1, AQUA.to_333());

  }
  void erase() {
    matrix.drawPixel(x, y, BLACK.to_333());
    matrix.drawPixel(x + 1, y, BLACK.to_333());
    matrix.drawPixel(x, y + 1, BLACK.to_333());
    matrix.drawPixel(x - 1, y, BLACK.to_333());
    matrix.drawPixel(x, y - 1, BLACK.to_333());
  }

  void set_x(int num) {
    x = num;
  }

  void set_y(int num) {
    y = num;
  }

  int get_x() {
    return x;
  }

  int get_y() {
    return y;
  }
  private: 
  int x;
  int y;
};

class Player {
  public: Player() {
    x = 0;
    y = 0;
    lives = 3;
    color = AQUA;
  }

  Color get_Color() {
    return color;
  }

  void set_Color(Color newColor) {
    color = newColor;
  }
  // getters
  int get_x() const {
    return x;
  }

  int get_y() const {
    return y;
  }

  int get_lives() const {
    return lives;
  }

  // setter
  void set_x(int x_arg) {
    x = x_arg;
    return;
  }

  // Modifies: lives
  void die() {
    lives--;
    return;
  }

  // draws the Player
  // calls: draw_with_rgb
  void draw() {
    draw_with_rgb(color);
    return;
  }

  // draws black where the Player used to be
  // calls: draw_with_rgb
  void erase() {
    Color color;
    color = BLACK;
    draw_with_rgb(color);
    return;
  }

  private: int x;
  int y;
  int lives;
  Color color;

  // sets private data members x and y to initial values
  void initialize(int x_arg, int y_arg) {
    x = x_arg;
    y = y_arg;
    return;
  }

  // draws the player
  void draw_with_rgb(Color color) {
    matrix.drawPixel(x, y, color.to_333());
    matrix.drawPixel(x - 1, y, color.to_333());
    matrix.drawPixel(x + 1, y, color.to_333());
    matrix.drawPixel(x, y + 1, color.to_333());

    // int row = x - 1;
    // int height = y;

    // for (int i = 0; i < 3; ++i) {
    //   if (row >= 0 && row <= 31) {
    //     matrix.drawPixel(row, y, color);
    //   }
    //   if (height == (y + 1)) {
    //     matrix.drawPixel(x, height, color);
    //   }
    //   ++row;
    //   ++height;
    // }
    // return;
  }
};

// declaration for ifHit function, its after invaders and canons class defs
int ifHit(Invader invaders[], Cannonball cannon);
int bottomOfInvader(int hitInvader, Cannonball canon, Invader invaders[]);
void movePlayer(Player & player, int potentiometer_value);
void currentPosition();
void moveInvadersFirst(Invader invaders[]);
void moveInvadersSecond(Invader invaders[]);
int ifHit(Invader invaders[], Player player);
int bottomOfInvader(int hitInvader, Player player, Invader invaders[]);
int ifHitColor(Invader invaders[], Cannonball ball);
int bottomOfColorInvader(int hitInvader, Cannonball canon, Invader invaders[]);
int bottomOfBoss(Boss boss, Cannonball ball, int level);
int bottomOfBoss(Boss boss, Player player, int level);
void moveToHere(int num, Player &player);
void moveToStart(Player &player);

class Game {

  public: Game() {
    level = 0;
    time = 0;
    isBeginning = true;
    isColorTime = false;
    isBossTime = false;
    isBossTenHit = false;
    isAiTime = false;
    numInvaders = 7;

  }

  int get_level() {
    return level;
  }

  Player get_player() {
    return player;
  }

  int set_level(int val) {
    level = val;
  }

  void set_isBeginning(bool value) {
    isBeginning = value;
  }

  bool get_isBeginning() {
    return isBeginning;
  }
  // advances the game simulation one step and renders the graphics
  // see spec for details of game
  // Modifies: global variable matrix
  void update(int potentiometer_value, bool button_pressed) {
    if (isBeginning) {
      dot1.set_x(27);
      dot1.set_y(12);
      dot2.set_x(27);
      dot2.set_y(4);
      if (potentiometer_value >= 500 && isColorTime == false) {
        dot2.draw();
        dot1.erase();
        if (button_pressed) {
          matrix.fillScreen(BLACK.to_333());
          ball.hit();
          matrix.setRotation(0);
          isAiTime = true;
          set_isBeginning(false);
          set_level(1);
          makeLevel(get_level());
          player.set_x(31);
          player.draw();
          ball.set_speed(45);
          // player.set_x(31);
          // player.draw();
          // moveToHere(27, player);
          // delay(3000);

          // AI play starts propably should make AI variable to set to true here
        }
      }
      else if (potentiometer_value < 500 && isColorTime == false) {
        dot1.draw();
        dot2.erase();
        if (button_pressed) {
          matrix.fillScreen(BLACK.to_333());
          matrix.setRotation(0);
          isColorTime = true;
          
          
          
        }
      }
    if (isColorTime) {
      Dot dot3;
      dot3.set_x(27);
      dot3.set_y(12);
      dot3.erase();
      matrix.fillScreen(BLACK.to_333());
      setColorInvaders();
      int count = 0;

     
      movePlayer(player, potentiometer_value);
      
      
    if (button_pressed && (!ball.has_been_fired())) {
      ball.fire(player.get_x(), 2);
    }

    if (ball.has_been_fired()) {
      int enemyNum = ifHitColor(colorInvaders, ball);


//add more to this later had just 10
      if (bottomOfColorInvader(enemyNum, ball, colorInvaders) == ball.get_y() && enemyNum != -1) {
        ball.hit();
        colorInvaders[enemyNum].addHit();
        if (enemyNum == 0) {
        player.set_Color(RED);
        ball.set_speed(17);
        }
        else if (enemyNum == 1) {
          player.set_Color(ORANGE);
          ball.set_speed(30);
        }
        else if (enemyNum == 2) {
          player.set_Color(YELLOW);
          ball.set_speed(60);
        }
        else if (enemyNum == 3) {
          player.set_Color(GREEN);
          ball.set_speed(85);
        }
        else if (enemyNum == 4) {
          player.set_Color(BLUE);
          ball.set_speed(105);
        }
      }
      else {
          ball.move();
        }

      for (int i = 0; i < 5; i++) {
        if (colorInvaders[i].get_hitTimes() == 2) {
          ball.hit();
          set_isBeginning(false);
          set_level(1);
          makeLevel(get_level());
        }
      }
    }
    }

      

    

    }

  if (!isBeginning && !isAiTime) {
    int numLives = player.get_lives();
    unsigned long current_time = millis();


  if (level == 5 || level == 10 && boss.get_hitPoints() > 0) {
    isBossTime = true;
  }
  else {
    isBossTime = false;
  }
  int timeAmount = 0;
  if (level == 10 && isBossTenHit) {
    timeAmount = 2000;
  }
  else {
    timeAmount = 3500;
  }
  //time amount is calculated above
    if (current_time > time + timeAmount) {
      if (!isBossTime) {
        bool firstRowStrength = false;
        for (int i = 8; i < 16; i++) {
          if (enemies[i].get_strength() > 0) {
            firstRowStrength = true;
          }
        }

        if (firstRowStrength) {
          moveInvadersSecond(enemies);
        } else if (!firstRowStrength) {
          moveInvadersFirst(enemies);
        }
        time = current_time;
      }
      else {
        boss.move();
        time = current_time;
      }
    }


   movePlayer(player, potentiometer_value);
      

    if (button_pressed && (!ball.has_been_fired())) {
      ball.fire(player.get_x(), 2);
    }

    if (ball.has_been_fired()) {
      if (!isBossTime) {
        int enemyNum = ifHit(enemies, ball);

        if (bottomOfInvader(enemyNum, ball, enemies) <= ball.get_y()) {
          
          enemies[enemyNum].hit();
          ball.hit();

        } else {
          ball.move();
        }
      }
      //here is when it is boss time
      else {
        // finds when the bottom of the boss equals the ball's y coordinate
        if (bottomOfBoss(boss, ball, level) <= ball.get_y()) {
          if (level == 10) {
            boss.set_eyeColor(RED);
            isBossTenHit = true;
          }
          boss.hit();
          ball.hit();
        }
        else {
          ball.move();
        }
      }
    }
  
    if (level_cleared()) {
      level++;
      reset_level();
    }



//checks if the enemy reaches the player or the ground
    bool ifEnemyThere = false;
    for (int i = 0; i < 16; i++) {
      if (enemies[i].get_strength() > 0 && (enemies[i].get_y() - 3) == 0) {
        ifEnemyThere = true;
      }
    }

    int enemyHit = ifHit(enemies, player);
    if (enemyHit > -1) {
      if (bottomOfInvader(enemyHit, player, enemies) == player.get_y()) {
        ifEnemyThere = true;
      }
    }

    //checks if the boss or the player reaches the ground
  if (isBossTime && (boss.get_y() - 3) == 0) {
    ifEnemyThere = true;
  }

  if (isBossTime && (bottomOfBoss(boss, player, level)) == player.get_y() + 1) {
    ifEnemyThere = true;
  }

    if (ifEnemyThere) {
      player.die();
    }

    if (player.get_lives() < 1) {
      game_over();
      return;
    }

    if (numLives != player.get_lives() && player.get_lives() != 0) {
      isBossTenHit = false;
      reset_level();
    }

    


    
   
  }
  // AI code
  if (isAiTime) {
        if (level == 1) {

                  int numLives = player.get_lives();
            unsigned long current_time = millis();


          if (level == 5 || level == 10 && boss.get_hitPoints() > 0) {
            isBossTime = true;
          }
          else {
            isBossTime = false;
          }
          int timeAmount = 0;
          if (level == 10 && isBossTenHit) {
            timeAmount = 2000;
          }
          else {
            timeAmount = 4000;
          }
          //time amount is calculated above
            if (current_time > time + timeAmount) {
              if (!isBossTime) {
                bool firstRowStrength = false;
                for (int i = 8; i < 16; i++) {
                  if (enemies[i].get_strength() > 0) {
                    firstRowStrength = true;
                  }
                }

                if (firstRowStrength) {
                  moveInvadersSecond(enemies);
                } else if (!firstRowStrength) {
                  moveInvadersFirst(enemies);
                }
                time = current_time;
              }
              else {
                boss.move();
                time = current_time;
              }
            }


          if (enemies[numInvaders].get_strength() > 0 && ball.has_been_fired() == false) {
            ball.fire(player.get_x(), 2);
          }

          

          if (ball.has_been_fired()) {
            if (bottomOfInvader(numInvaders, ball, enemies) <= ball.get_y()) {
            enemies[numInvaders].hit();
            ball.hit();

            } else {
              ball.move();
            }
          }
          
          if (enemies[numInvaders].get_strength() <= 0) {
            numInvaders--;
            moveToHere(player.get_x() - 4, player);
          }
          
          if (level_cleared()) {
          level++;
          numInvaders = 15;
          player.set_x(31);
          reset_level();
          player.draw();
          }
        }
        if (level > 1 && level != 5 && level != 10) {

          int numLives = player.get_lives();
          unsigned long current_time = millis();


          if (level == 5 || level == 10 && boss.get_hitPoints() > 0) {
            isBossTime = true;
          }
          else {
            isBossTime = false;
          }
          int timeAmount = 0;
          if (level == 10 && isBossTenHit) {
            timeAmount = 2000;
          }
          else {
            timeAmount = 4000;
          }
          //time amount is calculated above
            if (current_time > time + timeAmount) {
              if (!isBossTime) {
                bool firstRowStrength = false;
                for (int i = 8; i < 16; i++) {
                  if (enemies[i].get_strength() > 0) {
                    firstRowStrength = true;
                  }
                }

                if (firstRowStrength) {
                  moveInvadersSecond(enemies);
                } else if (!firstRowStrength) {
                  moveInvadersFirst(enemies);
                }
                time = current_time;
              }
              else {
                boss.move();
                time = current_time;
              }
            }



          if (enemies[numInvaders].get_strength() > 0 && ball.has_been_fired() == false) {
            ball.fire(player.get_x(), 2);
          }

          if (player.get_x() + 1 == 0) {
            ball.hit();
            moveToStart(player);
          }

          if (ball.has_been_fired()) {
            if (bottomOfInvader(numInvaders, ball, enemies) <= ball.get_y()) {
            enemies[numInvaders].hit();
            ball.hit();

            } else {
              ball.move();
            }
          }
          
          if (enemies[numInvaders].get_strength() <= 0) {
            numInvaders--;
            moveToHere(player.get_x() - 4, player);
          }
          
          if (level_cleared()) {
          level++;
          player.set_x(31);
          numInvaders = 15;
          reset_level();
          player.draw();
          }
        }
     if (level == 5 || level == 10) {

      int numLives = player.get_lives();
    unsigned long current_time = millis();


  if (level == 5 || level == 10 && boss.get_hitPoints() > 0) {
    isBossTime = true;
  }
  else {
    isBossTime = false;
  }
  int timeAmount = 0;
  if (level == 10 && isBossTenHit) {
    timeAmount = 2000;
  }
  else {
    timeAmount = 3500;
  }
  //time amount is calculated above
    if (current_time > time + timeAmount) {
      if (!isBossTime) {
        bool firstRowStrength = false;
        for (int i = 8; i < 16; i++) {
          if (enemies[i].get_strength() > 0) {
            firstRowStrength = true;
          }
        }

        if (firstRowStrength) {
          moveInvadersSecond(enemies);
        } else if (!firstRowStrength) {
          moveInvadersFirst(enemies);
        }
        time = current_time;
      }
      else {
        boss.move();
        time = current_time;
      }
    }


// moves the player to the middle of the boss
      moveToHere(15, player);

      if (boss.get_hitPoints() > 0 && ball.has_been_fired() == false) {
            ball.fire(player.get_x(), 2);
          }

      if (ball.has_been_fired()) {
            if (bottomOfBoss(boss, ball, level) <= ball.get_y()) {
              if (level == 10) {
            boss.set_eyeColor(RED);
            isBossTenHit = true;
          }
            boss.hit();
            ball.hit();

            } else {
              ball.move();
            }
          }

      if (level_cleared()) {
          level++;
          player.set_x(31);
          numInvaders = 15;
          reset_level();
          player.draw();
          }
     } 

  }

} // end of update function


void makeLevel(int levelVal) {
  level = levelVal;
  reset_level();
}

private:
int level;
unsigned long time;
Player player;
Cannonball ball;
Invader enemies[NUM_ENEMIES];
bool isBeginning;
Dot dot1;
Dot dot2;
Invader colorInvaders[5];
bool isColorTime;
bool isBossTime;
bool isBossTenHit;
bool isAiTime;
int numInvaders;



// boss creation + level detection
Boss boss;


// check if Player defeated all Invaders in current level
bool level_cleared() {
if (isBossTime) {
  if (boss.get_hitPoints() <= 0) {
    return true;
  }
  else {
    return false;
  }
}
else {
  int total = 0;
  for (int i = 0; i < NUM_ENEMIES; i++) {
    total += enemies[i].get_strength();
  }
  if (total == 0) {
    return true;
  } else {
    return false;
  }
}

}

void setColorInvaders() {
  int count = 1;
  int xVal = 28;
  for (int i = 0; i < 5; i++) {
    colorInvaders[i].initialize(xVal, 13, count);
    colorInvaders[i].draw();
    xVal -= 6;
    count++;
  }
}

// set up a level
void reset_level() {
    print_level(level);
    print_lives(player.get_lives());

    randomSeed(analogRead(4));

    // setting up level one
    if (level == 1) {
      int xVal = 0;
      for (int i = 0; i < 8; i++) {
        enemies[i].initialize(xVal + 2, 15, 1);
        enemies[i].draw();
        xVal += 4;
      }
    }
    // sets level 2
    else if (level == 2) {
      int xVal = 0;
      int strengthVal = 1;
      int yVal = 15;
      for (int i = 0; i < 16; i++) {
        strengthVal++;
        if (i == 8) {
          yVal = 10;
          xVal = 0;
          strengthVal = 1;
        }
        enemies[i].initialize(xVal + 2, yVal, strengthVal);
        enemies[i].draw();

        xVal += 4;
        if (strengthVal == 2) {
          strengthVal = 0;
        }
      }
    }
    // sets level 3
    else if (level == 3) {
      int xVal = 0;
      int strengthVal = 4;
      int yVal = 15;
      for (int i = 0; i < 16; i++) {
        strengthVal--;
        if (i == 8) {
          yVal = 10;
          xVal = 0;
          strengthVal = 1;
        }
        enemies[i].initialize(xVal + 2, yVal, strengthVal);
        enemies[i].draw();

        xVal += 4;
        if (strengthVal == 1) {
          strengthVal = 6;
        }
      }
    }
    // sets level 4
    else if (level == 4) {
      int xVal = 0;
      int strengthVal = 3;
      int yVal = 15;
      for (int i = 0; i < 8; i++) {
        strengthVal++;

        enemies[i].initialize(xVal + 2, 15, strengthVal);
        enemies[i].draw();

        xVal += 4;
        if (strengthVal == 5) {
          strengthVal = 3;
        }
      }
      // second row in level 4
      xVal = 0;
      strengthVal = 2;
      for (int i = 8; i < 16; i++) {
        strengthVal++;

        enemies[i].initialize(xVal + 2, 10, strengthVal);
        enemies[i].draw();

        xVal += 4;
        if (strengthVal == 3) {
          strengthVal = 1;
        }
      }

    }
    else if (level == 5) {
      boss.initialize(16, 15, 20);
      boss.draw();
    }
    else if (level == 10) {
      boss.initialize(16, 15, 16);
      boss.draw();
    }
    // sets levels 6 and up minus level 10
    else if (level > 5 && level != 10) {
      int xVal = 0;
      int strengthVal = 0;
      int yVal = 15;
      for (int i = 0; i < 16; i++) {
        strengthVal = random(1, 8);
        if (i == 8) {
          yVal = 10;
          xVal = 0;
        }
        enemies[i].initialize(xVal + 2, yVal, strengthVal);
        enemies[i].draw();

        xVal += 4;
      }
    }
  
}
};

// a global variable that represents the game Space Invaders
Game game;

// see https://www.arduino.cc/reference/en/language/structure/sketch/setup/
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();
  print_welcome_with_invaders();
  print_play_menu();



  //game.set_level(1);
  //game.makeLevel(game.get_level());
}

// see https://www.arduino.cc/reference/en/language/structure/sketch/loop/
void loop() {
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);
  Serial.println(potentiometer_value);

  game.update(potentiometer_value, button_pressed);

  delay(50);
}

void currentPosition() {
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);
  Serial.println(potentiometer_value);
}

// displays "Welcome!" screen with 3 invaders below it
void print_welcome_with_invaders() {
  matrix.setRotation(90);
  matrix.setTextSize(1); // Set the text size to 1
  matrix.setCursor(0, 0);
  matrix.println("Welcome!");
  delay(2500);
  matrix.setRotation(0);
  matrix.fillScreen(BLACK.to_333());
  Invader invader1(10, 10, 3); // Create the first invader
  Invader invader2(16, 8, 6); // Create the second invader (slightly higher)
  Invader invader3(22, 10, 3); // Create the third invader
  invader1.draw();
  invader2.draw();
  invader3.draw();
  delay(1000);
  matrix.fillScreen(BLACK.to_333());
  invader1.initialize(10, 8, 3);
  invader2.initialize(16, 10, 6);
  invader3.initialize(22, 8, 3);
  invader1.draw();
  invader2.draw();
  invader3.draw();
  delay(1000);
  matrix.fillScreen(BLACK.to_333());
  invader1.initialize(10, 10, 3);
  invader2.initialize(16, 8, 6);
  invader3.initialize(22, 10, 3);
  invader1.draw();
  invader2.draw();
  invader3.draw();
  delay(2000);

  matrix.fillScreen(BLACK.to_333());
}

void print_play_menu() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setRotation(90);
  matrix.setCursor(0,0);
  matrix.print("AI   Play");
}

// displays Level
void print_level(int level) {
  matrix.fillScreen(BLACK.to_333());
  matrix.setRotation(90);
  matrix.setCursor(0, 0);
  matrix.print("Level: ");
  matrix.print(level);
  delay(3000);
  matrix.fillScreen(BLACK.to_333());
  matrix.setRotation(0);
}

// displays number of lives
void print_lives(int lives) {
  matrix.fillScreen(BLACK.to_333());
  matrix.setRotation(90);
  matrix.setCursor(0, 0);
  matrix.print("Lives: ");
  matrix.print(lives);
  delay(3000);
  matrix.fillScreen(BLACK.to_333());
  matrix.setRotation(0);
}

// displays "game over"
void game_over() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setRotation(90);
  matrix.setCursor(0, 0);
  matrix.print("Game Over");
  delay(3000);
  matrix.fillScreen(BLACK.to_333());
  matrix.setRotation(0);
}
// returns the num of the invader that is on the path of the
// cannon or it reutrns -1 if there is no cannon
int ifHit(Invader invaders[], Cannonball cannon) {
  int xValCan = cannon.get_x();
  int range = floor(xValCan / 4.0);
  if (invaders[range + 8].get_strength() > 0) {
    return range + 8;
  }
  if (invaders[range].get_strength() > 0) {
    return range;
  }
  return -1;
}

int ifHit(Invader invaders[], Player player) {
  int xValCan = player.get_x();
  int range = floor(xValCan / 4.0);
  if (invaders[range + 8].get_strength() > 0) {
    return range + 8;
  }
  if (invaders[range].get_strength() > 0) {
    return range;
  }
  return -1;
}

int ifHitColor(Invader invaders[], Cannonball ball) {
  int xValCan = ball.get_x();
  if (xValCan > 1 && xValCan < 6) {
    return 4;
  }
  if (xValCan > 7 && xValCan < 12) {
    return 3;
  }
  if (xValCan > 13 && xValCan < 18) {
    return 2;
  }
  if (xValCan > 19 && xValCan < 24) {
    return 1;
  }
  if (xValCan > 25 && xValCan < 30) {
    return 0;
  }
  return -1;

}

// returns the yvalue of the bottom invader that was "hit" at the x coordinate
// of canon
int bottomOfInvader(int hitInvader, Cannonball canon, Invader invaders[]) {
  int xVal = 0;
  if (hitInvader < 8) {
    xVal = canon.get_x() - (hitInvader * 4);
  } else {
    xVal = canon.get_x() - ((hitInvader - 8) * 4);
  }
  int yVal = 0;
  if (xVal == 1 || xVal == 2) {
    yVal = invaders[hitInvader].get_y() - 3;
  } else if (xVal == 0 || xVal == 3) {
    yVal = invaders[hitInvader].get_y() - 4;
  }
  return yVal;
}

// find the y value for the bottom of a color invader based on where the canon was fired
int bottomOfColorInvader(int hitInvader, Cannonball canon, Invader invaders[]) {
  int xVal = 0;
  int yVal = 10;
  hitInvader = (hitInvader - 4) * -1;
  xVal = canon.get_x();
  xVal = xVal - (hitInvader * 4) - 2 - (hitInvader * 2);
if (xVal == 1 || xVal == 2) {
    yVal = 10;
  } else if (xVal == 0 || xVal == 3) {
    yVal = 9;
  }
  return yVal;

}

int bottomOfInvader(int hitInvader, Player player, Invader invaders[]) {
  int xVal = 0;
  if (hitInvader < 8) {
    xVal = player.get_x() - (hitInvader * 4);
  } else {
    xVal = player.get_x() - ((hitInvader - 8) * 4);
  }
  int yVal = 0;
  if (xVal == 1 || xVal == 2) {
    yVal = invaders[hitInvader].get_y() - 3;
  } else if (xVal == 0 || xVal == 3) {
    yVal = invaders[hitInvader].get_y() - 4;
  }
  return yVal;
}

int bottomOfBoss(Boss boss, Cannonball ball, int level) {
  int xVal = ball.get_x();
  int bossY = 0;
  if (level == 5 || level == 10) {
    if (xVal == 17 || xVal == 16 || xVal == 20 || xVal == 13) {
      bossY = boss.get_y() - 2;
    }
    else if (xVal == 18 || xVal == 15) {
      bossY = boss.get_y() - 4;
    }
    else if (xVal == 19 || xVal == 14) {
      bossY = boss.get_y() - 3;
    }
  }
  return bossY;
}

int bottomOfBoss(Boss boss, Player player, int level) {
  int xVal = player.get_x();
  int bossY = 0;
  if (level == 5 || level == 10) {
    if (xVal == 17 || xVal == 16 || xVal == 20 || xVal == 13) {
      bossY = boss.get_y() - 1;
    }
    else if (xVal == 18 || xVal == 15) {
      bossY = boss.get_y() - 3;
    }
    else if (xVal == 19 || xVal == 14) {
      bossY = boss.get_y() - 2;
    }
  }
  return bossY;
}

void movePlayer(Player & player, int potentiometer_value) {
  int valueInRange = map(potentiometer_value, 0, 999, 31, 0);
  valueInRange = constrain(valueInRange, 0, 31);
  player.erase();
  player.set_x(valueInRange);
  player.draw();
}

void moveInvadersFirst(Invader invaders[]) {
  for (int i = 0; i < 8; i++) {
    if (invaders[i].get_strength() > 0) {
      invaders[i].move();
    }
  }
}

void moveInvadersSecond(Invader invaders[]) {
  for (int i = 8; i < 16; i++) {
    if (invaders[i].get_strength() > 0) {
      invaders[i].move();
    }
  }
}

// functions for AI
void moveToHere(int num, Player &player) {
  int numToMove = player.get_x() - num;
  for (int i = 0; i < numToMove; i++) {
    player.erase();
    player.set_x(player.get_x() - 1);
    player.draw();
    delay(175);
  }
}

void moveToStart(Player &player) {
  for (int i = 0; i < 32; i++) {
    player.erase();
    player.set_x(player.get_x() + 1);
    player.draw();
    delay(80);
  }
}
