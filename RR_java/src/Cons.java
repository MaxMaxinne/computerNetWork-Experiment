import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.util.Collections;
import java.io.IOException;
import java.text.DecimalFormat;
import java.util.ArrayList;

public class Cons {
    public double currentTime=0;
    public double timePiece;
    public ArrayList<Double> result=new ArrayList<>();
//    public ArrayList<Double> display=new ArrayList<>();
    DecimalFormat df = new DecimalFormat("0.0000");


    public String str = "E://output3.txt";
    public String str_result="E://result1.txt";

    public static void write(String fileName, String content) {
        FileWriter writer = null;
        try {
            // 打开一个写文件器，构造函数中的第二个参数true表示以追加形式写文件
            writer = new FileWriter(fileName, true);
            writer.write(content);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if(writer != null){
                    writer.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }


    public Cons(double timePiece) {
        this.timePiece = timePiece;
    }

    public static void main(String args[]) {
        Cons cons = new Cons(10);
    }

    public int  CalMeanQueueLength(Prod producer) {
        //计算平均队列长度
        int index = 0;
        while(!producer.queue.isEmpty()&&index<producer.queue.size()&&producer.queue.get(index).comeTime<=currentTime) {
            index++;
        }
        producer.lengthDis.add(index+1);
        return index+1;
    }



    public void ConsTimePiece(Prod producer) throws IOException {
        //服务一个时间片的方法
        int QueueLength = 0;
        double time=timePiece;
        while((!producer.queue.isEmpty())&&0<time) {
            if(producer.queue.get(0).comeTime<=currentTime) {
//				hahaha = 0;
                pack temp = producer.queue.get(0);
                //每次给一个队列充足的一个时间片时间，当最后一个商品大于剩余时间时，阻塞该商品，
                //同时将剩余时间加入到下一个轮转的时间片中。
                if (temp.servTime>time){
                    temp.servTime-=time;
                    //System.out.println("队列"+producer.index+"   is too large,wait a timepiece");
                    return;
                }
                temp.CalLeaf(currentTime);
                currentTime += temp.weight;
                time-=temp.servTime;
                //输出包的信息
//				 HSSFRow rows = sheet.createRow(row);
//				 row ++;
//				 System.out.println("line:"+ producer.index +"pack:"+temp.index);
//				 rows.createCell((short) 0).setCellValue(producer.index);
//				 rows.createCell((short) 1).setCellValue(temp.index);
//				 
//				 rows.createCell((short) 3).setCellValue(temp.waitTime);
//				
                //QueueLength = CalMeanQueueLength(producer);
//                write(str,producer.index+"  "+temp.index+"  "+df.format(temp.waitTime)+"  "+QueueLength +"\r\n");
                //write(str,producer.index+"  "+"  "+df.format(temp.waitTime)+"\r\n");
                if(temp.index%10000==0)
                System.out.println("队列"+producer.index+" 包: "+temp.index+"   " + "waittime: "+df.format(temp.waitTime)+"   serveTime: "+df.format(temp.weight)+"  "+QueueLength);
                if (producer.index==1){
                    result.add(temp.waitTime);
                }
                producer.queue.remove(0);
                //System.out.println();
            }else {
                currentTime += 0.01;
//				hahaha++;
//				System.out.println("hahaha");
                break;
            }
        }
    }
    public int hahaha;
    public void Consume3(Prod producer1,Prod producer2, Prod producer3) throws IOException{
        //三队列的服务方法
        //此方法为带赤字的时间片调度算法,对三个生产者产生的包文件进行调度
        //	调度算法为：对三个生产者队列进行轮询，当有符合要求的文件时(Prod.time.get(0)<=currentTime)
        //	在不超过时间片长度的情况下可以一直对该生产者队列服务，但是一旦超过了时间片长度则轮询下个队列
        //	并且允许且只允许一个包的服务时间超过时间片
        //----------------------------------------------------------------
        int n = 1;
        while(!producer1.queue.isEmpty()||!producer2.queue.isEmpty()||!producer3.queue.isEmpty()) {
            if(n==1)      ConsTimePiece(producer1);
            else if(n==2) ConsTimePiece(producer2);
            else		  ConsTimePiece(producer3);

//			if(hahaha>=3) {
//				if(!producer1.queue.isEmpty()&&!producer2.queue.isEmpty()&&!producer3.queue.isEmpty())
//					currentTime = min3(producer1.queue.get(0).comeTime, producer2.queue.get(0).comeTime, producer3.queue.get(0).comeTime);
//				if(!producer1.queue.isEmpty()&&!producer2.queue.isEmpty()&&producer3.queue.isEmpty())
//					currentTime = min(producer1.queue.get(0).comeTime, producer2.queue.get(0).comeTime);
//				if(!producer1.queue.isEmpty()&&producer2.queue.isEmpty()&&!producer3.queue.isEmpty())
//					currentTime = min(producer1.queue.get(0).comeTime, producer3.queue.get(0).comeTime);
//				if(producer1.queue.isEmpty()&&!producer2.queue.isEmpty()&&!producer3.queue.isEmpty())
//					currentTime = min(producer2.queue.get(0).comeTime, producer3.queue.get(0).comeTime);
//				if(!producer1.queue.isEmpty()&&producer2.queue.isEmpty()&&producer3.queue.isEmpty())
//					currentTime = producer1.queue.get(0).comeTime;
//				if(!producer2.queue.isEmpty()&&producer1.queue.isEmpty()&&producer3.queue.isEmpty())
//					currentTime = producer2.queue.get(0).comeTime;
//				if(!producer3.queue.isEmpty()&&producer1.queue.isEmpty()&&producer2.queue.isEmpty())
//					currentTime = producer3.queue.get(0).comeTime;
//			}

//			else {
//				改变n值
            if(n==1) n=2;
            else if(n==2) n=3;
            else n=1;
//			}
        }

    }
    public double min(double a,double b) {
        return (a<b)?a:b;
    }
    public double min3(double a,double b,double c) {
        return (min(a,b)>c)?(min(a,b)):c;
    }
    public void deal_result(){
        Collections.sort(result);
        int len=result.size();
        int count=0;
        double d=result.get(len-1)/100;
        double min=-0.0000,max=d;
        double x=d/2;
        for (int i=0;i<len;i++){
            if (result.get(i)>=min&&result.get(i)<=max){
                count++;
            }
            else {
                write(str_result,x+"    "+((double) count)/len+'\n');
                x+=d;
                min+=d;
                max+=d;
                count=0;
            }
        }
//        for (int i =0;i<result.size();i++){
//            write(str_result,df.format(result.get(i))+'\n');
//        }
    }

}