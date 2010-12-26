//
// Generated file, do not edit! Created by opp_msgc 4.1 from messages/BurstControlPacket.msg.
//

#ifndef _BURSTCONTROLPACKET_M_H_
#define _BURSTCONTROLPACKET_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0401
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif

// cplusplus {{
#include "IPAddress.h"
// }}



/**
 * Class generated from <tt>messages/BurstControlPacket.msg</tt> by opp_msgc.
 * <pre>
 * message BurstControlPacket {
 *     @customize(true);  
 *     IPAddress srcAddress;
 *     IPAddress destAddress;
 *     simtime_t burstArrivalTime;
 *     simtime_t burstlength;
 *     int burstPort;
 *     int burstChannel;
 * }
 * </pre>
 *
 * BurstControlPacket_Base is only useful if it gets subclassed, and BurstControlPacket is derived from it.
 * The minimum code to be written for BurstControlPacket is the following:
 *
 * <pre>
 * class BurstControlPacket : public BurstControlPacket_Base
 * {
 *   public:
 *     BurstControlPacket(const char *name=NULL, int kind=0) : BurstControlPacket_Base(name,kind) {}
 *     BurstControlPacket(const BurstControlPacket& other) : BurstControlPacket_Base(other.getName()) {operator=(other);}
 *     BurstControlPacket& operator=(const BurstControlPacket& other) {BurstControlPacket_Base::operator=(other); return *this;}
 *     virtual BurstControlPacket *dup() const {return new BurstControlPacket(*this);}
 *     // ADD CODE HERE to redefine and implement pure virtual functions from BurstControlPacket_Base
 * };
 * </pre>
 *
 * The following should go into a .cc (.cpp) file:
 *
 * <pre>
 * Register_Class(BurstControlPacket);
 * </pre>
 */
class BurstControlPacket_Base : public ::cMessage
{
  protected:
    ::IPAddress srcAddress_var;
    ::IPAddress destAddress_var;
    simtime_t burstArrivalTime_var;
    simtime_t burstlength_var;
    int burstPort_var;
    int burstChannel_var;

    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const BurstControlPacket_Base&);
    // make constructors protected to avoid instantiation
    BurstControlPacket_Base(const char *name=NULL, int kind=0);
    BurstControlPacket_Base(const BurstControlPacket_Base& other);
    // make assignment operator protected to force the user override it
    BurstControlPacket_Base& operator=(const BurstControlPacket_Base& other);

  public:
    virtual ~BurstControlPacket_Base();
    virtual BurstControlPacket_Base *dup() const {throw cRuntimeError("You forgot to manually add a dup() function to class BurstControlPacket");}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual IPAddress& getSrcAddress();
    virtual const IPAddress& getSrcAddress() const {return const_cast<BurstControlPacket_Base*>(this)->getSrcAddress();}
    virtual void setSrcAddress(const IPAddress& srcAddress_var);
    virtual IPAddress& getDestAddress();
    virtual const IPAddress& getDestAddress() const {return const_cast<BurstControlPacket_Base*>(this)->getDestAddress();}
    virtual void setDestAddress(const IPAddress& destAddress_var);
    virtual simtime_t getBurstArrivalTime() const;
    virtual void setBurstArrivalTime(simtime_t burstArrivalTime_var);
    virtual simtime_t getBurstlength() const;
    virtual void setBurstlength(simtime_t burstlength_var);
    virtual int getBurstPort() const;
    virtual void setBurstPort(int burstPort_var);
    virtual int getBurstChannel() const;
    virtual void setBurstChannel(int burstChannel_var);
};


#endif // _BURSTCONTROLPACKET_M_H_
