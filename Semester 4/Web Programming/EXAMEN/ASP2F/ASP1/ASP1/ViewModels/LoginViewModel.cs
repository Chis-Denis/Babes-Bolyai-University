using System.ComponentModel.DataAnnotations;

namespace ASP1.ViewModels
{
    // ViewModel for handling user login data.
    public class LoginViewModel
    {
        // Email address of the user. Required for login and must be a valid email format.
        [Required]
        [EmailAddress]
        public string Email { get; set; }

        // Password for the user. Required for login and hidden for security.
        [Required]
        [DataType(DataType.Password)]
        public string Password { get; set; }

        // Option for the user to stay logged in across browser sessions.
        [Display(Name = "Remember me?")]
        public bool RememberMe { get; set; }
    }
} 