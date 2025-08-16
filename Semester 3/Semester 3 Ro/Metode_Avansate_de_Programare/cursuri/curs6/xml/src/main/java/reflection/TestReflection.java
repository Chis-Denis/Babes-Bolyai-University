package reflection;

import domain.Student;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class TestReflection {
    public static void main(String[] args) {
        ex1();
    }

    private static void ex1() {
        try {
            Class aClass = Class.forName("domain.Student");
            Constructor constructor = aClass.getDeclaredConstructor(String.class, int.class);
            Student student = (Student) constructor.newInstance("John", 223);
            Class superclass=aClass.getSuperclass();
            // Arrays.asList(superclass.getDeclaredMethods()).forEach(s-> System.out.println(s));
            Method method = superclass.getDeclaredMethod("setId", Object.class);
            method.invoke(student, 1L);
            System.out.println(student);
        } catch (ClassNotFoundException | NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
        //  1. Creati o noua instanta de domain.Person (folositi reflection)
        //        - initializato atributele private ale persoanei
        //        - afisati instanta de persoana

//        String classname="reflection.Person";
//        Class c= Student.class;
//
//        try {
//            Class aClass = Class.forName(classname);
//            Person person = (Person) aClass.newInstance();
//            Field name = aClass.getDeclaredField("name");
//
//            name.setAccessible(true);
//            name.set(person,"John");
//            System.out.println(person);
//        } catch (ClassNotFoundException e) {
//            e.printStackTrace();
//        } catch (IllegalAccessException e) {
//            e.printStackTrace();
//        } catch (InstantiationException e) {
//            e.printStackTrace();
//        } catch (NoSuchFieldException e) {
//            e.printStackTrace();
//        }
//
//        try {
//            Class aClass = Class.forName("reflection.Employee");
//            Employee person = (Employee) aClass.newInstance();
//            Field salary = aClass.getDeclaredField("salary");
//            Field name = aClass.getSuperclass().getDeclaredField("name");
//            name.setAccessible(true);
//            name.set(person, "John");
//            salary.setAccessible(true);
//            salary.set(person,12);
//            System.out.println(person);
//
//        } catch (ClassNotFoundException | IllegalAccessException e) {
//            e.printStackTrace();
//        } catch (InstantiationException e) {
//            e.printStackTrace();
//        } catch (NoSuchFieldException e) {
//            e.printStackTrace();
//        }



    }
}
