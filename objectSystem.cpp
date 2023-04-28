/*
    by Illya Parfonov (illya334 zeq52giw)
*/

#include <cstdlib>
#include <cstdio>

#include "objectSystem.h"

struct stCoord emtyCoord = { 0, 0, 0 };
struct stAngle emtyAngle = { 0, 0, 0 };
struct stScale emtyScale = { 0, 0, 0 };

struct stGroupObject mainGroup = {};


uint sortArrAdr(void **arr, uint elements) {
    if (arr == null) return 0;

    uint delElements = 0;

    for (uint i = 0; i < elements; i++) {
        if (arr[i] == null) {
            delElements++;
            for (uint j = i; j < elements; j++) arr[j] = arr[j + 1];
            i--;
        }
    }

    return delElements;
}

void *memcopy(void *arr, uint size) {
    byte* mem = (byte*)malloc(size);
    for (uint i = 0; i < size; i++) mem[i] = arr[i];
}

struct stEmtyObject* createObject(struct stGroupObject* group /* can be NULL - MainGroup or (-1) - without group */, uint8_t type, struct stCoord* coord, struct stAngle* angle, struct stScale* scale) {
    struct stEmtyObject* newObject = NULL;

    if (group == 0)
        group = &mainGroup;

    uint sizeStruct = 0;
    switch (type & ~obj_bVisible) {
    case obj_base:
        sizeStruct = sizeof(struct stBaseObject);
        break;
    case obj_emty:
        sizeStruct = sizeof(struct stEmtyObject);
        break;
    case obj_group:
        sizeStruct = sizeof(struct stGroupObject);
        break;
    case obj_cam:
        sizeStruct = sizeof(struct stCamObject);
        break;
    case obj_light:
        sizeStruct = sizeof(struct stLightObject);
        break;
    case obj_point:
        sizeStruct = sizeof(struct stPointObject);
        break;
    default:
        return NULL;
    }

    newObject = (struct stEmtyObject*) malloc(sizeStruct);

    newObject->type = type;
    newObject->name = NULL;

    if (type == obj_group) {
        ((struct stGroupObject*)newObject)->objects = null;
        ((struct stGroupObject*)newObject)->indexObject = null;
    }

    if (group != (struct stGroupObject*)(-1)) group->addObject(newObject);
    else {
        newObject->backGroup = null;
        newObject->indexThisObjectInBackGroup = null;
    }

    if (coord == null) coord = &emtyCoord;
    if (angle == null) angle = &emtyAngle;
    if (scale == null) scale = &emtyScale;

    newObject->pos.x = coord->x;
    newObject->pos.y = coord->y;
    newObject->pos.z = coord->z;

    newObject->angle.x = angle->x;
    newObject->angle.y = angle->y;
    newObject->angle.z = angle->z;

    newObject->scale.x = scale->x;
    newObject->scale.y = scale->y;
    newObject->scale.z = scale->z;

    newObject->color.red = 0;
    newObject->color.green = 0;
    newObject->color.blue = 0;
    newObject->color.alpha = 255;

#ifdef fuseDEBUG_OBJECTSYSTEM

#endif // fuseDEBUG_OBJECTSYSTEM

    return newObject;
}

void handlingObject(struct stGroupObject* object) { // TODO
    if ((object->type & ~obj_bVisible) == obj_group) {
        struct stGroupObject** objectsArr = (struct stGroupObject**)(object->objects);

        float DposX = object->pos.x - object->lastPos.x;
        float DposY = object->pos.y - object->lastPos.y;
        float DposZ = object->pos.z - object->lastPos.z;

        object->lastPos.x = object->pos.x;
        object->lastPos.y = object->pos.y;
        object->lastPos.z = object->pos.z;

        for (uint i = 0; i < object->indexObject; i++) {
            objectsArr[i]->pos.x += DposX;
            objectsArr[i]->pos.y += DposY;
            objectsArr[i]->pos.z += DposZ;

            if ((objectsArr[i]->type & ~obj_bVisible) == obj_group)
                handlingObject(objectsArr[i]);
        }
    }
}

void stEmtyObject::setScale(float x, float y, float z) {
    this->scale.x = x;
    this->scale.y = y;
    this->scale.z = z;
    handlingObject((struct stGroupObject*)this);
}
void stEmtyObject::setScaleX(float x) {
    this->scale.x = x;
    handlingObject((struct stGroupObject*)this);
}
void stEmtyObject::setScaleY(float y) {
    this->scale.y = y;
    handlingObject((struct stGroupObject*)this);
}
void stEmtyObject::setScaleZ(float z) {
    this->scale.z = z;
    handlingObject((struct stGroupObject*)this);
}

