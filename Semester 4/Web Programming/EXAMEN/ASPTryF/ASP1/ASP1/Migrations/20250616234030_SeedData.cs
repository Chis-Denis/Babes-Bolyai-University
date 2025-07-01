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
                table: "People",
                columns: new[] { "id", "name", "role" },
                values:
                new object[,]{
                    { 1, "Chisu", "Professor" },
                    { 2, "Aurel", "Student" },
                    { 3, "Ciolacu", "Student" }
                }
            );

            migrationBuilder.InsertData(
                table: "Courses",
                columns: new[] { "id", "professorID", "courseName", "participants", "grades" },
                values:
                new object[,]{
                    { 1, 1, "Math", "Aurel, Ciolacu", "9, 2" },
                    { 2, 1, "English", "Vlad, Tudor", "10, 2" },
                    { 3, 1, "History", "Petrica, Ciolacu", "5, 4" }
                }
            );
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DeleteData(
                table: "People",
                keyColumn: "id",
                keyValues: new object[] { 1, 2, 3 }
            );

            migrationBuilder.DeleteData(
                table: "Courses",
                keyColumn: "id",
                keyValues: new object[] { 1, 2, 3 }
            );
        }
    }
}
