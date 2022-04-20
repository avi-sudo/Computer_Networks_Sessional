
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Random;

//Work needed
public class Client {
    public static void main(String[] args) throws InterruptedException {
        //NetworkLayerServer.main(args);
        NetworkUtility networkUtility = new NetworkUtility("127.0.0.1", 4444);
        System.out.println("Connected to server");
        /**
         * Tasks
         */
        
        /*
        1. Receive EndDevice configuration from server
        2. Receive active client list from server
        3. for(int i=0;i<100;i++)
        4. {
        5.      Generate a random message
        6.      Assign a random receiver from active client list
        7.      if(i==20)
        8.      {
        9.            Send the message and recipient IP address to server and a special request "SHOW_ROUTE"
        10.           Display routing path, hop count and routing table of each router [You need to receive
                            all the required info from the server in response to "SHOW_ROUTE" request]
        11.     }
        12.     else
        13.     {
        14.           Simply send the message and recipient IP address to server.
        15.     }
        16.     If server can successfully send the message, client will get an acknowledgement along with hop count
                    Otherwise, client will get a failure message [dropped packet]
        17. }
        18. Report average number of hops and drop rate
        */
        Integer clientCount=(Integer)networkUtility.read();
        ArrayList<EndDevice>endDevices=(ArrayList<EndDevice>)networkUtility.read();
        Integer hop=0;
        Integer dropCount=0;
        Integer successCount=0;
        ArrayList<Router>RoutingPath=new ArrayList<>();
        ArrayList<ArrayList<RoutingTableEntry>>RT=new ArrayList<>();
        EndDevice ed1=endDevices.get(clientCount-1);
        System.out.println(ed1.getDeviceID());
        ArrayList<EndDevice> activeClientList=endDevices;
        for(int i=0;i<10;i++){
            Random random = new Random(System.currentTimeMillis());
            int r = Math.abs(random.nextInt(activeClientList.size()));
            //if(i==20) {
                Packet p = new Packet("hello", "SHOW_ROUTE", ed1.getIpAddress(), activeClientList.get(r).getIpAddress());
                networkUtility.write(p);

                RoutingPath = (ArrayList<Router>)networkUtility.read();
                Integer hopCount = (Integer)networkUtility.read();
                RT=(ArrayList<ArrayList<RoutingTableEntry>>)networkUtility.read();
                if (RoutingPath.size() != 0) {
                    int u=i+1;
                    System.out.println("--------------------------------");
                    System.out.println("Packet number: "+u);
                    System.out.println("Source IP: "+p.getSourceIP());
                    System.out.println("Destination IP: "+p.getDestinationIP());
                    System.out.print("Routing Path: ");
                    for (int k = 0; k < RoutingPath.size(); k++) {
                        System.out.print(RoutingPath.get(k).getRouterId() + " ");
                    }
                    System.out.println("HopCount: " + hopCount);
                    for (int j = 0; j < RT.size(); j++) {
                        System.out.println("Router: "+RoutingPath.get(j).getRouterId());
                        System.out.println("Dest Distance NextHop");
                        ArrayList<RoutingTableEntry> q=RT.get(j);
                        for(int k=0;k<q.size();k++) {
                            System.out.println(q.get(k).getRouterId()+" "+q.get(k).getDistance()+" "+q.get(k).getGatewayRouterId());
                            }
                        }
                    }
                else{
                    System.out.println("The packet with special msg is dropped");
                }
            //}
            /*else{
                Packet p=new Packet("hello","nothing",ed1.getIpAddress(),activeClientList.get(r).getIpAddress());
                networkUtility.write(p);

            }*/
            String msg=(String)networkUtility.read();
            if(msg.equals("Sending Successfully")){
                 hop+=(Integer)networkUtility.read();
                 successCount++;
            }
            else if(msg.equals("Dropped Packet")){
                 dropCount++;

            }
        }
        double averageHop;
        averageHop=(hop/10.0);
        System.out.println("Average number of hops: "+averageHop);
        System.out.println("Drop Rate: "+dropCount);
        networkUtility.closeConnection();
    }
}
