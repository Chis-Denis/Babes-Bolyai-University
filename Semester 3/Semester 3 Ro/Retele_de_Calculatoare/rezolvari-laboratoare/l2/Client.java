import java.net.*;
import java.io.*;
 
public class Client {
 
public static void main(String args[]) throws Exception {
  Socket c = new Socket("127.0.0.1", 1234);
  BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
  int a, b, s;
  System.out.print("a = ");
  a = Integer.parseInt(reader.readLine());
  

  DataInputStream socketIn = new DataInputStream(c.getInputStream());
  DataOutputStream socketOut = new DataOutputStream(c.getOutputStream());
  
  socketOut.writeShort(a);
  socketOut.flush();
  System.out.println("s-a scris");
  s = socketIn.readUnsignedShort();
  while(s != 0){
  	System.out.println(" " + s);
	s = socketIn.readUnsignedShort();
	socketOut.flush();
  }
  
    
  reader.close();
  c.close();  
}
 
}
