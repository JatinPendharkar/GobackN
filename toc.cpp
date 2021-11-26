/*
 * Toc111.cc
 *
 *  Created on: Feb 23, 2021
 *      Author: JATIN PENDHARKAR
 */


#include<stdio.h>
#include<string.h>
#include<omnetpp.h>

using namespace omnetpp;

class Toc: public cSimpleModule
{
private:
    int counter = 0;
    int sequence = 0;
    int seq_copy = 0;
    int window_size = 0;
    int initiate_msg_flag = 0;
    int lost_pkt_flag = 0;
    int multiple_pkt_lost_flag = 1;
    int repeat_flag = 1;
protected:
    virtual void handleMessage(cMessage *msg) override;
    virtual void generateMessage();
    virtual void receivedMessage(cMessage *msg);
    virtual void sendMessage(cMessage *msg);
    virtual void receivefirstpkt();
    virtual void receivelastpkt();
    virtual void Messagelost();
};

Define_Module(Toc);

void Toc:: handleMessage(cMessage *msg)
{
    if (uniform(0, 2) < 0.5)
    {
        EV << "Losing message\n";
        bubble("message lost");
        delete msg;

        lost_pkt_flag = 1;
        generateMessage();
        delete msg;
    }
    else
    {
        EV  << "Received Message, sending Ack\n";
        delete msg;
        send(new cMessage("Ack"),"out");

    }
}

void Toc:: generateMessage()
{
    char msgname[20];
    cMessage *msg;
    /* Generating Initial message, and sending window_size size */
    if(!initiate_msg_flag)
    {
        initiate_msg_flag = 1;
        strcpy(msgname, "Receive_Ready");
        msg = new cMessage(msgname);
        msg->addPar("ack_no");
        msg->addPar("win_size");
        msg->par("ack_no").setLongValue(sequence);
        msg->par("win_size").setLongValue(window_size);
        send(msg, "out");
    }
    else
    { /* Generating messages with Acknowledgment number*/
        if (counter <= window_size)
        {
            EV << "Inside generating ack no counter <= window\n";
            counter++;
            receivefirstpkt();
            else if
                  {
                      receivelastpkt();
                  }
                 else if
                  {
                               Messagelost();
                  }
                 else
                   {
                     counter = 0;
                     EV <<"Packetlost";
                   }
        }
    }
}



void Toc:: receivedMessage(cMessage *msg)
{
    if(msg->par("seq_no").longValue() == 0) {
        EV << "'Message received from: " << msg->getName() << "\n";
        EV << "'Data received: \n" << msg->par("seq_no").longValue() << "\n";
        window_size = par("win_size");
        delete msg;
    } else {
        EV << "'Message received from: " << msg->getName() << "\n";
        EV << "'sequence received: \n" << msg->par("seq_no").longValue() << "\n";
        seq_copy = msg->par("seq_no").longValue();
        EV <<"'repeat_flag: " << repeat_flag << "\n";
        if (seq_copy == 1 && repeat_flag) {      /* Start receiving the packets from 1 after the packet limit reached to 255 */
            repeat_flag = 0;
            sequence = 0;
        }
        delete msg;
    }
}

void Toc:: receivefirstpkt()
{
    if ((counter < window_size) && (seq_copy == (sequence + 1))) { /*In  a correct sequence*/
                        EV << "receiving the first packet from Tic\n";
                        if(seq_copy == 255) {
                            EV << " sequence is greater than 255\n";
                            repeat_flag = 1;
                        }
                        sequence = sequence + 1;
                        multiple_pkt_lost_flag = 1;
                    }
}

void Toc:: receivelastpkt()
{
    if ((counter == window_size) && (seq_copy == (sequence + 1)))
    {
        EV << "receiving the last packet of the window size from Tic\n";
        counter = 0;
        sequence = sequence + 1;
        sendMessage(cMessage *msg);
    }
}

void Toc:: Messagelost()
{
    if (lost_pkt_flag && multiple_pkt_lost_flag)
    {
        EV << "A packet no=" <<seq_copy+1<<"is skipped/lost\n";
        counter = 2;
        lost_pkt_flag = 0;
        multiple_pkt_lost_flag = 0;
        sendMessage(cMessage *msg);
    }
}

void Toc:: sendMessage(cMessage *msg)
{
    char msgname[20];
    strcpy(msgname, "Message from toc");
    msg = new cMessage(msgname);
    msg->addPar("ack_no");
    msg->par("ack_no").setLongValue(seq_count);
    send(msg, "out");
