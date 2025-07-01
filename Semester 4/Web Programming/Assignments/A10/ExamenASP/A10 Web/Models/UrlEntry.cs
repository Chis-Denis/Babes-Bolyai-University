using System.ComponentModel.DataAnnotations;

public class UrlEntry
{
    public int Id { get; set; } // Primary key
    [Required]
    [Url]
    public string? Url { get; set; } // The web link
    [Required]
    public string? Description { get; set; } // Description or comment
    [Required]
    public string? Category { get; set; } // Category for grouping
    public string? UserId { get; set; } // The user who owns this URL
} 