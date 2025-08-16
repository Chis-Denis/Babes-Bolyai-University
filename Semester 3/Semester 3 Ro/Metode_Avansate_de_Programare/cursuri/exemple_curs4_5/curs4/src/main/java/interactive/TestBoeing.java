package interactive;

@FunctionalInterface
interface Flyable<T> {
    int canFly(T t); // the hight reached by T
}

class Boeing implements Comparable<Boeing>{
    int height;

    public Boeing(int height) {
        this.height = height;
    }

    public int getHeight() {
        return height;
    }

    @Override
    public int compareTo(Boeing o) {
        return this.height-o.height;
    }
}
public class TestBoeing {
    public static void main(String[] args) {

        Flyable<Boeing> f=Boeing::getHeight;
        int n=f.canFly(new Boeing(23));



    }
}
