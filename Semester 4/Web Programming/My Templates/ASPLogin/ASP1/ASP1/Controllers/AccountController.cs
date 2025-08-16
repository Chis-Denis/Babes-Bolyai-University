using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using ASP1.Models;
using ASP1.ViewModels;

namespace ASP1.Controllers
{
    // AccountController handles user authentication, including registration and login.
    public class AccountController : Controller
    {
        private readonly UserManager<ApplicationUser> _userManager; // Manages user creation, passwords, etc.
        private readonly SignInManager<ApplicationUser> _signInManager; // Manages user login and logout.

        // Constructor injects UserManager and SignInManager for dependency injection.
        public AccountController(
            UserManager<ApplicationUser> userManager,
            SignInManager<ApplicationUser> signInManager)
        {
            _userManager = userManager;
            _signInManager = signInManager;
        }

        // GET: Displays the registration form.
        [HttpGet]
        public IActionResult Register()
        {
            return View();
        }

        // POST: Handles user registration form submission.
        [HttpPost]
        public async Task<IActionResult> Register(RegisterViewModel model)
        {
            // Check if the submitted model is valid based on data annotations.
            if (ModelState.IsValid)
            {
                // Create a new ApplicationUser object from the RegisterViewModel.
                var user = new ApplicationUser
                {
                    UserName = model.Email,
                    Email = model.Email,
                    FirstName = model.FirstName,
                    LastName = model.LastName
                };

                // Attempt to create the user with the provided password.
                var result = await _userManager.CreateAsync(user, model.Password);

                // If user creation is successful, sign in the user and redirect to home.
                if (result.Succeeded)
                {
                    await _signInManager.SignInAsync(user, isPersistent: false); // Sign in the user (not persistent, so cookie expires when browser closes).
                    return RedirectToAction("Index", "Home"); // Redirect to the Home page.
                }

                // If there are errors during user creation, add them to ModelState to display in the view.
                foreach (var error in result.Errors)
                {
                    ModelState.AddModelError(string.Empty, error.Description);
                }
            }
            // If model state is not valid, return the view with the current model to display validation errors.
            return View(model);
        }

        // GET: Displays the login form.
        [HttpGet]
        public IActionResult Login()
        {
            return View();
        }

        // POST: Handles user login form submission.
        [HttpPost]
        public async Task<IActionResult> Login(LoginViewModel model)
        {
            // Check if the submitted model is valid based on data annotations.
            if (ModelState.IsValid)
            {
                // Attempt to sign in the user with the provided email and password.
                var result = await _signInManager.PasswordSignInAsync(
                    model.Email,
                    model.Password,
                    model.RememberMe, // Persist the login across browser sessions if checked.
                    lockoutOnFailure: false); // Do not lock out account on failed login attempts for simplicity (can be configured).

                // If login is successful, redirect to the Home page.
                if (result.Succeeded)
                {
                    return RedirectToAction("Index", "Home");
                }

                // If login fails, add a generic error message to ModelState.
                ModelState.AddModelError(string.Empty, "Invalid login attempt.");
            }
            // If model state is not valid, return the view with the current model to display validation errors.
            return View(model);
        }

        // POST: Handles user logout.
        [HttpPost]
        public async Task<IActionResult> Logout()
        {
            await _signInManager.SignOutAsync(); // Sign out the current user.
            return RedirectToAction("Index", "Home"); // Redirect to the Home page (which will then redirect to Login).
        }
    }
} 