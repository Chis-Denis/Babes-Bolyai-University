package interactive;


import domain.Student;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

class Expression{
    private double a;
    private double b;

    public Expression(double a, double b) { this.a = a;this.b = b; }

    public static double patratBinom(double x, double y){
        return Math.pow(x+y,2);
    }

    public double distanta(double x, double y){
        return Expression.sqrt(Math.pow(x-a,2) +Math.pow(y-b,2));
    }

    public static double sqrt(double a) {
        return Math.sqrt(a);
    }
}

@FunctionalInterface
interface Formula{
    double  PI=3.14;
    double  calculate(double a, double b);

    default double sqrt(double a) {
        return Math.sqrt(a);
    }

    default double power(double a, double  b) {
        return Math.pow(a, b);
    }

    default double numarLaPatrat(double nr)
    {
        return power(nr,2);
    }

    default double numarLaCub(double nr)
    {
        return power(nr,3);
    }

    default double patratBinom(double x, double y){ return Math.pow(x+y,2); }

    static double cubBinom(double x, double y){ return Math.pow(x+y,3); }

    static double suma(double x, double y) {return x+y;}

}

public class TestC4 {
    public static void main(String[] args) {
        Formula f = Formula::suma;
        Formula f2 = Expression::patratBinom;
//        System.out.println(f.calculate(2.0, 3.0));
//        System.out.println(f2.calculate(2.0, 3.0));
//        f = Formula::cubBinom;
//        f = (double x, double y) -> {
//            double dif = x - y;
//            return dif;
//        };
//        f = (double x, double y) -> x - y;
//        f = (x, y) -> x - y;
//        System.out.println(f.calculate(3.0, 2.0));
//
//        Expression expression = new Expression(0.0, 0.0);
//        f = expression::distanta;
//        System.out.println(f.calculate(2, 2));
//
//        Student s=new Student("pop",2);
//        s.setId((long)2);
//        Student s1=new Student("pop",2);
//        s1.setId((long)3);
//        List<Student> list=new ArrayList<>(Arrays.asList(s1,s));
//        list.forEach(System.out::println);
//        list.forEach(student-> System.out.println(student));

        List<String> list1 = new ArrayList<>();
        list1.add("ab");
        list1.add("ab");
        list1.add("abce");
        list1.add("edabe");

        //list1.removeIf(x -> x.startsWith("a"));
        //list1.removeIf("");
        String cuv = "abcde";
        list1.removeIf(x -> cuv.startsWith(x));

        list1.forEach(System.out::println);

//        list1.removeIf(cuv::startsWith);
//        list1.removeIf(String::isEmpty);

    }
}
