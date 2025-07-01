namespace ASP1.Models;

public class Reservation
{
    public int id { get; set; }
    public string person { get; set; }
    public string date { get; set; }
    public string type { get; set; } // "Flight" or "Hotel"
    public int idReservedResource { get; set; } // flightID or hotelID
} 