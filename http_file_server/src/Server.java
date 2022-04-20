import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
    public static void main(String[] args) throws IOException, ClassNotFoundException {


        ServerSocket ss = new ServerSocket(6781);

        System.out.println("Server started.\nListening for connections on port : " + 6781 + " ...\n");
        //Socket s2=ss2.accept();
        //ObjectInputStream o=new ObjectInputStream(s2.getInputStream());
        //System.out.println(o.readObject());
        while (true){
            Socket s=ss.accept();
            //ObjectOutputStream oos=new ObjectOutputStream(s.getOutputStream());
            OutputStream oos=s.getOutputStream();
            //ObjectInputStream ois=new ObjectInputStream(s.getInputStream());
            InputStream ois=s.getInputStream();
            //BufferedReader in = new BufferedReader(new InputStreamReader(ois));
            //PrintWriter pr = new PrintWriter(oos);
            Thread ST=new ServerThread(s,oos,ois);
            ST.start();
        }

    }
}
