using EXAMEN.Data;
using EXAMEN.Models;
using Microsoft.EntityFrameworkCore;
using System.Linq;

namespace EXAMEN.Services
{
    public class PricingService
    {
        private readonly ApplicationDbContext _context;

        public PricingService(ApplicationDbContext context)
        {
            _context = context;
        }

        public int CalculatePrice(int roomId, DateTime checkInDate, DateTime checkOutDate)
        {
            var room = _context.HotelRooms.Find(roomId);
            if (room == null)
                throw new ArgumentException("Room not found");

            var totalRooms = _context.HotelRooms.Count();
            var roomsBookedInDateRange = _context.Reservations
                .Where(r => r.CheckInDate < checkOutDate && r.CheckOutDate > checkInDate)
                .Select(r => r.HotelRoomId)
                .Distinct()
                .Count();

            var occupancyRate = (double)roomsBookedInDateRange / totalRooms;

            if (occupancyRate <= 0.5)
                return room.BasePrice;
            else if (occupancyRate <= 0.8)
                return (int)(room.BasePrice * 1.2);
            else
                return (int)(room.BasePrice * 1.5);
        }

        public List<HotelRoom> GetAvailableRooms(DateTime checkInDate, DateTime checkOutDate)
        {
            var bookedRoomIds = _context.Reservations
                .Where(r => r.CheckInDate < checkOutDate && r.CheckOutDate > checkInDate)
                .Select(r => r.HotelRoomId)
                .Distinct()
                .ToList();

            return _context.HotelRooms
                .Where(r => !bookedRoomIds.Contains(r.Id))
                .ToList();
        }
    }
} 