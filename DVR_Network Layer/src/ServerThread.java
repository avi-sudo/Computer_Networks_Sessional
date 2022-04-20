

import java.util.ArrayList;
import java.util.Random;

public class ServerThread implements Runnable {

    NetworkUtility networkUtility;
    EndDevice endDevice;

    ServerThread(NetworkUtility networkUtility, EndDevice endDevice) {
        this.networkUtility = networkUtility;
        this.endDevice = endDevice;
        System.out.println("Server Ready for client " + NetworkLayerServer.clientCount);
        //NetworkLayerServer.clientCount++;
        new Thread(this).start();
    }

    @Override
    public void run() {
        /**
         * Synchronize actions with client.
         */
        
        /*
        Tasks:
        1. Upon receiving a packet and recipient, call deliverPacket(packet)
        2. If the packet contains "SHOW_ROUTE" request, then fetch the required information
                and send back to client
        3. Either send acknowledgement with number of hops or send failure message back to client
        */

        networkUtility.write(NetworkLayerServer.clientCount);
        networkUtility.write(NetworkLayerServer.endDevices);
        for(int i=0;i<10;i++) {
            Packet pkt=(Packet)networkUtility.read();
            Boolean flag=deliverPacket(pkt);
            if(pkt.getSpecialMessage().equals("SHOW_ROUTE")){
                networkUtility.write(pkt.RoutingPaths);
                networkUtility.write(pkt.hopcount);
                networkUtility.write(pkt.Temp);
            }
            if(flag){
                networkUtility.write("Sending Successfully");
                networkUtility.write(pkt.hopcount);
            }
            else{
                networkUtility.write("Dropped Packet");
            }
        }
        networkUtility.closeConnection();

    }


    public Boolean deliverPacket(Packet p) {

        Boolean f = false;
        double distance,distance2;
        Integer nextHopId = 0;
        int iteration=0;
        int size=NetworkLayerServer.routers.size();
        Integer id = NetworkLayerServer.deviceIDtoRouterID.get(NetworkLayerServer.endDeviceMap.get(p.getSourceIP()).getDeviceID());
        Router s = NetworkLayerServer.routerMap.get(id);
        Integer id2 = NetworkLayerServer.deviceIDtoRouterID.get(NetworkLayerServer.endDeviceMap.get(p.getDestinationIP()).getDeviceID());
        Router d = NetworkLayerServer.routerMap.get(id2);
        if (s.getState() == true) {
            p.RoutingPaths.add(s);
            p.Temp.add(s.getRoutingTable());
            while (nextHopId != id2){
                nextHopId = s.getRoutingTable().get(id2 - 1).getGatewayRouterId();
                iteration++;
                if (nextHopId > 0) {
                    Router n = NetworkLayerServer.routerMap.get(nextHopId);
                    distance = s.getRoutingTable().get(nextHopId - 1).getDistance();
                    if (n.getState() == false) {
                        f = false;
                        s.getRoutingTable().get(nextHopId - 1).setDistance(Constants.INFINITY);
                        s.getRoutingTable().get(nextHopId-1).setGatewayRouterId(-1);
                        RouterStateChanger.islocked = true;
                        //NetworkLayerServer.simpleDVR(s.getRouterId());
                        NetworkLayerServer.DVR(s.getRouterId());
                        RouterStateChanger.islocked = false;
                        synchronized (RouterStateChanger.msg) {
                            RouterStateChanger.msg.notifyAll();
                        }

                        p.RoutingPaths.clear();
                        p.Temp.clear();
                        break;
                        //return f;
                    }
                    //else if (distance >= Constants.INFINITY){// || distance2 >=Constants.INFINITY) {
                    else if(n.getRoutingTable().get(s.getRouterId()-1).getDistance()>=Constants.INFINITY){// || distance>=Constants.INFINITY){
                        f = true;
                        p.hopcount++;
                        s.getRoutingTable().get(nextHopId - 1).setDistance(1);
                        n.getRoutingTable().get(s.getRouterId()-1).setDistance(1);
                        n.getRoutingTable().get(s.getRouterId()-1).setGatewayRouterId(s.getRouterId());
                        RouterStateChanger.islocked = true;
                        //NetworkLayerServer.simpleDVR(n.getRouterId());
                        NetworkLayerServer.DVR(n.getRouterId());
                        p.Temp.add(n.getRoutingTable());
                        RouterStateChanger.islocked = false;
                        synchronized (RouterStateChanger.msg){
                        RouterStateChanger.msg.notifyAll();}
                        s = n;
                        p.RoutingPaths.add(n);
                        //p.Temp.add(n.getRoutingTable());

                    } else {
                        if(id==id2){
                            p.hopcount=0;
                            return true;
                        }
                        p.Temp.add(n.getRoutingTable());
                        s = n;
                        f = true;
                        p.hopcount++;
                        p.RoutingPaths.add(n);
                        if(iteration>size){
                            f=false;
                            p.Temp.clear();
                            p.RoutingPaths.clear();
                            break;
                        }

                    }
                } else {
                    RouterStateChanger.islocked=true;
                    //NetworkLayerServer.DVR(s.getRouterId());
                    RouterStateChanger.islocked=false;
                    synchronized (RouterStateChanger.msg){
                        RouterStateChanger.msg.notifyAll();
                    }
                    p.RoutingPaths.clear();
                    p.Temp.clear();
                    f = false;
                    break;
                    //return false;
                }
            }
        } else {

            p.RoutingPaths.clear();
            p.Temp.clear();
            f = false;
            //return false;
        }
        return f;
    }

        /*
        1. Find the router s which has an interface
                such that the interface and source end device have same network address.
        2. Find the router d which has an interface
                such that the interface and destination end device have same network address.
        3. Implement forwarding, i.e., s forwards to its gateway router x considering d as the destination.
                similarly, x forwards to the next gateway router y considering d as the destination,
                and eventually the packet reaches to destination router d.

            3(a) If, while forwarding, any gateway x, found from routingTable of router r is in down state[x.state==FALSE]
                    (i) Drop packet
                    (ii) Update the entry with distance Constants.INFTY
                    (iii) Block NetworkLayerServer.stateChanger.t
                    (iv) Apply DVR starting from router r.
                    (v) Resume NetworkLayerServer.stateChanger.t

            3(b) If, while forwarding, a router x receives the packet from router y,
                    but routingTableEntry shows Constants.INFTY distance from x to y,
                    (i) Update the entry with distance 1
                    (ii) Block NetworkLayerServer.stateChanger.t
                    (iii) Apply DVR starting from router x.
                    (iv) Resume NetworkLayerServer.stateChanger.t

        4. If 3(a) occurs at any stage, packet will be dropped,
            otherwise successfully sent to the destination router
        */



    /*@Override
    public boolean equals(Object obj) {
        return super.equals(obj); //To change body of generated methods, choose Tools | Templates.
    }*/
}
