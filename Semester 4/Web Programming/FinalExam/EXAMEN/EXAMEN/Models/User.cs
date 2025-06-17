using System.ComponentModel.DataAnnotations;

namespace EXAMEN.Models
{
    public class User
    {
        [Key]
        public int Id { get; set; }

        [Required]
        [StringLength(50)]
        public string Username { get; set; }

        [Required]
        public string Password { get; set; }

        public virtual ICollection<Reservation> Reservations { get; set; }
    }
}
