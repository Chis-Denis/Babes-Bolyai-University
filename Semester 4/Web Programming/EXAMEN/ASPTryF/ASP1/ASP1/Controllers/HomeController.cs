using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using ASP1.Models;
using ASP1.Data;
using Microsoft.EntityFrameworkCore;
using System.Linq;
using System.Collections.Generic;

namespace ASP1.Controllers;

public class HomeController : Controller
{
    // Logger for logging messages (e.g., debugging, errors)
    private readonly ILogger<HomeController> _logger;
    // Database context for interacting with the database
    private readonly ApplicationDbContext _context;

    // Constructor to inject the logger and database context
    public HomeController(ILogger<HomeController> logger, ApplicationDbContext context)
    {
        _logger = logger;
        _context = context;
    }

    // Default Index action - currently returns a basic view.
    public IActionResult Index()
    {
        return View();
    }

    // Privacy action - currently returns a basic view.
    public IActionResult Privacy()
    {
        return View();
    }

    // GET action for displaying the professor login form.
    public IActionResult ProfessorLogin()
    {
        return View();
    }

    // POST action for handling professor login form submission.
    [HttpPost]
    public IActionResult ProfessorLogin(string professorName)
    {
        // Check if the professor name is provided.
        if (string.IsNullOrWhiteSpace(professorName))
        {
            // Add a model error if the name is empty and return to the view.
            ModelState.AddModelError("professorName", "Professor name cannot be empty.");
            return View();
        }

        // Find the professor in the database by name and role.
        // .FirstOrDefault() retrieves the first element of a sequence, or a default value if no element is found.
        var professor = _context.People
            .FirstOrDefault(p => p.name == professorName && p.role == "Professor");

        // Check if a professor was found.
        if (professor == null)
        {
            // Add a model error if the professor is not found or not a Professor, and return to the view.
            ModelState.AddModelError("professorName", "Professor not found or not a Professor.");
            return View();
        }

        // Store professor's name and ID in session for subsequent requests.
        HttpContext.Session.SetString("ProfessorName", professorName);
        HttpContext.Session.SetInt32("ProfessorID", professor.id);

        // Redirect to the Professor Dashboard after successful login.
        return RedirectToAction("ProfessorDashboard");

    }

    // GET action for displaying the professor's dashboard with their courses.
    public IActionResult ProfessorDashboard()
    {
        // Retrieve professor's ID from session.
        var professorId = HttpContext.Session.GetInt32("ProfessorID");
        // If professor ID is not found in session, redirect to login page.
        if (!professorId.HasValue)
        {
            return RedirectToAction("ProfessorLogin");
        }

        // Retrieve courses taught by the logged-in professor.
        // .Where() filters a sequence of values based on a predicate.
        // .ToList() executes the query and returns a List.
        var courses = _context.Courses
            .Where(c => c.professorID == professorId.Value)
            .ToList();

        // Pass professor's name to the view using ViewBag.
        ViewBag.ProfessorName = HttpContext.Session.GetString("ProfessorName");
        // Return the view with the list of courses.
        return View(courses);
    }

    // GET action for displaying participants of a specific course.
    public IActionResult ViewCourseParticipants(int courseId)
    {
        // Retrieve professor's ID from session.
        var professorId = HttpContext.Session.GetInt32("ProfessorID");
        // If professor ID is not found in session, redirect to login page.
        if (!professorId.HasValue)
        {
            return RedirectToAction("ProfessorLogin");
        }

        // Find the specific course by its ID and professor ID.
        var course = _context.Courses
            .FirstOrDefault(c => c.id == courseId && c.professorID == professorId.Value);

        // If the course is not found, return a 404 Not Found response.
        if (course == null)
        {
            return NotFound();
        }

        // Parse the comma-separated participants string into a List<string>.
        // If the string is null or empty, initialize an empty list.
        var participants = string.IsNullOrEmpty(course.participants)
            ? new List<string>()
            : course.participants.Split(',').Select(p => p.Trim()).ToList();

        // Pass course name and ID to the view using ViewBag.
        ViewBag.CourseName = course.courseName;
        ViewBag.CourseId = course.id;
        // Return the view with the list of participants.
        return View(participants);
    }

    // GET action for displaying the grade assignment form.
    // It pre-fills the student name if coming from the ViewCourseParticipants page.
    public IActionResult AssignGrade(int courseId, string studentName)
    {
        // Retrieve professor's ID from session.
        var professorId = HttpContext.Session.GetInt32("ProfessorID");
        // If professor ID is not found in session, redirect to login page.
        if (!professorId.HasValue)
        {
            return RedirectToAction("ProfessorLogin");
        }

        // Find the specific course by its ID and professor ID.
        var course = _context.Courses
            .FirstOrDefault(c => c.id == courseId && c.professorID == professorId.Value);

        // If the course is not found, return a 404 Not Found response.
        if (course == null)
        {
            return NotFound();
        }

        // Initialize a new AssignGradeViewModel to pass data to the view.
        var model = new AssignGradeViewModel
        {
            CourseId = course.id,
            CourseName = course.courseName,
            StudentName = studentName
        };

        // Retrieve existing grades and participants to pre-fill the form if a grade already exists.
        var gradesList = string.IsNullOrEmpty(course.grades)
            ? new List<string>()
            : course.grades.Split(',').Select(g => g.Trim()).ToList();
        var participantsList = string.IsNullOrEmpty(course.participants)
            ? new List<string>()
            : course.participants.Split(',').Select(p => p.Trim()).ToList();

        // Find the student's index in the participants list.
        var studentIndex = participantsList.IndexOf(studentName);
        // If the student is found and has a corresponding grade, set the existing grade in the model.
        if (studentIndex != -1 && studentIndex < gradesList.Count)
        {
            model.ExistingGrade = gradesList[studentIndex];
            model.Grade = gradesList[studentIndex]; // Also set Grade property for the input field
        }
        else
        {
            // If student not found or no existing grade, set to empty.
            model.ExistingGrade = string.Empty;
            model.Grade = string.Empty;
        }

        // Return the view with the populated model.
        return View(model);
    }

