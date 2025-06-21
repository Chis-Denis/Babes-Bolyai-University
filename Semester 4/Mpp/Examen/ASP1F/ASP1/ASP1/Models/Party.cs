// Party model for Romanian political parties management system
namespace ASP1.Models;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

public class Party
{
    [Key]
    [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
    public int PartyId { get; set; }
    [Required]
    public string Name { get; set; }
    [Required]
    public string Description { get; set; }
    public string LogoUrl { get; set; } // Optional: logo image
    // Start of change: Remove [Required] from Candidates collection to fix create error *@
    public ICollection<Candidate> Candidates { get; set; } = new List<Candidate>();
    // End of change: Remove [Required] from Candidates collection to fix create error *@
} 