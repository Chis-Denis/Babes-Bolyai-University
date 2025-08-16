using System.ComponentModel.DataAnnotations;

namespace ASP1.ViewModels
{
    // ViewModel for handling new user registration data.
    public class RegisterViewModel
    {
        // First name of the user. Required for registration.
        [Required]
        [Display(Name = "First Name")]
        public string FirstName { get; set; }

        // Last name of the user. Required for registration.
        [Required]
        [Display(Name = "Last Name")]
        public string LastName { get; set; }

        // Email address of the user. Required for registration and must be a valid email format.
        [Required]
        [EmailAddress]
        public string Email { get; set; }

        // Password for the new user. Required, with minimum length and data type for security.
        [Required]
        [StringLength(100, ErrorMessage = "The {0} must be at least {2} characters long.", MinimumLength = 6)]
        [DataType(DataType.Password)]
        public string Password { get; set; }

        // Confirmation of the password. Must match the Password field.
        [DataType(DataType.Password)]
        [Display(Name = "Confirm password")]
        [Compare("Password", ErrorMessage = "The password and confirmation password do not match.")]
        public string ConfirmPassword { get; set; }
    }
} 