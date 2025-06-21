using Microsoft.EntityFrameworkCore;
using ASP1.Models;

namespace ASP1.Data;

public class ApplicationDbContext : DbContext
{
    public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options) : base(options)
    {
    }

    // public DbSet<Flight> Flights { get; set; }
    // public DbSet<Hotel> Hotels { get; set; }
    // public DbSet<Reservation> Reservations { get; set; }
    public DbSet<Party> Parties { get; set; }
    public DbSet<Candidate> Candidates { get; set; }

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        base.OnModelCreating(modelBuilder);
        // Start of change: Seed Romanian parties and candidates *@
        modelBuilder.Entity<Party>().HasData(
            new Party { PartyId = 1, Name = "PSD", Description = "Partidul Social Democrat", LogoUrl = "https://upload.wikimedia.org/wikipedia/commons/6/6e/Logo_PSD_2020.png" },
            new Party { PartyId = 2, Name = "PNL", Description = "Partidul Național Liberal", LogoUrl = "https://upload.wikimedia.org/wikipedia/commons/7/7e/Logo_PNL_2014.png" },
            new Party { PartyId = 3, Name = "USR", Description = "Uniunea Salvați România", LogoUrl = "https://upload.wikimedia.org/wikipedia/commons/2/2a/Logo_USR_2020.png" },
            new Party { PartyId = 4, Name = "AUR", Description = "Alianța pentru Unirea Românilor", LogoUrl = "https://upload.wikimedia.org/wikipedia/commons/2/2d/Logo_AUR_2020.png" }
        );
        modelBuilder.Entity<Candidate>().HasData(
            new Candidate { CandidateId = 1, Name = "Ion Popescu", Description = "Candidat PSD la Camera Deputaților.", ImageUrl = "https://randomuser.me/api/portraits/men/1.jpg", PartyId = 1, Age = 45, Position = "Deputat" },
            new Candidate { CandidateId = 2, Name = "Maria Ionescu", Description = "Candidat PNL la Senat.", ImageUrl = "https://randomuser.me/api/portraits/women/2.jpg", PartyId = 2, Age = 52, Position = "Senator" },
            new Candidate { CandidateId = 3, Name = "Andrei Vasilescu", Description = "Candidat USR la Camera Deputaților.", ImageUrl = "https://randomuser.me/api/portraits/men/3.jpg", PartyId = 3, Age = 38, Position = "Deputat" },
            new Candidate { CandidateId = 4, Name = "Elena Dumitru", Description = "Candidat AUR la Senat.", ImageUrl = "https://randomuser.me/api/portraits/women/4.jpg", PartyId = 4, Age = 41, Position = "Senator" }
        );
        // End of change: Seed Romanian parties and candidates *@
    }
} 