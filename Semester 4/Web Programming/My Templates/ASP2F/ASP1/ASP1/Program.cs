using Microsoft.EntityFrameworkCore;
using Microsoft.AspNetCore.Identity;
using ASP1.Data;
using ASP1.Models;

namespace ASP1
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            // Add services to the container.
            builder.Services.AddControllersWithViews(); // Adds MVC controllers and views to the service collection.

            // Configure the ApplicationDbContext to use SQL Server with the connection string from appsettings.json.
            builder.Services.AddDbContext<ApplicationDbContext>(options =>
                options.UseSqlServer(builder.Configuration.GetConnectionString("DefaultConnection")));

            // Add ASP.NET Core Identity services to the service collection.
            // Configures Identity to use ApplicationUser for user management and IdentityRole for role management.
            builder.Services.AddIdentity<ApplicationUser, IdentityRole>(options =>
            {
                // Password requirements configuration.
                options.Password.RequireDigit = true;
                options.Password.RequireLowercase = true;
                options.Password.RequireUppercase = true;
                options.Password.RequireNonAlphanumeric = true;
                options.Password.RequiredLength = 6;
            })
            .AddEntityFrameworkStores<ApplicationDbContext>() // Configures Identity to use Entity Framework Core for storing identity data.
            .AddDefaultTokenProviders(); // Adds default token providers for things like password reset tokens.

            // Configure application cookie settings for Identity.
            builder.Services.ConfigureApplicationCookie(options =>
            {
                options.LoginPath = "/Account/Login"; // Sets the login page path.
                options.LogoutPath = "/Account/Logout"; // Sets the logout page path.
                options.AccessDeniedPath = "/Account/AccessDenied"; // Sets the access denied page path.
            });

            var app = builder.Build();

            // Configure the HTTP request pipeline.
            if (!app.Environment.IsDevelopment())
            {
                app.UseExceptionHandler("/Home/Error"); // Configures error handling for non-development environments.
                // The default HSTS value is 30 days. You may want to change this for production scenarios
                app.UseHsts(); // Enforces HTTP Strict Transport Security.
            }

            app.UseHttpsRedirection(); // Redirects HTTP requests to HTTPS.
            app.UseStaticFiles(); // Enables serving static files (e.g., CSS, JavaScript, images).

            app.UseRouting(); // Enables routing middleware.

            app.UseAuthentication(); // Enables authentication middleware. This must be before UseAuthorization.
            app.UseAuthorization(); // Enables authorization middleware.

            // Configures the default route for MVC controllers.
            // The first page when running the project will be the Login page of the AccountController.
            app.MapControllerRoute(
                name: "default",
                pattern: "{controller=Account}/{action=Login}/{id?}");

            app.Run(); // Runs the application.
        }
    }
}
