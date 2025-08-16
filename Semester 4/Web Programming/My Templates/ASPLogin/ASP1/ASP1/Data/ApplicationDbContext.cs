using Microsoft.EntityFrameworkCore;
using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using ASP1.Models;

namespace ASP1.Data;

// ApplicationDbContext now inherits from IdentityDbContext to include Identity tables and functionality.
// It uses ApplicationUser as the custom user class.
public class ApplicationDbContext : IdentityDbContext<ApplicationUser>
{
    public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options) : base(options)
    {
    }

    public DbSet<Flight> Flights { get; set; }
    public DbSet<Hotel> Hotels { get; set; }
    public DbSet<Reservation> Reservations { get; set; }
} 