using Microsoft.AspNetCore.Mvc;
using ASP1.Data;
using System.Linq;

namespace ASP1.Controllers
{
    public class StatisticsController : Controller
    {
        private readonly ApplicationDbContext _context;
        public StatisticsController(ApplicationDbContext context)
        {
            _context = context;
        }

        public IActionResult Index()
        {
            var totalCandidates = _context.Candidates.Count();
            var totalParties = _context.Parties.Count();
            var candidatesPerParty = _context.Parties
                .Select(p => new PartyStatsViewModel
                {
                    PartyName = p.Name,
                    CandidateCount = p.Candidates.Count
                }).ToList();
            var stats = new StatisticsViewModel
            {
                TotalCandidates = totalCandidates,
                TotalParties = totalParties,
                CandidatesPerParty = candidatesPerParty
            };
            return View(stats);
        }
    }

    public class StatisticsViewModel
    {
        public int TotalCandidates { get; set; }
        public int TotalParties { get; set; }
        public List<PartyStatsViewModel> CandidatesPerParty { get; set; }
    }
    public class PartyStatsViewModel
    {
        public string PartyName { get; set; }
        public int CandidateCount { get; set; }
    }
} 