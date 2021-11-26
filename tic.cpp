/*
 * Tic111.cc
 *
 *  Created on: Feb 23, 2021
 *      Author: JATIN PENDHARKAR
 */

#include<stdio.h>
#include<string.h>
#include<omnetpp.h>

using namespace omnetpp;

class Tic: public cSimpleModule
{
private:
    simtime_t timeout;
    cMessage *timeoutEvent;
    int counter = 3;
    int seq_count = 0;
    int win_size = 0;
    int init_msg_flag = 0;

public:
    Tic();
    virtual ~Tic();

protected:

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void generateMessage();
    virtual void sendMessage(cMessage *msg);
};

Define_Module(Tic);

Tic::Tic()
{
    timeoutEvent = nullptr;
}

Tic::~Tic()
{
    cancelAndDelete(timeoutEvent);
     delete timeoutEvent;
}



void Tic:: initialize() {
    timeout = 3.0;
    timeoutEvent = new cMessage("timeoutEvent");
    // Generate and send initial message.
    EV << "Sending  initial control message\n";
    generateMessage();
    scheduleAt(simTime()+timeout, timeoutEvent);
}

void Tic:: handleMessage(cMessage *msg) {

if (msg == timeoutEvent) {
        EV<< "Timeout expired, re-sending message\n";
        bubble("resend");
        scheduleAt(simTime()+timeout, timeoutEvent);
        }
    else {  // message arrived
            // !! Acknowledgement received!!
        EV << "Query Message Received: " << msg->getName() << "Window Size?\n";
        delete msg;


        EV << "Timer cancelled.\n";
        cancelEvent(timeoutEvent);
        counter= 3;
        delete msg;
    }
}

void Tic::generateMessage()
{
    char msgname[10];
     int i= 0;
     if (!init_msg_flag) {    // sending initial Control message, Quering the window size of the receiver
         init_msg_flag = 1;
         strcpy(msgname,"Control_Request_Windowsize?");
         cMessage *msg = new cMessage(msgname);
         msg->addPar("seq_no");
         msg->par("seq_no").setLongValue(seq_count);
         sendMessage(msg);
     }
         else {                                              //To start with packet transmission
                 strcpy(msgname, "Packet from tic");
                                                           // set source and destination field//
                   for( ; i < win_size; i++) {

                    cMessage *msg[win_size];
                   seq_count = seq_count + 1;

                  msg[i] = new cMessage(msgname);
                  msg[i]->addPar("seq_no");
                  msg[i]->par("seq_no").setLongValue(seq_count);

                sendMessage(msg[i]);

                if (seq_count >= 255) {
                    seq_count = 0;
                }
            }
         }
          return;
}

//Send the message to Toc, //Store the message in buffer

void Tic::sendMessage(cMessage *msg)
{
        send(msg, "out");
}





