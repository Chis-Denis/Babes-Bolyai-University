using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using ASP1.Models;
using ASP1.Data;
using Microsoft.AspNetCore.Authorization;
using Microsoft.EntityFrameworkCore;
using ASP1.ViewModels;

namespace ASP1.Controllers;

[Authorize]
public class HomeController : Controller
{
    private readonly ILogger<HomeController> _logger;
    private readonly ApplicationDbContext _context;

    public HomeController(ILogger<HomeController> logger, ApplicationDbContext context)
    {
        _logger = logger;
        _context = context;
    }

    public IActionResult Index()
    {
        return View();
    }

    [HttpGet]
    public IActionResult BeginReservation()
    {
        // If someone tries to access BeginReservation via GET, redirect to Index.
        return RedirectToAction("Index");
    }

    [HttpPost]
    public IActionResult BeginReservation(string personName, string specificDate, string destinationCity)
    {
        DateTime parsedDate;
        if (!DateTime.TryParseExact(specificDate, "yyyy-MM-dd", System.Globalization.CultureInfo.InvariantCulture, System.Globalization.DateTimeStyles.None, out parsedDate))
        {
            parsedDate = DateTime.Today;
        }

        ViewBag.PersonName = personName;
        ViewBag.SpecificDate = parsedDate.ToString("yyyy-MM-dd", System.Globalization.CultureInfo.InvariantCulture);
        ViewBag.DestinationCity = destinationCity;
        return View("ReservationOptions");
    }

    [HttpPost]
    public IActionResult ShowFlights(string personName, string specificDate, string destinationCity)
    {
        DateTime parsedDate;
        if (!DateTime.TryParseExact(specificDate, "yyyy-MM-dd", System.Globalization.CultureInfo.InvariantCulture, System.Globalization.DateTimeStyles.None, out parsedDate))
        {
            parsedDate = DateTime.Today;
        }
        var formattedDate = parsedDate.ToString("yyyy-MM-dd", System.Globalization.CultureInfo.InvariantCulture);
        var flights = _context.Flights
                            .Where(f => f.destinationCity == destinationCity && f.date == formattedDate && f.availableSeats > 0)
                            .ToList();
        ViewBag.PersonName = personName;
        ViewBag.SpecificDate = formattedDate;
        ViewBag.DestinationCity = destinationCity;
        return View("FlightsList", flights);
    }

    [HttpPost]
    public IActionResult ShowHotels(string personName, string specificDate, string destinationCity)
    {
        DateTime parsedDate;
        if (!DateTime.TryParseExact(specificDate, "yyyy-MM-dd", System.Globalization.CultureInfo.InvariantCulture, System.Globalization.DateTimeStyles.None, out parsedDate))
        {
            parsedDate = DateTime.Today;
        }
        var formattedDate = parsedDate.ToString("yyyy-MM-dd", System.Globalization.CultureInfo.InvariantCulture);
        var hotels = _context.Hotels
                           .Where(h => h.city == destinationCity && h.date == formattedDate && h.availableRooms > 0)
                           .ToList();
        ViewBag.PersonName = personName;
        ViewBag.SpecificDate = formattedDate;
        ViewBag.DestinationCity = destinationCity;
        return View("HotelsList", hotels);
    }

    [HttpPost]
    public IActionResult ReserveFlight(int flightID, string personName, string specificDate, string destinationCity)
    {
        DateTime parsedDate;
        if (!DateTime.TryParseExact(specificDate, "yyyy-MM-dd", System.Globalization.CultureInfo.InvariantCulture, System.Globalization.DateTimeStyles.None, out parsedDate))
        {
            parsedDate = DateTime.Today;
        }
        var flight = _context.Flights.Find(flightID);
        if (flight != null && flight.availableSeats > 0)
        {
            flight.availableSeats--;
            _context.Reservations.Add(new Reservation { person = personName, type = "Flight", date = parsedDate.ToString("yyyy-MM-dd", System.Globalization.CultureInfo.InvariantCulture), idReservedResource = flightID });
            _context.SaveChanges();
            ViewBag.Message = "Flight reserved successfully!";
        }
        else
        {
            ViewBag.Message = "Flight not available or already booked.";
        }
        var flights = _context.Flights
                            .Where(f => f.destinationCity == destinationCity && f.date == parsedDate.ToString("yyyy-MM-dd", System.Globalization.CultureInfo.InvariantCulture) && f.availableSeats > 0)
                            .ToList();
        ViewBag.PersonName = personName;
        ViewBag.SpecificDate = parsedDate.ToString("yyyy-MM-dd", System.Globalization.CultureInfo.InvariantCulture);
        ViewBag.DestinationCity = destinationCity;
        return View("FlightsList", flights);
    }

