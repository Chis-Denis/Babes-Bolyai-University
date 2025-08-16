import domain.validators.StudentValidator;
import repository.StudentFileRepository;
import services.config.ApplicationContext;
import services.config.Config;

import java.util.*;
import java.util.function.BiFunction;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {
    public static void main(String[] args) {
        Map<Integer, String> map = new HashMap<>();
        map.put(1, "Ana");
        map.put(2, "Dan");
        map.put(3, "Lila");


        List<String> list = Arrays.asList("ab", "cd", "as", "1eg");
        List<String> result = list.stream()
                .filter(x->{
                    System.out.println("Filter " + x);
                    return x.startsWith("a");})
                .sorted((x,y)->{
                    System.out.println("Sorted " + x + " " + y);
                    return x.compareTo(y);})
                .map(x->{
                    System.out.println("Map " + x);
                    return x.toUpperCase();})
                //.forEach(System.out::println);
                .collect(Collectors.toList());
        System.out.println(result);

//      String  s=map.putIfAbsent(4, "Dan");
//        System.out.println(s);
//        System.out.println(map);
//        String  s=map.computeIfAbsent(2, (x)->{return "Ion";});
//        System.out.println(s);
//        System.out.println(map);

//        StudentFileRepository studentFileRepository = new StudentFileRepository(new StudentValidator(),
//                ApplicationContext.getPROPERTIES().getProperty("database.catalog.students"));
//
//        studentFileRepository.findAll().forEach(System.out::println);
//

    }
}
