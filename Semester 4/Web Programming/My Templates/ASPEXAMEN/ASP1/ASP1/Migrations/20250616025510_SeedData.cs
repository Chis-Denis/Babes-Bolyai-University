using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace ASP1.Migrations
{
    /// <inheritdoc />
    public partial class SeedData : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.InsertData(
                table: "Flights",
                columns: new[] { "flightID", "date", "destinationCity", "availableSeats" },
                values:
                new object[,]{
                    { 1, "2025-07-01", "New York", 100 },
                    { 2, "2025-07-01", "Los Angeles", 50 },
                    { 3, "2025-07-02", "New York", 75 }
                }
            );

            migrationBuilder.InsertData(
                table: "Hotels",
                columns: new[] { "hotelID", "hotelName", "date", "city", "availableRooms" },
                values:
                new object[,]{
                    { 1, "Grand Hyatt", "2025-07-01", "New York", 50 },
                    { 2, "The Plaza", "2025-07-01", "New York", 20 },
                    { 3, "Beverly Hilton", "2025-07-02", "Los Angeles", 30 }
                }
            );
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DeleteData(
                table: "Flights",
                keyColumn: "flightID",
                keyValues: new object[] { 1, 2, 3 }
            );

            migrationBuilder.DeleteData(
                table: "Hotels",
                keyColumn: "hotelID",
                keyValues: new object[] { 1, 2, 3 }
            );
        }
    }
}
