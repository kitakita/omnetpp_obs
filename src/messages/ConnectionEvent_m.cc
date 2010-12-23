//
// Generated file, do not edit! Created by opp_msgc 4.1 from messages/ConnectionEvent.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "ConnectionEvent_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(ConnectionEvent);

ConnectionEvent::ConnectionEvent(const char *name, int kind) : cMessage(name,kind)
{
    this->in_var = 0;
    this->out_var = 0;
}

ConnectionEvent::ConnectionEvent(const ConnectionEvent& other) : cMessage()
{
    setName(other.getName());
    operator=(other);
}

ConnectionEvent::~ConnectionEvent()
{
}

ConnectionEvent& ConnectionEvent::operator=(const ConnectionEvent& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    this->in_var = other.in_var;
    this->out_var = other.out_var;
    return *this;
}

void ConnectionEvent::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->in_var);
    doPacking(b,this->out_var);
}

void ConnectionEvent::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->in_var);
    doUnpacking(b,this->out_var);
}

int ConnectionEvent::getIn() const
{
    return in_var;
}

void ConnectionEvent::setIn(int in_var)
{
    this->in_var = in_var;
}

int ConnectionEvent::getOut() const
{
    return out_var;
}

void ConnectionEvent::setOut(int out_var)
{
    this->out_var = out_var;
}

class ConnectionEventDescriptor : public cClassDescriptor
{
  public:
    ConnectionEventDescriptor();
    virtual ~ConnectionEventDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(ConnectionEventDescriptor);

ConnectionEventDescriptor::ConnectionEventDescriptor() : cClassDescriptor("ConnectionEvent", "cMessage")
{
}

ConnectionEventDescriptor::~ConnectionEventDescriptor()
{
}

bool ConnectionEventDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ConnectionEvent *>(obj)!=NULL;
}

const char *ConnectionEventDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ConnectionEventDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int ConnectionEventDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *ConnectionEventDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "in",
        "out",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int ConnectionEventDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='i' && strcmp(fieldName, "in")==0) return base+0;
    if (fieldName[0]=='o' && strcmp(fieldName, "out")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ConnectionEventDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *ConnectionEventDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int ConnectionEventDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ConnectionEvent *pp = (ConnectionEvent *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ConnectionEventDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ConnectionEvent *pp = (ConnectionEvent *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getIn());
        case 1: return long2string(pp->getOut());
        default: return "";
    }
}

bool ConnectionEventDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ConnectionEvent *pp = (ConnectionEvent *)object; (void)pp;
    switch (field) {
        case 0: pp->setIn(string2long(value)); return true;
        case 1: pp->setOut(string2long(value)); return true;
        default: return false;
    }
}

const char *ConnectionEventDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *ConnectionEventDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ConnectionEvent *pp = (ConnectionEvent *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


