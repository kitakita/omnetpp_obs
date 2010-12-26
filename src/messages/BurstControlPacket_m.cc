//
// Generated file, do not edit! Created by opp_msgc 4.1 from messages/BurstControlPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "BurstControlPacket_m.h"

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




BurstControlPacket_Base::BurstControlPacket_Base(const char *name, int kind) : cMessage(name,kind)
{
    this->burstArrivalTime_var = 0;
    this->burstlength_var = 0;
    this->burstPort_var = 0;
    this->burstChannel_var = 0;
}

BurstControlPacket_Base::BurstControlPacket_Base(const BurstControlPacket_Base& other) : cMessage()
{
    setName(other.getName());
    operator=(other);
}

BurstControlPacket_Base::~BurstControlPacket_Base()
{
}

BurstControlPacket_Base& BurstControlPacket_Base::operator=(const BurstControlPacket_Base& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    this->srcAddress_var = other.srcAddress_var;
    this->destAddress_var = other.destAddress_var;
    this->burstArrivalTime_var = other.burstArrivalTime_var;
    this->burstlength_var = other.burstlength_var;
    this->burstPort_var = other.burstPort_var;
    this->burstChannel_var = other.burstChannel_var;
    return *this;
}

void BurstControlPacket_Base::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->srcAddress_var);
    doPacking(b,this->destAddress_var);
    doPacking(b,this->burstArrivalTime_var);
    doPacking(b,this->burstlength_var);
    doPacking(b,this->burstPort_var);
    doPacking(b,this->burstChannel_var);
}

void BurstControlPacket_Base::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->srcAddress_var);
    doUnpacking(b,this->destAddress_var);
    doUnpacking(b,this->burstArrivalTime_var);
    doUnpacking(b,this->burstlength_var);
    doUnpacking(b,this->burstPort_var);
    doUnpacking(b,this->burstChannel_var);
}

IPAddress& BurstControlPacket_Base::getSrcAddress()
{
    return srcAddress_var;
}

void BurstControlPacket_Base::setSrcAddress(const IPAddress& srcAddress_var)
{
    this->srcAddress_var = srcAddress_var;
}

IPAddress& BurstControlPacket_Base::getDestAddress()
{
    return destAddress_var;
}

void BurstControlPacket_Base::setDestAddress(const IPAddress& destAddress_var)
{
    this->destAddress_var = destAddress_var;
}

simtime_t BurstControlPacket_Base::getBurstArrivalTime() const
{
    return burstArrivalTime_var;
}

void BurstControlPacket_Base::setBurstArrivalTime(simtime_t burstArrivalTime_var)
{
    this->burstArrivalTime_var = burstArrivalTime_var;
}

simtime_t BurstControlPacket_Base::getBurstlength() const
{
    return burstlength_var;
}

void BurstControlPacket_Base::setBurstlength(simtime_t burstlength_var)
{
    this->burstlength_var = burstlength_var;
}

int BurstControlPacket_Base::getBurstPort() const
{
    return burstPort_var;
}

void BurstControlPacket_Base::setBurstPort(int burstPort_var)
{
    this->burstPort_var = burstPort_var;
}

int BurstControlPacket_Base::getBurstChannel() const
{
    return burstChannel_var;
}

void BurstControlPacket_Base::setBurstChannel(int burstChannel_var)
{
    this->burstChannel_var = burstChannel_var;
}

class BurstControlPacketDescriptor : public cClassDescriptor
{
  public:
    BurstControlPacketDescriptor();
    virtual ~BurstControlPacketDescriptor();

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

Register_ClassDescriptor(BurstControlPacketDescriptor);

BurstControlPacketDescriptor::BurstControlPacketDescriptor() : cClassDescriptor("BurstControlPacket", "cMessage")
{
}

BurstControlPacketDescriptor::~BurstControlPacketDescriptor()
{
}

bool BurstControlPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<BurstControlPacket_Base *>(obj)!=NULL;
}

const char *BurstControlPacketDescriptor::getProperty(const char *propertyname) const
{
    if (!strcmp(propertyname,"customize")) return "true";
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int BurstControlPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount(object) : 6;
}

unsigned int BurstControlPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *BurstControlPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "srcAddress",
        "destAddress",
        "burstArrivalTime",
        "burstlength",
        "burstPort",
        "burstChannel",
    };
    return (field>=0 && field<6) ? fieldNames[field] : NULL;
}

int BurstControlPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddress")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destAddress")==0) return base+1;
    if (fieldName[0]=='b' && strcmp(fieldName, "burstArrivalTime")==0) return base+2;
    if (fieldName[0]=='b' && strcmp(fieldName, "burstlength")==0) return base+3;
    if (fieldName[0]=='b' && strcmp(fieldName, "burstPort")==0) return base+4;
    if (fieldName[0]=='b' && strcmp(fieldName, "burstChannel")==0) return base+5;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *BurstControlPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "IPAddress",
        "IPAddress",
        "simtime_t",
        "simtime_t",
        "int",
        "int",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : NULL;
}

const char *BurstControlPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int BurstControlPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    BurstControlPacket_Base *pp = (BurstControlPacket_Base *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string BurstControlPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    BurstControlPacket_Base *pp = (BurstControlPacket_Base *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getSrcAddress(); return out.str();}
        case 1: {std::stringstream out; out << pp->getDestAddress(); return out.str();}
        case 2: return double2string(pp->getBurstArrivalTime());
        case 3: return double2string(pp->getBurstlength());
        case 4: return long2string(pp->getBurstPort());
        case 5: return long2string(pp->getBurstChannel());
        default: return "";
    }
}

bool BurstControlPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    BurstControlPacket_Base *pp = (BurstControlPacket_Base *)object; (void)pp;
    switch (field) {
        case 2: pp->setBurstArrivalTime(string2double(value)); return true;
        case 3: pp->setBurstlength(string2double(value)); return true;
        case 4: pp->setBurstPort(string2long(value)); return true;
        case 5: pp->setBurstChannel(string2long(value)); return true;
        default: return false;
    }
}

const char *BurstControlPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        "IPAddress",
        "IPAddress",
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<6) ? fieldStructNames[field] : NULL;
}

void *BurstControlPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    BurstControlPacket_Base *pp = (BurstControlPacket_Base *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getSrcAddress()); break;
        case 1: return (void *)(&pp->getDestAddress()); break;
        default: return NULL;
    }
}


