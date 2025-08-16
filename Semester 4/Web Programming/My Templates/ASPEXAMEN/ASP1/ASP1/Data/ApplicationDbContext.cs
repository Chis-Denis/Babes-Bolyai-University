using Microsoft.EntityFrameworkCore;
using ASP1.Models;

namespace ASP1.Data;

public class ApplicationDbContext : DbContext
{
    public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options) : base(options)
    {
    }

    public DbSet<Flight> Flights { get; set; }
    public DbSet<Hotel> Hotels { get; set; }
    public DbSet<Reservation> Reservations { get; set; }
} 