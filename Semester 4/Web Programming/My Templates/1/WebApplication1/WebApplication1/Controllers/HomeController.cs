using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using WebApplication1.Models;
using WebApplication1.Data;

namespace WebApplication1.Controllers;

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

    [HttpPost]
    public IActionResult BeginReservation(string personName, string specificDate, string destinationCity)
    {
        HttpContext.Session.SetString("PersonName", personName);
        HttpContext.Session.SetString("SpecificDate", specificDate);
        HttpContext.Session.SetString("DestinationCity", destinationCity);

        return RedirectToAction("ReservationOptions");
    }

    public IActionResult ReservationOptions()
    {
        ViewBag.PersonName = HttpContext.Session.GetString("PersonName");
        ViewBag.SpecificDate = HttpContext.Session.GetString("SpecificDate");
        ViewBag.DestinationCity = HttpContext.Session.GetString("DestinationCity");
        return View();
    }

    public IActionResult Flights()
    {
        var specificDate = HttpContext.Session.GetString("SpecificDate");
        var destinationCity = HttpContext.Session.GetString("DestinationCity");

        if (string.IsNullOrEmpty(specificDate) || string.IsNullOrEmpty(destinationCity))
        {
            return RedirectToAction("Index");
        }

        var flights = _context.Flights
            .Where(f => f.date == specificDate && f.destinationCity == destinationCity && f.availableSeats > 0)
            .ToList();

        ViewBag.PersonName = HttpContext.Session.GetString("PersonName");
        ViewBag.SpecificDate = specificDate;
        ViewBag.DestinationCity = destinationCity;

        return View(flights);
    }

    public IActionResult Hotels()
    {
        var specificDate = HttpContext.Session.GetString("SpecificDate");
        var destinationCity = HttpContext.Session.GetString("DestinationCity");

        if (string.IsNullOrEmpty(specificDate) || string.IsNullOrEmpty(destinationCity))
        {
            return RedirectToAction("Index");
        }

        var hotels = _context.Hotels
            .Where(h => h.date == specificDate && h.city == destinationCity && h.availableRooms > 0)
            .ToList();
        
        ViewBag.PersonName = HttpContext.Session.GetString("PersonName");
        ViewBag.SpecificDate = specificDate;
        ViewBag.DestinationCity = destinationCity;

        return View(hotels);
    }

    [HttpPost]
    public IActionResult ReserveFlight(int flightID)
    {
        var personName = HttpContext.Session.GetString("PersonName");
        var specificDate = HttpContext.Session.GetString("SpecificDate");

        if (string.IsNullOrEmpty(personName) || string.IsNullOrEmpty(specificDate))
        {
            return RedirectToAction("Index");
        }

        var flight = _context.Flights.Find(flightID);
        if (flight != null && flight.availableSeats > 0)
        {
            flight.availableSeats--;
            _context.Reservations.Add(new Reservation
            {
                person = personName,
                date = specificDate,
                type = "Flight",
                idReservedResource = flightID
            });
            _context.SaveChanges();
        }

        return RedirectToAction("Flights");
    }

    [HttpPost]
    public IActionResult ReserveHotel(int hotelID)
    {
        var personName = HttpContext.Session.GetString("PersonName");
        var specificDate = HttpContext.Session.GetString("SpecificDate");

        if (string.IsNullOrEmpty(personName) || string.IsNullOrEmpty(specificDate))
        {
            return RedirectToAction("Index");
        }

        var hotel = _context.Hotels.Find(hotelID);
        if (hotel != null && hotel.availableRooms > 0)
        {
            hotel.availableRooms--;
            _context.Reservations.Add(new Reservation
            {
                person = personName,
                date = specificDate,
                type = "Hotel",
                idReservedResource = hotelID
            });
            _context.SaveChanges();
        }

        return RedirectToAction("Hotels");
    }

    public IActionResult Reservations()
    {
        var personName = HttpContext.Session.GetString("PersonName");

        if (string.IsNullOrEmpty(personName))
        {
            return RedirectToAction("Index");
        }

        var reservations = _context.Reservations
            .Where(r => r.person == personName)
            .ToList();

        ViewBag.PersonName = personName;
        return View(reservations);
    }

    [HttpPost]
    public IActionResult CancelAllReservations()
    {
        var personName = HttpContext.Session.GetString("PersonName");

        if (string.IsNullOrEmpty(personName))
        {
            return RedirectToAction("Index");
        }

        var reservationsToCancel = _context.Reservations
            .Where(r => r.person == personName)
            .ToList();

        foreach (var reservation in reservationsToCancel)
        {
            if (reservation.type == "Flight")
            {
                var flight = _context.Flights.Find(reservation.idReservedResource);
                if (flight != null)
                { 
                    flight.availableSeats++;
                }
            }
            else if (reservation.type == "Hotel")
            {
                var hotel = _context.Hotels.Find(reservation.idReservedResource);
                if (hotel != null)
                {
                    hotel.availableRooms++;
                }
            }
            _context.Reservations.Remove(reservation);
        }
        _context.SaveChanges();

        return RedirectToAction("Reservations");
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }

    public IActionResult ShowDatabaseData()
    {
        var flights = _context.Flights.ToList();
        var hotels = _context.Hotels.ToList();
        var reservations = _context.Reservations.ToList();

        ViewBag.Flights = flights;
        ViewBag.Hotels = hotels;
        ViewBag.Reservations = reservations;

        return View();
    }

    public IActionResult SeedData()
    {
        // Clear existing data (optional, for testing purposes)
        _context.Flights.RemoveRange(_context.Flights);
        _context.Hotels.RemoveRange(_context.Hotels);
        _context.Reservations.RemoveRange(_context.Reservations);
        _context.SaveChanges();

        // Add 5 Flights
        for (int i = 1; i <= 5; i++)
        {
            _context.Flights.Add(new Flight
            {
                date = "2025-07-0" + i,
                destinationCity = "City" + i,
                availableSeats = 100 - (i * 10)
            });
        }

        // Add 5 Hotels
        for (int i = 1; i <= 5; i++)
        {
            _context.Hotels.Add(new Hotel
            {
                hotelName = "Hotel" + i,
                date = "2025-07-0" + i,
                city = "City" + i,
                availableRooms = 50 - (i * 5)
            });
        }

        // Add 5 Reservations (example, associate with existing flights/hotels)
        _context.Reservations.Add(new Reservation { person = "John Doe", date = "2025-07-01", type = "Flight", idReservedResource = 1 });
        _context.Reservations.Add(new Reservation { person = "Jane Smith", date = "2025-07-02", type = "Hotel", idReservedResource = 1 });
        _context.Reservations.Add(new Reservation { person = "John Doe", date = "2025-07-03", type = "Flight", idReservedResource = 2 });
        _context.Reservations.Add(new Reservation { person = "Peter Jones", date = "2025-07-04", type = "Hotel", idReservedResource = 2 });
        _context.Reservations.Add(new Reservation { person = "Jane Smith", date = "2025-07-05", type = "Flight", idReservedResource = 3 });

        _context.SaveChanges();

        return RedirectToAction("ShowDatabaseData");
    }
}
