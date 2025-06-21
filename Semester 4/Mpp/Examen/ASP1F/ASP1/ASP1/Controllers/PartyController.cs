using Microsoft.AspNetCore.Mvc;
using ASP1.Data;
using ASP1.Models;
using System.Linq;

namespace ASP1.Controllers;

public class PartyController : Controller
{
    private readonly ApplicationDbContext _context;
    public PartyController(ApplicationDbContext context)
    {
        _context = context;
    }

    // Start of change: List all parties *@
    public IActionResult Index()
    {
        var parties = _context.Parties.ToList();
        return View(parties);
    }
    // End of change: List all parties *@

    // Start of change: Show party details and its candidates *@
    public IActionResult Details(int id)
    {
        var party = _context.Parties
            .Where(p => p.PartyId == id)
            .Select(p => new Party {
                PartyId = p.PartyId,
                Name = p.Name,
                Description = p.Description,
                LogoUrl = p.LogoUrl,
                Candidates = _context.Candidates.Where(c => c.PartyId == p.PartyId).ToList()
            })
            .FirstOrDefault();
        if (party == null) return NotFound();
        return View(party);
    }
    // End of change: Show party details and its candidates *@

    // Start of change: Add Create actions for Party *@
    [HttpGet]
    public IActionResult Create()
    {
        return View();
    }
    [HttpPost]
    [ValidateAntiForgeryToken]
    public IActionResult Create(Party party)
    {
        if (ModelState.IsValid)
        {
            _context.Parties.Add(party);
            _context.SaveChanges();
            return RedirectToAction("Index");
        }
        return View(party);
    }
    // End of change: Add Create actions for Party *@

    // Start of change: Add Edit actions for Party *@
    [HttpGet]
    public IActionResult Edit(int id)
    {
        var party = _context.Parties.Find(id);
        if (party == null) return NotFound();
        return View(party);
    }
    [HttpPost]
    [ValidateAntiForgeryToken]
    public IActionResult Edit(Party party)
    {
        if (ModelState.IsValid)
        {
            _context.Parties.Update(party);
            _context.SaveChanges();
            return RedirectToAction("Index");
        }
        return View(party);
    }
    // End of change: Add Edit actions for Party *@

    // Start of change: Add Delete actions for Party *@
    [HttpGet]
    public IActionResult Delete(int id)
    {
        var party = _context.Parties.Find(id);
        if (party == null) return NotFound();
        return View(party);
    }
    [HttpPost, ActionName("Delete")]
    [ValidateAntiForgeryToken]
    public IActionResult DeleteConfirmed(int id)
    {
        var party = _context.Parties.Find(id);
        if (party != null)
        {
            _context.Parties.Remove(party);
            _context.SaveChanges();
        }
        return RedirectToAction("Index");
    }
    // End of change: Add Delete actions for Party *@
} 