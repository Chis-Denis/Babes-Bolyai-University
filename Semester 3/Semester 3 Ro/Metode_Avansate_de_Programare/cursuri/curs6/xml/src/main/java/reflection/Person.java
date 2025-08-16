package reflection;

public class Person {

    private String name;
//
//    public Person(String name) {
//        this.name = name;
//    }

    public Person(){
        this.name="";
    }

    @Override
    public String toString() {
        return "Person{" +
                "name='" + name + '\'' +
                '}';
    }
}
