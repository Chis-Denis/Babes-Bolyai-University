import domain.Student;
import domain.validators.StudentValidator;
import repository.StudentXMLFileRepository;
import services.config.ApplicationContext;

public class XMLTest {
    public static void main(String[] args) {

//        List<String> list = Arrays.asList("ab", "cd", "as", "1eg");
//        List<String> result = list.stream()
//                .filter(x->{
//                    System.out.println("Filter " + x);
//                    return x.startsWith("a");})
//                .sorted((x,y)->{
//                    System.out.println("Sorted " + x + " " + y);
//                    return x.compareTo(y);})
//                .map(x->{
//                    System.out.println("Map " + x);
//                    return x.toUpperCase();})
//                //.forEach(System.out::println);
//                .collect(Collectors.toList());
//        System.out.println(result);

        StudentValidator validator = new StudentValidator();
        String filename = ApplicationContext
                .getPROPERTIES()
                .getProperty("data.catalog.studentsXML");
        StudentXMLFileRepository repo = new StudentXMLFileRepository(validator, filename);

        Student s = new Student("Ana",223);
        s.setId(34L);
        repo.save(s);
        repo.findAll().forEach(student -> System.out.println(student));


    }
}
