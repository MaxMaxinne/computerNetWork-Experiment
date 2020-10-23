public class pack {
    public int index;
    public double comeTime;
//    public double leafTime;
    public double servTime;
    public double waitTime;
    public double weight;
    public pack(int index,double weight) {
        this.index = index;
        this.servTime= weight;
        this.weight = weight;
    }
    public void CalLeaf(double leafTime) {
        waitTime = (leafTime - comeTime);			//暂定为这样把，实际应该传入
    }
}