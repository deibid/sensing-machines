//
//  Fire.hpp
//  depth-assignment
//
//  Created by David on 10/21/19.
//

#include <stdio.h>
#include "ofColor.h"

class Fire {    
    
public:
    
    int x;
    int y;
    
    bool won = false;
    float fireSize = 60.0f;
    
    float burnFactor = 0.3f;
    float coolFactor = 10.0f;

    void setPosition(int x_, int y_);
    void update(bool swing);
    void draw();
};





