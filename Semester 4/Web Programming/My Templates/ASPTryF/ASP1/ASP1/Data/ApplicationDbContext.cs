using Microsoft.EntityFrameworkCore;
using ASP1.Models;

namespace ASP1.Data;

public class ApplicationDbContext : DbContext
{
    public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options) : base(options)
    {
    }

    public DbSet<Person> People { get; set; }
    public DbSet<Course> Courses { get; set; }
} 