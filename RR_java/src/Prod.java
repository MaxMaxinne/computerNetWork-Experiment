import java.util.ArrayList;
public class Prod {
    double Mu;
    int index;
    double  lamda;
    ArrayList<pack> queue;
    int length;
    double currentTime;     //用于记录当前时间线
    public ArrayList<Integer> lengthDis;
    //但队列初始化
    public Prod(double lamda) {
        this.lamda = lamda;
        queue = new ArrayList<pack>();
        lengthDis = new ArrayList<Integer>();
    }
    //多队列初始化
    public Prod(int index,double lamda,double Mu) {
        this.index = index;
        this.lamda = lamda;
        this.Mu = Mu;
        queue = new ArrayList<pack>();
        lengthDis = new ArrayList<Integer>();
    }
    public void getLength() {
        length = queue.size();
    }

    public void Run(int count) {
        for(int i=0;i<count;i++) {
            //_______用于调试

            //System.out.println("队列"+index+" 包"+i);

            //-------------
            double tempWeight=-Math.log(Math.random())/Mu;
            pack temppack = new pack(i,tempWeight);				//生成新的包
            temppack.comeTime =  currentTime;			//设置包的到来时间为当前时间
            double comeInter = -Math.log(Math.random())/lamda;								//测试阶段设置整形
            currentTime += comeInter;						//生产者计时器应该

            queue.add(temppack); 							//向队列中添加

        }
    }

}