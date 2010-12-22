//
// Generated file, do not edit! Created by opp_msgc 4.1 from messages/ConnectionEvent.msg.
//

#ifndef _CONNECTIONEVENT_M_H_
#define _CONNECTIONEVENT_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0401
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif



/**
 * Class generated from <tt>messages/ConnectionEvent.msg</tt> by opp_msgc.
 * <pre>
 * message ConnectionEvent {
 *     int inPort;
 *     int inChannel;
 *     int outPort;
 *     int outChannel;
 * }
 * </pre>
 */
class ConnectionEvent : public ::cMessage
{
  protected:
    int inPort_var;
    int inChannel_var;
    int outPort_var;
    int outChannel_var;

    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const ConnectionEvent&);

  public:
    ConnectionEvent(const char *name=NULL, int kind=0);
    ConnectionEvent(const ConnectionEvent& other);
    virtual ~ConnectionEvent();
    ConnectionEvent& operator=(const ConnectionEvent& other);
    virtual ConnectionEvent *dup() const {return new ConnectionEvent(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getInPort() const;
    virtual void setInPort(int inPort_var);
    virtual int getInChannel() const;
    virtual void setInChannel(int inChannel_var);
    virtual int getOutPort() const;
    virtual void setOutPort(int outPort_var);
    virtual int getOutChannel() const;
    virtual void setOutChannel(int outChannel_var);
};

inline void doPacking(cCommBuffer *b, ConnectionEvent& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, ConnectionEvent& obj) {obj.parsimUnpack(b);}


#endif // _CONNECTIONEVENT_M_H_