    // POST action for handling the submission of the grade assignment form.
    [HttpPost]
    public IActionResult AssignGrade(AssignGradeViewModel model)
    {
        // Retrieve professor's ID from session.
        var professorId = HttpContext.Session.GetInt32("ProfessorID");
        // If professor ID is not found in session, redirect to login page.
        if (!professorId.HasValue)
        {
            return RedirectToAction("ProfessorLogin");
        }

        // Find the specific course by its ID and professor ID.
        var course = _context.Courses
            .FirstOrDefault(c => c.id == model.CourseId && c.professorID == professorId.Value);

        // If the course is not found, return a 404 Not Found response.
        if (course == null)
        {
            return NotFound();
        }

        // Validate if student name and grade are provided.
        if (string.IsNullOrWhiteSpace(model.StudentName) || string.IsNullOrWhiteSpace(model.Grade))
        {
            // Add a model error and repopulate the view model properties for the view.
            ModelState.AddModelError(string.Empty, "Student name and grade are required.");
            model.CourseName = course.courseName;
            model.ExistingGrade = model.Grade; // Keep the entered grade if there's an error
            return View(model);
        }

        // Parse current participants and grades strings into Lists.
        var participantsList = string.IsNullOrEmpty(course.participants)
            ? new List<string>()
            : course.participants.Split(',').Select(p => p.Trim()).ToList();

        var gradesList = string.IsNullOrEmpty(course.grades)
            ? new List<string>()
            : course.grades.Split(',').Select(g => g.Trim()).ToList();

        // Check if the student already exists in the participants list.
        var studentIndex = participantsList.IndexOf(model.StudentName);
        if (studentIndex == -1)
        {
            // If student not found, add them to both participants and grades lists.
            participantsList.Add(model.StudentName);
            gradesList.Add(model.Grade); // Add new grade for the new student
        }
        else
        {
            // If student exists, update their grade if a corresponding entry exists.
            if (studentIndex < gradesList.Count)
            {
                gradesList[studentIndex] = model.Grade; // Update existing grade
            }
            else
            {
                // This case handles if a student is in participants but has no corresponding grade yet.
                // Add a new grade to maintain parallel lists.
                gradesList.Add(model.Grade);
            }
        }

        // Update the course entity with the new (or updated) participants and grades.
        course.participants = string.Join(", ", participantsList);
        course.grades = string.Join(", ", gradesList);

        // Mark the course entity as modified and save changes to the database.
        _context.Courses.Update(course);
        _context.SaveChanges(); // Synchronous save

        // Set a success message to be displayed on the redirected page.
        TempData["SuccessMessage"] = $"Grade for {model.StudentName} in {course.courseName} has been assigned/updated.";
        // Redirect back to the ViewCourseParticipants page for the current course.
        return RedirectToAction("ViewCourseParticipants", new { courseId = course.id });
    }

    // GET action for displaying all courses a specific student participates in.
    public IActionResult StudentCourses(string studentName)
    {
        // Retrieve professor's ID from session. (This action is usually initiated by a professor).
        var professorId = HttpContext.Session.GetInt32("ProfessorID");
        // If professor ID is not found in session, redirect to login page.
        if (!professorId.HasValue)
        {
            return RedirectToAction("ProfessorLogin");
        }

        List<Course> studentCourses = new List<Course>();
        // Only proceed if a student name is provided.
        if (!string.IsNullOrWhiteSpace(studentName))
        {
            // Retrieve all courses from the database.
            var allCourses = _context.Courses.ToList();
            // Iterate through each course to check for student participation.
            foreach (var course in allCourses)
            {
                // Parse the comma-separated participants string into a List<string>.
                var participantsList = string.IsNullOrEmpty(course.participants)
                    ? new List<string>()
                    : course.participants.Split(',').Select(p => p.Trim()).ToList();

                // Check if the student's name exists in the participants list (case-insensitive).
                if (participantsList.Contains(studentName.Trim(), StringComparer.OrdinalIgnoreCase))
                {
                    studentCourses.Add(course); // Add the course if the student is a participant.
                }
            }
        }

        // Pass the searched student's name to the view for display (and potentially re-filling the search box).
        ViewBag.StudentNameSearch = studentName;
        // Return the view with the list of courses the student is in.
        return View(studentCourses);
    }

    // Error action for displaying error details.
    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
