/* Game state */

extern int   score, lives;
extern struct ship player;
extern struct shot shot; 
extern double randInterval;
extern float radians(float degrees);
extern const float Dt;
extern struct particle *active;
extern bool inPlay, paused;
