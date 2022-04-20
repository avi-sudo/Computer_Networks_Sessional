import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.*;
import java.util.logging.Level;
import java.util.logging.Logger;

//Work needed
public class NetworkLayerServer {

    static int clientCount = 0;
    static ArrayList<Router> routers = new ArrayList<>();
    static RouterStateChanger stateChanger = null;
    static Map<IPAddress,Integer> clientInterfaces = new HashMap<>(); //Each map entry represents number of client end devices connected to the interface
    static Map<IPAddress, EndDevice> endDeviceMap = new HashMap<>();
    static ArrayList<EndDevice> endDevices = new ArrayList<>();
    static Map<Integer, Integer> deviceIDtoRouterID = new HashMap<>();
    static Map<IPAddress, Integer> interfacetoRouterID = new HashMap<>();
    static Map<Integer, Router> routerMap = new HashMap<>();


    public static void main(String[] args) {

        //Task: Maintain an active client list

        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(4444);
        } catch (IOException ex) {
            Logger.getLogger(NetworkLayerServer.class.getName()).log(Level.SEVERE, null, ex);
        }

        System.out.println("Server Ready: " + serverSocket.getInetAddress().getHostAddress());
        System.out.println("Creating router topology");

        readTopology();
        printRouters();

        initRoutingTables(); //Initialize routing tables for all routers

