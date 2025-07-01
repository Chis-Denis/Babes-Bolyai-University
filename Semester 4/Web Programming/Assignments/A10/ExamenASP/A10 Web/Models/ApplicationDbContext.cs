using Microsoft.EntityFrameworkCore;
using Microsoft.AspNetCore.Identity.EntityFrameworkCore;

// The application's database context, includes Identity and URL entries
public class ApplicationDbContext : IdentityDbContext
{
    // Constructor for dependency injection
    public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options) : base(options) { }

    public DbSet<UrlEntry> UrlEntries { get; set; } // Table for URL entries
} 