    [HttpPost]
    public IActionResult ReserveHotel(int hotelID, string personName, string specificDate, string destinationCity)
    {
        DateTime parsedDate;
        if (!DateTime.TryParseExact(specificDate, "yyyy-MM-dd", System.Globalization.CultureInfo.InvariantCulture, System.Globalization.DateTimeStyles.None, out parsedDate))
        {
            parsedDate = DateTime.Today;
        }
        var hotel = _context.Hotels.Find(hotelID);
        if (hotel != null && hotel.availableRooms > 0)
        {
            hotel.availableRooms--;
            _context.Reservations.Add(new Reservation { person = personName, type = "Hotel", date = parsedDate.ToString("yyyy-MM-dd", System.Globalization.CultureInfo.InvariantCulture), idReservedResource = hotelID });
            _context.SaveChanges();
            ViewBag.Message = "Hotel reserved successfully!";
        }
        else
        {
            ViewBag.Message = "Hotel not available or already booked.";
        }
        var hotels = _context.Hotels
                           .Where(h => h.city == destinationCity && h.date == parsedDate.ToString("yyyy-MM-dd", System.Globalization.CultureInfo.InvariantCulture) && h.availableRooms > 0)
                           .ToList();
        ViewBag.PersonName = personName;
        ViewBag.SpecificDate = parsedDate.ToString("yyyy-MM-dd", System.Globalization.CultureInfo.InvariantCulture);
        ViewBag.DestinationCity = destinationCity;
        return View("HotelsList", hotels);
    }

    public IActionResult Privacy()
    {
        return View();
    }

    public IActionResult ShowReservations(string personName, string specificDate, string destinationCity)
    {
        // Store ViewBag data for use in the view, maintaining context for filters.
        ViewBag.PersonName = personName;
        ViewBag.SpecificDate = specificDate;
        ViewBag.DestinationCity = destinationCity;

        // Query reservations for the specific person.
        var reservations = _context.Reservations
                                   .Where(r => r.person == personName)
                                   .ToList();

        return View(reservations);
    }

    [HttpPost]
    public IActionResult DeleteReservation(int id, string personName, string specificDate, string destinationCity)
    {
        // Find the reservation to delete by its ID.
        var reservationToDelete = _context.Reservations.Find(id);
        if (reservationToDelete != null)
        {
            // If the deleted reservation was a Flight, increment the available seats for that flight.
            if (reservationToDelete.type == "Flight")
            {
                var flight = _context.Flights.Find(reservationToDelete.idReservedResource);
                if (flight != null)
                {
                    flight.availableSeats++;
                }
            }
            // If the deleted reservation was a Hotel, increment the available rooms for that hotel.
            else if (reservationToDelete.type == "Hotel")
            {
                var hotel = _context.Hotels.Find(reservationToDelete.idReservedResource);
                if (hotel != null)
                {
                    hotel.availableRooms++;
                }
            }
            _context.Reservations.Remove(reservationToDelete); // Remove the reservation from the context.
            _context.SaveChanges(); // Save changes to the database.
        }
        // Restore ViewBag data before redirecting to ShowReservations, maintaining context.
        ViewBag.PersonName = personName;
        ViewBag.SpecificDate = specificDate;
        ViewBag.DestinationCity = destinationCity;
        // Redirect back to the ShowReservations page after deletion, preserving filters.
        return RedirectToAction("ShowReservations", new { personName = personName, specificDate = specificDate, destinationCity = destinationCity });
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
