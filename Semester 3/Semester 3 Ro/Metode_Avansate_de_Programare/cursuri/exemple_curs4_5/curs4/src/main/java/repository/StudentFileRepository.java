package repository;


import domain.Student;
import domain.validators.Validator;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.List;
import java.util.stream.Stream;

public class StudentFileRepository extends InMemoryRepository<Long, Student> {
    String fileName;

    private void loadData(){
        try {
            BufferedReader br = new BufferedReader(new FileReader(fileName));
            String line=null;
            while ((line=br.readLine())!=null){
                String[] args=  line.split(";");
                Student student= new Student(args[1],Integer.parseInt(args[2]));
                student.setId(Long.parseLong(args[0]));
                super.save(student);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void loadData2(){
        Path path=Paths.get(fileName);
        try {
            List<String> lines=Files.readAllLines(path);
            lines.forEach(linie -> {
                String[] args = linie.split(";");
                Student student= new Student(args[1],Integer.parseInt(args[2]));
                student.setId(Long.parseLong(args[0]));
             //   Entity e=createEntity(linie);
                super.save(student);
            });
        } catch (IOException e) {
            e.printStackTrace();
        }


    }

    private void writeToFile(Student student) {
        Path path = Paths.get(fileName);
        try ( BufferedWriter bufferedWriter = Files.newBufferedWriter(path, StandardOpenOption.APPEND);
        ) {
            bufferedWriter.write(student.getId() + ";" + student.getName() + ";" + student.getGroup());
            bufferedWriter.newLine();

        } catch (IOException e) {
            e.printStackTrace();
        }
//        finally {
//
//        }


    }

    public StudentFileRepository(Validator<Student> validator, String fileN) {
        super(validator);
        fileName=fileN;
        loadData();
    }


}
