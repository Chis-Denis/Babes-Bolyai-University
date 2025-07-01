using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Authorization;
using Microsoft.EntityFrameworkCore;
using System.Security.Claims;

[Authorize]
public class UrlEntriesController : Controller
{
    private readonly ApplicationDbContext _context;
    // Constructor: injects the database context
    public UrlEntriesController(ApplicationDbContext context)
    {
        _context = context;
    }

    // List URLs for the current user, with paging and category filter
    public async Task<IActionResult> Index(string category, int page = 1)
    {
        int pageSize = 4;
        var userId = User.FindFirstValue(ClaimTypes.NameIdentifier);
        var query = _context.UrlEntries.Where(u => u.UserId == userId);
        if (!string.IsNullOrEmpty(category))
            query = query.Where(u => u.Category == category);
        var total = await query.CountAsync();
        var urls = await query.OrderBy(u => u.Id).Skip((page - 1) * pageSize).Take(pageSize).ToListAsync();
        ViewBag.TotalPages = (int)Math.Ceiling(total / (double)pageSize);
        ViewBag.CurrentPage = page;
        ViewBag.Category = category;
        ViewBag.Categories = await _context.UrlEntries.Where(u => u.UserId == userId).Select(u => u.Category).Distinct().ToListAsync();
        return View(urls);
    }

    // Show the create form
    public IActionResult Create()
    {
        return View();
    }

    // Handle form POST for creating a new URL
    [HttpPost]
    [ValidateAntiForgeryToken]
    public async Task<IActionResult> Create(UrlEntry entry)
    {
        // Debug info for troubleshooting
        ViewBag.Debug = "POST action hit at " + DateTime.Now;
        var userId = User.FindFirstValue(ClaimTypes.NameIdentifier);
        if (string.IsNullOrEmpty(userId))
        {
            ModelState.AddModelError(string.Empty, "You must be logged in to add a URL. Please log in and try again.");
            return View(entry);
        }
        if (!ModelState.IsValid)
        {
            ViewBag.ModelStateErrors = string.Join("; ", ModelState.Values.SelectMany(v => v.Errors).Select(e => e.ErrorMessage));
        }
        if (ModelState.IsValid)
        {
            entry.UserId = userId; // Set the user ID
            _context.Add(entry); // Add to database
            await _context.SaveChangesAsync(); // Save changes
            return RedirectToAction(nameof(Index));
        }
        return View(entry);
    }

    // Show the edit form for a URL
    public async Task<IActionResult> Edit(int? id)
    {
        if (id == null) return NotFound();
        var entry = await _context.UrlEntries.FindAsync(id);
        if (entry == null || entry.UserId != User.FindFirstValue(ClaimTypes.NameIdentifier)) return NotFound();
        return View(entry);
    }

    // Handle form POST for editing a URL
    [HttpPost]
    [ValidateAntiForgeryToken]
    public async Task<IActionResult> Edit(int id, UrlEntry entry)
    {
        if (id != entry.Id) return NotFound();
        if (ModelState.IsValid)
        {
            try
            {
                entry.UserId = User.FindFirstValue(ClaimTypes.NameIdentifier); // Set user ID
                _context.Update(entry); // Update in database
                await _context.SaveChangesAsync(); // Save changes
            }
            catch (DbUpdateConcurrencyException)
            {
                if (!_context.UrlEntries.Any(e => e.Id == id)) return NotFound();
                else throw;
            }
            return RedirectToAction(nameof(Index));
        }
        return View(entry);
    }

    // Show the delete confirmation page
    public async Task<IActionResult> Delete(int? id)
    {
        if (id == null) return NotFound();
        var entry = await _context.UrlEntries.FindAsync(id);
        if (entry == null || entry.UserId != User.FindFirstValue(ClaimTypes.NameIdentifier)) return NotFound();
        return View(entry);
    }

    // Handle POST for deleting a URL
    [HttpPost, ActionName("Delete")]
    [ValidateAntiForgeryToken]
    public async Task<IActionResult> DeleteConfirmed(int id)
    {
        var entry = await _context.UrlEntries.FindAsync(id);
        if (entry != null && entry.UserId == User.FindFirstValue(ClaimTypes.NameIdentifier))
        {
            _context.UrlEntries.Remove(entry); // Remove from database
            await _context.SaveChangesAsync(); // Save changes
        }
        return RedirectToAction(nameof(Index));
    }

    // AJAX: Return a partial view for paged/category-filtered URLs
    public async Task<IActionResult> List(string category, int page = 1)
    {
        int pageSize = 4;
        var userId = User.FindFirstValue(ClaimTypes.NameIdentifier);
        var query = _context.UrlEntries.Where(u => u.UserId == userId);
        if (!string.IsNullOrEmpty(category))
            query = query.Where(u => u.Category == category);
        var urls = await query.OrderBy(u => u.Id).Skip((page - 1) * pageSize).Take(pageSize).ToListAsync();
        return PartialView("_UrlListPartial", urls);
    }
} 