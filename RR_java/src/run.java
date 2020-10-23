import java.io.IOException;
import java.util.ArrayList;

public class run {
    public static int count;  //整型足以描述10^8
    public static void main(String args[]) throws IOException{
        Prod p1 = new Prod(1,3,1);
        Prod p2 = new Prod(2,4,1);
        Prod p3 = new Prod(3,5,1);
        p1.Run(1000000);
        p2.Run(1000000);
        p3.Run(1000000);
        Cons c = new Cons(7);
        c.Consume3(p1,p2,p3);
        c.deal_result();
    }
}