void stEmtyObject::setPos(float x, float y, float z) {
    this->pos.x = x;
    this->pos.y = y;
    this->pos.z = z;
    handlingObject((struct stGroupObject*)this);
}
void stEmtyObject::setPosX(float x) {
    this->pos.x = x;
    handlingObject((struct stGroupObject*)this);
}
void stEmtyObject::setPosY(float y) {
    this->pos.y = y;
    handlingObject((struct stGroupObject*)this);
}
void stEmtyObject::setPosZ(float z) {
    this->pos.z = z;
    handlingObject((struct stGroupObject*)this);
}

void stEmtyObject::setAngle(float x, float y, float z) {
    this->angle.x = x;
    this->angle.y = y;
    this->angle.z = z;
    handlingObject((struct stGroupObject*)this);
}
void stEmtyObject::setAngleX(float x) {
    this->angle.x = x;
    handlingObject((struct stGroupObject*)this);
}
void stEmtyObject::setAngleY(float y) {
    this->angle.y = y;
    handlingObject((struct stGroupObject*)this);
}
void stEmtyObject::setAngleZ(float z) {
    this->angle.z = z;
    handlingObject((struct stGroupObject*)this);
}

void stEmtyObject::setColor(byte red, byte green, byte blue) {
    this->color.red = red;
    this->color.green = green;
    this->color.blue = blue;
}
void stEmtyObject::setColorRed(byte red) {
    this->color.red = red;
}
void stEmtyObject::setColorGreen(byte green) {
    this->color.green = green;
}
void stEmtyObject::setColorBlue(byte blue) {
    this->color.blue = blue;
}

void stGroupObject::addObject(struct stEmtyObject* object) {
    if (object == 0) return;

    this->indexObject++;

    if (this->objects == 0) {
        this->objects = (struct stEmtyObject**) malloc(sizeof(void*));
    } else {
        this->objects = (struct stEmtyObject**)realloc(this->objects, sizeof(void*) * (this->indexObject));
    }

    this->objects[this->indexObject - 1] = object;
    object->backGroup = (struct stGroupObject*)this;
    object->indexThisObjectInBackGroup = this->indexObject-1;
}
void stGroupObject::subObject(struct stEmtyObject* object) { // TODO
    if (this == object->backGroup && object != null && this->objects != null && this != null) {
        this->objects[object->indexThisObjectInBackGroup] = null;
        this->indexObject -= sortArrAdr((void**)this->objects, this->indexObject);
    }
}

struct stGroupObject* getMainGroup() {
    return &mainGroup;
}

void objectSystemInit() {
    mainGroup.name = (char*) "mainGroup";
    mainGroup.type = obj_group | obj_bVisible;
}

void delObject(struct stEmtyObject* object) {
    if (object->type == obj_group) {
        for (uint i = 0; i < ((struct stGroupObject*)object)->indexObject; i++) {
            struct stEmtyObject* tmpobj = ((struct stGroupObject*)object)->objects[i];

            if (tmpobj->type == obj_group) delObject(tmpobj);

            free(tmpobj);
        }
    }
    if (object != &mainGroup) {
        if(object->backGroup != null) object->backGroup->subObject(object);
        free(object);
    }
}

void moveObject(struct stEmtyObject* object, struct stGroupObject* toGroup) {
    if (object->backGroup != null) object->backGroup->subObject(object);
    toGroup->addObject(object);
}

struct stEmtyObject* copyObject(struct stEmtyObject* object, struct stGroupObject* toGroup) {
    
}

unsigned int _debugObjectTabs = 0;
void debugObject(struct stEmtyObject* object) { // TODO
    printf("%s[0x%X]%c(type = ", (object->name == NULL) ? "" : object->name, object, (object->type & obj_bVisible) ? 'V' : ' ');

    switch (object->type & ~obj_bVisible) {
    case obj_base:
        printf("base");
        break;
    case obj_emty:
        printf("emty");
        break;
    case obj_group:
        printf("group");
        break;
    case obj_cam:
        printf("cam");
        break;
    case obj_light:
        printf("light");
        break;
    case obj_point:
        printf("point");
        break;
    default:
        printf("%d", (object->type & ~obj_bVisible));
    }

    printf("; pos = {%.2f, %.2f, %.2f};\n\t\t\tangle = {%.2f, %.2f, %.2f};\n", object->pos.x, object->pos.y, object->pos.z, object->angle.x, object->angle.y, object->angle.z);
    for (unsigned int i = 0; i < _debugObjectTabs; i++) printf("\t");
    printf("\t\tscale = {%.2f, %.2f, %.2f}; color = {%d, %d, %d} )", object->scale.x, object->scale.y, object->scale.z, object->color.red, object->color.green, object->color.blue);

    if ((object->type & ~obj_bVisible) == obj_group) {
        printf(":\n");
        _debugObjectTabs++;

        for (unsigned int index = 0; index < ((struct stGroupObject*)object)->indexObject; index++) {
            for (unsigned int i = 0; i < _debugObjectTabs; i++) printf("\t");

            debugObject(((struct stGroupObject*)object)->objects[index]);

            printf("\n");
        }
    }
    else {
        printf(";\n");
    }

}