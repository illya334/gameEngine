/*
    By: Illya Parfonov | (UKR) Ілля Парфьонов [illya334, zeq52giw]
*/

#ifndef OBJECTSYSTEM_H_INCLUDED
    #define OBJECTSYSTEM_H_INCLUDED

    #include "define.h"

    extern struct stGroupObject *mainGroup;
    extern struct stGroupObject* baseGeometry;
    extern struct stCamObject* mainCamera;

    enum {

        obj_err         = -1,
        obj_nogoup      = -1,

        obj_emty        = 0,
        obj_base        = 1,
        obj_group       = 2,
        obj_cam         = 3,
        obj_light       = 4,
        obj_point       = 5,
        obj_rectangle   = 6,
        obj_2DSprite    = 7,

        // MAX 127 types object
        // Незабувати додавати ці типи у функції!

        obj_bVisible    = (1 << 7) // (obj_emty | obj_bVisible)
    };

    typedef struct stCoord {
        float x, y, z;
    } stCoord; // 12 bytes

    typedef struct stScale {
        float x, y, z;
    } stScale; // 12 bytes

    typedef struct stAngle {
        float x, y, z;
    } stAngle; // 12 bytes

    typedef struct stColor {
        byte red, green, blue, alpha;
    } stColor; // 4 bytes

    typedef struct stEmtyObject {
        byte type; // 0

        char* name;
        struct stGroupObject* backGroup;
        uint indexThisObjectInBackGroup;

        struct stScale scale;
        struct stCoord pos;
        struct stColor color;
        struct stAngle angle;

        void setScale(float x, float y, float z);
        void setScaleX(float x);
        void setScaleY(float y);
        void setScaleZ(float z);

        void setPos(float x, float y, float z);
        void setPosX(float x);
        void setPosY(float y);
        void setPosZ(float z);

        void setAngle(float x, float y, float z);
        void setAngleX(float x);
        void setAngleY(float y);
        void setAngleZ(float z);

        void setColor(byte red, byte green, byte blue);
        void setColorRed(byte red);
        void setColorGreen(byte green);
        void setColorBlue(byte blue);

        void setVisible(bool type);
    } stEmtyObject;

    // TODO
    typedef struct stBaseObject : public stEmtyObject {
        //byte type; // 1

        float* vertices;
        unsigned int lenVertices;

        void* fileModel;
        uint textureObj;

        // TODO
        uint addVertice(float x, float y, float z); // (-1) - error, else it is index.
        bool delVerticePos(float x, float y, float z);
        bool delVerticeIndex(uint index);
    } stBaseObject;

    typedef struct stGroupObject : public stEmtyObject {
        //byte type; // 2
        struct stCoord lastPos;
        struct stAngle lastAngle;
        struct stScale lastScale;

        struct stEmtyObject** objects;
        uint indexObject;

        void addObject(struct stEmtyObject* object);
        void subObject(struct stEmtyObject* object);
    } stGroupObject;

    typedef struct stCamObject : public stEmtyObject {
        //byte type; // 3
    } stCamObject;
    
    typedef struct stLightObject : public stEmtyObject {
        //byte type; // 4
    } stLightObject;

    typedef struct stPointObject : public stEmtyObject {
        //byte type; // 5
    } stPointObject;

    typedef struct stRectangleObject : public stEmtyObject {
        //byte type; // 6
        struct stCoord pos2;
        struct stCoord pos3;
        struct stCoord pos4;
    } stRectangleObject;

    typedef struct st2DSpriteObject : public stEmtyObject {
        //byte type; // 7
        void* fileModel;
        uint textureObj;

        struct stCoord pos2;
    } st2DSpriteObject;

    struct stEmtyObject* createObject(struct stGroupObject* group /* can be NULL - MainGroup or (-1) - without group */, uint8_t type, struct stCoord* coord /* can be null */, struct stAngle* angle /* can be null */, struct stScale* scale /* can be null */);
    void delObject(struct stEmtyObject* object);
    void moveObject(struct stEmtyObject* object, struct stGroupObject* toGroup);
    struct stEmtyObject* copyObject(struct stEmtyObject* object, struct stGroupObject* toGroup); // TODO
    uint getObjectSizeType(struct stEmtyObject* object);
    struct stGroupObject* getMainGroup();
    void objectSystemInit();
    struct stEmtyObject* findObjectByName(struct stGroupObject* group, char* name /*can be null*/, uint ignoreElements);
    struct stEmtyObject* findObjectByType(struct stGroupObject* group, byte type, uint ignoreElements);
    void debugObject(struct stEmtyObject* object);

#endif // OBJECTSYSTEM_H_INCLUDED
