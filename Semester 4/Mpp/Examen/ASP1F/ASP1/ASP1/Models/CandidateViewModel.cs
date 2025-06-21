using System.ComponentModel.DataAnnotations;

namespace ASP1.Models
{
    public class CandidateViewModel
    {
        [Required]
        public string Name { get; set; }
        [Required]
        public string Description { get; set; }
        [Required]
        public string ImageUrl { get; set; }
        [Range(18, 120, ErrorMessage = "Age must be between 18 and 120.")]
        public int Age { get; set; }
        [Required]
        public string Position { get; set; }
        public string PartyName { get; set; }
        public int CandidateId { get; set; }
    }
} 