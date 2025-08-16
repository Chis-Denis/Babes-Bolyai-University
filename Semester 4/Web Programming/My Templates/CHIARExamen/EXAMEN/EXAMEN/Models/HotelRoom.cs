using System.ComponentModel.DataAnnotations;

namespace EXAMEN.Models
{
    public class HotelRoom
    {
        [Key]
        public int Id { get; set; }

        [Required]
        [StringLength(10)]
        public string RoomNumber { get; set; }

        [Required]
        public int Capacity { get; set; }

        [Required]
        public int BasePrice { get; set; }

        public virtual ICollection<Reservation> Reservations { get; set; }
    }
}