        DVR(1); //Update routing table using distance vector routing until convergence
        //simpleDVR(1);
        for(Router w:routers){
            w.printRoutingTable();
        }
        //stateChanger = new RouterStateChanger();//Starts a new thread which turns on/off routers randomly depending on parameter Constants.LAMBDA
        while(true) {
            try {
                Socket socket = serverSocket.accept();
                System.out.println("Client" + (clientCount + 1) + " attempted to connect");
                EndDevice endDevice = getClientDeviceSetup();
                clientCount++;
                endDevices.add(endDevice);
                endDeviceMap.put(endDevice.getIpAddress(),endDevice);
                //System.out.println(endDeviceMap.get(endDevice.getIpAddress()).getDeviceID());
                new ServerThread(new NetworkUtility(socket),endDevice);
            } catch (IOException ex) {
                Logger.getLogger(NetworkLayerServer.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    public static void initRoutingTables() {
        for (Router router : routers) {
            if (router.getState() == true) {
                router.initiateRoutingTable();
            }
        }
    }

    public static synchronized void DVR(int startingRouterId) {
        /**
         * pseudocode
         */

        /*
        while(convergence)
        {
            //convergence means no change in any routingTable before and after executing the following for loop
            for each router r <starting from the router with routerId = startingRouterId, in any order>
            {
                1. T <- getRoutingTable of the router r
                2. N <- find routers which are the active neighbors of the current router r
                3. Update routingTable of each router t in N using the
                   routing table of r [Hint: Use t.updateRoutingTable(r)]
            }
        }
        */
        //RouterStateChanger.islocked=true;
        ArrayList<Integer>AL=new ArrayList<>();
        AL.add(startingRouterId);
        for(int j=1;j<=NetworkLayerServer.routers.size();j++){
            if(j!=startingRouterId){
                AL.add(j);
            }
        }
       // System.out.println("Locked"+AL.size());
        Boolean flag=true;
        Boolean f=true;
        while(f) {
            ArrayList<Boolean>b=new ArrayList<>();
            for (int i =0; i < AL.size();i++) {
                Router r = NetworkLayerServer.routerMap.get(AL.get(i));
                if (r.getState() == true){
                    //ArrayList<RoutingTableEntry>T=r.getRoutingTable();
                    for (int j = 0; j < r.getNeighborRouterIDs().size(); j++) {
                        Router t = NetworkLayerServer.routerMap.get(r.getNeighborRouterIDs().get(j));
                        if (t.getState() == true) {
                            flag = t.sfupdateRoutingTable(r);
                            b.add(flag);
                        }
                    }
                }
            }
            for(int k=0;k<b.size();k++){
                if(b.contains(true)){
                    f=true;
                    break;
                }
                else{
                    f=false;
                }
            }
        }
        //System.out.println("Unlocked");
        //synchronized (RouterStateChanger.msg){
        //RouterStateChanger.msg.notifyAll();}
        //RouterStateChanger.islocked=false;
    }

    public static synchronized void simpleDVR(int startingRouterId) {
        //RouterStateChanger.islocked=true;
        ArrayList<Integer>AL=new ArrayList<>();
        AL.add(startingRouterId);
        for(int i=1;i<=NetworkLayerServer.routers.size();i++){
            if(i!=startingRouterId){
                AL.add(i);
            }
        }
        //System.out.println("Thread blocked"+AL.size());
        Boolean flag=true;
        Boolean f=true;
        int it=1;
        int s=NetworkLayerServer.routers.size();
        while(f && it<s) {
            it++;
            ArrayList<Boolean>b=new ArrayList<>();
            for (int i = 0; i < AL.size(); i++) {
                Router r = NetworkLayerServer.routerMap.get(AL.get(i));
                if (r.getState() == true) {
                    //ArrayList<RoutingTableEntry>T=r.getRoutingTable();
                    for (int j = 0; j < r.getNeighborRouterIDs().size(); j++) {
                        Router t = NetworkLayerServer.routerMap.get(r.getNeighborRouterIDs().get(j));
                        if (t.getState() == true) {
                            flag = t.updateRoutingTable(r);
                            b.add(flag);
                        }
                    }
                }
            }
            for(int k=0;k<b.size();k++){
                if(b.contains(true)){
                    f=true;
                    break;
                }
                else{
                    f=false;
                }
            }//System.out.println();

        }
        //System.out.println("Thread unlocked");
        //RouterStateChanger.islocked=false;
    }

    public static EndDevice getClientDeviceSetup() {
        Random random = new Random(System.currentTimeMillis());
        int r = Math.abs(random.nextInt(clientInterfaces.size()));

        //System.out.println("Size: " + clientInterfaces.size() + "\n" + r);

        IPAddress ip = null;
        IPAddress gateway = null;

        int i = 0;
        for (Map.Entry<IPAddress, Integer> entry : clientInterfaces.entrySet()) {
            IPAddress key = entry.getKey();
            Integer value = entry.getValue();
            if(i == r) {
                gateway = key;
                ip = new IPAddress(gateway.getBytes()[0] + "." + gateway.getBytes()[1] + "." + gateway.getBytes()[2] + "." + (value+2));
                value++;
                clientInterfaces.put(key, value);
                deviceIDtoRouterID.put(endDevices.size()+1, interfacetoRouterID.get(key));
                break;
            }
            i++;
        }

        EndDevice device = new EndDevice(ip, gateway, endDevices.size()+1);

        System.out.println("Device : " + ip + "::::" + gateway);
        //System.out.println(device.getDeviceID());
        return device;
    }

    public static void printRouters() {
        for(int i = 0; i < routers.size(); i++) {
            System.out.println("------------------\n" + routers.get(i));
        }
    }

    public static String strrouters() {
        String string = "";
        for (int i = 0; i < routers.size(); i++) {
            string += "\n------------------\n" + routers.get(i).strRoutingTable();
        }
        string += "\n\n";
        return string;
    }

    public static void readTopology() {
        Scanner inputFile = null;
        try {
            inputFile = new Scanner(new File("topology.txt"));
            //skip first 27 lines
            int skipLines = 27;
           /* for(int i = 0; i < skipLines; i++) {
                inputFile.nextLine();
            }*/

            //start reading contents
            while(inputFile.hasNext()) {
                inputFile.nextLine();
                int routerId;
                ArrayList<Integer> neighborRouters = new ArrayList<>();
                ArrayList<IPAddress> interfaceAddrs = new ArrayList<>();
                Map<Integer, IPAddress> interfaceIDtoIP = new HashMap<>();

                routerId = inputFile.nextInt();

                int count = inputFile.nextInt();
                for(int i = 0; i < count; i++) {
                    neighborRouters.add(inputFile.nextInt());
                }
                count = inputFile.nextInt();
                inputFile.nextLine();

                for(int i = 0; i < count; i++) {
                    String string = inputFile.nextLine();
                    IPAddress ipAddress = new IPAddress(string);
                    interfaceAddrs.add(ipAddress);
                    interfacetoRouterID.put(ipAddress, routerId);

                    /**
                     * First interface is always client interface
                     */
                    if(i == 0) {
                        //client interface is not connected to any end device yet
                        clientInterfaces.put(ipAddress, 0);
                    }
                    else {
                        interfaceIDtoIP.put(neighborRouters.get(i - 1), ipAddress);
                    }
                }
                Router router = new Router(routerId, neighborRouters, interfaceAddrs, interfaceIDtoIP);
                routers.add(router);
                routerMap.put(routerId, router);
            }


        } catch (FileNotFoundException ex) {
            Logger.getLogger(NetworkLayerServer.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

}
