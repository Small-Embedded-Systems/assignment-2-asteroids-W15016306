float norm(float value, float min, float max);
float lerp(float min, float max, float value);
float map(float value, float lower, float upper, float min, float max);

extern bool shieldCollision, buttonPressed, shield;
extern int damage;
typedef uint16_t colour_t;

#define rgb(r,g,b) ((uint16_t)(31 * r /255)<<11 | (uint16_t)(63 * g /255)<<5 | (uint16_t)(31 * b /255))

int randrange(int from, int to);
float dist(coordinate_t p1, coordinate_t p2);
extern const float pi;
extern float radians(float degrees);
void scoreControler(void);
void damageControler(void);
