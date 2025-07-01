using Microsoft.EntityFrameworkCore;
using ASP1.Data;

namespace ASP1
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            // Add services to the container.
            builder.Services.AddControllersWithViews();

            // This is used to add the DbContext to the container (explain how it works)
            builder.Services.AddDbContext<ApplicationDbContext>(options =>
            options.UseSqlServer(builder.Configuration.GetConnectionString("DefaultConnection")));

            builder.Services.AddSession(options =>
            {
                options.IdleTimeout = TimeSpan.FromMinutes(30);
                options.Cookie.HttpOnly = true;
                options.Cookie.IsEssential = true;
            });

            var app = builder.Build();

            // Configure the HTTP request pipeline.
            if (!app.Environment.IsDevelopment())
            {
                app.UseExceptionHandler("/Home/Error");
                // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
                app.UseHsts();
            }

            // This is used to redirect HTTP to HTTPS
            app.UseHttpsRedirection();

            // This is used to serve static files like CSS, JavaScript, and images
            app.UseStaticFiles();

            // This is used to route the request to the appropriate controller and action
            app.UseRouting();

            // This is used to use session
            app.UseSession();

            // This is used to authorize the request
            app.UseAuthorization();

            // This is used to map the request to the appropriate controller and action
            app.MapControllerRoute(
                name: "default",
                pattern: "{controller=Home}/{action=ProfessorLogin}/{id?}");

            app.Run();
        }
    }
}
