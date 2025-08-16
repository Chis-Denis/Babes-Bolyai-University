using EXAMEN.Data;
using EXAMEN.Models;
using Microsoft.EntityFrameworkCore;

namespace EXAMEN.Data
{
    public static class SeedData
    {
        public static void Initialize(IServiceProvider serviceProvider)
        {
            using (var context = new ApplicationDbContext(
                serviceProvider.GetRequiredService<
                    DbContextOptions<ApplicationDbContext>>()))
            {
                // Look for any users.
                if (context.Users.Any())
                {
                    return;   // DB has been seeded
                }

                context.Users.AddRange(
                    new User
                    {
                        Username = "admin",
                        Password = "admin"
                    },
                    new User
                    {
                        Username = "user",
                        Password = "user"
                    }
                );
                context.SaveChanges();

                // Look for any rooms.
                if (context.HotelRooms.Any())
                {
                    return;   // DB has been seeded
                }

                context.HotelRooms.AddRange(
                    new HotelRoom
                    {
                        RoomNumber = "101",
                        Capacity = 2,
                        BasePrice = 100
                    },
                    new HotelRoom
                    {
                        RoomNumber = "102",
                        Capacity = 3,
                        BasePrice = 120
                    },
                    new HotelRoom
                    {
                        RoomNumber = "201",
                        Capacity = 4,
                        BasePrice = 150
                    },
                    new HotelRoom
                    {
                        RoomNumber = "202",
                        Capacity = 2,
                        BasePrice = 110
                    }
                );
                context.SaveChanges();
            }
        }
    }
} 