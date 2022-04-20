import java.io.*;
import java.net.Socket;
import java.util.Scanner;

public class Client {
    public static void main(String[] args) throws IOException {
        while(true) {

            Scanner sc=new Scanner(System.in);
            Socket cs = new Socket("localhost", 6781);
            OutputStream os=cs.getOutputStream();
            //ObjectOutputStream os = new ObjectOutputStream(cs.getOutputStream());
            //InputStream is=cs.getInputStream();
            //ObjectInputStream is = new ObjectInputStream(cs.getInputStream());
            String input=sc.nextLine();
            Thread CT = new ClientThread(cs,input, os);//, is);
            CT.start();
        }
    }
}
