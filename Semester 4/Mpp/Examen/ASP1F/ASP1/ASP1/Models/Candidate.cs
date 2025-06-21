// Candidate model for Romanian political parties management system
namespace ASP1.Models;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

public class Candidate
{
    [Key]
    [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
    public int CandidateId { get; set; }
    [Required]
    public string Name { get; set; }
    [Required]
    public string Description { get; set; }
    [Required]
    public string ImageUrl { get; set; }
    [Range(18, 120, ErrorMessage = "Age must be between 18 and 120.")]
    public int Age { get; set; } // Optional: candidate age
    [Required]
    public string Position { get; set; } // Optional: candidate position
    public int? PartyId { get; set; }
    public Party Party { get; set; }
} 