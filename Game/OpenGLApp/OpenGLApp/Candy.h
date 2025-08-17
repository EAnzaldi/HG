#include "MovingObject.h"

enum class CandyType { Bonus1, Malus1 };

class Candy : public MovingObject {
public:
    CandyType type;
    int duration; // seconds

    Candy(glm::vec2 position, glm::vec3 size, Model* model, TextureObject* texture, bool repeatWidth, glm::vec2 velocity, bool moveRight);


private:

};