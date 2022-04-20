
import java.io.*;
import java.net.Socket;
import java.nio.file.Files;
import java.util.Date;
import java.io.BufferedReader;
import java.io.PrintWriter;
import java.io.File;


public class ServerThread extends Thread {
    Socket s;
    //PrintWriter pr;
    //BufferedReader in;

    OutputStream oos;
    InputStream ois;
   // File logFile=new File("E:\\Aa study material (3-2)\\Network\\HttpOffline\\log.txt");
    File logFile=new File(".\\log.txt");
    PrintWriter pw=new PrintWriter(new FileWriter(logFile,true));

    public ServerThread(Socket s,OutputStream oos , InputStream ois) throws IOException {
        this.s=s;
        this.oos=oos;
        this.ois=ois;

    }


    @Override
    public void run() {
        File f=null;
        String content="";
        FileInputStream fis=null;
        BufferedInputStream bis=null;
        File fi=null;
        BufferedReader in=null;
        PrintWriter pr=null;
        FileOutputStream fos=null;

        //System.out.println("HI");
        /*File file = new File("index.html");
        FileInputStream fis = null;
        try {
            fis = new FileInputStream(file);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        BufferedReader br = null;
         try {
         br = new BufferedReader(new InputStreamReader(fis, "UTF-8"));
         } catch (UnsupportedEncodingException e) {
         e.printStackTrace();
         }
        StringBuilder sb = new StringBuilder();
        String line;
       try {
            while(( line = br.readLine()) != null ) {
                sb.append( line );
                sb.append( '\n' );
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        String content = sb.toString();*/
        //System.out.println(content);
       // String content = "abc";
        //while(true)
        //{
        //Socket s = serverConnect.accept();
         in = new BufferedReader(new InputStreamReader(this.ois));
         pr = new PrintWriter(this.oos);
        String input ="";
        try {

            input = in.readLine();



        } catch (IOException e) {
            e.printStackTrace();
        }
        //System.out.println(input);


        // String content = "<html>Hello</html>";
        if (input == null)
        {
            //continue;
            //System.out.println("Error");
        }
        else if (input.length() > 0) {

            if (input.startsWith("GET")) {
                pw.write("HTTP Request: ");
                pw.write(input+"\r\n");
                pw.write("HTTP Response: \r\n");
                //pr.write("HTTP/1.1 200 OK\r\n");
                //System.out.println("HTTP/1.1 200 OK\r\n");
                //pr.write("Server: Java HTTP Server: 1.0\r\n");
                //pr.write("Date: " + new Date() + "\r\n");


                String[] arr=input.split(" ");
                if(arr[1].equalsIgnoreCase("/")){
                     f=new File("root");
                     //System.out.println(f.getName());
                }else{
                    //String a=arr[1].substring(1);
                     f=new File("root"+arr[1]);
                    //System.out.println(f.getName()+"hghhg");
                     //System.out.println(arr[1]);
                     //f=new File("root/dir1");
                }

                if(f.isDirectory()){
                    pr.write("HTTP/1.1 200 OK\r\n");
                    pw.write("HTTP/1.1 200 OK\r\n");
                    pr.write("Server: Java HTTP Server: 1.0\r\n");
                    pw.write("Server: Java HTTP Server: 1.0\r\n");
                    pr.write("Date: " + new Date() + "\r\n");
                    pw.write("Date: " + new Date() + "\r\n");
                     content=makeHTMLCode(f);
                     //System.out.println(content);
                    pr.write("Content-Type: text/html\r\n");
                    pw.write("Content-Type: text/html\r\n");
                    pr.write("Content-Length: " + content.length() + "\r\n");
                    pw.write("Content-Length: " + content.length() + "\r\n");
                   // System.out.println(content.length());
                    pr.write("\r\n");
                    pw.write("\r\n");
                    pr.write(content);
                    pw.write(content+"\r\n");
                    pr.flush();
                    pw.flush();
                }else if(f.isFile()){
                    pr.write("HTTP/1.1 200 OK\r\n");
                    pw.write("HTTP/1.1 200 OK\r\n");
                    pr.write("Server: Java HTTP Server: 1.0\r\n");
                    pw.write("Server: Java HTTP Server: 1.0\r\n");
                    pr.write("Date: " + new Date() + "\r\n");
                    pw.write("Date: " + new Date() + "\r\n");

                     //fi=new File(String.valueOf(f));
                    try {
                        pr.write("Content-Type: application/x-forcedownload\r\n");
                        pw.write("Content-Type: application/x-forcedownload\r\n");
                        pr.write("Content-Length: " + f.length() + "\r\n");
                        pw.write("Content-Length: " + f.length() + "\r\n");
                        //System.out.println(f.length());
                        pr.write("\r\n");
                        pw.write("\r\n");
                        pr.flush();
                        pw.flush();
                         fis=new FileInputStream(f);
                         bis=new BufferedInputStream(fis);
                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    }


                    int count=0;
                    byte[] array=new byte[4096];
                    try {
                        //ObjectOutputStream o=new ObjectOutputStream(this.oos);
                        //System.out.println();
                        while((count=bis.read(array))>0){
                            //System.out.println(count);
                            this.oos.write(array,0,count);
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }else{
                    File file1=new File("error.html");

                    FileInputStream fis1 = null;
                    try {
                        fis1 = new FileInputStream(file1);
                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    }
                    BufferedReader br = null;
                    try {
                        br = new BufferedReader(new InputStreamReader(fis1, "UTF-8"));
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                    }
                    StringBuilder sb = new StringBuilder();
                    String line;
                    try {
                        while(( line = br.readLine()) != null ) {
                            sb.append( line );
                            sb.append( '\n' );
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                    String content1 = sb.toString();
                       // byte[] ar = Files.readAllBytes(file1.toPath());

                        pr.write("HTTP/1.1 404 NotFound\r\n");
                        pw.write("HTTP/1.1 404 NotFound\r\n");
                        System.out.println("HTTP/1.1 404 NotFound\r\n");
                        pr.write("Server: Java HTTP Server: 1.0\r\n");
                        pw.write("Server: Java HTTP Server: 1.0\r\n");
                        pr.write("Date: " + new Date() + "\r\n");
                        pw.write("Date: " + new Date() + "\r\n");
                        pr.write("Content-Type: text/html\r\n");
                        pw.write("Content-Type: text/html\r\n");
                        pr.write("Content-Length: " + content1.length() + "\r\n");
                        pw.write("Content-Length: " + content1.length() + "\r\n");
                        pr.write("\r\n");
                        pw.write("\r\n");
                        pr.write(content1);
                        pw.write(content1+"\r\n");
                        pr.flush();
                        pw.flush();

                }

                /*pr.write("HTTP/1.1 200 OK\r\n");
                System.out.println("HTTP/1.1 200 OK\r\n");
                pr.write("Server: Java HTTP Server: 1.0\r\n");
                pr.write("Date: " + new Date() + "\r\n");

                pr.write("Content-Length: " + content.length() + "\r\n");
                pr.write("\r\n");
                //pr.write(content);
                pr.flush();*/
            } else {

                String[] arr = input.split(" ");
                if (new File(arr[1]).exists()) {


                fi = new File("root\\" + arr[1]);

                try {
                    fos = new FileOutputStream(fi);

                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                }
                int count = 0;
                byte[] array = new byte[4096];
                try {
                    //ObjectOutputStream o=new ObjectOutputStream(this.oos);
                    //System.out.println();
                    while ((count = this.ois.read(array)) > 0) {
                        //System.out.println(count);
                        fos.write(array, 0, count);

                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
                }else{
                    System.out.println("Invalid File");
                }
            }
        }

        try {
            this.s.close();
            this.oos.close();
            this.ois.close();
            //System.out.println("Closed");
        } catch (IOException e) {
            e.printStackTrace();
        }

        //}
    }
    public String makeHTMLCode(File file){
        String st="";
        st+="<html>\n" +
                "\t<head>\n" +
                "\t\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n" +
                "        <link rel=\"icon\" href=\"data:,\">\n" +
                "\t</head>\n" +
                "\t<body>\n" +
                "\t\t<h1> Welcome to CSE 322 Offline 1</h1>\n";// +
               // "\t</body>\n" +
                //"</html>";
                st+="<ul>\n";
        File[] files=file.listFiles();
        for(int i=0;i<files.length;i++){
           // System.out.println(files[i].getPath().substring(4));
            if(files[i].isDirectory()){
                st+="<li><b><a href="+files[i].getPath().substring(4)+">"+files[i].getName()+"</a></b></li>\n";
            }else{
            st+="<li><a href="+files[i].getPath().substring(4)+">"+files[i].getName()+"</a></li>\n";
            }
        }
        st+="</ul>"+
                "\t</body>\n" +
        "</html>\r\n";
        return  st;
    }


}
