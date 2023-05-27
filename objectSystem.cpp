/*
    by Illya Parfonov (illya334 zeq52giw)
*/

#include <cstdlib>
#include <cstdio>

#include "objectSystem.h"

struct stCoord emtyCoord = { 0, 0, 0 };
struct stAngle emtyAngle = { 0, 0, 0 };
struct stScale emtyScale = { 0, 0, 0 };

struct stGroupObject *mainGroup;
struct stGroupObject *baseGeometryGroup;
struct stCamObject   *mainCamera;

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
    for (uint i = 0; i < size; i++) mem[i] = ((byte*)arr)[i];
    return (void*)mem;
}

struct stEmtyObject* createObject(struct stGroupObject* group /* can be NULL - MainGroup or (-1) - without group */, uint8_t type, struct stCoord* coord, struct stAngle* angle, struct stScale* scale) {
    struct stEmtyObject* newObject = NULL;

    if (group == 0)
        group = mainGroup;

    uint sizeStruct = 0;
    switch (type & ~objType_bVisible) {
    case objType_3Dmodel:
        sizeStruct = sizeof(struct st3DmodelObject);
        break;
    case objType_emty:
        sizeStruct = sizeof(struct stEmtyObject);
        break;
    case objType_group:
        sizeStruct = sizeof(struct stGroupObject);
        break;
    case objType_cam:
        sizeStruct = sizeof(struct stCamObject);
        break;
    case objType_light:
        sizeStruct = sizeof(struct stLightObject);
        break;
    case objType_point:
        sizeStruct = sizeof(struct stPointObject);
        break;
    case objType_rectangle:
        sizeStruct = sizeof(struct stRectangleObject);
        break;
    case objType_2DSprite:
        sizeStruct = sizeof(struct st2DSpriteObject);
        break;
    default:
        return NULL;
    }

    newObject = (struct stEmtyObject*) malloc(sizeStruct);

    newObject->type = type;
    newObject->name = NULL;

    if ((type & ~objType_bVisible) == objType_group) {
        ((struct stGroupObject*)newObject)->objects = null;
        ((struct stGroupObject*)newObject)->indexObject = null;
    }

    if (group != (struct stGroupObject*)(-1)) group->addObject(newObject);
    else {
        newObject->backGroup = null;
        newObject->indexThisObjectInBackGroup = null;
    }

