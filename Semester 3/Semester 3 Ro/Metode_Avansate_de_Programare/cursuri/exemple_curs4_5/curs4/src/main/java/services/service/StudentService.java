package services.service;


import domain.Student;
import repository.CrudRepository;

public class StudentService {
    private CrudRepository<Long, Student> repository;

    public StudentService(CrudRepository<Long, Student> repository) {
        this.repository = repository;
    }
}
