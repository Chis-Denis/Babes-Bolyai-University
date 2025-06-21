using Microsoft.AspNetCore.Mvc;

namespace ASP1.Controllers;

// Start of change: Minimal HomeController for default route *@
public class HomeController : Controller
{
    public IActionResult Index()
    {
        return View();
    }
    public IActionResult Privacy()
    {
        return View();
    }
}
// End of change: Minimal HomeController for default route *@ 