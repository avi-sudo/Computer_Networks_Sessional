import java.io.*;
import java.net.Socket;

public class ClientThread extends Thread{
    Socket cs;
    OutputStream os;
    //ObjectInputStream is;
    String input="";
    public ClientThread(Socket cs,String input,OutputStream os){//,ObjectInputStream is){
        this.cs=cs;
        this.input=input;
        this.os=os;
        //this.is=is;
    }


    @Override
    public void run() {
        File fi=null;
        BufferedReader in=null;
        PrintWriter pr=null;
        FileInputStream fis=null;
        BufferedInputStream bis=null;

        pr = new PrintWriter(this.os);
        if(input.length()>0){

                pr.write(input+"\r\n");
                 pr.flush();
                 String[] arr=input.split(" ");
                 //fi=new File("root\\"+arr[1]);
            if(new File(arr[1]).exists()) {
                try {
                    fis = new FileInputStream(arr[1]);
                    bis = new BufferedInputStream(fis);
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                }

                int count = 0;
                byte[] array = new byte[4096];
                try {
                    //ObjectOutputStream o=new ObjectOutputStream(this.oos);
                    //System.out.println();
                    while ((count = bis.read(array)) > 0) {
                        //System.out.println(count);
                        this.os.write(array, 0, count);

                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
                try {
                    this.cs.close();
                    this.os.close();

                    //System.out.println("Closed");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }else{
                System.out.println("Invalid File");
            }
        }
    }
}
