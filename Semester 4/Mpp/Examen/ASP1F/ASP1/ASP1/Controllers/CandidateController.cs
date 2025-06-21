using Microsoft.AspNetCore.Mvc;
using ASP1.Data;
using ASP1.Models;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.Extensions.DependencyInjection;

namespace ASP1.Controllers;

public class CandidateController : Controller
{
    private readonly ApplicationDbContext _context;
    private static CancellationTokenSource _cts;
    private static Task _generatorTask;
    private readonly IServiceProvider _serviceProvider;
    public CandidateController(ApplicationDbContext context, IServiceProvider serviceProvider)
    {
        _context = context;
        _serviceProvider = serviceProvider;
    }

    // Start of change: List all candidates *@
    public IActionResult Index()
    {
        var candidates = _context.Candidates.ToList();
        return View(candidates);
    }
    // End of change: List all candidates *@

    // Start of change: Show candidate details (name, description, image, party name, id, etc.) *@
    public IActionResult Details(int id)
    {
        var candidate = _context.Candidates
            .Where(c => c.CandidateId == id)
            .Select(c => new {
                c.CandidateId,
                c.Name,
                c.Description,
                c.ImageUrl,
                c.Age,
                c.Position,
                PartyName = c.Party.Name,
                c.PartyId
            })
            .FirstOrDefault();
        if (candidate == null) return NotFound();
        ViewBag.PartyName = candidate.PartyName;
        return View(candidate);
    }
    // End of change: Show candidate details *@

    // Start of change: Add Create actions for Candidate *@
    [HttpGet]
    public IActionResult Create()
    {
        return View();
    }

    [HttpPost]
    [ValidateAntiForgeryToken]
    public IActionResult Create(CandidateViewModel model)
    {
        Party party = null;
        if (!string.IsNullOrWhiteSpace(model.PartyName))
        {
            party = _context.Parties.FirstOrDefault(p => p.Name == model.PartyName);
            if (party == null)
            {
                party = new Party { Name = model.PartyName, Description = "Added via candidate form", LogoUrl = "" };
                _context.Parties.Add(party);
                _context.SaveChanges();
            }
        }

        var candidate = new Candidate
        {
            Name = model.Name,
            Description = model.Description,
            ImageUrl = model.ImageUrl,
            Age = model.Age,
            Position = model.Position,
            PartyId = party?.PartyId
        };

        if (ModelState.IsValid)
        {
            _context.Candidates.Add(candidate);
            _context.SaveChanges();
            return RedirectToAction("Index");
        }
        return View(model);
    }
    // End of change: Add Create actions for Candidate *@

    // Start of change: Add Edit actions for Candidate *@
    [HttpGet]
    public IActionResult Edit(int id)
    {
        var candidate = _context.Candidates.Find(id);
        if (candidate == null) return NotFound();
        var partyName = candidate.PartyId.HasValue ? _context.Parties.FirstOrDefault(p => p.PartyId == candidate.PartyId)?.Name : null;
        var model = new CandidateViewModel
        {
            Name = candidate.Name,
            Description = candidate.Description,
            ImageUrl = candidate.ImageUrl,
            Age = candidate.Age,
            Position = candidate.Position,
            PartyName = partyName
        };
        ViewData["CandidateId"] = candidate.CandidateId;
        return View(model);
    }

    [HttpPost]
    [ValidateAntiForgeryToken]
    public IActionResult Edit(int id, CandidateViewModel model)
    {
        var candidate = _context.Candidates.Find(id);
        if (candidate == null) return NotFound();
        Party party = null;
        if (!string.IsNullOrWhiteSpace(model.PartyName))
        {
            party = _context.Parties.FirstOrDefault(p => p.Name == model.PartyName);
            if (party == null)
            {
                party = new Party { Name = model.PartyName, Description = "Added via candidate edit", LogoUrl = "" };
                _context.Parties.Add(party);
                _context.SaveChanges();
            }
        }
        candidate.Name = model.Name;
        candidate.Description = model.Description;
        candidate.ImageUrl = model.ImageUrl;
        candidate.Age = model.Age;
        candidate.Position = model.Position;
        candidate.PartyId = party?.PartyId;
        if (ModelState.IsValid)
        {
            _context.Candidates.Update(candidate);
            _context.SaveChanges();
            return RedirectToAction("Index");
        }
        return View(model);
    }
    // End of change: Add Edit actions for Candidate *@

    // Start of change: Add Delete actions for Candidate *@
    [HttpGet]
    public IActionResult Delete(int id)
    {
        var candidate = _context.Candidates.Find(id);
        if (candidate == null) return NotFound();
        return View(candidate);
    }
    [HttpPost, ActionName("Delete")]
    [ValidateAntiForgeryToken]
    public IActionResult DeleteConfirmed(int id)
    {
        var candidate = _context.Candidates.Find(id);
        if (candidate != null)
        {
            _context.Candidates.Remove(candidate);
            _context.SaveChanges();
        }
        return RedirectToAction("Index");
    }
    // End of change: Add Delete actions for Candidate *@

    [HttpPost]
    [ValidateAntiForgeryToken]
    public IActionResult StartGenerating()
    {
        if (_generatorTask == null || _generatorTask.IsCompleted)
        {
            _cts = new CancellationTokenSource();
            _generatorTask = Task.Run(async () =>
            {
                using var scope = _serviceProvider.CreateScope();
                var db = scope.ServiceProvider.GetRequiredService<ApplicationDbContext>();
                var rnd = new Random();
                string[] names = { "Alex", "Maria", "Ion", "Elena", "Vlad", "Ana", "George", "Diana" };
                string[] positions = { "Deputat", "Senator", "Primar", "Consilier" };
                string[] parties = { "PSD", "PNL", "USR", "AUR", "PMP" };
                while (!_cts.IsCancellationRequested)
                {
                    var name = names[rnd.Next(names.Length)] + " " + names[rnd.Next(names.Length)];
                    var desc = "Generated candidate";
                    var img = "https://randomuser.me/api/portraits/lego/" + rnd.Next(1, 10) + ".jpg";
                    var age = rnd.Next(18, 80);
                    var pos = positions[rnd.Next(positions.Length)];
                    var partyName = parties[rnd.Next(parties.Length)];
                    var party = db.Parties.FirstOrDefault(p => p.Name == partyName);
                    if (party == null)
                    {
                        party = new Models.Party { Name = partyName, Description = "Generated party", LogoUrl = "" };
                        db.Parties.Add(party);
                        db.SaveChanges();
                    }
                    var candidate = new Models.Candidate
                    {
                        Name = name,
                        Description = desc,
                        ImageUrl = img,
                        Age = age,
                        Position = pos,
                        PartyId = party.PartyId
                    };
                    db.Candidates.Add(candidate);
                    db.SaveChanges();
                    await Task.Delay(1000, _cts.Token);
                }
            }, _cts.Token);
        }
        return RedirectToAction("Index");
    }

    [HttpPost]
    [ValidateAntiForgeryToken]
    public IActionResult StopGenerating()
    {
        _cts?.Cancel();
        return RedirectToAction("Index");
    }
} 