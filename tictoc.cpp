//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

simple Sender
{
    parameters:
        volatile double delay @unit(s);
        @display("i=block/routing");
    gates:
        output out;
}



simple Receiver
{
    parameters:
        @display("i=block/process");
    gates:
        input in;
}



simple Trafficshaper
{
    parameters:
        volatile double Capacity @unit();
        volatile double tokenBucket @unit();
        volatile double tokenDelay @unit(s);
        @display("i=block/process");
    gates:
        input in;
        output out;
}



network Mainnetwork
{
    @display("bgb=435,353");
    submodules:
        sender: Sender {
            parameters:
                @display("i=,cyan;p=326,249");
        }
        trafficshaper: Trafficshaper {
            parameters:
                @display("i=,gold;p=192,151");
        }
        receiver: Receiver {
            parameters:
                @display("i=,gold;p=46,39");
        }
    connections:
        sender.out --> {  delay = 100ms; } --> trafficshaper.in;
        receiver.in <-- {  delay = 100ms; } <-- trafficshaper.out;
}
