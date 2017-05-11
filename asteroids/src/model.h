/* Asteroids Model */
struct point {
    float x,y;
};

typedef struct point coordinate_t;
typedef struct point vector_t;
extern struct shot shot;
extern bool collision;
extern double x1, y1, x2, y2, x3, y3, acceleration;
extern float newHeading, currentHeading, bulletAcceleration;
extern int destroyedAsteroids, firedCount, destroyedAsteroids,
hits, asteroidCount, totalAsteroids, firedCount;

struct ship {
	float heading;
    coordinate_t p;
    vector_t     v;
};


typedef struct particle {
	float x,y, v_x, v_y, heading, ttl, distanceCount;
	bool active;
	int size,count, idnum, type;
    struct particle *next;
} node_t;


void initialise_heap(void);
node_t *allocnode(void);
void freenode(node_t *n);
void physics(void);
void particle_system(void);
void wrapping(void);
void shipLeft(void);
void shipRight(void);
void shipUp(void);
void shipDown(void);
void addRock(void);
void setFire(void);
