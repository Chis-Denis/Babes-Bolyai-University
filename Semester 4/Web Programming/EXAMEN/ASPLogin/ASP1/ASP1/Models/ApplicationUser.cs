using Microsoft.AspNetCore.Identity;

namespace ASP1.Models
{
    // ApplicationUser extends the default IdentityUser to add custom profile properties
    public class ApplicationUser : IdentityUser
    {
        // Custom property for the user's first name
        public string? FirstName { get; set; }
        // Custom property for the user's last name
        public string? LastName { get; set; }
    }
} 