namespace ASP1.Models;

public class Course
{
    public int id { get; set; }
    public int professorID { get; set; }
    public string courseName { get; set; }
    public string participants { get; set; }
    public string grades { get; set; }
}