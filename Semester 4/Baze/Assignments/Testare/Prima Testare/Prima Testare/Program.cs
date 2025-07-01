using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.SqlClient;
using System.Data;

namespace Prima_Testare
{
    class Program
    {
        static void Main(string[] args)
        {
            string connectionString = "Data SourCe=DESKTOP-MR70AB0;" + "Initial Catalog=Aircraft DealershipH;Integrated Security=True;";
            SqlConnection connection = new SqlConnection(connectionString);
            connection.Open();
            Console.WriteLine("Connection opened!");
            string query = "SELECT * FROM Aircraft";
            SqlCommand command = new SqlCommand(query, connection);
            SqlDataReader reader = command.ExecuteReader();
            while (reader.Read())
            {
                Console.WriteLine(reader["AircraftID"]);
                Console.WriteLine(reader["AircraftType"]);
                Console.WriteLine(reader["AircraftModel"]);
                Console.WriteLine(reader["AircraftPrice"]);
                Console.WriteLine(reader["AircraftYear"]);
                Console.WriteLine("\n");
            }
            connection.Close();
            Console.WriteLine("Connection closed!");

        }

        public void UserInterface()
        {
            while (true)
            {
                PrintMenu();
                int option = Convert.ToInt32(Console.ReadLine());
                switch (option)
                {
                    case 1:
                        PrintMenuDedicated();

                        int option2 = Convert.ToInt32(Console.ReadLine());
                        switch (option2)
                        {
                            case 1:
                                //AddCustomerParent();
                                break;
                            case 2:
                                //UpdateCustomerParent();
                                break;
                            case 3:
                                //DeleteCustomerParent();
                                break;
                            case 4:
                                //DisplayCustomersParent();
                                break;
                            case 5:
                                //Exit();
                                break;
                            default:
                                Console.WriteLine("Invalid option!");
                                break;
                        }

                        break;
                    case 2:
                        PrintMenuDedicated();

                        int option3 = Convert.ToInt32(Console.ReadLine());
                        switch (option3)
                        {
                            case 1:
                                //AddCustomerChild();
                                break;
                            case 2:
                                //UpdateCustomerChild();
                                break;
                            case 3:
                                //DeleteCustomerChild();
                                break;
                            case 4:
                                //DisplayCustomersChild();
                                break;
                            case 5:
                                //Exit();
                                break;
                            default:
                                Console.WriteLine("Invalid option!");
                                break;
                        }

                        break;
                    default:
                        Console.WriteLine("Invalid option!");
                        break;
                }
            }
        }

        public static void PrintMenuDedicated()
        {
            Console.WriteLine("1. Add a new customer");
            Console.WriteLine("2. Update a customer");
            Console.WriteLine("3. Delete a customer");
            Console.WriteLine("4. Display all customers");
            Console.WriteLine("5. Exit");
        }

        public static void PrintMenu()
        {
            Console.WriteLine("1.Parent Table");
            Console.WriteLine("2.Child Table");
        }

        public void ConnectToTables()
        {
            string conString = "Data Source=DESKTOP-MR70AB0;" + "Initial Catalog=Aircraft DealershipH;Integrated Security=True;";
            SqlConnection con = new SqlConnection(conString);
            con.Open();
            Console.WriteLine("Connection opened!");
        }

        public void CloseConnection()
        {
            string conString = "Data Source=DESKTOP-MR70AB0;" + "Initial Catalog=Aircraft DealershipH;Integrated Security=True;";
            SqlConnection con = new SqlConnection(conString);
            con.Close();
            Console.WriteLine("Connection closed!");
        }



    }
}
