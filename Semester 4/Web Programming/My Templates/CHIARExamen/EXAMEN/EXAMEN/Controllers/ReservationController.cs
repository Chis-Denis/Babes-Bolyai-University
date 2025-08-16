using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Authorization;
using EXAMEN.Data;
using EXAMEN.Models;
using EXAMEN.Services;
using System.Security.Claims;
using System.Linq;
using Microsoft.EntityFrameworkCore;

namespace EXAMEN.Controllers
{
    [Authorize]
    public class ReservationController : Controller
    {
        private readonly ApplicationDbContext _context;
        private readonly PricingService _pricingService;

        public ReservationController(ApplicationDbContext context, PricingService pricingService)
        {
            _context = context;
            _pricingService = pricingService;
        }

        [HttpGet]
        public IActionResult Search()
        {
            return View();
        }

        [HttpPost]
        public IActionResult Search(DateTime checkInDate, DateTime checkOutDate)
        {
            if (checkInDate >= checkOutDate)
            {
                ModelState.AddModelError("", "Check-out date must be after check-in date");
                return View();
            }

            var availableRooms = _pricingService.GetAvailableRooms(checkInDate, checkOutDate);
            ViewBag.CheckInDate = checkInDate;
            ViewBag.CheckOutDate = checkOutDate;
            return View("AvailableRooms", availableRooms);
        }

        [HttpGet]
        public IActionResult Reserve(int roomId, DateTime checkInDate, DateTime checkOutDate)
        {
            var room = _context.HotelRooms.Find(roomId);
            if (room == null)
                return NotFound();

            var totalPrice = _pricingService.CalculatePrice(roomId, checkInDate, checkOutDate);
            ViewBag.Room = room;
            ViewBag.CheckInDate = checkInDate;
            ViewBag.CheckOutDate = checkOutDate;
            ViewBag.TotalPrice = totalPrice;

            return View();
        }

        [HttpPost]
        public IActionResult Reserve(int roomId, DateTime checkInDate, DateTime checkOutDate, int numberOfGuests)
        {
            var userId = int.Parse(User.FindFirst(ClaimTypes.NameIdentifier).Value);
            var totalPrice = _pricingService.CalculatePrice(roomId, checkInDate, checkOutDate);

            var reservation = new Reservation
            {
                UserId = userId,
                HotelRoomId = roomId,
                CheckInDate = checkInDate,
                CheckOutDate = checkOutDate,
                NumberOfGuests = numberOfGuests,
                TotalPrice = totalPrice
            };

            _context.Reservations.Add(reservation);
            _context.SaveChanges();

            return RedirectToAction("MyReservations");
        }

        public IActionResult MyReservations()
        {
            var userId = int.Parse(User.FindFirst(ClaimTypes.NameIdentifier).Value);
            var reservations = _context.Reservations
                .Include(r => r.HotelRoom)
                .Where(r => r.UserId == userId)
                .OrderByDescending(r => r.CheckInDate)
                .ToList();

            return View(reservations);
        }

        // GET: Reservation/Delete/5
        public IActionResult Delete(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var reservation = _context.Reservations
                .Include(r => r.HotelRoom)
                .Include(r => r.User)
                .FirstOrDefault(m => m.Id == id);

            if (reservation == null)
            {
                return NotFound();
            }

            return View(reservation);
        }

        // POST: Reservation/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public IActionResult DeleteConfirmed(int id)
        {
            var reservation = _context.Reservations.Find(id);
            if (reservation != null)
            {
                _context.Reservations.Remove(reservation);
                _context.SaveChanges();
            }
            return RedirectToAction(nameof(MyReservations));
        }
    }
} 