    if ((type & ~objType_bVisible) == objType_3Dmodel) {
        ((struct st3DmodelObject*)newObject)->textureObj = -1;
        ((struct st3DmodelObject*)newObject)->fileModel = null;
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

    return newObject;
}

// TODO!!!!
void handlingObject(struct stGroupObject* object) { // TODO
    
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

void stEmtyObject::setVisible(bool type) {
    if (this == null) return;
    if (type == true) this->type = this->type | objType_bVisible;
    else this->type = this->type & ~objType_bVisible;
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
    return mainGroup;
}

void objectSystemInit() {
    mainGroup = (struct stGroupObject*)createObject((struct stGroupObject*)objType_nogoup, objType_group | objType_bVisible, null, null, null);
    mainGroup->name = (char*) "mainGroup";

    baseGeometryGroup = (struct stGroupObject*)createObject(mainGroup, objType_group | objType_bVisible, null, null, null);
    baseGeometryGroup->name = (char*) "baseGeometryGroup";

    mainCamera = (struct stCamObject*)createObject(mainGroup, objType_cam, null, null, null);
    mainCamera->name = (char*)"mainCamera";
}

void delObject(struct stEmtyObject* object) {
    if (object == null) return;

    if ((object->type & ~objType_bVisible) == objType_group && ((struct stGroupObject*)object)->objects != null) {
        for (uint i = 0; i < ((struct stGroupObject*)object)->indexObject; i++) {
            struct stEmtyObject* tmpobj = ((struct stGroupObject*)object)->objects[i];

            if ((object->type & ~objType_bVisible) == objType_group) delObject(tmpobj);

            free(tmpobj);
        }
    }
    if (object != mainGroup) {
        if(object->backGroup != null) object->backGroup->subObject(object);
        free(object);
    }
}

void moveObject(struct stEmtyObject* object, struct stGroupObject* toGroup) {
    if (object->backGroup != null) object->backGroup->subObject(object);
    toGroup->addObject(object);
}

struct stEmtyObject* copyObject(struct stEmtyObject* object, struct stGroupObject* toGroup) {
    if (object == null) return null;

    struct stEmtyObject* newObject = (struct stEmtyObject*)memcopy(object, getObjectSizeType(object));
    ((struct stGroupObject*)newObject)->indexObject = 0;
    if ((object->type & ~objType_bVisible) == objType_group && ((struct stGroupObject*)object)->objects != null) {
        for (uint i = 0; i < ((struct stGroupObject*)object)->indexObject; i++) {
            ((struct stGroupObject*)newObject)->objects[i] = copyObject(((struct stGroupObject*)object)->objects[i], null);
            ((struct stGroupObject*)newObject)->indexObject++;
        }
    }

    if (toGroup != null) moveObject(newObject, toGroup);

    return newObject;
}

uint getObjectSizeType(struct stEmtyObject* object) {
    if (object == null) return 0;
    switch (object->type & ~objType_bVisible) {
        case objType_emty:
            return sizeof(struct stEmtyObject);
        case objType_3Dmodel:
            return sizeof(struct st3DmodelObject);
        case objType_group:
            return sizeof(struct stGroupObject);
        case objType_cam:
            return sizeof(struct stCamObject);
        case objType_light:
            return sizeof(struct stLightObject);
        case objType_point:
            return sizeof(struct stPointObject);
        case objType_rectangle:
            return sizeof(struct stRectangleObject);
        case objType_2DSprite:
            return sizeof(struct st2DSpriteObject);
        default:
            return 0;
    }
}

struct stEmtyObject* findObjectByName(struct stGroupObject* group, char* name /*can be null*/, uint ignoreElements) {
    if (group == null) return null;

    for (uint i = 0; i < group->indexObject; i++) {
        if (group->objects[i]->name == name) {
            if (ignoreElements > 0) ignoreElements--;
            else return group->objects[i];
        }
        else if ((group->objects[i]->type & ~objType_bVisible) == objType_group)
            return findObjectByName((struct stGroupObject*)group->objects[i], name, ignoreElements);
    }

    return null;
}

struct stEmtyObject* findObjectByType(struct stGroupObject* group, byte type, uint ignoreElements) {
    if (group == null) return null;

    for (uint i = 0; i < group->indexObject; i++) {
        if ((group->objects[i]->type & type) == type) {
            if (ignoreElements > 0) ignoreElements--;
            else return group->objects[i];
        }
        else if ((group->objects[i]->type & ~objType_bVisible) == objType_group) {
            struct stEmtyObject* obj = findObjectByType((struct stGroupObject*)group->objects[i], type, ignoreElements);
            if (obj != null) return obj;
            if (i >= group->indexObject) return null;
        }
    }

    return null;
}

unsigned int _debugObjectTabs = 0;
void debugObject(struct stEmtyObject* object) { // TODO
    if (object == null) return;
    printf("%s[0x%X]%c(type = ", (object->name == NULL) ? "" : object->name, (uint)((void*)object), (object->type & objType_bVisible) ? 'V' : ' ');

    switch (object->type & ~objType_bVisible) {
    case objType_3Dmodel:
        printf("base");
        break;
    case objType_emty:
        printf("emty");
        break;
    case objType_group:
        printf("group");
        break;
    case objType_cam:
        printf("cam");
        break;
    case objType_light:
        printf("light");
        break;
    case objType_point:
        printf("point");
        break;
    case objType_2DSprite:
        printf("2Dsprite");
        break;
    default:
        printf("%d", (object->type & ~objType_bVisible));
    }

    printf("; pos = {%.2f, %.2f, %.2f};\n\t\t\tangle = {%.2f, %.2f, %.2f};\n", object->pos.x, object->pos.y, object->pos.z, object->angle.x, object->angle.y, object->angle.z);
    for (unsigned int i = 0; i < _debugObjectTabs; i++) printf("\t");
    printf("\t\tscale = {%.2f, %.2f, %.2f}; color = {%d, %d, %d} )", object->scale.x, object->scale.y, object->scale.z, object->color.red, object->color.green, object->color.blue);

    if ((object->type & ~objType_bVisible) == objType_group) {
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