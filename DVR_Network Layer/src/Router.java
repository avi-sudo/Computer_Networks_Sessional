//Work needed

import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

public class Router implements Serializable{
    private int routerId;
    private int numberOfInterfaces;
    private ArrayList<IPAddress> interfaceAddresses;//list of IP address of all interfaces of the router
    private ArrayList<RoutingTableEntry> routingTable;//used to implement DVR
    private ArrayList<Integer> neighborRouterIDs;//Contains both "UP" and "DOWN" state routers
    private Boolean state;//true represents "UP" state and false is for "DOWN" state
    private Map<Integer, IPAddress> gatewayIDtoIP;
    public Router() {
        interfaceAddresses = new ArrayList<>();
        routingTable = new ArrayList<>();
        neighborRouterIDs = new ArrayList<>();

        /**
         * 80% Probability that the router is up
         */
        Random random = new Random();
        double p = random.nextDouble();
        state=true;
       /* if(p < 0.80) state = true;
        else state = false;*/

        numberOfInterfaces = 0;
    }

    public Router(int routerId, ArrayList<Integer> neighborRouters, ArrayList<IPAddress> interfaceAddresses, Map<Integer, IPAddress> gatewayIDtoIP) {
        this.routerId = routerId;
        this.interfaceAddresses = interfaceAddresses;
        this.neighborRouterIDs = neighborRouters;
        this.gatewayIDtoIP = gatewayIDtoIP;
        routingTable = new ArrayList<>();



        /**
         * 80% Probability that the router is up
         */
        Random random = new Random();
        double p = random.nextDouble();
        state=true;
       /* if(p < 0.80) state = true;
        else state = false;*/

        numberOfInterfaces = interfaceAddresses.size();
    }

    @Override
    public String toString() {
        String string = "";
        string += "Router ID: " + routerId + "\n" + "Interfaces: \n";
        for (int i = 0; i < numberOfInterfaces; i++) {
            string += interfaceAddresses.get(i).getString() + "\t";
        }
        string += "\n" + "Neighbors: \n";
        for(int i = 0; i < neighborRouterIDs.size(); i++) {
            string += neighborRouterIDs.get(i) + "\t";
        }
        return string;
    }



    /**
     * Initialize the distance(hop count) for each router.
     * for itself, distance=0; for any connected router with state=true, distance=1; otherwise distance=Constants.INFTY;
     */
    public void initiateRoutingTable() {
        //this.routingTable.clear();
        for(int i=1;i<=NetworkLayerServer.routers.size();i++){
            Router r=NetworkLayerServer.routerMap.get(this.routerId);
            if(i==this.routerId){
                RoutingTableEntry RTE=new RoutingTableEntry(i,0,i);
                this.addRoutingTableEntry(RTE);
            }
            else if(this.neighborRouterIDs.contains(i) && r.getState()==true && NetworkLayerServer.routerMap.get(i).getState()==true){
                RoutingTableEntry RTE=new RoutingTableEntry(i,1,i);
                this.addRoutingTableEntry(RTE);
            }
            else{
                RoutingTableEntry RTE=new RoutingTableEntry(i,Constants.INFINITY,-1);
                this.addRoutingTableEntry(RTE);
            }
        }
        
    }

    /**
     * Delete all the routingTableEntry
     */
    public void clearRoutingTable() {
        this.routingTable.clear();
    }

    /**
     * Update the routing table for this router using the entries of Router neighbor
     * @param neighbor
     */
    public boolean updateRoutingTable(Router neighbor) {
        Boolean flag=false;
        for(int i=1;i<=NetworkLayerServer.routers.size();i++) {
            if (i != this.routerId) {
                double d = 1 + neighbor.routingTable.get(i-1).getDistance();
                if (d < this.routingTable.get(i - 1).getDistance()) {
                    flag=true;
                    this.routingTable.get(i - 1).setDistance(d);
                    this.routingTable.get(i - 1).setGatewayRouterId(neighbor.getRouterId());
                }
            }
        }
        return flag;
    }

    public boolean sfupdateRoutingTable(Router neighbor) {
        Boolean flag=false;
        for(int i=1;i<=NetworkLayerServer.routers.size();i++) {
            if (i != this.routerId){
                double d = 1 + neighbor.routingTable.get(i - 1).getDistance();
                if (this.routingTable.get(i - 1).getGatewayRouterId() == neighbor.getRouterId() ||(d < this.routingTable.get(i - 1).getDistance() && neighbor.routingTable.get(i - 1).getGatewayRouterId() != this.routerId)) {
                    if(d!=this.routingTable.get(i-1).getDistance()) {
                        if (d < Constants.INFINITY || this.routingTable.get(i - 1).getDistance() < Constants.INFINITY) {
                            flag = true;
                        }
                    }
                    this.routingTable.get(i - 1).setDistance(d);
                    this.routingTable.get(i - 1).setGatewayRouterId(neighbor.getRouterId());
                }
            }
        }
        return flag;
    }

    /**
     * If the state was up, down it; if state was down, up it
     */
    public void revertState() {
        state = !state;
        if(state) { initiateRoutingTable();
            //NetworkLayerServer.DVR(this.routerId);
           // NetworkLayerServer.simpleDVR(this.routerId);
            }
        else { clearRoutingTable(); }
    }

    public int getRouterId() {
        return routerId;
    }

    public void setRouterId(int routerId) {
        this.routerId = routerId;
    }

    public int getNumberOfInterfaces() {
        return numberOfInterfaces;
    }

    public void setNumberOfInterfaces(int numberOfInterfaces) {
        this.numberOfInterfaces = numberOfInterfaces;
    }

    public ArrayList<IPAddress> getInterfaceAddresses() {
        return interfaceAddresses;
    }

    public void setInterfaceAddresses(ArrayList<IPAddress> interfaceAddresses) {
        this.interfaceAddresses = interfaceAddresses;
        numberOfInterfaces = interfaceAddresses.size();
    }

    public ArrayList<RoutingTableEntry> getRoutingTable() {
        return routingTable;
    }

    public void addRoutingTableEntry(RoutingTableEntry entry) {
        this.routingTable.add(entry);
    }

    public ArrayList<Integer> getNeighborRouterIDs() {
        return neighborRouterIDs;
    }

    public void setNeighborRouterIDs(ArrayList<Integer> neighborRouterIDs) { this.neighborRouterIDs = neighborRouterIDs; }

    public Boolean getState() {
        return state;
    }

    public void setState(Boolean state) {
        this.state = state;
    }

    public Map<Integer, IPAddress> getGatewayIDtoIP() { return gatewayIDtoIP; }

    public void printRoutingTable() {
        System.out.println("Router " + routerId);
        System.out.println("DestID Distance Nexthop");
        for (RoutingTableEntry routingTableEntry : routingTable) {
            System.out.println(routingTableEntry.getRouterId() + " " + routingTableEntry.getDistance() + " " + routingTableEntry.getGatewayRouterId());
        }
        System.out.println("-----------------------");
    }
    public String strRoutingTable() {
        String string = "Router" + routerId + "\n";
        string += "DestID Distance Nexthop\n";
        for (RoutingTableEntry routingTableEntry : routingTable) {
            string += routingTableEntry.getRouterId() + " " + routingTableEntry.getDistance() + " " + routingTableEntry.getGatewayRouterId() + "\n";
        }

        string += "-----------------------\n";
        return string;
    }

}
