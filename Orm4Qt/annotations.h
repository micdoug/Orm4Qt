#ifndef ANNOTATIONS_H
#define ANNOTATIONS_H

#ifndef ORM4QT_STDSTRING
#include "propertyconcrete.h"
#else
#include "propertyconcretestdstring.h"
#endif
#include "class.h"
#include <memory>
#include "log.h"

#ifdef ORM4QT_DEBUG_OL
#define DEBUG_CREATE_CLASS qCDebug(ORM4QT_OL) << QString("Started creating a new reflection for Class defined in the file '%1' .").arg(__FILE__);
#define DEBUG_COPY_REFLECTION_OBJECT qCDebug(ORM4QT_OL) << QString("Detected a copy object. The tags will be copied from the source and the properties will be rebuild.");
#define DEBUG_REFLECTION_ALREADY_EXISTS qCDebug(ORM4QT_OL) << QString("The reflection object is already initialized. Returning the internal reference.");
#define DEBUG_FINISH_CLASS qCDebug(ORM4QT_OL) << QString("Finished creating a new reflection for Class defined in the file '%1' .").arg(__FILE__);
#define DEBUG_CREATE_PROPERTY(NAME) qCDebug(ORM4QT_OL) << QString("Started creating a new reflection for the Property '%1' defined in the file '%2' .").arg(#NAME).arg(__FILE__);
#define DEBUG_FINISH_PROPERTY(NAME) qCDebug(ORM4QT_OL) << QString("Finished creating a new reflection for the Property '%1' defined in the file '%2' .").arg(#NAME).arg(__FILE__);
#else
#define DEBUG_CREATE_CLASS
#define DEBUG_COPY_REFLECTION_OBJECT
#define DEBUG_REFLECTION_ALREADY_EXISTS
#define DEBUG_FINISH_CLASS
#define DEBUG_CREATE_PROPERTY(NAME)
#define DEBUG_FINISH_PROPERTY(NAME)
#endif

#define ORM4QT_BEGIN \
    private: \
    std::shared_ptr<Orm4Qt::Class> m_reflectionObject =  nullptr;\
    public:\
    std::shared_ptr<Orm4Qt::Class> reflection()\
    {\
        DEBUG_CREATE_CLASS \
        if(m_reflectionObject.use_count()>1) {\
            m_reflectionObject = std::shared_ptr<Orm4Qt::Class>(new Orm4Qt::Class(*m_reflectionObject.get()));\
            DEBUG_COPY_REFLECTION_OBJECT\
        }\
        else if(m_reflectionObject == nullptr) {\
            m_reflectionObject = std::shared_ptr<Orm4Qt::Class>(new Orm4Qt::Class());

#define ORM4QT_END \
        DEBUG_FINISH_CLASS \
        return m_reflectionObject;\
    }

#define CLASS(...) \
        m_reflectionObject->addTagsFromString(#__VA_ARGS__); \
    }\
    else {\
        DEBUG_REFLECTION_ALREADY_EXISTS\
        return m_reflectionObject;\
    }\
    Orm4Qt::Property *p = nullptr;

#define PROPERTY(NAME, ...) \
    DEBUG_CREATE_PROPERTY(NAME) \
    p = new Orm4Qt::PropertyConcrete<decltype(NAME)>([&]() -> decltype(NAME)& { return this->NAME; }); \
    p->addTagsFromString(#__VA_ARGS__); \
    p->addTag("name", #NAME); \
    m_reflectionObject->addProperty(p);\
    DEBUG_FINISH_PROPERTY(NAME)

#define COPYTAGS(OTHER) m_reflectionObject(OTHER.m_reflectionObject)

#endif // ANNOTATIONS_H
